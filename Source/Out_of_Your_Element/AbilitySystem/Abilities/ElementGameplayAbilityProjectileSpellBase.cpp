// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementGameplayAbilityProjectileSpellBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"

void UElementGameplayAbilityProjectileSpellBase::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AActor* Caster = GetAvatarActorFromActorInfo())
	{
		const FVector Offset = Caster->GetActorForwardVector() * ProjectileSpawnOffset;
		const FVector Location = Caster->GetActorLocation() + Offset;
		const FRotator Rotation = Caster->GetActorRotation();
		const FTransform Transform(Rotation, Location);
		const int Level = GetAbilityLevel(Handle, ActorInfo);

		ShootProjectile(Transform, Level);
	}
}

void UElementGameplayAbilityProjectileSpellBase::ShootProjectile(
	const FTransform& Transform,
	const int Level
) const
{
	if (AElementProjectileBase* Projectile =
		GetWorld()->SpawnActorDeferred<AElementProjectileBase>(ProjectileClass, Transform)
	)
	{
		Projectile->Level = Level;

		if (AActor* Caster = GetAvatarActorFromActorInfo())
		{
			Projectile->Caster = Caster;
			Projectile->ProjectileSphereComponent->IgnoreActorWhenMoving(Caster, true);

			if (const ACharacter* CasterCharacter = Cast<ACharacter>(Caster))
			{
				CasterCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
			}
		}

		if (DamageGameplayEffect)
		{
			const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

			SpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				BaseDamage
			);

			Projectile->DamageGameplayEffectSpecHandle = SpecHandle;
		}

		UGameplayStatics::FinishSpawningActor(Projectile, Transform);
	}
}
