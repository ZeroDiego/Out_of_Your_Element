// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbilityProjectileBase.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Animation/ElementAnimInstance.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"

void UElementGameplayAbilityProjectileBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	SetAttackingTrue();

	UAbilityTask_WaitGameplayEvent* WaitCastingTimeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ElementGameplayTags::Abilities_Casting_BaseAttack);
	WaitCastingTimeEvent->EventReceived.AddDynamic(this, &UElementGameplayAbilityProjectileBase::OnAnimationFinished);
	WaitCastingTimeEvent->ReadyForActivation();
}

void UElementGameplayAbilityProjectileBase::SetAttackingTrue()
{
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		if (UElementAnimInstance* ElementAnimInstance = Cast<UElementAnimInstance>(
			ElementCharacterBase->GetMesh()->GetAnimInstance()))
		{
			if (ElementAnimInstance->AttackType == EAttackType::None)
			{
				ElementAnimInstance->AttackType = AttackType;
				if (ElementAnimInstance->CastingGameplayEffect)
				{
					const FGameplayEffectSpecHandle CastingGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ElementAnimInstance->CastingGameplayEffect);
					
					ElementAnimInstance->ActiveCastingGameplayEffect = ElementCharacterBase->GetAbilitySystemComponent()->
						BP_ApplyGameplayEffectSpecToSelf(CastingGameplayEffectSpecHandle);
				}
			}
		}
	}
}

void UElementGameplayAbilityProjectileBase::OnAnimationFinished(FGameplayEventData GameplayEventData)
{
	if (const AActor* Caster = GetAvatarActorFromActorInfo())
	{
		const FVector Offset = Caster->GetActorForwardVector() * ProjectileSpawnOffset;
		const FVector Location = Caster->GetActorLocation() + Offset;
		const FRotator Rotation = Caster->GetActorRotation();
		ShootProjectile(Location, Rotation);
	}

	UAbilityTask_WaitGameplayEvent* WaitAbilityFinishedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ElementGameplayTags::Abilities_Casting);
	WaitAbilityFinishedEvent->EventReceived.AddDynamic(this, &UElementGameplayAbilityProjectileBase::OnAbilityFinished);
	WaitAbilityFinishedEvent->ReadyForActivation();
}

void UElementGameplayAbilityProjectileBase::ShootProjectile(const FVector& Location, const FRotator& Direction)
{
	ShootProjectile(FTransform(Direction, Location));
}

void UElementGameplayAbilityProjectileBase::ShootProjectile(const FTransform& Transform)
{
	if (AElementProjectileBase* Projectile =
		GetWorld()->SpawnActorDeferred<AElementProjectileBase>(ProjectileClass, Transform)
	)
	{
		if (AActor* Caster = GetAvatarActorFromActorInfo())
		{
			Projectile->ProjectileSphereComponent->IgnoreActorWhenMoving(Caster, true);

			if (const ACharacter* CasterCharacter = Cast<ACharacter>(Caster))
			{
				CasterCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
			}
		}

		Projectile->ElementVfx = ProjectileVfx;
		Projectile->ElementPoofVfx = ProjectileHitVfx;

		if (DamageGameplayEffect)
		{
			const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

			SpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				BaseDamage
			);

			Projectile->GameplayEffectSpecHandle = SpecHandle;
		}

		Projectile->SourceAbility = this;

		UGameplayStatics::FinishSpawningActor(Projectile, Transform);
	}
}

void UElementGameplayAbilityProjectileBase::OnAbilityFinished(FGameplayEventData GameplayEventData)
{
	SetAttackingFalse();

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.2f);
	DelayTask->OnFinish.AddDynamic(this, &UElementGameplayAbilityProjectileBase::OnDelayFinished);
	DelayTask->ReadyForActivation();
}

void UElementGameplayAbilityProjectileBase::SetAttackingFalse()
{
	if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		if (UElementAnimInstance* ElementAnimInstance = Cast<UElementAnimInstance>(
			ElementCharacterBase->GetMesh()->GetAnimInstance()))
		{
			ElementAnimInstance->AttackType = EAttackType::None;
			ElementCharacterBase->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(
				ElementAnimInstance->ActiveCastingGameplayEffect);
		}
	}
}

void UElementGameplayAbilityProjectileBase::OnDelayFinished()
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
