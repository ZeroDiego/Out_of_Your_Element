#include "ElementHealthAttributeSet.h"

#include "AbilitySystemInterface.h"
#include "Out_of_Your_Element/AbilitySystem/Executions/ElementDamageExecution.h"
#include "GameplayEffectExtension.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"

// TODO Test this thoroughly
void UElementHealthAttributeSet::UpdateResistances(const FGameplayEffectSpec& Spec, const bool Removed)
{
	const float Modifier = Removed ? -1 : 1;
	const TObjectPtr<const UGameplayEffect>& Def = Spec.Def;
	if (const FGameplayTag& DamageType = Def->GetAssetTags().First(); DamageType.IsValid())
	{
		FResistance* Resistance = DamageResistances.Find(DamageType);

		const bool bFound = static_cast<bool>(Resistance);
		FResistance EmptyOrNew = FResistance();

		FResistance& New = bFound ? *Resistance : EmptyOrNew;
		const FResistance Old = FResistance(New);

		auto& [
			DamageResistancePercent,
			DamageResistanceFixed,
			HealPercent,
			HealFixed
		] = New;

		DamageResistancePercent = FMath::Clamp(
			DamageResistancePercent + Modifier * Spec.GetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Resistance_Percent,
				false
			),
			0.0f,
			1.0f
		);

		DamageResistanceFixed += Modifier * Spec.GetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Resistance_Fixed,
			false
		);

		HealPercent += Modifier * Spec.GetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Heal_Percent,
			false
		);

		HealFixed += Modifier * Spec.GetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Heal_Fixed,
			false
		);

		if (Old != New)
		{
			if (!bFound)
			{
				DamageResistances.Add(DamageType, New);
			}
			else if (New.IsEmpty())
			{
				DamageResistances.Remove(DamageType);
			}

			if (OnResistanceChanged.IsBound())
			{
				OnResistanceChanged.Broadcast(this, DamageType, Old, New);
			}
		}
	}
}

UElementHealthAttributeSet::UElementHealthAttributeSet()
{
	if (const IAbilitySystemInterface* Asi = Cast<IAbilitySystemInterface>(GetOuter()))
	{
		OnDamageTaken.AddUniqueDynamic(this, &UElementHealthAttributeSet::SetLastDamageTaken);

		if (UAbilitySystemComponent* Asc = Asi->GetAbilitySystemComponent())
		{
			Asc->OnAnyGameplayEffectRemovedDelegate().AddWeakLambda(
				this,
				[this](const FActiveGameplayEffect& Effect)
				{
					if (Effect.GetDuration() != FGameplayEffectConstants::INSTANT_APPLICATION)
						UpdateResistances(Effect.Spec, true);
				}
			);

			Asc->OnActiveGameplayEffectAddedDelegateToSelf.AddWeakLambda(
				this, [this](UAbilitySystemComponent*, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle)
				{
					UpdateResistances(Spec, false);
				}
			);
		}
	}
}

bool UElementHealthAttributeSet::GetResistanceByTag(
	const FGameplayTag& DamageType,
	float& OutDamagePercent,
	float& OutDamageFixed,
	float& OutHealPercent,
	float& OutHealFixed
) const
{
	if (const FResistance* Resistance = DamageResistances.Find(DamageType))
	{
		OutDamagePercent = Resistance->DamageResistancePercent;
		OutDamageFixed = Resistance->DamageResistanceFixed;
		OutHealPercent = Resistance->HealPercent;
		OutHealFixed = Resistance->HealFixed;

		return true;
	}

	OutDamagePercent = 0;
	OutDamageFixed = 0;
	OutHealPercent = 0;
	OutHealFixed = 0;
	return false;
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

	if (Attribute == GetHealthAttribute())
	{
		if (OldValue > 0.0f && NewValue <= 0.0f)
		{
			if (LastDamageTaken)
			{
				OnDeath.Broadcast(GetOwningActor(), *LastDamageTaken);
			}
			else
			{
				const FDamageTaken DamageTaken(OldValue - NewValue, false, FGameplayTag::EmptyTag);
				OnDeath.Broadcast(GetOwningActor(), DamageTaken);
			}
		}

		LastDamageTaken = nullptr;
	}

	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool IsElementalDamageEffectExecution(const FGameplayEffectExecutionDefinition& Def)
{
	return Def.CalculationClass->IsChildOf(UElementDamageExecution::StaticClass());
}

void UElementHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectSpec& Spec = Data.EffectSpec;
	const TObjectPtr<const UGameplayEffect>& Def = Spec.Def;

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (GetHealth() > 0.0f)
		{
			// ElementalEffectExecution will calculate resistances and apply damage
			if (!Def->Executions.IsEmpty() &&
				Def->Executions.ContainsByPredicate(IsElementalDamageEffectExecution))
			{
				SetDamage(0.0f);
				return;
			}

			const float DamageValue = GetDamage();
			const float OldHealthValue = GetHealth();
			const float MaxHealthValue = GetMaxHealth();
			const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);

			const FGameplayEffectContextHandle& Context = Spec.GetContext();
			OnDamageTaken.Broadcast(FDamageTaken(
				DamageValue,
				false,
				FGameplayTag::EmptyTag,
				Context.GetOriginalInstigator(),
				Context.GetEffectCauser()
			));

			if (OldHealthValue != NewHealthValue)
				SetHealth(NewHealthValue);
		}

		SetDamage(0.0f);
	}
}

void UElementHealthAttributeSet::SetLastDamageTaken(const FDamageTaken& DamageTaken)
{
	LastDamageTaken = MakeUnique<FDamageTaken>(DamageTaken);
}
