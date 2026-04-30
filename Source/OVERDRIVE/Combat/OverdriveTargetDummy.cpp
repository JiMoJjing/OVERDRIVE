// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveTargetDummy.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

AOverdriveTargetDummy::AOverdriveTargetDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	TargetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMeshComponent"));
	RootComponent = TargetMeshComponent;
}

void AOverdriveTargetDummy::HandleWeaponHit_Implementation(AActor* InstigatorActor, const FHitResult& HitResult)
{
	++HitCount;

	DrawDebugString(GetWorld(), HitResult.ImpactPoint + FVector(0.0f, 0.0f, 30.0f), FString::Printf(TEXT("Hit %d"), HitCount), nullptr, FColor::Yellow, 1.5f, true);

	UE_LOG(LogTemp, Log, TEXT("Overdrive target dummy hit. Target=%s Instigator=%s HitCount=%d"), *GetNameSafe(this), *GetNameSafe(InstigatorActor), HitCount);
}
