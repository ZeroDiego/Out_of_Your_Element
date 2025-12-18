#include "ElementDamageExecution.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
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

	const UElementAbilitySystemComponent* ElementAbilitySystemComponent = Cast<UElementAbilitySystemComponent>(
		ExecutionParams.GetTargetAbilitySystemComponent()
	);

	if (!ElementAbilitySystemComponent)
		return;

	const UElementHealthAttributeSet* HealthAttributeSet = Cast<const UElementHealthAttributeSet>(
		ElementAbilitySystemComponent->GetAttributeSet(UElementHealthAttributeSet::StaticClass())
	);

	if (!HealthAttributeSet || HealthAttributeSet->GetHealth() == 0.0f)
		return;

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
	float HealPercent = 0.0f;
	float HealFixed = 0.0f;

	HealthAttributeSet->GetResistanceByTag(
		DamageType,
		DamageResistancePercent,
		DamageResistanceFixed,
		HealPercent,
		HealFixed
	);

	const float TotalDamage = DamageTaken * (1.0f - DamageResistancePercent) - DamageResistanceFixed
		- (DamageTaken * HealPercent + HealFixed);

	if (FMath::IsNearlyZero(DamageTaken, .01f))
		return;

	if (HealthAttributeSet)
	{
		const FGameplayEffectContextHandle& Context = DamageSpec.GetContext();
		HealthAttributeSet->OnDamageTaken.Broadcast(FDamageTaken(
			TotalDamage,
			true,
			DamageType,
			Context.GetOriginalInstigator(),
			Context.GetEffectCauser()
		));
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UElementHealthAttributeSet::GetHealthAttribute(),
		EGameplayModOp::Additive,
		-TotalDamage
	));
}
