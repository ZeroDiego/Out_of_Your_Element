#include "ElementalDamageEffectExecution.h"
#include "ElementGameplayTags.h"
#include "HealthAttributeSet.h"

UElementalDamageEffectExecution::UElementalDamageEffectExecution()
{
	const FGameplayEffectAttributeCaptureDefinition AttributeCaptureDef(
		UHealthAttributeSet::GetDamageResistanceAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		true
	);

	RelevantAttributesToCapture.Add(AttributeCaptureDef);
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

	float DamageResistance = 0.0f;

	{
		// Get all tags on source and target
		FAggregatorEvaluateParameters EvalParams;
		EvalParams.SourceTags = DamageSpec.CapturedSourceTags.GetAggregatedTags();
		EvalParams.TargetTags = DamageSpec.CapturedTargetTags.GetAggregatedTags();

		// Filter source modifiers by the DamageType tag
		EvalParams.AppliedSourceTagFilter = FGameplayTagContainer(DamageType);

		// Prepare to capture the damage resistance modifiers from the target
		const FGameplayEffectAttributeCaptureDefinition AttributeCaptureDef(
			UHealthAttributeSet::GetDamageResistanceAttribute(),
			EGameplayEffectAttributeCaptureSource::Target,
			true
		);

		// Capture and calculate damage resistance modifiers from the target with the DamageType tag
		const bool Evaluated = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			AttributeCaptureDef,
			EvalParams,
			DamageResistance
		);

		UE_LOG(LogTemp, Display, TEXT("Damage resistance for '%s': [%d] %.2f"), *DamageType.ToString(), Evaluated, DamageResistance);
	}

	const float TotalDamage = DamageTaken * (1.0f - FMath::Clamp(DamageResistance, 0.0f, 1.0f));
	UE_LOG(LogTemp, Display, TEXT("Base Damage val: %.2f | Total Damage: %.2f"), DamageTaken, TotalDamage);

	if (TotalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UHealthAttributeSet::GetDamageAttribute(),
			EGameplayModOp::Additive,
			TotalDamage
		));
	}
}
