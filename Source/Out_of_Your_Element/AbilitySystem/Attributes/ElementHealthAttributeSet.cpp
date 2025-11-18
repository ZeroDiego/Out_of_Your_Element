#include "ElementHealthAttributeSet.h"

#include "Out_of_Your_Element/AbilitySystem/Executions/ElementDamageExecution.h"
#include "GameplayEffectExtension.h"

UElementHealthAttributeSet::UElementHealthAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void UElementHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		if (NewValue < 1.0f)
			NewValue = 1.0f;

		if (GetHealth() > NewValue)
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent())
				AbilitySystemComponent->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UElementHealthAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute,
	const float OldValue,
	const float NewValue
)
{
	CHECK_AND_NOTIFY_UPDATE(Health, MaxHealth);

	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool IsElementalDamageEffectExecution(const FGameplayEffectExecutionDefinition& Def)
{
	return Def.CalculationClass->IsChildOf(UElementDamageExecution::StaticClass());
}

void UElementHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// ElementalEffectExecution will calculate resistances and apply damage
		if (const TObjectPtr<const UGameplayEffect>& Def = Data.EffectSpec.Def;
			Def && !Def->Executions.IsEmpty() &&
			Def->Executions.ContainsByPredicate(IsElementalDamageEffectExecution))
		{
			SetDamage(0.0f);
			return;
		}

		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);

		if (OldHealthValue != NewHealthValue)
			SetHealth(NewHealthValue);

		UE_LOG(LogTemp, Display, TEXT("Final Damage val: %.2f"), DamageValue);

		SetDamage(0.0f);
	}
}
