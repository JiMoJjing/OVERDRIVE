// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdrivePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "OverdrivePlayerState.h"
#include "../Lobby/OverdriveLobbyGameMode.h"
#include "../UI/OverdriveLobbyWidget.h"

void AOverdrivePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() && ShouldShowLobbyWidget())
	{
		ShowLobbyWidget();
	}
}

void AOverdrivePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	RefreshLobbyWidgetBindings();
}

void AOverdrivePlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	if (IsLocalController())
	{
		HandlePostSeamlessTravelOnClient();
	}
	else if (HasAuthority())
	{
		ClientHandlePostSeamlessTravel();
	}
}

void AOverdrivePlayerController::RequestSetReady(bool bNewReady)
{
	ServerSetReady(bNewReady);
}

void AOverdrivePlayerController::RequestStartMission()
{
	ServerRequestStartMission();
}

void AOverdrivePlayerController::ShowLobbyWidget()
{
	if (!IsLocalController() || !ShouldShowLobbyWidget() || IsValid(LobbyWidget))
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
	RefreshLobbyWidgetBindings();
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

void AOverdrivePlayerController::ServerRequestStartMission_Implementation()
{
	AOverdriveLobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<AOverdriveLobbyGameMode>();
	if (IsValid(LobbyGameMode))
	{
		LobbyGameMode->RequestStartMission(this);
	}
}

void AOverdrivePlayerController::ClientHandlePostSeamlessTravel_Implementation()
{
	HandlePostSeamlessTravelOnClient();
}

void AOverdrivePlayerController::HandlePostSeamlessTravelOnClient()
{
	HideLobbyWidget();
}

void AOverdrivePlayerController::RefreshLobbyWidgetBindings()
{
	UOverdriveLobbyWidget* OverdriveLobbyWidget = Cast<UOverdriveLobbyWidget>(LobbyWidget);
	if (IsValid(OverdriveLobbyWidget))
	{
		OverdriveLobbyWidget->RefreshLobbyBindings();
	}
}

bool AOverdrivePlayerController::ShouldShowLobbyWidget() const
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	return World->GetMapName().Contains(TEXT("Lobby"));
}
