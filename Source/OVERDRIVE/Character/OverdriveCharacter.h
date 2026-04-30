// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "OverdriveCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UOverdriveCameraComponent;
class USpringArmComponent;
class AOverdriveWeaponBase;

UENUM(BlueprintType)
enum class EOverdriveCharacterRotationMode : uint8
{
	MovementDirection,
	ControllerYaw
};

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	AOverdriveWeaponBase* GetEquippedWeapon() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Movement")
	bool IsAiming() const;

protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartAim();

	void StopAim();

	void StartFire();

	void StopFire();

	void RefreshDesiredRotationMode();

	void SetCharacterRotationMode(EOverdriveCharacterRotationMode NewRotationMode);

	void ApplyCharacterRotationMode();

	void AddDefaultInputMappingContext() const;

	void SpawnDefaultWeapon();

	void EquipWeapon(AOverdriveWeaponBase* NewWeapon);

	void AttachEquippedWeapon() const;

	void HandleFire();

	void SetAiming(bool bNewIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bNewIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_IsAiming();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Camera")
	TObjectPtr<UOverdriveCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Input")
	int32 DefaultMappingPriority = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Movement")
	EOverdriveCharacterRotationMode RotationMode = EOverdriveCharacterRotationMode::MovementDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Movement")
	bool bWantsToAim = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsAiming, VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Movement")
	bool bIsAiming = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Movement")
	bool bWantsToFire = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Weapon", meta = (ClampMin = "0.0"))
	float AimTraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Weapon", meta = (ClampMin = "0.0"))
	float FireTraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Weapon")
	TSubclassOf<AOverdriveWeaponBase> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OVERDRIVE|Weapon")
	FName WeaponAttachSocketName = TEXT("Weapon_R");

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AOverdriveWeaponBase> EquippedWeapon;
};
