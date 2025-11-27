// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

#include "ElementAICharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIDeathEvent);

USTRUCT(Blueprintable)
struct FDefaultGameplayEffectTags
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> Tags;
};

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementAICharacterBase : public AElementCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn), Category = "Effects")
	TMap<TSubclassOf<UGameplayEffect>, FDefaultGameplayEffectTags> DefaultGameplayEffects;
	
public:
	AElementAICharacterBase();

	UBehaviorTree* GetBehaviorTree() const;

	//UPROPERTY(BlueprintReadWrite)
	//int32 AIHealth;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 MaxAIHealth = 100;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//float AIDamage = 20;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//TArray<TSubclassOf<class ACollectableBox>>AIDrop;

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<class UElementHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> UsableAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class UAudioComponent* AudioComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Effects")
	//class UNiagaraSystem* DamageEffect;
	//UPROPERTY(EditDefaultsOnly, Category = "Effects")
	//UNiagaraSystem* DeathEffect;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIDeathEvent, AAI_Main*, DeadEnemy);
	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FAIDeathEvent OnEnemyDied;
public:
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(BlueprintReadOnly)
	//bool bIsDead = false;

	//FVector LastKnownLocation;
};
