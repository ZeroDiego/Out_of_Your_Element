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

bool UElementAbilitySystemComponent::HasAssetTagByHandle(
	const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayTag Tag
) const
{
	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle))
	{
		return AbilitySpec->Ability->GetAssetTags().HasTag(Tag);
	}

	return false;
}

int UElementAbilitySystemComponent::GetAbilityLevel(const FGameplayAbilitySpecHandle AbilityHandle) const
{
	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle))
	{
		return AbilitySpec->Level;
	}

	return -1;
}

void UElementAbilitySystemComponent::SetAbilityLevel(FGameplayAbilitySpec* AbilitySpec, const int NewLevel)
{
	if (AbilitySpec)
	{
		AbilitySpec->Level = NewLevel;
		MarkAbilitySpecDirty(*AbilitySpec);

		if (OnAbilityUpgraded.IsBound())
			OnAbilityUpgraded.Broadcast(AbilitySpec->Handle);
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

FGameplayTag UElementAbilitySystemComponent::GetAbilityTagByHandle(const FGameplayAbilitySpecHandle AbilityHandle) const
{
	if (const FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityHandle))
		return Spec->Ability->GetAssetTags().First();

	return FGameplayTag::EmptyTag;
}

void UElementAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (OnAbilityUnlocked.IsBound())
		OnAbilityUnlocked.Broadcast(AbilitySpec.Handle);
}
