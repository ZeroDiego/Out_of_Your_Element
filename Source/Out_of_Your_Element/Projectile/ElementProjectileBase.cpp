// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementProjectileBase.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Math/UnitConversion.h"

// Sets default values
AElementProjectileBase::AElementProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ProjectileMesh"));
	RootComponent = ProjectileMeshComponent;
	ProjectileMeshComponent->SetRelativeScale3D(ProjectileScale);
	ProjectileMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	ProjectileMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel1);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityScale;

	// Create the Niagara component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false; // We activate it in BeginPlay
}

// Called when the game starts or when spawned
void AElementProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ElementVfx)
	{
		NiagaraComponent->SetAsset(ElementVfx);
		NiagaraComponent->Activate(true);
	}

	SetLifeSpan(LifeTime);
}

// Called every frame
void AElementProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
