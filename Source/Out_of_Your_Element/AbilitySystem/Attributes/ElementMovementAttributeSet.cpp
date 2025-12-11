#include "ElementMovementAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"

void UElementMovementAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1000);
	}

	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UElementMovementAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute,
	const float OldValue,
	float NewValue
)
{
	if (Attribute == GetMovementSpeedAttribute())
	{
		if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
		{
			if (UCharacterMovementComponent* CharacterMovementComponent =
				Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				NewValue = FMath::Clamp(NewValue, 0.0f, 1000);
				CharacterMovementComponent->MaxWalkSpeed = NewValue;
				OnMovementSpeedChanged.Broadcast(this, OldValue, NewValue);
			}
		}
	}

	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
