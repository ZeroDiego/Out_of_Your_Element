// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "ElementGameplayAbilityProjectileSpellBase.h"
#include "ElementGameplayAbility_WaterGun.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_WaterGun : public UElementGameplayAbilityProjectileSpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Degrees"), Category="WaterGun")
	float Spread = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WaterGun")
	int ProjectileCount = 3;

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
