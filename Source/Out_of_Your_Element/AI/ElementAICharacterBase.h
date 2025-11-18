// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Out_of_Your_Element/AbilitySystem/ElementAbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"

#include "ElementAICharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIDeathEvent);

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AElementAICharacterBase();

	UBehaviorTree* GetBehaviorTree() const;

	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	UElementAbilitySystemComponent* ElementAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UElementHealthAttributeSet> HealthAttributeSet;

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
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SlowGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> HitStunGameplayEffect;

	//UPROPERTY(BlueprintReadWrite)
	//bool bIsAttacking = false;

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

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	//UPROPERTY(BlueprintReadOnly)
	//bool bIsDead = false;

	//FVector LastKnownLocation;
};
