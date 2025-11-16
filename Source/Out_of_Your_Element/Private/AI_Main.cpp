#include "AI_Main.h"


#include "Components/AudioComponent.h"
#include "AI_Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Navigation/PathFollowingComponent.h"
//#include "NiagaraFunctionLibrary.h"
#include "EngineUtils.h"

//#include "PlayerCharacter.h"
#include "HealthAttributeSet.h"
#include "ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

/* ─────────────────────────────────────────────── */
/*                   CONSTRUCTOR                   */
/* ─────────────────────────────────────────────── */
AAI_Main::AAI_Main()
{
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AISoundComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Creates an ability system component
	ElementAbilitySystemComponent = CreateDefaultSubobject<UElementAbilitySystemComponent>(
		TEXT("ElementAbilitySystemComponent"));

	// Creates an attribute set for health points
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("Health Attribute Set"));

	// Adds functionality for overlapping with other actors
	OnActorBeginOverlap.AddDynamic(this, &AAI_Main::OnActorOverlap);
}

/* ─────────────────────────────────────────────── */
UBehaviorTree* AAI_Main::GetBehaviorTree() const { return BehaviorTree; }

/* ─────────────────────────────────────────────── */
void AAI_Main::BeginPlay()
{
	Super::BeginPlay();

	if (ElementAbilitySystemComponent)
	{
		if (AAIController* AIController = GetController<AAIController>())
		{
			ElementAbilitySystemComponent->InitAbilityActorInfo(AIController, this);
		}
		else
		{
			ElementAbilitySystemComponent->InitAbilityActorInfo(this, this);
		}

		/*
		for (TSubclassOf<UGameplayAbility>& Ability : UsableAbilities)
		{
			if (Ability)
			{
				ElementAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
			}
		}
		*/
	}

	/*AIHealth = MaxAIHealth;
	

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bUseRVOAvoidance             = true;
		Move->AvoidanceConsiderationRadius = 300.f;
		Move->AvoidanceWeight              = 0.7f;
	}	//AI Crowd avoidance försök

	LastKnownLocation = GetActorLocation();*/
}

/* ─────────────────────────────────────────────── */
void AAI_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ElementAbilitySystemComponent->GetOwnedGameplayTags().IsValid())
	{
		for (FGameplayTag Tag : ElementAbilitySystemComponent->GetOwnedGameplayTags())
		{
			if (Tag.IsValid())
			{
				if (Tag.GetTagName() == TEXT("Abilities.Water"))
				{
					GetCharacterMovement()->MaxWalkSpeed = 150;
				}
				else if (Tag.GetTagName() == TEXT("Abilities.Nature"))
				{
					GetController()->StopMovement();
					if (const AAI_Controller* AIController = Cast<AAI_Controller>(GetController()))
					{
						AIController->GetBrainComponent()->StopLogic("HitStun");
					}
				}
			}
		}
	}
	else
	{
		if (const AAI_Controller* AIController = Cast<AAI_Controller>(GetController()))
		{
			AIController->GetBrainComponent()->StartLogic();
		}

		GetCharacterMovement()->MaxWalkSpeed = 300;
	}


	/*
	for (TSubclassOf<UGameplayEffect>& Effect : ElementAbilitySystemComponent->GetActiveGameplayEffects())
	{

	}
	*/
}

void AAI_Main::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor && OtherActor)
	{
		if (const AProjectileBase* ProjectileBase = Cast<AProjectileBase>(OtherActor))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this, ProjectileBase->ElementPoofVfx, OverlappedActor->GetActorLocation(), FRotator(1),
				FVector(1), true, true, ENCPoolMethod::AutoRelease, true);

			if (ProjectileBase->GameplayEffectSpecHandle.IsValid())
			{
				ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
					ProjectileBase->GameplayEffectSpecHandle);

				FGameplayTagContainer TagContainer;
				ProjectileBase->GameplayEffectSpecHandle.Data->GetAllAssetTags(TagContainer);
				for (FGameplayTag Tag : TagContainer)
				{
					if (Tag.IsValid())
					{
						if (Tag.GetTagName() == TEXT("Damage.Type.Water"))
						{
							const FGameplayEffectContextHandle Context;
							ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(SlowGameplayEffect, 1, Context);
						}

						if (Tag.GetTagName() == TEXT("Damage.Type.Nature"))
						{
							if (ACharacter* Character = Cast<ACharacter>(OverlappedActor))
							{
								FVector OtherActorForwardVector = OtherActor->GetActorForwardVector();
								OtherActorForwardVector.X *= 2000;
								OtherActorForwardVector.Y *= 2000;
								OtherActorForwardVector.Z = 0;
								const FGameplayEffectContextHandle Context;
								ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(
									HitStunGameplayEffect, 1, Context);
								Character->LaunchCharacter(OtherActorForwardVector, true, true);
							}
						}
					}
				}
			}

			OtherActor->Destroy();
		}
	}
}
