// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"
#include "ElementZoneBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementZoneBase : public AActor
{
	GENERATED_BODY()

public:
	AElementZoneBase();

	UPROPERTY(EditAnywhere)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayAbility* SourceAbility;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ZoneNiagaraComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* ZoneSphereComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void DoDamage() const;

public:
	void InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
	                    UGameplayAbility* NewSourceAbility, UNiagaraSystem* ZoneVfx, const float Radius,
	                    const float LifeSpan);
};
