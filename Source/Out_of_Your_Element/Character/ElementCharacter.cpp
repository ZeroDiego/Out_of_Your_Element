// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacter.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayAbilitiesModule.h"
#include "AbilitySystemGlobals.h"

AElementCharacter::AElementCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoomRef = CreateDefaultSubobject<USpringArmComponent>(FName("Camera Boom"));

	CameraBoomRef->SetupAttachment(RootComponent);
	CameraBoomRef->SetUsingAbsoluteRotation(true);
	CameraBoomRef->TargetArmLength = TargetArmLength;
	CameraBoomRef->SetRelativeRotation(CameraRotation);
	CameraBoomRef->bDoCollisionTest = false;

	CameraRef = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));

	CameraRef->SetupAttachment(CameraBoomRef);
	CameraRef->bUsePawnControlRotation = false;

	// Creates a custom scene component called firing offset used for projectile spawn location
	FiringOffsetRef = CreateDefaultSubobject<UElementFiringOffset>(TEXT("FiringOffset"));
	FiringOffsetRef->SetupAttachment(RootComponent);
	FiringOffsetRef->SetRelativeLocation(FiringOffset);

	ElementAbilitySystemComponent =
		CreateDefaultSubobject<UElementAbilitySystemComponent>(TEXT("ElementAbilitySystemComponent"));
	HealthAttributeSet = CreateDefaultSubobject<UElementHealthAttributeSet>(TEXT("Health Attribute Set"));

	OnActorBeginOverlap.AddDynamic(this, &AElementCharacter::OnActorOverlap);
}

void AElementCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AActor* Owner = this;
	if (const APlayerState* CurrentPlayerState = GetPlayerState())
		if (APlayerController* PlayerController = CurrentPlayerState->GetPlayerController())
			Owner = PlayerController;

	ElementAbilitySystemComponent->InitAbilityActorInfo(Owner, this);

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(
		GetAbilitySystemComponent(),
		*GetClass()->GetName(),
		1,
		true
	);

	HealthAttributeSet->InitHealth(HealthAttributeSet->GetMaxHealth());
}

void AElementCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UGameplayAbility>& Ability : UsableAbilities)
	{
		if (Ability)
		{
			ElementAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		}
	}

	if (CursorWidgetClass)
	{
		if (APlayerController* CurrentController = Cast<APlayerController>(GetController()))
		{
			if (CurrentController->IsLocalController())
			{
				CursorWidgetRef = CreateWidget(CurrentController, CursorWidgetClass, TEXT("Cursor"));
				FVector2D CursorPosition;
				CurrentController->GetMousePosition(CursorPosition.X, CursorPosition.Y);
				CursorWidgetRef->SetPositionInViewport(CursorPosition);
				CursorWidgetRef->AddToPlayerScreen();
			}
		}
	}

	DoCycleElement(0);
}

void AElementCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AElementCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// ReSharper disable once CppMemberFunctionMayBeConst -- cannot be const, will break add unique dynamic
void AElementCharacter::OnInputMethodChange(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	if (const UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
	{
		const FHardwareDeviceIdentifier InputDevice = InputDeviceSubsystem->GetInputDeviceHardwareIdentifier(DeviceId);

		if (!InputDevice.IsValid())
			return;

		if (InputDevice.PrimaryDeviceType == EHardwareDevicePrimaryType::KeyboardAndMouse)
		{
			if (CursorWidgetRef && !CursorWidgetRef->IsVisible())
			{
				if (const APlayerController* CurrentController = Cast<APlayerController>(GetController()))
				{
					FVector2D CursorPosition;
					CurrentController->GetMousePosition(CursorPosition.X, CursorPosition.Y);
					CursorWidgetRef->SetPositionInViewport(CursorPosition);
				}

				CursorWidgetRef->AddToPlayerScreen();
			}
		}
		else
		{
			if (CursorWidgetRef && CursorWidgetRef->IsVisible())
			{
				CursorWidgetRef->RemoveFromParent();
			}
		}
	}
}

void AElementCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
		{
			InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddUniqueDynamic(
				this, &AElementCharacter::OnInputMethodChange
			);
		}

		EnhancedInputComponent->BindAction(
			BaseAttackAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::DoBaseAttack
		);

		EnhancedInputComponent->BindAction(
			HeavyAttackAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::DoHeavyAttack
		);

		EnhancedInputComponent->BindAction(
			SpecialAttackAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::DoSpecialAttack
		);

		EnhancedInputComponent->BindAction(
			CycleElementAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::CycleElement
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::Move
		);

		EnhancedInputComponent->BindAction(
			MouseLookAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::MouseLook
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::Look
		);
	}
}

void AElementCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AElementCharacter::MouseLook(const FInputActionValue& Value)
{
	if (const APlayerController* CurrentController = Cast<APlayerController>(GetController()))
	{
		if (CurrentController->IsLocalPlayerController())
		{
			if (CursorWidgetRef)
			{
				FVector2D CursorPosition;
				CurrentController->GetMousePosition(CursorPosition.X, CursorPosition.Y);
				CursorWidgetRef->SetPositionInViewport(CursorPosition);
			}

			if (FHitResult HitResult; CurrentController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
			{
				const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(
					GetActorLocation(), HitResult.Location
				);

				FRotator CurrentRotation = GetActorRotation();
				CurrentRotation.Yaw = LookRotation.Yaw;
				SetActorRotation(CurrentRotation);
			}
		}
	}
}

void AElementCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X);
}

void AElementCharacter::CycleElement(const FInputActionValue& Value)
{
	const float In = Value.Get<float>();
	DoCycleElement(In > 0 ? FMath::CeilToInt(In) : FMath::FloorToInt(In));
}

void AElementCharacter::DoBaseAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& BaseAttack = ActiveElement.BaseAttackAbility;

	if (!BaseAttack)
		return;

	for (FGameplayTag Tag : ElementAbilitySystemComponent->GetOwnedGameplayTags())
	{
		if (Tag.IsValid())
		{
			if (Tag.GetTagName() == TEXT("Abilities.BaseAttack") || Tag.GetTagName() == TEXT("Abilities.Water"))
			{
				return;
			}
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	OnAttackDelegate.Broadcast(FAttackData{
		.Element = ActiveElement,
		.Ability = BaseAttack
	});
}

void AElementCharacter::DoBaseAttackHelperFunction(const TSubclassOf<UGameplayAbility>& BaseAttack)
{
	if (ElementAbilitySystemComponent->TryActivateAbilityByClass(BaseAttack))
	{
		const FGameplayEffectContextHandle AnimationDelayBaseAttackGameplayEffectContextHandle;
		ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(AnimationDelayBaseAttackGameplayEffect, 1,
		                                                            AnimationDelayBaseAttackGameplayEffectContextHandle);
	}
}


void AElementCharacter::DoHeavyAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& HeavyAttack = ActiveElement.HeavyAttackAbility;

	if (!HeavyAttack)
		return;

	if (ElementAbilitySystemComponent->TryActivateAbilityByClass(HeavyAttack))
	{
		bIsAttacking = true;
		OnAttackDelegate.Broadcast(FAttackData{
			.Element = ActiveElement,
			.Ability = HeavyAttack
		});
	}
}

void AElementCharacter::DoSpecialAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& SpecialAttack = ActiveElement.SpecialAttackAbility;

	if (!SpecialAttack)
		return;

	if (ElementAbilitySystemComponent->TryActivateAbilityByClass(SpecialAttack))
	{
		bIsAttacking = true;
		OnAttackDelegate.Broadcast(FAttackData{
			.Element = ActiveElement,
			.Ability = SpecialAttack
		});
	}
}

void AElementCharacter::DoCycleElement(const int Amount)
{
	if (Elements.IsEmpty())
		return;

	ActiveElementIndex = (ActiveElementIndex + Amount) % Elements.Num();
	if (ActiveElementIndex < 0)
		ActiveElementIndex = Elements.Num() - 1; // TODO Improve this :sweat_smile:

	const FElement OldElement = ActiveElement;
	ActiveElement = Elements[ActiveElementIndex];
	OnElementChangedDelegate.Broadcast(OldElement, ActiveElement);
}

void AElementCharacter::DoMove(const float Right, const float Forward)
{
	if (const AController* CurrentController = GetController())
	{
		if (CurrentController->IsLocalPlayerController())
		{
			const FRotator Rotation = CurrentController->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FRotationMatrix YawMatrix = FRotationMatrix(YawRotation);
			const FVector ForwardDirection = YawMatrix.GetUnitAxis(EAxis::X);
			const FVector RightDirection = YawMatrix.GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, Forward);
			AddMovementInput(RightDirection, Right);
		}
	}
}

void AElementCharacter::DoLook(const float Yaw)
{
	if (GetController()->IsLocalPlayerController())
	{
		const FRotator CurrentRotation = GetActorRotation();

		const FRotator NewRotation = {
			CurrentRotation.Pitch,
			FMath::Fmod(CurrentRotation.Yaw + Yaw, 360),
			CurrentRotation.Roll
		};

		SetActorRotation(NewRotation);
	}
}

void AElementCharacter::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("AElementCharacter::OnActorOverlap"));

	if (OverlappedActor && OtherActor)
	{
		if (const AElementProjectileBase* ProjectileBase = Cast<AElementProjectileBase>(OtherActor))
		{
			ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(ProjectileBase->GameplayEffectSpecHandle);
			OtherActor->Destroy();
		}
	}
}
