// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "DiegoCharacter.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API ADiegoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADiegoCharacter();
	
	UPROPERTY(EditAnywhere, Category = "C++")
	float TargetArmLength = 500.0f;
	UPROPERTY(EditAnywhere, Category = "C++")
	FVector SocketOffset = FVector(0.0f, 0.0f, 500.0f);
	UPROPERTY(EditAnywhere, Category = "C++")
	FRotator CameraRotation = FRotator(-45.0f, 0.0f, 0.0f);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Cube;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
};
