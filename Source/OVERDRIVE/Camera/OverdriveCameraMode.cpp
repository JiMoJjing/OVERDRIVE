// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveCameraMode.h"

void FOverdriveCameraModeView::Blend(const FOverdriveCameraModeView& Other, float Weight)
{
	const float ClampedWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	TargetArmLength = FMath::Lerp(TargetArmLength, Other.TargetArmLength, ClampedWeight);
	SocketOffset = FMath::Lerp(SocketOffset, Other.SocketOffset, ClampedWeight);
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, ClampedWeight);
}

UOverdriveCameraMode::UOverdriveCameraMode()
{
}

void UOverdriveCameraMode::UpdateCameraMode(float DeltaTime)
{
	if (BlendTime <= 0.0f)
	{
		BlendAlpha = 1.0f;
		BlendWeight = 1.0f;
		return;
	}

	const float BlendDelta = DeltaTime / BlendTime;
	BlendAlpha = FMath::Clamp(BlendAlpha + BlendDelta, 0.0f, 1.0f);
	BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, BlendExponent);
}

const FOverdriveCameraModeView& UOverdriveCameraMode::GetCameraModeView() const
{
	return CameraModeView;
}

float UOverdriveCameraMode::GetBlendTime() const
{
	return BlendTime;
}

float UOverdriveCameraMode::GetBlendWeight() const
{
	return BlendWeight;
}

void UOverdriveCameraMode::SetBlendWeight(float NewBlendWeight)
{
	BlendWeight = FMath::Clamp(NewBlendWeight, 0.0f, 1.0f);
	BlendAlpha = BlendWeight;
}

UOverdriveCameraMode_Default::UOverdriveCameraMode_Default()
{
	CameraModeView.TargetArmLength = 300.0f;
	CameraModeView.SocketOffset = FVector(0.0f, 100.0f, 75.0f);
	CameraModeView.FieldOfView = 90.0f;
	BlendTime = 0.0f;
}

UOverdriveCameraMode_Aiming::UOverdriveCameraMode_Aiming()
{
	CameraModeView.TargetArmLength = 100.0f;
	CameraModeView.SocketOffset = FVector(0.0f, 50.0f, 75.0f);
	CameraModeView.FieldOfView = 90.0f;
	BlendTime = 0.2f;
	BlendExponent = 4.0f;
}
