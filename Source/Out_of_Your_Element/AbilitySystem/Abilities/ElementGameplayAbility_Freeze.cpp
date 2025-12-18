#include "ElementGameplayAbility_Freeze.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"

void UElementGameplayAbility_Freeze::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AElementCharacterBase* Caster = Cast<AElementCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		static const TArray<TEnumAsByte<EObjectTypeQuery>> FreezeTypes = {
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		};

		const UWorld* World = Caster->GetWorld();
		const FVector Location = Caster->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, FreezeActivateParticle, Location);

		const FGameplayTagContainer Burning(ElementGameplayTags::Status_Burning);
		BP_RemoveGameplayEffectFromOwnerWithGrantedTags(Burning);

		const int Level = GetAbilityLevel(Handle, ActorInfo);
		const float ActualFreezeRadius = Level < 2 ? FreezeRadius : FreezeRadius + AdditionalFreezeRadius;

		DrawDebugSphere(World, Location, ActualFreezeRadius, 32, FColor::Red, false, 5.0f);

		if (TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(
				World,
				Location,
				ActualFreezeRadius,
				{UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)},
				AElementZoneBase::StaticClass(),
				TArray<AActor*>(),
				OutActors)
		)
		{
			for (AActor* const& OutActor : OutActors)
			{
				OutActor->Destroy();
			}
		}

		if (TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(
				World,
				Location,
				ActualFreezeRadius,
				FreezeTypes,
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
			for (AActor* const& OutActor : OutActors)
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
}
