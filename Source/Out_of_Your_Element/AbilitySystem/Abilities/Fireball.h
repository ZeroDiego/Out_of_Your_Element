// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Fireball.generated.h"

class AProjectileBase;
class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UFireball : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//Override ActiveAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> FireballGameplayEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> ProjectileBase;

	UPROPERTY(EditAnywhere)
	float FireballDamageDuration;

	UPROPERTY(EditAnywhere)
	float FireballDamagePerSecond;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* FireballVfx;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* FireballPoofVfx;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* FireballDotVfx;
};
