// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacter.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbilityRangedSpellBase.h"
#include "Out_of_Your_Element/System/ElementGameInstance.h"

AElementCharacter::AElementCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = TargetArmLength;
	CameraBoom->SetRelativeRotation(CameraRotation);
	CameraBoom->bDoCollisionTest = false;

	CameraRef = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraRef->bUsePawnControlRotation = false;
	CameraRef->SetupAttachment(CameraBoom);

	AimMarker = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AimMarker"));
	AimMarker->SetupAttachment(RootComponent);
	AimMarker->SetAutoActivate(false);
}

bool AElementCharacter::IsCastingSpell() const
{
	if (!ElementAbilitySystemComponent)
		return false;

	TArray<UElementGameplayAbilitySpellBase*> ActiveSpells;
	ElementAbilitySystemComponent->GetActiveAbilitiesWithTags(
		FGameplayTagContainer(ElementGameplayTags::Abilities_Casting),
		ActiveSpells
	);

	return !ActiveSpells.IsEmpty();
}

bool AElementCharacter::CanAttack() const
{
	return GetWorld() && !UGameplayStatics::IsGamePaused(GetWorld()) && IsAlive() && !IsCastingSpell();
}

void AElementCharacter::GiveXP(const FGameplayTag& Element, int XP)
{
	const UWorld* World = GetWorld();
	if (!World)
		return;

	if (UElementGameInstance* Egi = World->GetGameInstance<UElementGameInstance>())
	{
		Egi->GlobalVariables.AddInt(TEXT("Stats.XP Gained"), XP);
	}

	if (const FLevelUpData* LevelUpData = ElementLevelUpMap.Find(Element))
	{
		FExperience& Experience = ElementXPMap.FindOrAdd(Element);
		const FExperience OldExperience = FExperience(Experience);
		auto& [Current, CurrentLevel] = Experience;

		const TArray<FLevelData>& Levels = LevelUpData->Levels;
		const int AvailableLevels = Levels.Num();

		while (
			CurrentLevel < AvailableLevels &&
			XP > 0
		)
		{
			const auto& LevelData = Levels[CurrentLevel];
			const auto& [RequiredXPForLevelUp, AbilityToUnlock] = LevelData;
			const int RemainingXP = RequiredXPForLevelUp - Current;
			const int XPToObtain = FMath::Min(XP, RemainingXP);

			XP -= XPToObtain;
			Current = (Current + XPToObtain) % RequiredXPForLevelUp;

			if (OnExperienceChangedDelegate.IsBound())
				OnExperienceChangedDelegate.Broadcast(Element, OldExperience, Experience, LevelData);

			if (Current == 0)
			{
				++CurrentLevel;
				GetAbilitySystemComponent()->K2_GiveAbility(AbilityToUnlock);
				if (OnLevelUpDelegate.IsBound())
					OnLevelUpDelegate.Broadcast(Element, Experience, LevelData);

				if (UElementGameInstance* Egi = World->GetGameInstance<UElementGameInstance>())
				{
					Egi->GlobalVariables.AddInt(TEXT("Stats.TotalLevel"), XP);
				}
			}
		}
	}
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

	DoCycleElement(0);
	MouseLook();
}

void AElementCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController->IsLocalPlayerController())
	{
		if (APlayerController* CurrentController = Cast<APlayerController>(GetController()))
		{
			if (CursorWidgetClass && !CursorWidgetRef)
				CursorWidgetRef = CreateWidget(CurrentController, CursorWidgetClass, TEXT("Cursor"));
		}

		// Delay so world is fully loaded and traceable.
		// TODO Find a better solution!!!
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateWeakLambda(this, [this]
			{
				MouseLook();

				if (CursorWidgetRef)
					CursorWidgetRef->AddToViewport(1);

				if (AimMarker)
					AimMarker->Activate();
			}),
			1.0f,
			false
		);
	}
}

void AElementCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (CursorWidgetRef)
		CursorWidgetRef->RemoveFromParent();

	if (AimMarker)
		AimMarker->DeactivateImmediate();
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
			SelectFireElementAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::SelectFireElement
		);

		EnhancedInputComponent->BindAction(
			SelectNatureElementAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::SelectNatureElement
		);

		EnhancedInputComponent->BindAction(
			SelectWaterElementAction,
			ETriggerEvent::Triggered,
			this,
			&AElementCharacter::SelectWaterElement
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

void AElementCharacter::OnDeath(AActor* DyingActor, const FDamageTaken& DamageTaken)
{
	Super::OnDeath(DyingActor, DamageTaken);

	if (CursorWidgetRef)
		CursorWidgetRef->RemoveFromParent();

	if (AimMarker)
		AimMarker->DeactivateImmediate();

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
}

void AElementCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AElementCharacter::MouseLook()
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

			if (FHitResult SpellHitResult; UElementGameplayAbilityRangedSpellBase::TraceSpell(this, SpellHitResult))
			{
				const FVector& SpellLocation = SpellHitResult.ImpactPoint;
				const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(
					GetActorLocation(), SpellLocation
				);

				FRotator CurrentRotation = GetActorRotation();
				CurrentRotation.Yaw = LookRotation.Yaw;
				SetActorRotation(CurrentRotation);

				if (AimMarker)
				{
					if (UElementGameplayAbilityRangedSpellBase::CanPlace(SpellHitResult))
					{
						AimMarker->Activate();
						AimMarker->SetWorldLocation(SpellLocation);

						const auto MarkerRotation = FRotationMatrix::MakeFromZ(SpellHitResult.ImpactNormal).Rotator();
						AimMarker->SetWorldRotation(MarkerRotation);
					}
					else
					{
						AimMarker->DeactivateImmediate();
					}
				}
			}
			else
			{
				if (AimMarker)
				{
					AimMarker->DeactivateImmediate();
				}
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

void AElementCharacter::DoAttack(const TSubclassOf<UGameplayAbility>& Attack) const
{
	const UWorld* World = GetWorld();
	if (!World)
		return;

	if (!Attack)
		return;

	if (!CanAttack())
		return;

	if (ElementAbilitySystemComponent->TryActivateAbilityByClass(Attack))
	{
		const UGameplayAbility* CDO = Attack->GetDefaultObject<UGameplayAbility>();

		float MaxDuration = 0.0f;
		if (const FGameplayTagContainer* CooldownTags = CDO->GetCooldownTags())
		{
			const FGameplayEffectQuery CooldownQuery =
				FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags);

			const TArray<float> AbilityDurations =
				ElementAbilitySystemComponent->GetActiveEffectsDuration(CooldownQuery);

			for (const float Duration : AbilityDurations)
			{
				if (Duration > MaxDuration)
					MaxDuration = Duration;
			}
		}

		OnAttackDelegate.Broadcast(FAttackData{
			.Element = ActiveElement,
			.Ability = Attack,
			.Cooldown = MaxDuration
		});

		if (UElementGameInstance* Egi = World->GetGameInstance<UElementGameInstance>())
		{
			Egi->GlobalVariables.AddInt(TEXT("Stats.Abilities.Total"), 1);

			const FString AbilityId = Attack->GetName();
			const FString PerAbilityKey = FString::Printf(TEXT("Stats.Abilities.%s.Uses"), *AbilityId);

			Egi->GlobalVariables.AddInt(PerAbilityKey, 1);
		}
	}
}

void AElementCharacter::StartBaseAttack()
{
	DoAttack(ActiveElement.BaseAttackAbility);
}

void AElementCharacter::StartHeavyAttack()
{
	DoAttack(ActiveElement.HeavyAttackAbility);
}

void AElementCharacter::StartSpecialAttack()
{
	DoAttack(ActiveElement.SpecialAttackAbility);
}

void AElementCharacter::DoCycleElement(const int Amount)
{
	if (Elements.IsEmpty())
		return;

	ActiveElementIndex = (ActiveElementIndex + Amount) % Elements.Num();
	if (ActiveElementIndex < 0)
		ActiveElementIndex += Elements.Num();

	const FElement& OldElement = ActiveElement;
	ActiveElement = Elements[ActiveElementIndex];

	if (OnElementChangedDelegate.IsBound())
		OnElementChangedDelegate.Broadcast(OldElement, ActiveElement, Amount);
}

void AElementCharacter::SelectElement(const int Index)
{
	ActiveElementIndex = Index % Elements.Num();
	if (ActiveElementIndex < 0)
		ActiveElementIndex += Elements.Num();

	const FElement& OldElement = ActiveElement;
	ActiveElement = Elements[ActiveElementIndex];

	if (OnElementChangedDelegate.IsBound())
		OnElementChangedDelegate.Broadcast(OldElement, ActiveElement, 0);
}

void AElementCharacter::SelectFireElement()
{
	SelectElement(0);
}

void AElementCharacter::SelectNatureElement()
{
	SelectElement(1);
}

void AElementCharacter::SelectWaterElement()
{
	SelectElement(2);
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
