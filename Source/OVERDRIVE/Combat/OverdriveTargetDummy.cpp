// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveTargetDummy.h"

#include "../GAS/OverdriveAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

AOverdriveTargetDummy::AOverdriveTargetDummy()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TargetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMeshComponent"));
	RootComponent = TargetMeshComponent;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UOverdriveAttributeSet>(TEXT("AttributeSet"));
}

void AOverdriveTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOverdriveAttributeSet::GetHealthAttribute()).AddUObject(this, &AOverdriveTargetDummy::HandleHealthChanged);
	}
}

UAbilitySystemComponent* AOverdriveTargetDummy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOverdriveTargetDummy::HandleWeaponHit_Implementation(AActor* InstigatorActor, const FHitResult& HitResult)
{
	if (bIsDead)
	{
		return;
	}

	++HitCount;

	DrawDebugString(GetWorld(), HitResult.ImpactPoint + FVector(0.0f, 0.0f, 30.0f), FString::Printf(TEXT("Hit %d"), HitCount), nullptr, FColor::Yellow, 1.5f, true);

	UE_LOG(LogTemp, Log, TEXT("Overdrive target dummy hit. Target=%s Instigator=%s HitCount=%d"), *GetNameSafe(this), *GetNameSafe(InstigatorActor), HitCount);
}

void AOverdriveTargetDummy::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	if (!HasAuthority())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Overdrive target dummy health changed. Target=%s Old=%.2f New=%.2f"), *GetNameSafe(this), ChangeData.OldValue, ChangeData.NewValue);

	if (ChangeData.NewValue <= 0.0f)
	{
		HandleDeath();
	}
}

void AOverdriveTargetDummy::HandleDeath()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	ApplyDeathState();

	ForceNetUpdate();
	UE_LOG(LogTemp, Log, TEXT("Overdrive target dummy dead. Target=%s"), *GetNameSafe(this));
}

void AOverdriveTargetDummy::ApplyDeathState()
{
	if (TargetMeshComponent)
	{
		TargetMeshComponent->SetCollisionEnabled(bIsDead ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
		TargetMeshComponent->SetHiddenInGame(bIsDead);
	}
}

void AOverdriveTargetDummy::OnRep_IsDead()
{
	ApplyDeathState();
}

void AOverdriveTargetDummy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverdriveTargetDummy, bIsDead);
}
