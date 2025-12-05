// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbilityProjectileBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"

void UElementGameplayAbilityProjectileBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (const AActor* Caster = GetAvatarActorFromActorInfo())
	{
		const FVector Offset = Caster->GetActorForwardVector() * ProjectileSpawnOffset;
		const FVector Location = Caster->GetActorLocation() + Offset;
		const FRotator Rotation = Caster->GetActorRotation();
		ShootProjectile(Location, Rotation);
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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
