// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementZoneBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Out_of_Your_Element/AbilitySystem/Abilities/ElementGameplayAbility_FireZone.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

// Sets default values
AElementZoneBase::AElementZoneBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ZoneSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("ZoneSphereComponent"));
	ZoneSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ZoneSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ZoneSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AElementZoneBase::OnZoneBeginOverlap);
	ZoneSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AElementZoneBase::OnZoneEndOverlap);
	RootComponent = ZoneSphereComponent;

	ZoneNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("ZoneNiagaraComponent"));
	ZoneNiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AElementZoneBase::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), ZoneSphereComponent->GetScaledSphereRadius(), 24, FColor::Blue,
	                false, 10.0f,
	                0, 2.0f);
}

void AElementZoneBase::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& Sweep)
{
	if (AElementCharacterBase* ElementCharacterBase = Cast<AElementCharacterBase>(OtherActor))
	{
		FTimerDelegate ZoneTickTimerDelegate;
		ZoneTickTimerDelegate.BindUObject(this, &AElementZoneBase::OnZoneTick, ElementCharacterBase);
		GetWorld()->GetTimerManager().SetTimer(
			ZoneTickTimerHandle,
			ZoneTickTimerDelegate,
			ZoneTickRate,
			true,
			0.0f);
	}
}

void AElementZoneBase::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(ZoneTickTimerHandle);
}

void AElementZoneBase::OnZoneTick(AElementCharacterBase* ElementCharacterBase)
{
	DrawDebugSphere(GetWorld(), ElementCharacterBase->GetActorLocation(), ZoneSphereComponent->GetScaledSphereRadius(),
	                24, FColor::Red, false, ZoneTickRate,
	                0, 1.0f);

	ElementCharacterBase->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
		GameplayEffectSpecHandle);

	if (const UElementGameplayAbility_FireZone* FireZone
		= Cast<UElementGameplayAbility_FireZone>(SourceAbility))
	{
		if (FireZone->FireZoneDotVfx)
		{
			ElementCharacterBase->NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				FireZone->FireZoneDotVfx,
				ElementCharacterBase->GetRootComponent(), NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::Type::KeepRelativeOffset,
				true,
				true
			);
		}
	}
}

// Called every frame
void AElementZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AElementZoneBase::InitializeZone(const FGameplayEffectSpecHandle& NewGameplayEffectSpecHandle,
                                      UGameplayAbility* NewSourceAbility, const float NewZoneTickRate,
                                      const float Radius, const float LifeSpan)
{
	GameplayEffectSpecHandle = NewGameplayEffectSpecHandle;
	SourceAbility = NewSourceAbility;
	ZoneTickRate = NewZoneTickRate;
	ZoneSphereComponent->SetSphereRadius(Radius);
	SetLifeSpan(LifeSpan);
}
