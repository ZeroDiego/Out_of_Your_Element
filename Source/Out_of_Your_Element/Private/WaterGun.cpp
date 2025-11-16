// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGun.h"
#include "GameFramework/Character.h"
#include "ElementAbilitySystemComponent.h"
#include "ElementGameplayTags.h"
#include "FiringOffset.h"
#include "ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void UWaterGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                const FGameplayEventData* TriggerEventData)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			
			const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->
			                                                   GetComponentLocation();
			const FRotator SpawnProjectileRotation = Character->GetActorRotation();
			const FTransform SpawnProjectileTransform(SpawnProjectileRotation, SpawnProjectileLocation);

			if (AProjectileBase* WaterGun = GetWorld()->SpawnActorDeferred<AProjectileBase>(
				ProjectileBase,
				SpawnProjectileTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				WaterGun->ProjectileMeshComponent->IgnoreActorWhenMoving(Actor, true);
				WaterGun->ProjectileMeshComponent->SetMaterial(0, WaterGunMaterial);

				// projectile VFX
				WaterGun->ElementVFX = WaterVFX;

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
