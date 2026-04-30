// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveWeaponBase.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AOverdriveWeaponBase::AOverdriveWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(false);

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(SceneRoot);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(WeaponMeshComponent, TEXT("Muzzle"));
}

UStaticMeshComponent* AOverdriveWeaponBase::GetWeaponMeshComponent() const
{
	return WeaponMeshComponent;
}

USceneComponent* AOverdriveWeaponBase::GetMuzzleComponent() const
{
	return MuzzleComponent;
}

FTransform AOverdriveWeaponBase::GetMuzzleTransform() const
{
	return MuzzleComponent ? MuzzleComponent->GetComponentTransform() : GetActorTransform();
}
