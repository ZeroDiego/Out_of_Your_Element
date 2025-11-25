// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void AElementPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (const UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				FModifyContextOptions Opts = {};
				Opts.bNotifyUserSettings = true;
				Subsystem->AddMappingContext(CurrentContext, 0, Opts);
			}
		}
	}
}

bool AElementPlayerController::InputKey(const FInputKeyEventArgs& Params)
{
	const bool bHandledBySuper = Super::InputKey(Params);

	const EInputEvent EventType = Params.Event;
	if (EventType == IE_Pressed ||
		EventType == IE_Repeat ||
		EventType == IE_Released)
	{
		const bool bIsGamepad = Params.IsGamepad();

		if (bIsGamepad)
		{
			CurrentInputDevice = EInputDeviceType::Gamepad;
			bUsingGamepad      = true;
		}
		else
		{
			CurrentInputDevice = EInputDeviceType::KeyboardMouse;
			bUsingGamepad      = false;
		}
	}

	return bHandledBySuper;
}