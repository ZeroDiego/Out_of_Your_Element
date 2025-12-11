// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "ElementCharacterBase.generated.h"

UCLASS(Blueprintable)
class OUT_OF_YOUR_ELEMENT_API AElementCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementMovementAttributeSet> MovementAttributeSet;

public:
	AElementCharacterBase();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float IsAlive() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ElementAbilitySystemComponent;
	}

protected:
	virtual void PostInitializeComponents() override;
};
