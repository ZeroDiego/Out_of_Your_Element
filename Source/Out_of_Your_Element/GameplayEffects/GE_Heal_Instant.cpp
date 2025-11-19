#include "GE_Heal_Instant.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"

UGE_Heal_Instant::UGE_Heal_Instant()
{
	// This effect applies instantly
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Create a modifier that ADDs to Health
	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UElementHealthAttributeSet::GetHealthAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;

	// Amount healed → change this number to whatever you want
	Modifier.ModifierMagnitude = FScalableFloat(20.0f);

	Modifiers.Add(Modifier);
}