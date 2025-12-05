// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_WaterGun.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

UElementGameplayAbility_WaterGun::UElementGameplayAbility_WaterGun()
{
	AttackType = EAttackType::BaseAttack;
}

void UElementGameplayAbility_WaterGun::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::SetAttackingTrue();

	UAbilityTask_WaitGameplayEvent* WaitCastingTimeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ElementGameplayTags::Abilities_Casting_BaseAttack);
	WaitCastingTimeEvent->EventReceived.AddDynamic(this, &UElementGameplayAbility_WaterGun::OnAnimationFinished);
	WaitCastingTimeEvent->ReadyForActivation();
}

void UElementGameplayAbility_WaterGun::OnAnimationFinished(FGameplayEventData GameplayEventData)
{
	if (const AActor* Caster = GetAvatarActorFromActorInfo())
	{
		const float StepAngle = Spread / ProjectileCount;
		const int LeftSteps = ProjectileCount / 2;
		FVector Forward = Caster->GetActorForwardVector();

		// Move vector to far left of arc
		Forward = Forward.RotateAngleAxis(
			-1 * StepAngle * LeftSteps,
			FVector::ZAxisVector
		);

		for (int i = 0; i < ProjectileCount; ++i)
		{
			const FVector Offset = Forward * ProjectileSpawnOffset;
			const FVector Location = Caster->GetActorLocation() + Offset;
			const FRotator Rotation = Forward.Rotation();
			ShootProjectile(Location, Rotation);

			Forward = Forward.RotateAngleAxis(
				StepAngle,
				FVector::ZAxisVector
			);
		}
	}

	UAbilityTask_WaitGameplayEvent* WaitAbilityFinishedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ElementGameplayTags::Abilities_Casting);
	WaitAbilityFinishedEvent->EventReceived.AddDynamic(this, &UElementGameplayAbility_WaterGun::OnAbilityFinished);
	WaitAbilityFinishedEvent->ReadyForActivation();
}

void UElementGameplayAbility_WaterGun::OnAbilityFinished(FGameplayEventData GameplayEventData)
{
	Super::OnAbilityFinished(GameplayEventData);
}
