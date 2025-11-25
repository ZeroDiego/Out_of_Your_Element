// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementZoneBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbility_FireZone.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

// Sets default values
AElementZoneBase::AElementZoneBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ZoneSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("ZoneSphereComponent"));
	ZoneSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ZoneSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = ZoneSphereComponent;

	ZoneNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("ZoneNiagaraComponent"));
	ZoneNiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AElementZoneBase::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), ZoneSphereComponent->GetScaledSphereRadius(), 24, FColor::Blue,
	                false, 10.0f,
	                0, 2.0f);
}

void AElementZoneBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AElementZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappedActors;
	ZoneSphereComponent->GetOverlappingActors(OverlappedActors, AElementCharacterBase::StaticClass());

	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(OverlappedActor))
		{
			for (FGameplayTag Tag : ElementCharacterBase->GetAbilitySystemComponent()->GetOwnedGameplayTags())
			{
				if (Tag.IsValid())
				{
					if (Tag.GetTagName() == TEXT("Abilities.Fire"))
					{
						return;
					}
				}
			}

			ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
				GameplayEffectSpecHandle);
		}
	}
}

void AElementZoneBase::InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
                                      UGameplayAbility* NewSourceAbility,
                                      const float Radius, const float LifeSpan)
{
	GameplayEffectSpecHandle = NewGameplayEffectSpecHandle;
	SourceAbility = NewSourceAbility;
	ZoneSphereComponent->SetSphereRadius(Radius);
	SetLifeSpan(LifeSpan);
}
