// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "ElementCharacterBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraComponent* FireDotNiagaraComponent;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* FireDotVfx;

	// Sets default values for this character's properties
	AElementCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ElementAbilitySystemComponent;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void FireDamageHandler(FGameplayTag Tag, const int32 NewCount) const;

	void WaterDamageHandler(FGameplayTag Tag, const int32 NewCount) const;

	void NatureDamageHandler(FGameplayTag Tag, const int32 NewCount) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
