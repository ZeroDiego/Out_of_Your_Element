#include "ElementAICharacterBase.h"


#include "Components/AudioComponent.h"
#include "ElementalAIController.h"
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
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbility_Fireball.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "Out_of_Your_Element/Projectile/ElementProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

/* ─────────────────────────────────────────────── */
/*                   CONSTRUCTOR                   */
/* ─────────────────────────────────────────────── */
AElementAICharacterBase::AElementAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AISoundComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Creates an ability system component
	ElementAbilitySystemComponent = CreateDefaultSubobject<UElementAbilitySystemComponent>(
		TEXT("ElementAbilitySystemComponent"));

	// Creates an attribute set for health points
	HealthAttributeSet = CreateDefaultSubobject<UElementHealthAttributeSet>(TEXT("Health Attribute Set"));

	// Adds functionality for overlapping with other actors
	OnActorBeginOverlap.AddDynamic(this, &AElementAICharacterBase::OnActorOverlap);
}

/* ─────────────────────────────────────────────── */
UBehaviorTree* AElementAICharacterBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

void AElementAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(
		GetAbilitySystemComponent(),
		*GetClass()->GetName(),
		1,
		true
	);

	HealthAttributeSet->InitHealth(HealthAttributeSet->GetMaxHealth());

	if (AAIController* AIController = GetController<AAIController>())
	{
		ElementAbilitySystemComponent->InitAbilityActorInfo(AIController, this);
	}
	else
	{
		ElementAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

/* ─────────────────────────────────────────────── */
void AElementAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	/*
	for (TSubclassOf<UGameplayAbility>& Ability : UsableAbilities)
	{
		if (Ability)
		{
			ElementAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		}
	}
	*/

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
void AElementAICharacterBase::Tick(float DeltaTime)
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
					if (const AElementalAIController* AIController = Cast<AElementalAIController>(GetController()))
					{
						AIController->GetBrainComponent()->StopLogic("HitStun");
					}
				}
			}
		}
	}
	else
	{
		if (const AElementalAIController* AIController = Cast<AElementalAIController>(GetController()))
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

void AElementAICharacterBase::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor && OtherActor)
	{
		if (const AElementProjectileBase* ProjectileBase = Cast<AElementProjectileBase>(OtherActor))
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

				if (const UElementGameplayAbility_Fireball* Fireball
					= Cast<UElementGameplayAbility_Fireball>(ProjectileBase->SourceAbility))
				{
					if (Fireball->FireballDotVfx)
					{
						UNiagaraFunctionLibrary::SpawnSystemAttached(
							Fireball->FireballDotVfx,
							OverlappedActor->GetRootComponent(), NAME_None,
							FVector::ZeroVector,
							FRotator::ZeroRotator,
							EAttachLocation::Type::KeepRelativeOffset,
							true
						);
					}
				}
			}

			OtherActor->Destroy();
		}
	}
}
