// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementProjectileBase.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbility_Fireball.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"
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

void AElementProjectileBase::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor && OtherActor)
	{
		if (const AElementProjectileBase* ProjectileBase = Cast<AElementProjectileBase>(OverlappedActor))
		{
			if (AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(OtherActor))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this, ProjectileBase->ElementPoofVfx, ElementCharacterBase->GetActorLocation(), FRotator(1),
					FVector(1), true, true, ENCPoolMethod::AutoRelease, true);

				if (ProjectileBase->GameplayEffectSpecHandle.IsValid())
				{
					ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
						ProjectileBase->GameplayEffectSpecHandle);

					FGameplayTagContainer TagContainer;
					ProjectileBase->GameplayEffectSpecHandle.Data->GetAllAssetTags(TagContainer);
					for (FGameplayTag Tag : TagContainer)
					{
						if (Tag.IsValid())
						{
							const FGameplayEffectContextHandle Context;
							
							if (Tag == ElementGameplayTags::Damage_Type_Water)
							{
								ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(
									SlowGameplayEffect, 1, Context);
							}

							if (Tag == ElementGameplayTags::Damage_Type_Nature)
							{
								FVector ProjectileBaseForwardVector = ProjectileBase->GetActorForwardVector();
								ProjectileBaseForwardVector.X *= 2000;
								ProjectileBaseForwardVector.Y *= 2000;
								ProjectileBaseForwardVector.Z = 0;
								ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(
									HitStunGameplayEffect, 1, Context);
								ElementCharacterBase->LaunchCharacter(ProjectileBaseForwardVector, true, true);
							}
						}
					}

					OnProjectileHitDelegate.Broadcast(ElementCharacterBase);
				}
			}
		}
	}
}
