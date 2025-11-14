// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementAbilitySystemComponent.h"
#include "FiringOffset.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "ElementCharacter.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AElementCharacter();

	// A float that determines how far away the camera will be from the player
	UPROPERTY(EditAnywhere, Category = "C++")
	float TargetArmLength = 500.0f;

	// Offset from the initial TargetArmLength
	UPROPERTY(EditAnywhere, Category = "C++")
	FVector SocketOffset = FVector(0.0f, 0.0f, 500.0f);

	// Rotation of the camera
	UPROPERTY(EditAnywhere, Category = "C++")
	FRotator CameraRotation = FRotator(-45.0f, 0.0f, 0.0f);

	// Firing offset
	UPROPERTY(EditAnywhere, Category = "C++")
	FVector FiringOffset = FVector(100.0f, 0.0f, 0.0f);

	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	// Abilities array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> UsableAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> FireballEffect;

	UPROPERTY()
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	// Camera component ref
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraRef;

	// Spring arm component ref
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoomRef;

	// Firing offset ref
	UPROPERTY(VisibleAnywhere)
	UFiringOffset* FiringOffsetRef;
};
