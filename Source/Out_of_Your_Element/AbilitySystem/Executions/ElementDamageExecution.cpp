#include "ElementDamageExecution.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"

bool FindAnyExact(const FGameplayTagContainer& Target, const FGameplayTagContainer& Source, FGameplayTag& Found)
{
	for (TArray<FGameplayTag>::TConstIterator TagIterator = Target.CreateConstIterator(); TagIterator; ++TagIterator)
	{
		if (Source.HasTagExact(*TagIterator))
		{
			Found = *TagIterator;
			return true;
		}
	}

	return false;
}

void UElementDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);


	const FGameplayEffectSpec& DamageSpec = ExecutionParams.GetOwningSpec();
	const FGameplayEffectModifiedAttribute* DamageAttribute =
		DamageSpec.GetModifiedAttribute(UElementHealthAttributeSet::GetDamageAttribute());

	if (!DamageAttribute)
	{
		UE_LOG(LogAbilitySystemComponent, Error, TEXT(
			       "An elemental damage was triggered without damage? Make sure the Damage Attribute is set as a modifier!"
		       ));

		return;
	}

	const float DamageTaken = DamageAttribute->TotalMagnitude;

	FGameplayTag DamageType;
	{
		FGameplayTagContainer AssetTags;
		DamageSpec.GetAllAssetTags(AssetTags);

		const FGameplayTagContainer DamageTypes = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
			ElementGameplayTags::Damage_Type_Fire,
			ElementGameplayTags::Damage_Type_Water,
			ElementGameplayTags::Damage_Type_Nature
		});

		if (!FindAnyExact(DamageTypes, AssetTags, DamageType))
			return;
	}

	if (FMath::IsNearlyZero(DamageTaken, .02f))
		return;

	float DamageResistancePercent = 0.0f;
	float DamageResistanceFixed = 0.0f;

	{
		FGameplayTagContainer AssetTags;
		for (FActiveGameplayEffectIterator<const FActiveGameplayEffect, FActiveGameplayEffectsContainer> It =
			     ExecutionParams.GetTargetAbilitySystemComponent()->
			                     GetActiveGameplayEffects().
			                     CreateConstIterator(); It; ++It
		)
		{
			const FActiveGameplayEffect& ActiveEffect = *It;
			const FGameplayEffectSpec& Spec = ActiveEffect.Spec;

			Spec.GetAllAssetTags(AssetTags);
			if (!AssetTags.HasTagExact(DamageType))
				continue;

			DamageResistancePercent += Spec.GetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Resistance_Percent,
				false
			);

			DamageResistanceFixed += Spec.GetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Resistance_Fixed,
				false
			);
		}

		UE_LOG(LogTemp, Display, TEXT("Damage resistance for '%s': %.2f%% + %.2f"), *DamageType.ToString(),
		       DamageResistancePercent * 100, DamageResistanceFixed);
	}

	const float TotalDamage =
		DamageTaken * (1.0f - FMath::Clamp(DamageResistancePercent, 0.0f, 1.0f)) - DamageResistanceFixed;

	UE_LOG(LogTemp, Display, TEXT("Base Damage val: %.2f | Total Damage: %.2f"), DamageTaken, TotalDamage);

	if (FMath::IsNearlyZero(DamageTaken, .01f))
		return;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UElementHealthAttributeSet::GetHealthAttribute(),
		EGameplayModOp::Additive,
		-TotalDamage
	));
}
