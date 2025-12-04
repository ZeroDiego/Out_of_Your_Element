#include "ElementGameplayAbility_Freeze.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"

void UElementGameplayAbility_Freeze::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (AElementCharacterBase* Caster = Cast<AElementCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		};

		const UWorld* World = Caster->GetWorld();
		const FVector Location = Caster->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, FreezeActivateParticle, Location);

		DrawDebugSphere(
			World,
			Location,
			FreezeRadius,
			16,
			FColor::Red,
			true,
			20
		);

		if (TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(
				World,
				Location,
				FreezeRadius,
				GroundTypes,
				AElementAICharacterBase::StaticClass(),
				{Caster},
				OutActors)
		)
		{
			const FGameplayEffectSpecHandle FreezeGameplayEffectSpecHandle =
				MakeOutgoingGameplayEffectSpec(FreezeGameplayEffectClass, UGameplayEffect::INVALID_LEVEL);

			const FGameplayEffectSpecHandle DamageGameplayEffectSpecHandle =
				MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, UGameplayEffect::INVALID_LEVEL);

			DamageGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage, BaseDamage);

			UElementAbilitySystemComponent* CasterAsc = Caster->ElementAbilitySystemComponent;
			for (AActor* OutActor : OutActors)
			{
				if (const AElementCharacterBase* Frozen = Cast<AElementCharacterBase>(OutActor))
				{
					UElementAbilitySystemComponent* FrozenAsc = Frozen->ElementAbilitySystemComponent;
					CasterAsc->BP_ApplyGameplayEffectSpecToTarget(FreezeGameplayEffectSpecHandle, FrozenAsc);
					CasterAsc->BP_ApplyGameplayEffectSpecToTarget(DamageGameplayEffectSpecHandle, FrozenAsc);
				}
			}
		}
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
