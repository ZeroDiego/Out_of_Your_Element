// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementMeteor.h"

#include "ElementZoneBase.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

AElementMeteor::AElementMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElementMeteor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	SetLifeSpan(SummoningTime);
}

void AElementMeteor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FMath::Lerp(TargetLocation, StartLocation, GetLifeSpan() / SummoningTime));
}

void AElementMeteor::LifeSpanExpired()
{
	Super::LifeSpanExpired();

	const FTransform Transform = GetTransform();
	if (AElementZoneBase* MeteorZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
		MeteorZoneClass,
		Transform
	))
	{
		MeteorZone->InitializeZone(
			DotGameplayEffectSpecHandle,
			MeteorZoneVfx,
			MeteorZoneRadius,
			MeteorZoneLifeSpan
		);

		UGameplayStatics::FinishSpawningActor(MeteorZone, Transform);

		TArray<AActor*> HitActors;
		MeteorZone->GetOverlappingActors(HitActors, AElementCharacterBase::StaticClass());
		for (AActor* HitActor : HitActors)
		{
			if (const AElementCharacterBase* HitCharacter = Cast<AElementCharacterBase>(HitActor))
			{
				HitCharacter->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
					ImpactGameplayEffectSpecHandle
				);
			}
		}
	}
}
