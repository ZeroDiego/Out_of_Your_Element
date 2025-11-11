// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	// Projectile scale
	UPROPERTY(EditAnywhere)
	FVector ProjectileScale = FVector(0.2f, 0.2f, 0.2f);

	// Projectile initial speed
	UPROPERTY(EditAnywhere)
	float ProjectileInitialSpeed = 800.0f;

	// Projectile max speed
	UPROPERTY(EditAnywhere)
	float ProjectileMaxSpeed = 8500.0f;

	// Projectile gravity scale
	UPROPERTY(EditAnywhere)
	float GravityScale = 0.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Projectile mesh ref
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Projectile;

	// Projectile movement component ref
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
};
