// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "ElementGameplayAbilityProjectileBase.h"
#include "ElementGameplayAbility_IceLance.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_IceLance : public UElementGameplayAbilityProjectileBase
{
	GENERATED_BODY()

public:
	UElementGameplayAbility_IceLance();
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void OnAnimationFinished(FGameplayEventData GameplayEventData) override;
	virtual void OnAbilityFinished(FGameplayEventData GameplayEventData) override;
};
