// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "ElementPlayerController.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category ="Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;

protected:
	virtual void SetupInputComponent() override;
};
