// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementProjectileBase.h"

#include "AbilitySystemGlobals.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
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

	OnActorHit.AddDynamic(this, &AElementProjectileBase::OnHit);
}

void AElementProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GameplayEffectSpecHandle.IsValid() && DamageGameplayEffect)
	{
		if (const UGameplayEffect* DamageEffectCDO = DamageGameplayEffect.GetDefaultObject())
		{
			FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();

			GameplayEffectSpecHandle = FGameplayEffectSpecHandle(
				new FGameplayEffectSpec(DamageEffectCDO, FGameplayEffectContextHandle(EffectContext))
			);

			GameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				Damage
			);
		}
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

void AElementProjectileBase::DoProjectileHit(AActor* HitActor)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitActor))
	{
		AbilitySystemInterface->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(GameplayEffectSpecHandle);
	}

	if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(HitActor))
	{
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
	OnProjectileHit.Broadcast(this, HitActor, ShouldDestroy);

	if (ShouldDestroy)
		Destroy();
}

void AElementProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	DoProjectileHit(OtherActor);
}
