#pragma once

#include "ElementGameplayAbilityBase.h"
#include "ElementGameplayAbility_Thorns.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Thorns : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="Thorns")
	float Length = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Degrees"), Category="Thorns")
	float Spread = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="Thorns")
	float MinSpacing = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Thorns")
	int ThornCount = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Thorns")
	float ThornLifespan = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AElementThorn> AThornClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
