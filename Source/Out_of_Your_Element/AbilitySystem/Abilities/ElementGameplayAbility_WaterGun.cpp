// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_WaterGun.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Out_of_Your_Element/Projectile//ElementProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void UElementGameplayAbility_WaterGun::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnProjectileOffset = Character->GetActorForwardVector() * SpawningOffset;
			const FVector SpawnProjectileLocation = Character->GetActorLocation() + SpawnProjectileOffset;
			const FRotator SpawnProjectileRotation = Character->GetActorRotation();
			const FTransform SpawnProjectileTransform(SpawnProjectileRotation, SpawnProjectileLocation);

			if (AElementProjectileBase* WaterGun = GetWorld()->SpawnActorDeferred<AElementProjectileBase>(
				ProjectileBase,
				SpawnProjectileTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				Character->GetCapsuleComponent()->IgnoreActorWhenMoving(WaterGun, true);
				WaterGun->ProjectileSphereComponent->IgnoreActorWhenMoving(Actor, true);

				// projectile VFX
				WaterGun->ElementVfx = WaterVfx;
				WaterGun->ElementPoofVfx = WaterPoofVfx;

				const FGameplayEffectSpecHandle WaterGunGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
					WaterGunGameplayEffect,
					1);
				WaterGunGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
					ElementGameplayTags::Abilities_Parameters_Damage,
					10);
				WaterGun->GameplayEffectSpecHandle = WaterGunGameplayEffectSpecHandle;
				UGameplayStatics::FinishSpawningActor(WaterGun, SpawnProjectileTransform);
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
