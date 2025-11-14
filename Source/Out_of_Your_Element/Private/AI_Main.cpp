#include "AI_Main.h"


#include "Components/AudioComponent.h"
#include "AI_Controller.h"


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

	/*
	for (TSubclassOf<UGameplayEffect>& Effect : ElementAbilitySystemComponent->GetActiveGameplayEffects())
	{

	}
	*/
}

void AAI_Main::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("AAI_Main::OnActorOverlap"));

	if (OverlappedActor && OtherActor)
	{
		if (const AProjectileBase* ProjectileBase = Cast<AProjectileBase>(OtherActor))
		{
			ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(ProjectileBase->GameplayEffectSpecHandle);
			OtherActor->Destroy();
		}
	}
}
