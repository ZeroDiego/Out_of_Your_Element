// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_RockThrow.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementFiringOffset.h"
#include "Out_of_Your_Element/Projectile//ElementProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void UElementGameplayAbility_RockThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnProjectileLocation = Character->GetComponentByClass<UElementFiringOffset>()
			                                                 ->GetComponentLocation();
			const FRotator SpawnProjectileRotation = Character->GetActorRotation();
			const FTransform SpawnProjectileTransform(SpawnProjectileRotation, SpawnProjectileLocation);

			if (AElementProjectileBase* RockThrow = GetWorld()->SpawnActorDeferred<AElementProjectileBase>(
				ProjectileBase,
				SpawnProjectileTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				RockThrow->ProjectileMeshComponent->IgnoreActorWhenMoving(Actor, true);

				// projectile VFX
				RockThrow->ElementVfx = RockVfx;
				RockThrow->ElementPoofVfx = RockPoofVfx;

				const FGameplayEffectSpecHandle RockThrowGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
					RockThrowGameplayEffect,
					1);
				RockThrowGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
					ElementGameplayTags::Abilities_Parameters_Damage,
					10);
				RockThrow->GameplayEffectSpecHandle = RockThrowGameplayEffectSpecHandle;
				UGameplayStatics::FinishSpawningActor(RockThrow, SpawnProjectileTransform);
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
