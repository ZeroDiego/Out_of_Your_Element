// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacter.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Out_of_Your_Element/Animation/ElementAnimNotify.h"

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

	InitAnimations();

	DoCycleElement(0);
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
			&AElementCharacter::StartBaseAttack
		);

		EnhancedInputComponent->BindAction(
			HeavyAttackAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::StartHeavyAttack
		);

		EnhancedInputComponent->BindAction(
			SpecialAttackAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::StartSpecialAttack
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

			static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
				UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2),
			};

			if (FHitResult HitResult; CurrentController->GetHitResultUnderCursorForObjects(
				GroundTypes, false, HitResult))
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

void AElementCharacter::DoAttack(const TSubclassOf<UGameplayAbility>& Attack)
{
	ElementAbilitySystemComponent->TryActivateAbilityByClass(Attack);
}

void AElementCharacter::StartBaseAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& BaseAttack = ActiveElement.BaseAttackAbility;

	if (!BaseAttack)
		return;

	const UGameplayEffect* CooldownEffect = BaseAttack.GetDefaultObject()->GetCooldownGameplayEffect();

	if (!CooldownEffect)
		return;

	FGameplayEffectSpec TempSpec(CooldownEffect, ElementAbilitySystemComponent->MakeEffectContext(), 1);

	OnAttackDelegate.Broadcast(FAttackData{
		.Element = ActiveElement,
		.Ability = BaseAttack,
		.Cooldown = TempSpec.GetDuration()
	});

	AbilityToUseOnDoAttack = EAttackType::BaseAttack;

	PlayAnimMontage(BaseAttackMontage);
}

void AElementCharacter::StartHeavyAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& HeavyAttack = ActiveElement.HeavyAttackAbility;

	if (!HeavyAttack)
		return;

	const UGameplayEffect* CooldownEffect = HeavyAttack.GetDefaultObject()->GetCooldownGameplayEffect();

	if (!CooldownEffect)
		return;

	FGameplayEffectSpec TempSpec(CooldownEffect, ElementAbilitySystemComponent->MakeEffectContext(), 1);

	OnAttackDelegate.Broadcast(FAttackData{
		.Element = ActiveElement,
		.Ability = HeavyAttack,
		.Cooldown = TempSpec.GetDuration()
	});

	AbilityToUseOnDoAttack = EAttackType::HeavyAttack;

	PlayAnimMontage(HeavyAttackMontage);
}

void AElementCharacter::StartSpecialAttack()
{
	if (!ElementAbilitySystemComponent)
		return;

	const TSubclassOf<UGameplayAbility>& SpecialAttack = ActiveElement.SpecialAttackAbility;

	if (!SpecialAttack)
		return;

	const UGameplayEffect* CooldownEffect = SpecialAttack.GetDefaultObject()->GetCooldownGameplayEffect();

	if (!CooldownEffect)
		return;

	FGameplayEffectSpec TempSpec(CooldownEffect, ElementAbilitySystemComponent->MakeEffectContext(), 1);

	OnAttackDelegate.Broadcast(FAttackData{
		.Element = ActiveElement,
		.Ability = SpecialAttack,
		.Cooldown = TempSpec.GetDuration()
	});

	AbilityToUseOnDoAttack = EAttackType::SpecialAttack;

	PlayAnimMontage(SpecialAttackMontage);
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
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw = FMath::Fmod(Rotation.Yaw + Yaw, 360);
		SetActorRotation(Rotation);
	}
}

void AElementCharacter::InitAnimations()
{
	if (BaseAttackMontage)
	{
		const auto NotifyEvents = BaseAttackMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto BaseAttackNotify = Cast<UElementAnimNotify>(EventNotify.Notify))
			{
				BaseAttackNotify->OnNotified.AddUObject(this, &AElementCharacter::OnElementAnimNotify);
			}
		}
	}
}

void AElementCharacter::OnElementAnimNotify(const EAnimNotifyType NotifyType)
{
	DoAttack(ActiveElement.BaseAttackAbility);

	switch (NotifyType)
	{
	case EAnimNotifyType::AttackStart:
		switch (AbilityToUseOnDoAttack)
			case EAttackType::BaseAttack:
				DoAttack(ActiveElement.BaseAttackAbility);
			break;
			case EAttackType::SpecialAttack:
				DoAttack(ActiveElement.SpecialAttackAbility);
			break;
			case EAttackType::HeavyAttack:
				DoAttack(ActiveElement.HeavyAttackAbility);
			break;
		break;
	case EAnimNotifyType::AttackEnd:
		break;
	}
}
