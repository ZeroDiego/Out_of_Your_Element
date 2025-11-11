// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementAbilitySystemComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "ElementPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AElementPlayerController();

	// Input Mapping Context
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DiegoMappingContext;

	// Input Action Movement Vertical
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementVerticalAction;

	// Input Action Movement Horizontal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementHorizontalAction;

	// Input Action Movement Horizontal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* BaseAttackAction;

	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	UElementAbilitySystemComponent* DiegoAbilitySystemComponent;

	// Abilities array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ApplyMovement();

	void BaseAttack(const FInputActionValue& Value);

	void MovementVertical(const FInputActionValue& Value);
	
	void MovementHorizontal(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;
	
	virtual void Tick(const float DeltaSeconds) override;

private:
	FVector2D InputVector = FVector2D::ZeroVector;

	bool bShouldRotateTowardsMouse = true;
};
