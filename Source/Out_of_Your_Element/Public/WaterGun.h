// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Abilities/GameplayAbility.h"
#include "WaterGun.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UWaterGun : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//Override ActiveAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* WaterGunMaterial;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> WaterGunGameplayEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> ProjectileBase;
};
