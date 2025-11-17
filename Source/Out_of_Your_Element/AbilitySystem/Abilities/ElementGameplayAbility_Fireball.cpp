// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_Fireball.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementFiringOffset.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void UElementGameplayAbility_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

			if (AElementProjectileBase* Fireball = GetWorld()->SpawnActorDeferred<AElementProjectileBase>(
				ProjectileBase,
				SpawnProjectileTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				Fireball->ProjectileMeshComponent->IgnoreActorWhenMoving(Actor, true);

				// projectile VFX
				Fireball->ElementVfx = FireballVfx;
				Fireball->ElementPoofVfx = FireballPoofVfx;

				const FGameplayEffectSpecHandle FireballGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
					FireballGameplayEffect,
					1);
				FireballGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
					ElementGameplayTags::Abilities_Parameters_Duration,
					FireballDamageDuration);
				FireballGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
					ElementGameplayTags::Abilities_Parameters_Damage,
					FireballDamagePerSecond);
				Fireball->GameplayEffectSpecHandle = FireballGameplayEffectSpecHandle;
				Fireball->SourceAbility = this;
				UGameplayStatics::FinishSpawningActor(Fireball, SpawnProjectileTransform);
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
