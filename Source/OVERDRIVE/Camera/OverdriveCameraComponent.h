// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "OverdriveCameraMode.h"
#include "OverdriveCameraComponent.generated.h"

class UOverdriveCameraModeStack;

UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent))
class OVERDRIVE_API UOverdriveCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UOverdriveCameraComponent();

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	void SetAimingCameraMode(bool bEnableAimingCamera);

private:
	void EnsureCameraModeStack();

	void ApplyCameraModeView(const FOverdriveCameraModeView& CameraModeView, FMinimalViewInfo& DesiredView);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOverdriveCameraMode> DefaultCameraModeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOverdriveCameraMode> AimingCameraModeClass;

	UPROPERTY(Transient)
	TObjectPtr<UOverdriveCameraModeStack> CameraModeStack;

	bool bWantsAimingCameraMode = false;
};
