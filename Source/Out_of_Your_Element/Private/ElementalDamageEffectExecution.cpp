#include "ElementalDamageEffectExecution.h"
#include "ElementGameplayTags.h"


UElementalDamageEffectExecution::UElementalDamageEffectExecution()
{
}

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

void UElementalDamageEffectExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& DamageSpec = ExecutionParams.GetOwningSpec();
	const float DamageTaken = DamageSpec
		.GetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Damage,
			true,
			0.0f
		);

	FGameplayTag DamageType;
	{
		FGameplayTagContainer AssetTags;
		DamageSpec.GetAllAssetTags(AssetTags);

		if (!FindAnyExact(ElementGameplayTags::DamageTypes, AssetTags, DamageType))
			return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s Damage Taken: %.2f"), *DamageType.ToString(), DamageTaken);
}
