// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OverdriveCameraMode.h"
#include "UObject/Object.h"
#include "OverdriveCameraModeStack.generated.h"

UCLASS()
class OVERDRIVE_API UOverdriveCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	void PushCameraMode(TSubclassOf<UOverdriveCameraMode> CameraModeClass);

	void PopCameraMode(TSubclassOf<UOverdriveCameraMode> CameraModeClass);

	void EvaluateStack(float DeltaTime, FOverdriveCameraModeView& OutCameraModeView);

private:
	UOverdriveCameraMode* FindCameraModeInstance(TSubclassOf<UOverdriveCameraMode> CameraModeClass) const;

	void BlendStack(FOverdriveCameraModeView& OutCameraModeView) const;

	void StartTransitionFromCurrentView();

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UOverdriveCameraMode>> CameraModeStack;

	FOverdriveCameraModeView TransitionSourceView;

	float TransitionBlendTime = 0.0f;

	float TransitionBlendElapsedTime = 0.0f;

	bool bHasTransitionSourceView = false;
};
