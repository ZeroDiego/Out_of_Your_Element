// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementAbilitySystemComponent.h"
#include "FiringOffset.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "ElementCharacter.generated.h"

class UInputAction;

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

protected:
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

public:
	AElementCharacter();

	UFUNCTION()
	void OnInputMethodChange(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	void Move(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(const float Yaw);
};
