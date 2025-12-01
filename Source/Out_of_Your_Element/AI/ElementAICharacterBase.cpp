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

	FGameplayEffectContext* Context = new FGameplayEffectContext(this, this);
	const FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(Context);
	for (const auto& DefaultGameplayEffect : DefaultGameplayEffects)
	{
		const UGameplayEffect* Effect = DefaultGameplayEffect.Key->GetDefaultObject<UGameplayEffect>();
		FGameplayEffectSpec NewSpec = FGameplayEffectSpec(Effect, ContextHandle);
		NewSpec.SetByCallerTagMagnitudes = DefaultGameplayEffect.Value.Tags;
		ElementAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(NewSpec);
	}

	for (TSubclassOf<UGameplayAbility>& Ability : UsableAbilities)
	{
		if (Ability)
		{
			ElementAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		}
	}

	/*AIHealth = MaxAIHealth;
	

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bUseRVOAvoidance             = true;
		Move->AvoidanceConsiderationRadius = 300.f;
		Move->AvoidanceWeight              = 0.7f;
	}rpuc	//AI Crowd avoidance försök

	LastKnownLocation = GetActorLocation();*/
}

/* ─────────────────────────────────────────────── */
void AElementAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
