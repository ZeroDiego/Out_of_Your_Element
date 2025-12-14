// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "ElementMeteor.generated.h"

class UGameplayEffect;
class UNiagaraSystem;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementMeteor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation = FVector(NAN);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float SummoningTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category="Meteor")
	UNiagaraSystem* MeteorIndicator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	TSubclassOf<UGameplayEffect> ImpactDamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	float ImpactDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	TSubclassOf<UGameplayEffect> DotDamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float DotDamageDuration = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	float DotDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	TSubclassOf<class AElementZoneBase> FlameZoneClass;

	UPROPERTY(EditDefaultsOnly, Category="Meteor")
	UNiagaraSystem* FlameZoneVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	float FlameZoneRadius = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float FlameZoneLifeSpan = 10;

	UPROPERTY(EditDefaultsOnly, Category="Meteor")
	UNiagaraSystem* ImpactVfx;

	UPROPERTY(EditDefaultsOnly, Category="Meteor")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Meteor|Decal")
	UMaterialInterface* ImpactDecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Meteor|Decal")
	FVector ImpactDecalSize = FVector(300.f, 300.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category="Meteor|Decal")
	float ImpactDecalLifetime = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle DotGameplayEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle ImpactGameplayEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	AActor* Caster = nullptr;

private:
	float CurrentTime;
	FVector StartLocation;

public:
	AElementMeteor();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void LifeSpanExpired() override;
};
