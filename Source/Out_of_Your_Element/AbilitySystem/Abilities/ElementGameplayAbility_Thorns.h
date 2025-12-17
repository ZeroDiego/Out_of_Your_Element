#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbility_Thorns.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Thorns : public UElementGameplayAbilitySpellBase
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thorns")
	TSubclassOf<class AElementThorn> AThornClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="Thorns Level 2")
	float AdditionalLength = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Degrees"), Category="Thorns Level 2")
	float AdditionalSpread = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Thorns Level 2")
	int AdditionalThornCount = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Thorns Level 2")
	float DamageMultiplier = 0.75f;

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
