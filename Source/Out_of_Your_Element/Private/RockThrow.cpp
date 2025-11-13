// Fill out your copyright notice in the Description page of Project Settings.


#include "RockThrow.h"
#include "GameFramework/Character.h"
#include "ElementAbilitySystemComponent.h"
#include "ElementGameplayTags.h"
#include "FiringOffset.h"
#include "ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void URockThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
			const FRotator SpawnProjectileRotation = Character->GetActorRotation();
			const FTransform SpawnProjectileTransform(SpawnProjectileRotation, SpawnProjectileLocation);
			if (AProjectileBase* RockThrow = GetWorld()->SpawnActorDeferred<AProjectileBase>(AProjectileBase::StaticClass(), SpawnProjectileTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				RockThrow->Projectile->IgnoreActorWhenMoving(Actor, true);
				RockThrow->Projectile->SetMaterial(0, RockThrowMaterial);
				const FGameplayEffectSpecHandle RockThrowGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(RockThrowGameplayEffect, 1);
				RockThrowGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(ElementGameplayTags::Abilities_Parameters_Damage, 10);
				RockThrow->GameplayEffectSpecHandle = RockThrowGameplayEffectSpecHandle;
				UGameplayStatics::FinishSpawningActor(RockThrow, SpawnProjectileTransform);
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}