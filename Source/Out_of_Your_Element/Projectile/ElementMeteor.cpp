// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementMeteor.h"

#include "AbilitySystemGlobals.h"
#include "ElementZoneBase.h"
#include "NiagaraFunctionLibrary.h"
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

	if (TargetLocation.ContainsNaN())
	{
		constexpr double MaxGroundCheckDistance = -10000;
		const FVector MaxEnd = StartLocation * GetActorUpVector() * MaxGroundCheckDistance;
		constexpr ECollisionChannel Ground = ECC_GameTraceChannel2;
		if (FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				MaxEnd,
				Ground
			)
		)
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Meteor ground found!"));
			Destroy();
			return;
		}
	}

	if (!ImpactGameplayEffectSpecHandle.IsValid())
	{
		if (const UGameplayEffect* ImpactDamageEffectCDO = ImpactDamageGameplayEffect.GetDefaultObject())
		{
			FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();

			ImpactGameplayEffectSpecHandle = FGameplayEffectSpecHandle(
				new FGameplayEffectSpec(ImpactDamageEffectCDO, FGameplayEffectContextHandle(EffectContext))
			);

			ImpactGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				ImpactDamage
			);
		}
	}

	if (!DotGameplayEffectSpecHandle.IsValid())
	{
		if (const UGameplayEffect* DotDamageEffectCDO = DotDamageGameplayEffect.GetDefaultObject())
		{
			FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();

			DotGameplayEffectSpecHandle = FGameplayEffectSpecHandle(
				new FGameplayEffectSpec(DotDamageEffectCDO, FGameplayEffectContextHandle(EffectContext))
			);

			DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Duration,
				DotDamageDuration
			);

			DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				DotDamage
			);
		}
	}

	SetLifeSpan(SummoningTime);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MeteorIndicator,
		TargetLocation
	);
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
	if (AElementZoneBase* FlameZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
		FlameZoneClass,
		Transform
	))
	{
		FlameZone->InitializeZone(
			DotGameplayEffectSpecHandle,
			FlameZoneVfx,
			FlameZoneRadius,
			FlameZoneLifeSpan
		);

		UGameplayStatics::FinishSpawningActor(FlameZone, Transform);

		TArray<AActor*> HitActors;
		FlameZone->GetOverlappingActors(HitActors, AElementCharacterBase::StaticClass());
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
	if (ImpactVfx)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactVfx,
			GetActorLocation()
		);
	}
	if (ImpactCameraShake)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC->ClientStartCameraShake(ImpactCameraShake);
		}
	}
	if (ImpactDecalMaterial)
	{
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			ImpactDecalMaterial,
			ImpactDecalSize,
			TargetLocation,
			FRotator(-90.f, FMath::RandRange(0.f, 360.f), 0.f),  
			ImpactDecalLifetime
		);
		
	}
}
