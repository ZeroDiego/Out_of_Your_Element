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
	// Sets default values for this actor's properties
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

	UPROPERTY(VisibleAnywhere)
	FTimerHandle ZoneTickTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ZoneTickRate;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& Sweep);

	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnZoneTick(AElementCharacterBase* ElementCharacterBase);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
	                    UGameplayAbility* NewSourceAbility, const float NewZoneTickRate, const float Radius,
	                    const float LifeSpan);
};
