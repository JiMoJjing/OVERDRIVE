// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverdriveHitReactInterface.h"
#include "OverdriveTargetDummy.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class OVERDRIVE_API AOverdriveTargetDummy : public AActor, public IOverdriveHitReactInterface
{
	GENERATED_BODY()

public:
	AOverdriveTargetDummy();

	virtual void HandleWeaponHit_Implementation(AActor* InstigatorActor, const FHitResult& HitResult) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Target")
	TObjectPtr<UStaticMeshComponent> TargetMeshComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Target")
	int32 HitCount = 0;
};
