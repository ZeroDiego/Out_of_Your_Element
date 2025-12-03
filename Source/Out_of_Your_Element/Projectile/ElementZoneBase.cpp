// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementZoneBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbility_FireZone.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

// Sets default values
AElementZoneBase::AElementZoneBase()
{
	ZoneSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("ZoneSphereComponent"));
	ZoneSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ZoneSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = ZoneSphereComponent;

	ZoneNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("ZoneNiagaraComponent"));
	ZoneNiagaraComponent->SetupAttachment(RootComponent);
}

void AElementZoneBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Handle = FTimerHandle();
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AElementZoneBase::DoDamage, 1.0f, true);
}

void AElementZoneBase::DoDamage() const
{
	TArray<AActor*> OverlappedActors;
	ZoneSphereComponent->GetOverlappingActors(OverlappedActors, AElementCharacterBase::StaticClass());

	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (const AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(OverlappedActor))
		{
			/*
			for (FGameplayTag Tag : ElementCharacterBase->GetAbilitySystemComponent()->GetOwnedGameplayTags())
			{
				if (Tag.IsValid())
				{
					if (Tag == ElementGameplayTags::Abilities_Fire)
					{
						return;
					}
				}
			}*/

			ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
				GameplayEffectSpecHandle);
		}
	}
}

void AElementZoneBase::InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
                                      UGameplayAbility* NewSourceAbility, UNiagaraSystem* ZoneVfx,
                                      const float Radius, const float LifeSpan)
{
	GameplayEffectSpecHandle = NewGameplayEffectSpecHandle;
	SourceAbility = NewSourceAbility;
	ZoneSphereComponent->SetSphereRadius(Radius);
	SetLifeSpan(LifeSpan);

	ZoneNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		ZoneVfx,
		GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		true
	);
}
