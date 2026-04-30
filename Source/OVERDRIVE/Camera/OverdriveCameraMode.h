// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OverdriveCameraMode.generated.h"

USTRUCT(BlueprintType)
struct FOverdriveCameraModeView
{
	GENERATED_BODY()

public:
	void Blend(const FOverdriveCameraModeView& Other, float Weight);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	float TargetArmLength = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	FVector SocketOffset = FVector(0.0f, 100.0f, 75.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	float FieldOfView = 90.0f;
};

UCLASS(Abstract, Blueprintable)
class OVERDRIVE_API UOverdriveCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UOverdriveCameraMode();

	virtual void UpdateCameraMode(float DeltaTime);

	const FOverdriveCameraModeView& GetCameraModeView() const;

	float GetBlendTime() const;

	float GetBlendWeight() const;

	void SetBlendWeight(float NewBlendWeight);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	FOverdriveCameraModeView CameraModeView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera", meta = (ClampMin = "0.0"))
	float BlendTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera", meta = (ClampMin = "0.1"))
	float BlendExponent = 4.0f;

private:
	float BlendAlpha = 1.0f;

	float BlendWeight = 1.0f;
};

UCLASS()
class OVERDRIVE_API UOverdriveCameraMode_Default : public UOverdriveCameraMode
{
	GENERATED_BODY()

public:
	UOverdriveCameraMode_Default();
};

UCLASS()
class OVERDRIVE_API UOverdriveCameraMode_Aiming : public UOverdriveCameraMode
{
	GENERATED_BODY()

public:
	UOverdriveCameraMode_Aiming();
};
