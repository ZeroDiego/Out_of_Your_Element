// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffect.h"
#include "Out_of_Your_Element/Utillity/BlueprintUtility.h"
#include "ElementProjectileBase.generated.h"

class AElementProjectileBase;

USTRUCT(BlueprintType)
struct FProjectileHitEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AElementProjectileBase* Projectile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AActor* HitActor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FMutableBool ShouldDestroy;

	UPROPERTY(BlueprintReadOnly)
	FVector NormalImpulse = FVector::ZeroVector;

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

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ProjectileInitialSpeed = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ProjectileMaxSpeed = 8500.0f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float LifeTime = 5.0f;

	UPROPERTY(BlueprintReadWrite, Category="Projectile")
	FGameplayEffectSpecHandle DamageGameplayEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Damage = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	class UNiagaraSystem* ElementPoofVfx;

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FOnProjectileHit OnProjectileHit;

	UPROPERTY(BlueprintReadOnly, Category="Projectile")
	AActor* Caster;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category="Projectile")
	int Level = 1;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ProjectileVfx;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* ProjectileSphereComponent;

protected:
	virtual void BeginPlay() override;

	virtual void LifeSpanExpired() override;

	virtual void DoProjectileHit(const FProjectileHitEvent& PreEvent);

private:
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
