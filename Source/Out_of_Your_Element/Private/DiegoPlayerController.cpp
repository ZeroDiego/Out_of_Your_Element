// Fill out your copyright notice in the Description page of Project Settings.


#include "DiegoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void ADiegoPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RotatePlayerTowardsMouse();
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

void ADiegoPlayerController::RotatePlayerTowardsJoystick(const FInputActionValue& Value) const
{
	ACharacter* CurrentChar = this->GetCharacter();
	if (!CurrentChar) return;

	const float XValue = Value.Get<FVector2D>().X;
	const float YValue = Value.Get<FVector2D>().Y;

	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(FVector(0.0, 0.0, 0.0), FVector(XValue, YValue, 0.0));
	
	CurrentChar->SetActorRotation(NewRot);
}

void ADiegoPlayerController::RotatePlayerTowardsMouse() const
{
	if (FVector MouseLocation, MouseDirection; this->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
	{
		// Get local reference to the controller's character
		ACharacter* CurrentChar = this->GetCharacter();
		if (!CurrentChar) return;

		FVector CharacterLocation = CurrentChar->GetActorLocation();

		// Ray trace to find the intersection with the plane the character is on
		FVector Start = MouseLocation;
		FVector End = MouseLocation + MouseDirection * 10000.0f;  // Extend the ray

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(CurrentChar);
        
		if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, Params))
		{
			// Ensure the hit location is on the same plane as the character
			FVector HitLocation = HitResult.Location;
			FVector CharacterPlaneLocation = FVector(HitLocation.X, HitLocation.Y, CharacterLocation.Z);

			FVector Direction = CharacterPlaneLocation - CharacterLocation;

			// Calculate the new rotation
			FRotator TargetRotation = Direction.Rotation();
			FRotator CharRotation = CurrentChar->GetActorRotation();
			FRotator NewRot = FRotator(CharRotation.Pitch, TargetRotation.Yaw, CharRotation.Roll);

			// Set the new rotation
			if(bShouldRotateTowardsMouse)
			{
				CurrentChar->SetActorRotation(NewRot);
			}
		}
	}
}
