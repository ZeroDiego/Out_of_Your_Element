// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "ElementWallBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementWallBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElementWallBase();

	UPROPERTY(EditAnywhere)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayAbility* SourceAbility;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* WallNiagaraComponent;
	
	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* RockWallPopOutVfx;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
	                    UGameplayAbility* NewSourceAbility, UNiagaraSystem* WallPopInVfx, UNiagaraSystem* WallPopOutVfx,
	                    const FVector& Scale,
	                    const FVector& SpawnLocation,
	                    const float LifeSpan);
};
