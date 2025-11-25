// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Abilities/GameplayAbility.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"
#include "ElementGameplayAbility_FireZone.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_FireZone : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//Override ActiveAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AElementZoneBase> ElementZoneBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> FireDotDamageGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float FireZoneDotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float FireZoneTickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float FireZoneDotDamageDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float FireZoneRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float FireZoneLifeSpan;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* FireZoneDotVfx;
};
