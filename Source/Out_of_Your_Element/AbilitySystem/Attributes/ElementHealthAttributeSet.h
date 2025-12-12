#pragma once

#include "CoreMinimal.h"
#include "ElementAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ElementHealthAttributeSet.generated.h"

USTRUCT(BlueprintType)
struct FDamageTaken
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Element;
};

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

public:
	UElementHealthAttributeSet();

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UFUNCTION()
	void SetLastDamageTaken(const FDamageTaken& DamageTaken);
};
