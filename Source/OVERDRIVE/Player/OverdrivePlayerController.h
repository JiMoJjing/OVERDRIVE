// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverdrivePlayerController.generated.h"

UCLASS()
class OVERDRIVE_API AOverdrivePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void RequestSetReady(bool bNewReady);

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void ShowLobbyWidget();

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void HideLobbyWidget();

private:
	UFUNCTION(Server, Reliable)
	void ServerSetReady(bool bNewReady);

private:
	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Lobby")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> LobbyWidget;
};
