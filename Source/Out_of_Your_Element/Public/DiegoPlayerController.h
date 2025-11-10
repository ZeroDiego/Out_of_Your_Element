// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "DiegoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API ADiegoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADiegoPlayerController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DiegoMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementVerticalAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementHorizontalAction;

protected:
	void ApplyMovement();

	void MovementVertical(const FInputActionValue& Value);
	
	void MovementHorizontal(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;
	
	virtual void Tick(const float DeltaSeconds) override;

private:
	FVector2D InputVector = FVector2D::ZeroVector;

	bool bShouldRotateTowardsMouse = true;
};
