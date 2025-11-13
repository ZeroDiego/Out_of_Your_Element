// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Math/UnitConversion.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	if (UStaticMesh* ProjectileMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/EditorMeshes/EditorSphere"))))
	{
		Projectile->SetStaticMesh(ProjectileMesh);
		Projectile->SetupAttachment(RootComponent);
	}

	Projectile->SetRelativeScale3D(ProjectileScale);
	Projectile->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	Projectile->SetCollisionObjectType(ECC_GameTraceChannel1);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityScale;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

