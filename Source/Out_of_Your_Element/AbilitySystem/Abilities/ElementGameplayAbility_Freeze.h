#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbility_Freeze.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Freeze : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze")
	float FreezeRadius = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze")
	TSubclassOf<UGameplayEffect> FreezeGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze")
	class UNiagaraSystem* FreezeActivateParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Freeze Level 2")
	float AdditionalFreezeRadius = 250;

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
