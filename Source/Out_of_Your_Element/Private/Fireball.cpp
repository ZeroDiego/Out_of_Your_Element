// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "GameFramework/Character.h"
#include "ElementAbilitySystemComponent.h"
#include "ElementGameplayTags.h"
#include "FiringOffset.h"
#include "ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

void UFireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
			const FRotator SpawnProjectileRotation = Character->GetActorRotation();
			const FTransform SpawnProjectileTransform(SpawnProjectileRotation, SpawnProjectileLocation);
			if (AProjectileBase* Fireball = GetWorld()->SpawnActorDeferred<AProjectileBase>(AProjectileBase::StaticClass(), SpawnProjectileTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				Fireball->Projectile->IgnoreActorWhenMoving(Actor, true);
				Fireball->Projectile->SetMaterial(0, FireballMaterial);
				const FGameplayEffectSpecHandle FireballGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(FireballGameplayEffect, 1);
				FireballGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(ElementGameplayTags::Abilities_Parameters_Damage, 10);
				Fireball->GameplayEffectSpecHandle = FireballGameplayEffectSpecHandle;
				UGameplayStatics::FinishSpawningActor(Fireball,SpawnProjectileTransform);
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
