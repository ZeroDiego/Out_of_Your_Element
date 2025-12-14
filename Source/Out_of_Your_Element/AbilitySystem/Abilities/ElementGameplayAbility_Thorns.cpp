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
	const int ThornsToSpawn = ThornCount - OutThornPositions.Num();
	for (int i = 0; i < ThornsToSpawn; ++i)
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
	const FVector OffsetStart = Start + SearchDistance;
	const FVector End = Start - SearchDistance;
	if (FHitResult HitResult;
		World->LineTraceSingleByChannel(
			HitResult,
			OffsetStart + SearchDistance,
			End,
			Ground
		)
	)
	{
		DrawDebugLine(World, Start, HitResult.Location, FColor::Green, false, 5);
		DrawDebugLine(World, OffsetStart, HitResult.Location, FColor::Green, false, 5);
		DrawDebugLine(World, HitResult.Location, End, FColor::Red, false, 5);

		OutLocation = HitResult.Location;
		return true;
	}

	return false;
}

void UElementGameplayAbility_Thorns::CastSpell(
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

		// Always spawn thorn on cursor if in cone
		if (const APlayerController* PlayerController = Cast<APlayerController>(Caster->GetController()))
		{
			if (PlayerController->IsLocalPlayerController())
			{
				static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
					UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2),
				};

				if (FHitResult MouseCursorHitResult; PlayerController->GetHitResultUnderCursorForObjects(
					GroundTypes,
					false,
					MouseCursorHitResult
				))
				{
					// Is cursor close enough
					const FVector2D CursorLocation = FVector2D(MouseCursorHitResult.Location);
					if (const FVector2D RelativeCursorLocation = CursorLocation - Center;
						RelativeCursorLocation.SquaredLength() <= Length * Length
					)
					{
						// Is cursor inside cone angle
						const FVector2D DirectionNormalized = Direction.GetSafeNormal();
						const FVector2D CursorNormalized = RelativeCursorLocation.GetSafeNormal();
						const double Dot = FVector2D::DotProduct(DirectionNormalized, CursorNormalized);

						if (const double Angle = FMath::Acos(Dot);
							Angle <= Spread / 2.0f
						)
						{
							ThornPositions.Add(CursorLocation);
						}
					}
				}
			}
		}

		CalculateThornPositions(
			Center,
			Direction,
			Length,
			Spread,
			MinSpacing,
			ThornCount,
			ThornPositions
		);

		constexpr float AllowedHeightDifference = 50.0f;
		const FVector SearchDistance = Caster->GetActorUpVector() * AllowedHeightDifference;
		for (const FVector2D& ThornPosition2D : ThornPositions)
		{
			if (FVector ThornPosition(ThornPosition2D, Height);
				FindGroundLocation(World, ThornPosition, SearchDistance, ThornPosition)
			)
			{
				const FTransform SpawnTransform(FRotator::ZeroRotator, ThornPosition);
				if (AElementThorn* Thorn = World->SpawnActorDeferred<AElementThorn>(AThornClass, SpawnTransform))
				{
					Thorn->SetLifeSpan(ThornLifespan);
					Thorn->DamageGameplayEffect = DamageGameplayEffect;
					Thorn->BaseDamage = BaseDamage;
					Thorn->Caster = Caster;
					Thorn->ThornDistance = (ThornPosition2D - Center).Length() / Length;
					UGameplayStatics::FinishSpawningActor(Thorn, SpawnTransform);
				}
			}
		}
	}
}
