// Fill out your copyright notice in the Description page of Project Settings.


#include "DiegoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

ADiegoPlayerController::ADiegoPlayerController()
{
	if (UInputMappingContext* InputMappingContext = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, TEXT("/Game/Characters/DiegoCharacter/IMC_DiegoCharacter"))))
	{
		DiegoMappingContext = InputMappingContext;
	}
	if (UInputAction* VerticalInputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Characters/DiegoCharacter/IA_MovementVertical"))))
	{
		MovementVerticalAction = VerticalInputAction;
	}
	if (UInputAction* HorizontalInputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Characters/DiegoCharacter/IA_MovementHorizontal"))))
	{
		MovementHorizontalAction = HorizontalInputAction;
	}

	SetShowMouseCursor(true);
}

void ADiegoPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADiegoPlayerController::SetupInputComponent()
{
	// Set up gameplay key bindings
	Super::SetupInputComponent();
	
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DiegoMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup Movement
		EnhancedInputComponent->BindAction(MovementHorizontalAction, ETriggerEvent::Triggered, this, &ADiegoPlayerController::MovementHorizontal);
		EnhancedInputComponent->BindAction(MovementVerticalAction, ETriggerEvent::Triggered, this, &ADiegoPlayerController::MovementVertical);
			
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Get Y value of vector
void ADiegoPlayerController::MovementHorizontal(const FInputActionValue& Value)
{
	InputVector.Y = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Get X value of vector
void ADiegoPlayerController::MovementVertical(const FInputActionValue& Value)
{
	InputVector.X = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Apply both X and Y values to and place the vector to player character
void ADiegoPlayerController::ApplyMovement()
{
	//Normalize input so diagonal input is not hypotenuse from input
	FVector2D NormalizedInput = InputVector.GetSafeNormal();

	// Get rotation
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// Y input
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (GetCharacter())
	{
		GetCharacter()->AddMovementInput(RightDirection, NormalizedInput.Y);
	}

	// X input
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	if (GetCharacter())
	{
		GetCharacter()->AddMovementInput(ForwardDirection, NormalizedInput.X);
	}

	// reset vector at the end
	InputVector = FVector2D::ZeroVector;
}
