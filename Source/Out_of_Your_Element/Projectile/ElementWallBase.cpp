// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementWallBase.h"

#include "NiagaraFunctionLibrary.h"

// Sets default values
AElementWallBase::AElementWallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AElementWallBase::BeginPlay()
{
	Super::BeginPlay();
}

void AElementWallBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	WallNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		RockWallPopOutVfx,
		GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		true
	);
}

// Called every frame
void AElementWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AElementWallBase::InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
                                      UGameplayAbility* NewSourceAbility, UNiagaraSystem* WallPopInVfx,
                                      UNiagaraSystem* WallPopOutVfx, const FVector& Scale,
                                      const FVector& SpawnLocation, const float LifeSpan)
{
	GameplayEffectSpecHandle = NewGameplayEffectSpecHandle;
	SourceAbility = NewSourceAbility;
	MeshComponent->SetRelativeScale3D(Scale);
	MeshComponent->SetRelativeLocation(SpawnLocation);
	SetLifeSpan(LifeSpan);

	WallNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		WallPopInVfx,
		GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		true
	);

	RockWallPopOutVfx = WallPopOutVfx;
}
