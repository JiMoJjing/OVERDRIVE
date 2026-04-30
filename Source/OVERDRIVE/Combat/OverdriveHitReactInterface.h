// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OverdriveHitReactInterface.generated.h"

UINTERFACE(BlueprintType)
class OVERDRIVE_API UOverdriveHitReactInterface : public UInterface
{
	GENERATED_BODY()
};

class OVERDRIVE_API IOverdriveHitReactInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "OVERDRIVE|HitReact")
	void HandleWeaponHit(AActor* InstigatorActor, const FHitResult& HitResult);
};
