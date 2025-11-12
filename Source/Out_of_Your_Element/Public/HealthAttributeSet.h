#pragma once

#include "CoreMinimal.h"
#include "ElementAttributeSet.h"
#include "HealthAttributeSet.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UHealthAttributeSet : public UElementAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttributeSet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);
	
	ATTRIBUTE_EVENT(Health);
	ATTRIBUTE_EVENT(MaxHealth);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};
