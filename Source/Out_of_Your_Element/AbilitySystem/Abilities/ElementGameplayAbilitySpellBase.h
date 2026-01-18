// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ElementGameplayAbilitySpellBase.generated.h"

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilitySpellBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	);

	virtual void EndSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	);

	UPROPERTY(EditDefaultsOnly, Category="Element Ability")
	UAnimMontage* AbilityMontage;

	UPROPERTY(EditDefaultsOnly, Category="Element Ability")
	TSubclassOf<UGameplayEffect> CastingGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Element Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Element Ability")
	USoundBase* ActivationSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	float BaseDamage = 25.0f;
};
