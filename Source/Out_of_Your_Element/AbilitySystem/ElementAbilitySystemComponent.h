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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityUnlockedDelegate, FGameplayAbilitySpecHandle, AbilityHandle);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityUpgradedDelegate, FGameplayAbilitySpecHandle, AbilityHandle);

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAbilityUnlockedDelegate OnAbilityUnlocked;

	UPROPERTY(BlueprintAssignable)
	FAbilityUpgradedDelegate OnAbilityUpgraded;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
	                                TArray<class UElementGameplayAbilitySpellBase*>& ActiveAbilities) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAssetTagByHandle(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAbilityLevel(FGameplayAbilitySpecHandle AbilityHandle) const;

	void SetAbilityLevel(FGameplayAbilitySpec* AbilitySpec, int NewLevel);

	UFUNCTION(BlueprintCallable)
	void SetAbilityLevelByClass(TSubclassOf<UGameplayAbility> AbilityClass, int NewLevel);

	UFUNCTION(BlueprintCallable)
	void SetAbilityLevelByHandle(FGameplayAbilitySpecHandle AbilityHandle, int NewLevel);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetAbilityTagByHandle(FGameplayAbilitySpecHandle AbilityHandle) const;

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};
