// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementGameplayAbilityRangedSpellBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"
#include "ElementGameplayAbility_FireZone.generated.h"

class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_FireZone : public UElementGameplayAbilityRangedSpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="FireZone")
	TSubclassOf<AElementZoneBase> FireZoneClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="FireZone")
	float FireZoneDamageDuration = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="FireZone")
	float FireZoneRadius = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="FireZone")
	float FireZoneLifeSpan = 10;

	UPROPERTY(EditDefaultsOnly, Category="FireZone")
	UNiagaraSystem* FireZoneVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="FireZone Level 2")
	float AdditionalFireZoneRadius = 125;

	UPROPERTY(EditDefaultsOnly, Category="FireZone Level 2")
	UNiagaraSystem* LargeFireZoneVfx;

protected:
	virtual void CastSpellAtLocation(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData,
		const FVector& Location
	) override;
};
