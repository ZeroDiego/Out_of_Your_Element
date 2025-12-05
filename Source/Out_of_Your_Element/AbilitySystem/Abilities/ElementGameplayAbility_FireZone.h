// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "ElementGameplayAbilityBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"
#include "ElementGameplayAbility_FireZone.generated.h"

class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_FireZone : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AElementZoneBase> FireZoneClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="FireZone")
	float FireZoneDamageDuration = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireZone")
	float FireZoneRadius = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="FireZone")
	float FireZoneLifeSpan = 10;

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	UNiagaraSystem* FireZoneVfx;
};
