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
#include "Out_of_Your_Element/Character/ElementCharacter.h"
#include "Out_of_Your_Element/System/ElementGameInstance.h"

/* ─────────────────────────────────────────────── */
/*                   CONSTRUCTOR                   */
/* ─────────────────────────────────────────────── */
AElementAICharacterBase::AElementAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AISoundComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

/* ─────────────────────────────────────────────── */
UBehaviorTree* AElementAICharacterBase::GetBehaviorTree() const
{
	return BehaviorTree;
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

void AElementAICharacterBase::OnDeath(AActor* DyingActor, const FDamageTaken& DamageTaken)
{
	Super::OnDeath(DyingActor, DamageTaken);
	
	if (DroppedXP > 0)
	{
		if (AElementCharacter* Caster = Cast<AElementCharacter>(DamageTaken.Instigator))
		{
			Caster->GiveXP(DamageTaken.Element, DroppedXP);
		}
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,          // key (-1 = new message)
				5.0f,        // seconds
				FColor::Red,
				TEXT("World not found!")
			);
		}
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // key (-1 = new message)
			5.0f,        // seconds
			FColor::Green,
			TEXT("World found!")
		);
	}
			
	if (UElementGameInstance* Egi = World->GetGameInstance<UElementGameInstance>())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,          // key (-1 = new message)
				5.0f,        // seconds
				FColor::Green,
				TEXT("Stats updated!")
			);
		}
		Egi->GlobalVariables.AddInt(TEXT("Stats.Kills.Total"), 1);

		const FString AbilityId = GetClass()->GetName();
		const FString PerAbilityKey = FString::Printf(TEXT("Stats.Kills.Type.%s"), *AbilityId);

		Egi->GlobalVariables.AddInt(PerAbilityKey, 1);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,          // key (-1 = new message)
				5.0f,        // seconds
				FColor::Red,
				TEXT("Stats Not Updated!")
			);
		}
	}
}
