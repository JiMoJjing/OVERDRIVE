// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverdriveWeaponBase.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS(Blueprintable)
class OVERDRIVE_API AOverdriveWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AOverdriveWeaponBase();

	UStaticMeshComponent* GetWeaponMeshComponent() const;

	USceneComponent* GetMuzzleComponent() const;

	FTransform GetMuzzleTransform() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Weapon")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Weapon")
	TObjectPtr<USceneComponent> MuzzleComponent;
};
