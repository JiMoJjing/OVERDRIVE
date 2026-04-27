// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OverdrivePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyReadyChanged, bool, bNewReady);

UCLASS()
class OVERDRIVE_API AOverdrivePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AOverdrivePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	bool IsReady() const;

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void SetReady(bool bNewReady);

private:
	UFUNCTION()
	void OnRep_IsReady();

public:
	UPROPERTY(BlueprintAssignable, Category = "OVERDRIVE|Lobby")
	FOnLobbyReadyChanged OnLobbyReadyChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady = false;
};
