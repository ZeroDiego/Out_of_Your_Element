// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Out_of_Your_Element/Utillity/BlueprintUtility.h"
#include "ElementProjectileBase.generated.h"

class AElementProjectileBase;

USTRUCT(BlueprintType)
struct FProjectileHitEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AElementProjectileBase* Projectile;

	UPROPERTY(BlueprintReadOnly)
	AActor* HitActor;

	UPROPERTY(BlueprintReadOnly)
	FMutableBool ShouldDestroy;

	UPROPERTY(BlueprintReadOnly)
	FVector NormalImpulse;

	UPROPERTY(BlueprintReadOnly)
	FHitResult Hit;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProjectileHit,
	const FProjectileHitEvent&, Event
);

UCLASS(Blueprintable)
class OUT_OF_YOUR_ELEMENT_API AElementProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AElementProjectileBase();

	UPROPERTY(EditAnywhere)
	FVector ProjectileScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	float ProjectileInitialSpeed = 800.0f;

	UPROPERTY(EditAnywhere)
	float ProjectileMaxSpeed = 8500.0f;

	UPROPERTY(EditAnywhere)
	float GravityScale = 0.0f;

	UPROPERTY(EditAnywhere)
	float LifeTime = 5.0f;

	UPROPERTY(EditAnywhere)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 25.0f;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* ProjectileSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* ElementPoofVfx;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;

	UPROPERTY(BlueprintReadOnly)
	AActor* Caster;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Level = 1;

protected:
	virtual void BeginPlay() override;

	virtual void LifeSpanExpired() override;

	virtual void DoProjectileHit(const FProjectileHitEvent& PreEvent);

private:
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
