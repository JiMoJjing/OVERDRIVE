// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveCameraModeStack.h"

void UOverdriveCameraModeStack::PushCameraMode(TSubclassOf<UOverdriveCameraMode> CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	if (CameraModeStack.Num() > 0 && CameraModeStack.Last()->GetClass() == CameraModeClass)
	{
		return;
	}

	StartTransitionFromCurrentView();

	if (UOverdriveCameraMode* ExistingCameraMode = FindCameraModeInstance(CameraModeClass))
	{
		CameraModeStack.Remove(ExistingCameraMode);
		ExistingCameraMode->SetBlendWeight(0.0f);
		CameraModeStack.Add(ExistingCameraMode);
		TransitionBlendTime = ExistingCameraMode->GetBlendTime();
		return;
	}

	UOverdriveCameraMode* NewCameraMode = NewObject<UOverdriveCameraMode>(this, CameraModeClass);
	if (!NewCameraMode)
	{
		return;
	}

	NewCameraMode->SetBlendWeight(0.0f);
	CameraModeStack.Add(NewCameraMode);
	TransitionBlendTime = NewCameraMode->GetBlendTime();
}

void UOverdriveCameraModeStack::PopCameraMode(TSubclassOf<UOverdriveCameraMode> CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	for (int32 Index = CameraModeStack.Num() - 1; Index >= 0; --Index)
	{
		UOverdriveCameraMode* CameraMode = CameraModeStack[Index];
		if (CameraMode && CameraMode->GetClass() == CameraModeClass)
		{
			StartTransitionFromCurrentView();
			TransitionBlendTime = CameraMode->GetBlendTime();
			CameraModeStack.RemoveAt(Index);
			return;
		}
	}
}

void UOverdriveCameraModeStack::EvaluateStack(float DeltaTime, FOverdriveCameraModeView& OutCameraModeView)
{
	for (UOverdriveCameraMode* CameraMode : CameraModeStack)
	{
		if (CameraMode)
		{
			CameraMode->UpdateCameraMode(DeltaTime);
		}
	}

	BlendStack(OutCameraModeView);

	if (!bHasTransitionSourceView)
	{
		return;
	}

	if (TransitionBlendTime <= 0.0f)
	{
		bHasTransitionSourceView = false;
		return;
	}

	TransitionBlendElapsedTime = FMath::Min(TransitionBlendElapsedTime + DeltaTime, TransitionBlendTime);
	const float BlendWeight = TransitionBlendElapsedTime / TransitionBlendTime;

	FOverdriveCameraModeView BlendedView = TransitionSourceView;
	BlendedView.Blend(OutCameraModeView, BlendWeight);
	OutCameraModeView = BlendedView;

	if (TransitionBlendElapsedTime >= TransitionBlendTime)
	{
		bHasTransitionSourceView = false;
	}
}

UOverdriveCameraMode* UOverdriveCameraModeStack::FindCameraModeInstance(TSubclassOf<UOverdriveCameraMode> CameraModeClass) const
{
	for (UOverdriveCameraMode* CameraMode : CameraModeStack)
	{
		if (CameraMode && CameraMode->GetClass() == CameraModeClass)
		{
			return CameraMode;
		}
	}

	return nullptr;
}

void UOverdriveCameraModeStack::BlendStack(FOverdriveCameraModeView& OutCameraModeView) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutCameraModeView = FOverdriveCameraModeView();
		return;
	}

	OutCameraModeView = CameraModeStack[0]->GetCameraModeView();

	for (int32 Index = 1; Index < CameraModeStack.Num(); ++Index)
	{
		const UOverdriveCameraMode* CameraMode = CameraModeStack[Index];
		if (CameraMode)
		{
			OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
		}
	}
}

void UOverdriveCameraModeStack::StartTransitionFromCurrentView()
{
	if (CameraModeStack.Num() == 0)
	{
		bHasTransitionSourceView = false;
		TransitionBlendElapsedTime = 0.0f;
		TransitionBlendTime = 0.0f;
		return;
	}

	BlendStack(TransitionSourceView);
	bHasTransitionSourceView = true;
	TransitionBlendElapsedTime = 0.0f;
}
