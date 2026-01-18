// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementCharacterBase.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Out_of_Your_Element/AbilitySystem/Element.h"
#include "ElementCharacter.generated.h"

class UInputAction;

UENUM()
enum EAttackType
{
	BaseAttack,
	HeavyAttack,
	SpecialAttack
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FElement Element;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(BlueprintReadOnly)
	float Cooldown = 0.0f;
};

USTRUCT(BlueprintType)
struct FExperience
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Current = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentLevel = 0;
};

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int RequiredXPForLevelUp = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityToUnlock;
};

USTRUCT(BlueprintType)
struct FLevelUpData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelData> Levels;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttack, FAttackData, AttackData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnElementChanged,
	FElement, OldElement,
	FElement, NewElement,
	int, CycleAmount
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnExperienceChanged,
	const FGameplayTag&, Element,
	const FExperience&, OldExperience,
	const FExperience&, NewExperience,
	const FLevelData&, LevelData
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnLevelUp,
	const FGameplayTag&, Element,
	const FExperience&, Experience,
	const FLevelData&, LevelData
);

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementCharacter : public AElementCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 1250.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraRotation = FRotator(-52.5f, 180.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Shooting")
	FVector FiringOffset = FVector(100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FElement> Elements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> UsableAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffects")
	TSubclassOf<UGameplayEffect> AnimationDelayBaseAttackGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffects")
	TSubclassOf<UGameplayEffect> AnimationDelaySpecialAttackGameplayEffect;

	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<UUserWidget> CursorWidgetClass;

	UPROPERTY(BlueprintAssignable)
	FOnAttack OnAttackDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnElementChanged OnElementChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BaseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SpecialAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* AimMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XP")
	TMap<FGameplayTag, FLevelUpData> ElementLevelUpMap;

	UPROPERTY(BlueprintAssignable)
	FOnExperienceChanged OnExperienceChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLevelUp OnLevelUpDelegate;

	bool IsBaseAttackHeld = false;
	bool IsHeavyAttackHeld = false;
	bool IsSpecialAttackHeld = false;
	double LastScrollTime = 0;

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* BaseAttackAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SpecialAttackAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CycleElementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SelectFireElementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SelectNatureElementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SelectWaterElementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

private:
	UPROPERTY()
	UUserWidget* CursorWidgetRef;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	int ActiveElementIndex;

	UPROPERTY(VisibleAnywhere)
	FElement ActiveElement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="XP", meta=(AllowPrivateAccess))
	TMap<FGameplayTag, FExperience> ElementXPMap;

	FTimerHandle RecastTimerHandle;

public:
	AElementCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCastingSpell() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FElement& GetActiveElementRef()
	{
		return ActiveElement;
	}

	UFUNCTION(BlueprintCallable)
	void GiveXP(const FGameplayTag& Element, int XP);

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void OnDeath(AActor* DyingActor, const FDamageTaken& DamageTaken) override;

private:
	UFUNCTION()
	void OnInputMethodChange(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	void OnBaseAttackInput(const FInputActionValue& InputValue);
	void OnHeavyAttackInput(const FInputActionValue& InputValue);
	void OnSpecialAttackInput(const FInputActionValue& InputValue);

	void Move(const FInputActionValue& Value);
	void MouseLook();
	void Look(const FInputActionValue& Value);
	void CycleElement(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	void DoLook(const float Yaw);

	UFUNCTION(BlueprintCallable, Category="Input")
	void StartBaseAttack();

	UFUNCTION(BlueprintCallable, Category="Input")
	void StartHeavyAttack();

	UFUNCTION(BlueprintCallable, Category="Input")
	void StartSpecialAttack();

	UFUNCTION(BlueprintCallable)
	void DoAttack(const TSubclassOf<UGameplayAbility>& Attack) const;

	UFUNCTION(BlueprintCallable, Category="Input")
	void DoCycleElement(const int Amount);

	void SelectElement(const int Index);

	UFUNCTION(BlueprintCallable, Category="Input")
	void SelectFireElement();

	UFUNCTION(BlueprintCallable, Category="Input")
	void SelectNatureElement();

	UFUNCTION(BlueprintCallable, Category="Input")
	void SelectWaterElement();
};
