// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_WaterGun.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"

void UElementGameplayAbility_WaterGun::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AActor* Caster = GetAvatarActorFromActorInfo())
	{
		const float StepAngle = Spread / ProjectileCount;
		const int LeftSteps = ProjectileCount / 2;
		FVector Forward = Caster->GetActorForwardVector();

		// Move vector to far left of arc
		Forward = Forward.RotateAngleAxis(
			-1 * StepAngle * LeftSteps,
			FVector::ZAxisVector
		);

		const int Level = GetAbilityLevel(Handle, ActorInfo);
		const int SpawnCount = Level < 2 ? ProjectileCount : ProjectileCount + AdditionalProjectiles;
		for (int i = 0; i < SpawnCount; ++i)
		{
			const FVector Offset = Forward * ProjectileSpawnOffset;
			const FVector Location = Caster->GetActorLocation() + Offset;
			const FRotator Rotation = Forward.Rotation();
			const FTransform Transform(Rotation, Location);
			ShootProjectile(Transform, Level);

			Forward = Forward.RotateAngleAxis(
				StepAngle,
				FVector::ZAxisVector
			);
		}
	}
}
