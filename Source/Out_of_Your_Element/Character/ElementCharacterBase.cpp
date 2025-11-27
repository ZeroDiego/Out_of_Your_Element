// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AI/ElementalAIController.h"

// Sets default values
AElementCharacterBase::AElementCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireDotNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraComponent"));
	FireDotNiagaraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AElementCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	ElementAbilitySystemComponent->RegisterGameplayTagEvent(ElementGameplayTags::Abilities_Fire,
	                                                        EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &AElementCharacterBase::FireDamageHandler);

	ElementAbilitySystemComponent->RegisterGameplayTagEvent(ElementGameplayTags::Abilities_Water,
	                                                        EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &AElementCharacterBase::WaterDamageHandler);

	ElementAbilitySystemComponent->RegisterGameplayTagEvent(ElementGameplayTags::Abilities_Nature,
	                                                        EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &AElementCharacterBase::NatureDamageHandler);
}

void AElementCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AElementCharacterBase::FireDamageHandler(FGameplayTag Tag, const int32 NewCount) const
{
	if (NewCount > 0)
	{
		FireDotNiagaraComponent->SetAsset(FireDotVfx);
	}
	else
	{
		FireDotNiagaraComponent->SetAsset(nullptr);
	}
}

void AElementCharacterBase::WaterDamageHandler(FGameplayTag Tag, const int32 NewCount) const
{
	OnWaterDamageTakenDelegate.Broadcast(NewCount);
}

void AElementCharacterBase::NatureDamageHandler(FGameplayTag Tag, const int32 NewCount) const
{
	if (NewCount > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;

		if (AElementalAIController* AIController = Cast<AElementalAIController>(GetController()))
		{
			AIController->StopMovement();
			AIController->GetBrainComponent()->StopLogic("HitStun");
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300;

		if (const AElementalAIController* AIController = Cast<AElementalAIController>(GetController()))
		{
			AIController->GetBrainComponent()->StartLogic();
		}
	}
}

// Called every frame
void AElementCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AElementCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
