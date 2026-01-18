#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbility_Freeze.generated.h"

class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Freeze : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="Freeze")
	float FreezeRadius = 500;

	UPROPERTY(EditDefaultsOnly, Category="Freeze")
	TSubclassOf<UGameplayEffect> FreezeGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Freeze")
	UNiagaraSystem* FreezeActivateParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Centimeters"), Category="Freeze Level 2")
	float AdditionalFreezeRadius = 250;

	UPROPERTY(EditDefaultsOnly, Category="Freeze Level 2")
	UNiagaraSystem* LargeFreezeActivateParticle;

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
