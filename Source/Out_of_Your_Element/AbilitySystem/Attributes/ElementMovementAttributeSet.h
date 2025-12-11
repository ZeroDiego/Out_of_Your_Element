#pragma once

#include "AbilitySystemComponent.h"
#include "ElementAttributeSet.h"
#include "ElementMovementAttributeSet.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementMovementAttributeSet : public UElementAttributeSet
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UElementMovementAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintAssignable, Meta = (HideFromModifiers))
	FAttributeChangedEvent OnMovementSpeedChanged;

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};
