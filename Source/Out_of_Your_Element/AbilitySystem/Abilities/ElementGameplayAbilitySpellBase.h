// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ElementGameplayAbilitySpellBase.generated.h"

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilitySpellBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) PURE_VIRTUAL(UElementGameplayAbilityBase::CastSpell);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	UAnimMontage* AbilityMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	float BaseDamage = 25.0f;

private:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
