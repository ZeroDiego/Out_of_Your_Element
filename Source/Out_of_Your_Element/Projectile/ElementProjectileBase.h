// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "ElementProjectileBase.generated.h"


UCLASS(Blueprintable)
class OUT_OF_YOUR_ELEMENT_API AElementProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElementProjectileBase();

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

	UPROPERTY(EditAnywhere)
	float LifeTime = 3.0f;

	UPROPERTY(EditAnywhere)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditAnywhere)
	UGameplayAbility* SourceAbility;

	// Projectile mesh component ref
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* ElementVfx;

	// Projectile VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* ElementPoofVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE")
	TSubclassOf<UGameplayEffect> SlowGameplayEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE")
	TSubclassOf<UGameplayEffect> HitStunGameplayEffect;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;

	// Projectile movement component ref
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
