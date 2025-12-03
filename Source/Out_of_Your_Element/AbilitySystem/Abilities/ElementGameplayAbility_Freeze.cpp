#include "ElementGameplayAbility_Freeze.h"

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
	if (AActor* Caster = GetAvatarActorFromActorInfo())
	{
		static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		};

		DrawDebugSphere(
			Caster->GetWorld(),
			Caster->GetActorLocation(),
			FreezeRadius,
			16,
			FColor::Red,
			true,
			20
		);

		if (TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(
				Caster->GetWorld(),
				Caster->GetActorLocation(),
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

			for (AActor* OutActor : OutActors)
			{
				if (const AElementCharacterBase* Frozen = Cast<AElementCharacterBase>(OutActor))
				{
					UElementAbilitySystemComponent* AbilitySystemComponent = Frozen->ElementAbilitySystemComponent;
					AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(FreezeGameplayEffectSpecHandle);
					AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(DamageGameplayEffectSpecHandle);
				}
			}
		}
	}


	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
