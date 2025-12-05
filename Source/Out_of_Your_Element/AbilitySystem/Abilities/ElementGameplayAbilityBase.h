// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "Abilities/GameplayAbility.h"
#include "Out_of_Your_Element/Animation/ElementAnimInstance.h"
#include "ElementGameplayAbilityBase.generated.h"

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override PURE_VIRTUAL(UElementGameplayAbilityBase::ActivateAbility);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Element Ability")
	float BaseDamage = 25.0f;

	UPROPERTY(BlueprintReadOnly)
	EAttackType AttackType = EAttackType::None;
};
