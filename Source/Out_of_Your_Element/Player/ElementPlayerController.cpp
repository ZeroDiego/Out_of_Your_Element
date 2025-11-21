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
