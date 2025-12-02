#pragma once

#include "ElementGameplayAbilityBase.h"
#include "ElementGameplayAbility_Freeze.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Freeze : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze")
	float FreezeRadius = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze")
	TSubclassOf<UGameplayEffect> FreezeGameplayEffectClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
