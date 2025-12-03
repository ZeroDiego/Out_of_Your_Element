#include "ElementGameplayAbility_Thorns.h"

#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"
#include "Out_of_Your_Element/Projectile/ElementThorn.h"

static void CalculateThornPosition(
	const FVector2D& Center,
	const FVector2D& Direction,
	const float Length,
	const float HalfAngleRad,
	FVector2D& OutPosition
)
{
	const float Theta = FMath::FRandRange(-HalfAngleRad, HalfAngleRad);
	const float CosT = FMath::Cos(Theta);
	const float SinT = FMath::Sin(Theta);

	const FVector2D RotatedDir(
		Direction.X * CosT - Direction.Y * SinT,
		Direction.X * SinT + Direction.Y * CosT
	);

	OutPosition = Center + Length * FMath::FRand() * RotatedDir;
}

static bool IsThornPositionTooClose(
	const float MinSpacingSq,
	const FVector2D& ThornPosition,
	const TArray<FVector2D>& ExistingThornPositions
)
{
	for (const FVector2D& ExistingThornPosition : ExistingThornPositions)
	{
		if (FVector2D::DistSquared(ThornPosition, ExistingThornPosition) < MinSpacingSq)
			return false;
	}

	return true;
}

static void CalculateThornPositions(
	const FVector2D& Center,
	const FVector2D& Direction,
	const float Length,
	const float Spread,
	const float MinSpacing,
	const int ThornCount,
	TArray<FVector2D>& OutThornPositions
)
{
	const float MinSpacingSq = MinSpacing * MinSpacing;
	const float HalfAngleRad = FMath::DegreesToRadians(Spread / 2.0f);

	FVector2D ThornPosition;
	for (int i = 0; i < ThornCount; ++i)
	{
		for (int RetryCount = 0; RetryCount < 16; ++RetryCount)
		{
			CalculateThornPosition(Center, Direction, Length, HalfAngleRad, ThornPosition);
			if (IsThornPositionTooClose(MinSpacingSq, ThornPosition, OutThornPositions))
			{
				OutThornPositions.Add(ThornPosition);
				break;
			}
		}
	}
}

bool FindGroundLocation(const UWorld* World, const FVector& Start, const FVector& SearchDistance, FVector& OutLocation)
{
	constexpr ECollisionChannel Ground = ECC_GameTraceChannel2;
	const FVector End = Start - SearchDistance;
	if (FHitResult HitResult;
		World->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			Ground
		)
	)
	{
		DrawDebugLine(World, Start, HitResult.Location, FColor::Green, false, 5);
		DrawDebugLine(World, HitResult.Location, End, FColor::Red, false, 5);

		OutLocation = HitResult.Location;
		return true;
	}

	return false;
}

void UElementGameplayAbility_Thorns::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (const AElementCharacterBase* Caster = Cast<AElementCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		TArray<FVector2D> ThornPositions;
		ThornPositions.Reserve(ThornCount);

		const FVector ActorLocation = Caster->GetActorLocation();
		const float Height = ActorLocation.Z - Caster->GetSimpleCollisionHalfHeight();
		UWorld* World = Caster->GetWorld();

		const FVector2D Center(ActorLocation);
		const FVector2D Direction(Caster->GetActorForwardVector());

		CalculateThornPositions(
			Center,
			Direction,
			Length,
			Spread,
			MinSpacing,
			ThornCount,
			ThornPositions
		);

		const FVector SearchDistance = Caster->GetActorUpVector() * 25;
		for (const FVector2D& ThornPosition2D : ThornPositions)
		{
			if (FVector ThornPosition(ThornPosition2D, Height);
				FindGroundLocation(World, ThornPosition, SearchDistance, ThornPosition)
			)
			{
				DrawDebugSphere(
					World,
					ThornPosition,
					5.f,
					8,
					FColor::Emerald,
					false,
					5.f
				);

				const FTransform SpawnTransform(FRotator::ZeroRotator, ThornPosition);
				if (AElementThorn* Thorn = World->SpawnActorDeferred<AElementThorn>(AThornClass, SpawnTransform))
				{
					Thorn->SetLifeSpan(ThornLifespan);
					Thorn->DamageGameplayEffect = DamageGameplayEffect;
					Thorn->BaseDamage = BaseDamage;
					Thorn->Caster = Caster;
					UGameplayStatics::FinishSpawningActor(Thorn, SpawnTransform);
				}
			}
		}
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
