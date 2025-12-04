// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "ElementCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireDamageTaken, const int32, NewTagCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaterDamageTaken, const int32, NewTagCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNatureDamageTaken, const int32, NewTagCount);

UCLASS(Blueprintable)
class OUT_OF_YOUR_ELEMENT_API AElementCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	UPROPERTY(BlueprintAssignable)
	FOnFireDamageTaken OnFireDamageTakenDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnWaterDamageTaken OnWaterDamageTakenDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnNatureDamageTaken OnNatureDamageTakenDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementHealthAttributeSet> HealthAttributeSet;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementMovementAttributeSet> MovementAttributeSet;

public:
	AElementCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ElementAbilitySystemComponent;
	}

protected:
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

	void FireDamageHandler(FGameplayTag Tag, const int32 NewCount) const;

	void WaterDamageHandler(FGameplayTag Tag, const int32 NewCount) const;

	void NatureDamageHandler(FGameplayTag Tag, const int32 NewCount) const;
};
