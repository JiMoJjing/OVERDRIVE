// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdrivePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "OverdrivePlayerState.h"
#include "../Lobby/OverdriveLobbyGameMode.h"

void AOverdrivePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		ShowLobbyWidget();
	}
}

void AOverdrivePlayerController::RequestSetReady(bool bNewReady)
{
	ServerSetReady(bNewReady);
}

void AOverdrivePlayerController::ShowLobbyWidget()
{
	if (!IsLocalController() || IsValid(LobbyWidget))
	{
		return;
	}

	if (!LobbyWidgetClass)
	{
		LobbyWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WBP_LobbyTest.WBP_LobbyTest_C"));
	}

	if (!LobbyWidgetClass)
	{
		return;
	}

	LobbyWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
	if (!IsValid(LobbyWidget))
	{
		return;
	}

	LobbyWidget->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

void AOverdrivePlayerController::HideLobbyWidget()
{
	if (IsValid(LobbyWidget))
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}
}

void AOverdrivePlayerController::ServerSetReady_Implementation(bool bNewReady)
{
	AOverdrivePlayerState* OverdrivePlayerState = GetPlayerState<AOverdrivePlayerState>();
	if (!IsValid(OverdrivePlayerState))
	{
		return;
	}

	OverdrivePlayerState->SetReady(bNewReady);

	AOverdriveLobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<AOverdriveLobbyGameMode>();
	if (IsValid(LobbyGameMode))
	{
		LobbyGameMode->RefreshLobbySummary();
	}
}
