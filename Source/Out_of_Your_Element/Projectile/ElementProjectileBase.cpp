// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementProjectileBase.h"

#include "AbilitySystemGlobals.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h"

AElementProjectileBase::AElementProjectileBase()
{
	ProjectileSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("ProjectileSphereComponent"));
	RootComponent = ProjectileSphereComponent;
	ProjectileSphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
	ProjectileSphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	ProjectileSphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileSphereComponent->SetCanEverAffectNavigation(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->SetPlaneConstraintEnabled(true); // Constraint set in begin play

	ProjectileVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	ProjectileVfx->SetupAttachment(RootComponent);

	OnActorHit.AddDynamic(this, &AElementProjectileBase::OnHit);
}

void AElementProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!DamageGameplayEffectSpecHandle.IsValid() && DamageGameplayEffect)
	{
		if (const UGameplayEffect* DamageEffectCDO = DamageGameplayEffect.GetDefaultObject())
		{
			FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();

			DamageGameplayEffectSpecHandle = FGameplayEffectSpecHandle(
				new FGameplayEffectSpec(DamageEffectCDO, FGameplayEffectContextHandle(EffectContext))
			);

			DamageGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				Damage
			);
		}
	}

	ProjectileMovement->SetPlaneConstraintFromVectors(GetActorForwardVector(), GetActorRightVector());
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

void AElementProjectileBase::DoProjectileHit(const FProjectileHitEvent& PreEvent)
{
	AActor* const& HitActor = PreEvent.HitActor;

	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitActor))
	{
		AbilitySystemInterface->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(DamageGameplayEffectSpecHandle);
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

	OnProjectileHit.Broadcast(PreEvent);
	if (PreEvent.ShouldDestroy)
		Destroy();
}

void AElementProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	DoProjectileHit(FProjectileHitEvent(this, OtherActor, true, NormalImpulse, Hit));
}
