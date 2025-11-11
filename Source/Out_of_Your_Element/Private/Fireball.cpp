// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "GameFramework/Character.h"
#include "ElementAbilitySystemComponent.h"
#include "FiringOffset.h"
#include "ProjectileBase.h"

void UFireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
		const FRotator SpawnProjectileRotation = Character->GetActorRotation();
		const FActorSpawnParameters ActorSpawnParams;
		GetWorld()->SpawnActor<AProjectileBase>(SpawnProjectileLocation, SpawnProjectileRotation, ActorSpawnParams);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
