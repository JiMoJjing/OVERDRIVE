// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "OverdriveCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class OVERDRIVE_API AOverdriveCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOverdriveCharacter();

	virtual void PawnClientRestart() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void AddDefaultInputMappingContext() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	int32 DefaultMappingPriority = 0;
};
