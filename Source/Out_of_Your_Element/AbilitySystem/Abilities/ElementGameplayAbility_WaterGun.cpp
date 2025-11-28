// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_WaterGun.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"

void UElementGameplayAbility_WaterGun::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
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

		for (int i = 0; i < ProjectileCount; ++i)
		{
			const FVector Offset = Forward * ProjectileSpawnOffset;
			const FVector Location = Caster->GetActorLocation() + Offset;
			const FRotator Rotation = Forward.Rotation();
			ShootProjectile(Location, Rotation);

			Forward = Forward.RotateAngleAxis(
				StepAngle,
				FVector::ZAxisVector
			);
		}
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
