// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveCharacter.h"

#include "../Camera/OverdriveCameraComponent.h"
#include "../Player/OverdrivePlayerState.h"
#include "../Weapons/OverdriveWeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

AOverdriveCharacter::AOverdriveCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	ApplyCharacterRotationMode();

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 100.0f, 75.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UOverdriveCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AOverdriveCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	AddDefaultInputMappingContext();
}

void AOverdriveCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilityActorInfo();

	SpawnDefaultWeapon();
	GrantDefaultAbilities();
}

void AOverdriveCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilityActorInfo();
}

void AOverdriveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOverdriveCharacter::Move);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOverdriveCharacter::Look);
	}

	if (AimAction)
	{
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AOverdriveCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AOverdriveCharacter::StopAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Canceled, this, &AOverdriveCharacter::StopAim);
	}

	if (FireAction)
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AOverdriveCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AOverdriveCharacter::StopFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &AOverdriveCharacter::StopFire);
	}
}

AOverdriveWeaponBase* AOverdriveCharacter::GetEquippedWeapon() const
{
	return EquippedWeapon;
}

bool AOverdriveCharacter::IsAiming() const
{
	return bIsAiming;
}

void AOverdriveCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller || MovementVector.IsNearlyZero())
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AOverdriveCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!Controller || LookAxisVector.IsNearlyZero())
	{
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AOverdriveCharacter::StartAim()
{
	bWantsToAim = true;
	SetAiming(true);
	RefreshDesiredRotationMode();

	if (FollowCamera)
	{
		FollowCamera->SetAimingCameraMode(true);
	}
}

void AOverdriveCharacter::StopAim()
{
	bWantsToAim = false;
	SetAiming(false);
	RefreshDesiredRotationMode();

	if (FollowCamera)
	{
		FollowCamera->SetAimingCameraMode(false);
	}
}

void AOverdriveCharacter::StartFire()
{
	bWantsToFire = true;
	RefreshDesiredRotationMode();

	ActivateWeaponFireAbility();
}

void AOverdriveCharacter::StopFire()
{
	bWantsToFire = false;
	RefreshDesiredRotationMode();
}

void AOverdriveCharacter::RefreshDesiredRotationMode()
{
	const EOverdriveCharacterRotationMode DesiredRotationMode = (bWantsToAim || bWantsToFire)
		? EOverdriveCharacterRotationMode::ControllerYaw
		: EOverdriveCharacterRotationMode::MovementDirection;

	SetCharacterRotationMode(DesiredRotationMode);
}

void AOverdriveCharacter::SetCharacterRotationMode(EOverdriveCharacterRotationMode NewRotationMode)
{
	if (RotationMode == NewRotationMode)
	{
		return;
	}

	RotationMode = NewRotationMode;
	ApplyCharacterRotationMode();
}

void AOverdriveCharacter::ApplyCharacterRotationMode()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (!CharacterMovementComponent)
	{
		return;
	}

	const bool bUseControllerYawRotation = RotationMode == EOverdriveCharacterRotationMode::ControllerYaw;

	bUseControllerRotationYaw = bUseControllerYawRotation;
	CharacterMovementComponent->bOrientRotationToMovement = !bUseControllerYawRotation;
}

void AOverdriveCharacter::AddDefaultInputMappingContext() const
{
	if (!DefaultMappingContext)
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->AddMappingContext(DefaultMappingContext, DefaultMappingPriority);
}

void AOverdriveCharacter::InitializeAbilityActorInfo()
{
	AOverdrivePlayerState* OverdrivePlayerState = GetPlayerState<AOverdrivePlayerState>();
	if (!OverdrivePlayerState)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = OverdrivePlayerState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(OverdrivePlayerState, this);
}

void AOverdriveCharacter::SpawnDefaultWeapon()
{
	if (!HasAuthority() || !DefaultWeaponClass || EquippedWeapon)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOverdriveWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AOverdriveWeaponBase>(DefaultWeaponClass, GetActorTransform(), SpawnParameters);
	if (!SpawnedWeapon)
	{
		return;
	}

	EquipWeapon(SpawnedWeapon);
}

void AOverdriveCharacter::EquipWeapon(AOverdriveWeaponBase* NewWeapon)
{
	if (!HasAuthority() || !NewWeapon)
	{
		return;
	}

	EquippedWeapon = NewWeapon;
	EquippedWeapon->SetOwner(this);
	AttachEquippedWeapon();

	EquippedWeapon->ForceNetUpdate();
	ForceNetUpdate();
}

void AOverdriveCharacter::AttachEquippedWeapon() const
{
	if (!EquippedWeapon || !GetMesh())
	{
		return;
	}

	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
}

void AOverdriveCharacter::GrantDefaultAbilities()
{
	if (!HasAuthority() || !DefaultWeaponFireAbility)
	{
		return;
	}

	AOverdrivePlayerState* OverdrivePlayerState = GetPlayerState<AOverdrivePlayerState>();
	if (!OverdrivePlayerState)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = OverdrivePlayerState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent || AbilitySystemComponent->FindAbilitySpecFromClass(DefaultWeaponFireAbility))
	{
		return;
	}

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultWeaponFireAbility, 1, INDEX_NONE, this));
}

void AOverdriveCharacter::ActivateWeaponFireAbility()
{
	if (!DefaultWeaponFireAbility)
	{
		return;
	}

	const AOverdrivePlayerState* OverdrivePlayerState = GetPlayerState<AOverdrivePlayerState>();
	if (!OverdrivePlayerState)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = OverdrivePlayerState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->TryActivateAbilityByClass(DefaultWeaponFireAbility);
}

void AOverdriveCharacter::SetAiming(bool bNewIsAiming)
{
	if (bIsAiming == bNewIsAiming)
	{
		return;
	}

	bIsAiming = bNewIsAiming;

	if (HasAuthority())
	{
		ForceNetUpdate();
	}
	else
	{
		ServerSetAiming(bNewIsAiming);
	}
}

void AOverdriveCharacter::ServerSetAiming_Implementation(bool bNewIsAiming)
{
	SetAiming(bNewIsAiming);
}

void AOverdriveCharacter::OnRep_EquippedWeapon()
{
	AttachEquippedWeapon();
}

void AOverdriveCharacter::OnRep_IsAiming()
{
}

void AOverdriveCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverdriveCharacter, EquippedWeapon);
	DOREPLIFETIME(AOverdriveCharacter, bIsAiming);
}
