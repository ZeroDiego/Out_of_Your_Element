// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementAbilitySystemComponent.h"

#include "Abilities/ElementGameplayAbilitySpellBase.h"

void UElementAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
                                                                TArray<UElementGameplayAbilitySpellBase*>&
                                                                ActiveAbilities) const
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		GameplayTagContainer,
		AbilitiesToActivate,
		false
	);

	for (const FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		for (TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();
		     UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UElementGameplayAbilitySpellBase>(ActiveAbility));
		}
	}
}

void UElementAbilitySystemComponent::SetAbilityLevel(FGameplayAbilitySpec* AbilitySpec, const int NewLevel)
{
	if (AbilitySpec)
	{
		AbilitySpec->Level = NewLevel;
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UElementAbilitySystemComponent::SetAbilityLevelByClass(const TSubclassOf<UGameplayAbility> AbilityClass,
                                                            const int NewLevel)
{
	SetAbilityLevel(FindAbilitySpecFromClass(AbilityClass), NewLevel);
}

void UElementAbilitySystemComponent::SetAbilityLevelByHandle(const FGameplayAbilitySpecHandle AbilityHandle,
                                                             const int NewLevel)
{
	SetAbilityLevel(FindAbilitySpecFromHandle(AbilityHandle), NewLevel);
}
