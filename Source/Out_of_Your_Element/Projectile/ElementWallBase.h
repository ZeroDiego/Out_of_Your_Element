// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "ElementWallBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementWallBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	float DefaultHealth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn), Category = "Effects")
	TMap<TSubclassOf<UGameplayEffect>, FDefaultGameplayEffectTags> DefaultGameplayEffects;

	UPROPERTY(BlueprintReadWrite)
	const class AElementCharacterBase* Caster;

public:
	AElementWallBase();

	UFUNCTION(BlueprintCallable)
	void DoDamage() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ElementAbilitySystemComponent;
	}

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChangeEvent(UAttributeSet* AttributeSet, float OldValue, float NewValue);
};
