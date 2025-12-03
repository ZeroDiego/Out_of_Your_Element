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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProjectileHit,
                                               AElementProjectileBase*, Projectile,
                                               AActor*, HitActor,
                                               FMutableBool, ShouldDestory);

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
	float LifeTime = 3.0f;

	UPROPERTY(EditAnywhere)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayAbility* SourceAbility;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* ProjectileSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* ElementVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* ElementPoofVfx;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;

protected:
	virtual void BeginPlay() override;

	virtual void LifeSpanExpired() override;

private:
	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
