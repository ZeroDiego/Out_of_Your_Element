// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "ElementAbilitySystemComponent.generated.h"

USTRUCT(Blueprintable)
struct FDefaultGameplayEffectTags
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> Tags;
};

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
	                                TArray<class UElementGameplayAbilitySpellBase*>& ActiveAbilities) const;
};
