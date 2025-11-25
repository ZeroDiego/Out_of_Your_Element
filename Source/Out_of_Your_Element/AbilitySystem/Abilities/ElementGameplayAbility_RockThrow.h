// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ElementGameplayAbility_RockThrow.generated.h"

class AElementProjectileBase;
class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_RockThrow : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//Override ActiveAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> RockThrowGameplayEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AElementProjectileBase> ProjectileBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Floats")
	float RockThrowDamage;
	
	UPROPERTY(EditAnywhere, Category="Floats")
	float SpawningOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* RockVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* RockPoofVfx;
};
