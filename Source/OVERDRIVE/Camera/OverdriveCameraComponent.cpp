// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveCameraComponent.h"

#include "OverdriveCameraModeStack.h"
#include "GameFramework/SpringArmComponent.h"

UOverdriveCameraComponent::UOverdriveCameraComponent()
{
	DefaultCameraModeClass = UOverdriveCameraMode_Default::StaticClass();
	AimingCameraModeClass = UOverdriveCameraMode_Aiming::StaticClass();
}

void UOverdriveCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	EnsureCameraModeStack();
	if (!CameraModeStack)
	{
		return;
	}

	if (bWantsAimingCameraMode)
	{
		CameraModeStack->PushCameraMode(AimingCameraModeClass);
	}
	else
	{
		CameraModeStack->PopCameraMode(AimingCameraModeClass);
	}

	FOverdriveCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);
	ApplyCameraModeView(CameraModeView, DesiredView);
}

void UOverdriveCameraComponent::SetAimingCameraMode(bool bEnableAimingCamera)
{
	bWantsAimingCameraMode = bEnableAimingCamera;
}

void UOverdriveCameraComponent::EnsureCameraModeStack()
{
	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UOverdriveCameraModeStack>(this);
		CameraModeStack->PushCameraMode(DefaultCameraModeClass);
	}
}

void UOverdriveCameraComponent::ApplyCameraModeView(const FOverdriveCameraModeView& CameraModeView, FMinimalViewInfo& DesiredView)
{
	FieldOfView = CameraModeView.FieldOfView;
	DesiredView.FOV = CameraModeView.FieldOfView;

	USpringArmComponent* SpringArmComponent = Cast<USpringArmComponent>(GetAttachParent());
	if (!SpringArmComponent)
	{
		return;
	}

	SpringArmComponent->TargetArmLength = CameraModeView.TargetArmLength;
	SpringArmComponent->SocketOffset = CameraModeView.SocketOffset;
}
