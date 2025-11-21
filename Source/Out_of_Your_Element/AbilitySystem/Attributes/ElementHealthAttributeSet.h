#pragma once

#include "CoreMinimal.h"
#include "ElementAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ElementHealthAttributeSet.generated.h"

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

public:
	UElementHealthAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UElementHealthAttributeSet, Damage);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UElementHealthAttributeSet, DamageResistance);

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FAttributeChangedEvent OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FAttributeChangedEvent OnMaxHealthChanged;

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
