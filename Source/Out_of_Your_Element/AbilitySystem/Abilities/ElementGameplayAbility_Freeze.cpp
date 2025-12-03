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
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		static const TArray<AActor*> EmptyIgnore;
		static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		};

		DrawDebugSphere(
			Actor->GetWorld(),
			Actor->GetActorLocation(),
			FreezeRadius,
			16,
			FColor::Red,
			true,
			20
		);

		if (TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(
				Actor->GetWorld(),
				Actor->GetActorLocation(),
				FreezeRadius,
				GroundTypes,
				AElementAICharacterBase::StaticClass(),
				EmptyIgnore,
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
				if (const AElementAICharacterBase* Enemy = Cast<AElementAICharacterBase>(OutActor))
				{
					UElementAbilitySystemComponent* AbilitySystemComponent = Enemy->ElementAbilitySystemComponent;
					AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(FreezeGameplayEffectSpecHandle);
					AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(DamageGameplayEffectSpecHandle);
				}
			}
		}
	}


	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
