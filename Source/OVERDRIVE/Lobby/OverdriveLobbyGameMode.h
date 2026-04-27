// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverdriveLobbyGameMode.generated.h"

UCLASS()
class OVERDRIVE_API AOverdriveLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverdriveLobbyGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void RefreshLobbySummary();
};
