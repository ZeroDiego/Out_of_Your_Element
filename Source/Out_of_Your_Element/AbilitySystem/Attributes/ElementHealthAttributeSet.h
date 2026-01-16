#pragma once

#include "CoreMinimal.h"
#include "ElementAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Out_of_Your_Element/AbilitySystem/Executions/ElementDamageExecution.h"
#include "ElementHealthAttributeSet.generated.h"

USTRUCT(BlueprintType)
struct FDamageTaken
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasElement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Element = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator = nullptr; // Example: Player

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Cause = nullptr; // Example: Fireball
};

USTRUCT(BlueprintType)
struct FResistance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageResistancePercent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageResistanceFixed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealPercent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealFixed = 0;

	friend bool operator==(const FResistance& Lhs, const FResistance& RHS)
	{
		return Lhs.DamageResistancePercent == RHS.DamageResistancePercent
			&& Lhs.DamageResistanceFixed == RHS.DamageResistanceFixed
			&& Lhs.HealPercent == RHS.HealPercent
			&& Lhs.HealFixed == RHS.HealFixed;
	}

	friend bool operator!=(const FResistance& Lhs, const FResistance& RHS)
	{
		return !(Lhs == RHS);
	}

	bool IsEmpty() const
	{
		return DamageResistancePercent == 0 &&
			DamageResistanceFixed == 0 &&
			HealPercent == 0 &&
			HealFixed == 0;
	};
};

DECLARE_MULTICAST_DELEGATE_FourParams(
	FResistanceChangedEvent,
	class UElementHealthAttributeSet*,
	const FGameplayTag& DamageType,
	const FResistance&,
	const FResistance&
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FDamageTakenEvent,
	const FDamageTaken&, DamageTaken
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FDeathEvent,
	AActor*, DyingActor,
	const FDamageTaken&, DamageTaken
);

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementHealthAttributeSet : public UElementAttributeSet
{
	GENERATED_BODY()

	friend void UElementDamageExecution::Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters&,
		FGameplayEffectCustomExecutionOutput&
	) const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageResistance;

	// Evil hackery
	TUniquePtr<FDamageTaken> LastDamageTaken;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, Damage);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UElementHealthAttributeSet, DamageResistance);

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FAttributeChangedEvent OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FAttributeChangedEvent OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FDamageTakenEvent OnDamageTaken;

	FResistanceChangedEvent OnResistanceChanged;

private:
	TMap<FGameplayTag, FResistance> DamageResistances;

public:
	UElementHealthAttributeSet();

	bool GetResistanceByTag(
		const FGameplayTag& DamageType,
		OUT float& OutDamagePercent,
		OUT float& OutDamageFixed,
		OUT float& OutHealPercent,
		OUT float& OutHealFixed
	) const;

	UFUNCTION(BlueprintCallable, DisplayName="GetResistanceByTag")
	FResistance BP_GetResistanceByTag(const FGameplayTag& DamageType) const;

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void NotifyDamageTaken(const FDamageTaken& DamageTaken);

	void UpdateResistances(const FGameplayEffectSpec& Spec, bool Removed);
};
