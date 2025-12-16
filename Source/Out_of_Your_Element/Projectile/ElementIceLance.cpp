#include "ElementIceLance.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void AElementIceLance::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileSphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	OnActorBeginOverlap.AddUniqueDynamic(this, &AElementIceLance::OnOverlap);
}

void AElementIceLance::ShootIceLance(const FTransform& Transform, ACharacter* IgnoreWhenMoving) const
{
	if (AElementIceLance* IceLance =
		GetWorld()->SpawnActorDeferred<AElementIceLance>(GetClass(), Transform)
	)
	{
		IceLance->Level = Level;
		IceLance->CanSplit = false;
		IceLance->GameplayEffectSpecHandle = GameplayEffectSpecHandle;

		if (Caster)
		{
			IceLance->Caster = Caster;
			IceLance->ProjectileSphereComponent->IgnoreActorWhenMoving(Caster, true);

			if (const ACharacter* CasterCharacter = Cast<ACharacter>(Caster))
			{
				CasterCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(IceLance, true);
			}
		}

		if (IgnoreWhenMoving)
		{
			IceLance->ProjectileSphereComponent->IgnoreActorWhenMoving(IgnoreWhenMoving, true);
			IgnoreWhenMoving->GetCapsuleComponent()->IgnoreActorWhenMoving(IceLance, true);
		}

		UGameplayStatics::FinishSpawningActor(IceLance, Transform);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic -- Used in delegate
void AElementIceLance::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ProjectileSphereComponent->GetMoveIgnoreActors().Contains(OtherActor))
		return;

	const FMutableBool ShouldDestroy = !static_cast<bool>(Cast<AElementCharacterBase>(OtherActor));
	DoProjectileHit(FProjectileHitEvent(this, OtherActor, ShouldDestroy, FVector::ZeroVector, FHitResult()));

	if (Level >= 2)
	{
		if (CanSplit)
		{
			CanSplit = false;

			const int ActualProjectileCount = ProjectileCount + 1;
			const float StepAngle = Spread / ActualProjectileCount;
			const int LeftSteps = ActualProjectileCount / 2;
			FVector Forward = GetActorForwardVector();

			// Move vector to far left of arc
			Forward = Forward.RotateAngleAxis(
				-1 * StepAngle * LeftSteps,
				FVector::ZAxisVector
			);

			const int MiddleProjectile = FMath::RoundToInt(ActualProjectileCount / 2.0f) - 1;
			for (int i = 0; i < ActualProjectileCount; ++i)
			{
				if (i != MiddleProjectile)
				{
					const FVector Location = GetActorLocation();
					const FRotator Rotation = Forward.Rotation();
					const FTransform Transform(Rotation, Location);
					ShootIceLance(Transform, Cast<ACharacter>(OtherActor));
				}

				Forward = Forward.RotateAngleAxis(
					StepAngle,
					FVector::ZAxisVector
				);
			}
		}
	}
}
