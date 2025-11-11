// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Fireball.h"
#include "GameFramework/Character.h"

AElementPlayerController::AElementPlayerController()
{
	if (UInputMappingContext* InputMappingContext = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, TEXT("/Game/Characters/IMC_DiegoCharacter"))))
	{
		DiegoMappingContext = InputMappingContext;
	}
	if (UInputAction* VerticalInputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Characters/IA_MovementVertical"))))
	{
		MovementVerticalAction = VerticalInputAction;
	}
	
	UInputAction* HorizontalInputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Characters/IA_MovementHorizontal")));
	MovementHorizontalAction = HorizontalInputAction;
		
	if (UInputAction* BaseAttackInputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Characters/IA_BaseAttack"))))
	{
		BaseAttackAction = BaseAttackInputAction;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: BaseInputAction null"));
	}

	// Creates an ability system component in BP_DiegoCharacter
	DiegoAbilitySystemComponent = CreateDefaultSubobject<UElementAbilitySystemComponent>(TEXT("DiegoAbilitySystemComponent"));

	// Show mouse cursor during gameplay
	SetShowMouseCursor(true);
}

void AElementPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AElementPlayerController::SetupInputComponent()
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
		EnhancedInputComponent->BindAction(MovementHorizontalAction, ETriggerEvent::Triggered, this, &AElementPlayerController::MovementHorizontal);
		EnhancedInputComponent->BindAction(MovementVerticalAction, ETriggerEvent::Triggered, this, &AElementPlayerController::MovementVertical);

		// Setup Fireball
		EnhancedInputComponent->BindAction(BaseAttackAction, ETriggerEvent::Triggered, this, &AElementPlayerController::BaseAttack);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Get Y value of vector
void AElementPlayerController::MovementHorizontal(const FInputActionValue& Value)
{
	InputVector.Y = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Get X value of vector
void AElementPlayerController::MovementVertical(const FInputActionValue& Value)
{
	InputVector.X = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Base Attack
void AElementPlayerController::BaseAttack(const FInputActionValue& Value)
{
	if (DiegoAbilitySystemComponent)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent not found!!!"));
	}
}

void AElementPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DiegoAbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
		{
			if (Ability)
			{
				DiegoAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
			}
		}
	}
}

// Apply both X and Y values to and place the vector to player character
void AElementPlayerController::ApplyMovement()
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
