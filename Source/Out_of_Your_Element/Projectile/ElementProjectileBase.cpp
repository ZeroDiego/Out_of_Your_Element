// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementProjectileBase.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h"

// Sets default values
AElementProjectileBase::AElementProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("ProjectileSphereComponent"));
	RootComponent = ProjectileSphereComponent;
	ProjectileSphereComponent->SetRelativeScale3D(ProjectileScale);
	ProjectileSphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	ProjectileSphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityScale;

	// Create the Niagara component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false; // We activate it in BeginPlay

	// Adds functionality for overlapping with other actors
	OnActorBeginOverlap.AddDynamic(this, &AElementProjectileBase::OnActorOverlap);
}

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

void AElementProjectileBase::LifeSpanExpired()
{
	Super::LifeSpanExpired();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ElementPoofVfx,
		GetActorLocation()
	);
}

// ReSharper disable once CppMemberFunctionMayBeConst -- Cannot be const. Used by overlap delegate
void AElementProjectileBase::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(OtherActor))
	{
		ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(GameplayEffectSpecHandle);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ElementPoofVfx,
			ElementCharacterBase->GetActorLocation()
		);
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ElementPoofVfx,
			GetActorLocation()
		);
	}

	const FMutableBool ShouldDestroy = true;
	OnProjectileHit.Broadcast(this, OtherActor, ShouldDestroy);

	if (ShouldDestroy)
	{
		Destroy();
	}
}
