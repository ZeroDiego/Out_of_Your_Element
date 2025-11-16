// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementAbilitySystemComponent.h"
#include "FiringOffset.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "Element.h"
#include "InputActionValue.h"
#include "ElementCharacter.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FElement Element;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttack, FAttackData, AttackData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnElementChanged, FElement, OldElement, FElement, NewElement);

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraRotation = FRotator(-45.0f, 180.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Shooting")
	FVector FiringOffset = FVector(100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FElement> Elements;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> UsableAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> FireballEffect;

	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<UUserWidget> CursorWidgetClass;

	UPROPERTY()
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(BlueprintAssignable)
	FOnAttack OnAttackDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnElementChanged OnElementChangedDelegate;

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
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsAttacking;

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY()
	UUserWidget* CursorWidgetRef;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraRef;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoomRef;

	UPROPERTY(VisibleAnywhere)
	UFiringOffset* FiringOffsetRef;

	UPROPERTY(VisibleAnywhere)
	int ActiveElementIndex;

	UPROPERTY(VisibleAnywhere)
	FElement ActiveElement;

public:
	AElementCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UFUNCTION()
	void OnInputMethodChange(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	void Move(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CycleElement(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(const float Yaw);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoBaseAttack();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoHeavyAttack();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSpecialAttack();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoCycleElement(const int Amount);
};
