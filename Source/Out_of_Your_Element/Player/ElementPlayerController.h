// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "ElementPlayerController.generated.h"

UENUM(BlueprintType)
enum class EInputDeviceType : uint8
{
    KeyboardMouse UMETA(DisplayName = "Keyboard & Mouse"),
    Gamepad       UMETA(DisplayName = "Gamepad")
};

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual bool InputKey(const FInputKeyEventArgs& Params) override;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    EInputDeviceType CurrentInputDevice = EInputDeviceType::KeyboardMouse;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bUsingGamepad = false;

protected:
	UPROPERTY(EditAnywhere, Category ="Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;

    
protected:
	UFUNCTION(BlueprintCallable)
	virtual void SetupInputComponent() override;
};
