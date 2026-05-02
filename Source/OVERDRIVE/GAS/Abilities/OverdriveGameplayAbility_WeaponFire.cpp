// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveGameplayAbility_WeaponFire.h"

#include "../../Character/OverdriveCharacter.h"
#include "../OverdriveAttributeSet.h"
#include "../../Weapons/OverdriveWeaponBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameplayCueManager.h"
#include "GameplayTagsManager.h"
#include "TimerManager.h"

UOverdriveGameplayAbility_WeaponFire::UOverdriveGameplayAbility_WeaponFire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ImpactGameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Weapon.Impact"), false);
}

void UOverdriveGameplayAbility_WeaponFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	StartFireRateTimer();

	if (ActorInfo && ActorInfo->IsLocallyControlled())
	{
		SendClientTargetData(Handle, ActorInfo, ActivationInfo);
		return;
	}

	if (HasAuthority(&ActivationInfo))
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &UOverdriveGameplayAbility_WeaponFire::OnTargetDataReadyCallback);
		if (!AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(Handle, ActivationInfo.GetActivationPredictionKey()))
		{
			return;
		}

		return;
	}
	
	return;
}

void UOverdriveGameplayAbility_WeaponFire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FireRateTimerHandle);
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).RemoveAll(this);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOverdriveGameplayAbility_WeaponFire::StartFireRateTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	const float FireRateDelay = FMath::Max(FireRate, 0.01f);
	World->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UOverdriveGameplayAbility_WeaponFire::HandleFireRateElapsed, FireRateDelay, false);
}

void UOverdriveGameplayAbility_WeaponFire::HandleFireRateElapsed()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOverdriveGameplayAbility_WeaponFire::SendClientTargetData(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FGameplayAbilityTargetDataHandle TargetDataHandle = MakeClientTargetData(ActorInfo);
	if (TargetDataHandle.Num() <= 0)
	{
		return;
	}

	if (HasAuthority(&ActivationInfo))
	{
		if (AOverdriveCharacter* OverdriveCharacter = Cast<AOverdriveCharacter>(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr))
		{
			const FHitResult* HitResult = TargetDataHandle.Get(0) ? TargetDataHandle.Get(0)->GetHitResult() : nullptr;
			if (HitResult)
			{
				PerformServerValidatedTrace(OverdriveCharacter, HitResult->TraceStart, (HitResult->TraceEnd - HitResult->TraceStart).GetSafeNormal());
			}
		}
		return;
	}

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent);
	AbilitySystemComponent->CallServerSetReplicatedTargetData(Handle, ActivationInfo.GetActivationPredictionKey(), TargetDataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);
}

void UOverdriveGameplayAbility_WeaponFire::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

	AOverdriveCharacter* OverdriveCharacter = Cast<AOverdriveCharacter>(GetAvatarActorFromActorInfo());
	const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(0);
	const FHitResult* HitResult = TargetData ? TargetData->GetHitResult() : nullptr;
	if (!OverdriveCharacter || !HitResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_WeaponFire received invalid target data."));
		return;
	}

	PerformServerValidatedTrace(OverdriveCharacter, HitResult->TraceStart, (HitResult->TraceEnd - HitResult->TraceStart).GetSafeNormal());
}

FGameplayAbilityTargetDataHandle UOverdriveGameplayAbility_WeaponFire::MakeClientTargetData(const FGameplayAbilityActorInfo* ActorInfo) const
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return TargetDataHandle;
	}

	AOverdriveCharacter* OverdriveCharacter = Cast<AOverdriveCharacter>(ActorInfo->AvatarActor.Get());
	if (!OverdriveCharacter)
	{
		return TargetDataHandle;
	}

	AOverdriveWeaponBase* EquippedWeapon = OverdriveCharacter->GetEquippedWeapon();
	AController* Controller = OverdriveCharacter->GetController();
	UWorld* World = OverdriveCharacter->GetWorld();
	if (!EquippedWeapon || !Controller || !World)
	{
		return TargetDataHandle;
	}
	if (World->GetNetMode() == NM_DedicatedServer)
	{
		return TargetDataHandle;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(OverdriveGAWeaponFire), false, OverdriveCharacter);
	QueryParams.AddIgnoredActor(OverdriveCharacter);
	QueryParams.AddIgnoredActor(EquippedWeapon);

	const FVector AimTraceStart = ViewLocation;
	const FVector AimTraceEnd = AimTraceStart + ViewRotation.Vector() * AimTraceDistance;

	FHitResult AimHitResult;
	const bool bAimHit = World->LineTraceSingleByChannel(AimHitResult, AimTraceStart, AimTraceEnd, ECC_Visibility, QueryParams);
	const FVector AimTargetPoint = bAimHit ? AimHitResult.ImpactPoint : AimTraceEnd;

	const FVector FireTraceStart = EquippedWeapon->GetMuzzleTransform().GetLocation();
	FVector FireTraceDirection = (AimTargetPoint - FireTraceStart).GetSafeNormal();
	if (FireTraceDirection.IsNearlyZero())
	{
		FireTraceDirection = ViewRotation.Vector();
	}

	const FVector FireTraceEnd = FireTraceStart + FireTraceDirection * FireTraceDistance;

	FHitResult FireHitResult;
	const bool bFireHit = World->LineTraceSingleByChannel(FireHitResult, FireTraceStart, FireTraceEnd, ECC_Visibility, QueryParams);

	DrawDebugLine(World, AimTraceStart, AimTargetPoint, FColor::Blue, false, 2.0f, 0, 1.0f);
	DrawDebugSphere(World, AimTargetPoint, 10.0f, 12, FColor::Blue, false, 2.0f);

	const FVector FireDebugEnd = bFireHit ? FireHitResult.ImpactPoint : FireTraceEnd;
	DrawDebugLine(World, FireTraceStart, FireDebugEnd, FColor::Green, false, 2.0f, 0, 1.5f);
	if (bFireHit)
	{
		DrawDebugSphere(World, FireHitResult.ImpactPoint, 12.0f, 12, FColor::Green, false, 2.0f);
	}

	FHitResult AimTargetDataHit = AimHitResult;
	AimTargetDataHit.TraceStart = AimTraceStart;
	AimTargetDataHit.TraceEnd = AimTraceEnd;
	AimTargetDataHit.Location = AimTargetPoint;
	AimTargetDataHit.ImpactPoint = AimTargetPoint;

	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(AimTargetDataHit));

	UE_LOG(LogTemp, Log, TEXT("GA_WeaponFire client predicted trace. Hit=%s Actor=%s AimTarget=%s FireEnd=%s"),
		bFireHit ? TEXT("true") : TEXT("false"),
		*GetNameSafe(FireHitResult.GetActor()),
		*AimTargetPoint.ToCompactString(),
		*FireDebugEnd.ToCompactString());

	return TargetDataHandle;
}

void UOverdriveGameplayAbility_WeaponFire::PerformServerValidatedTrace(AOverdriveCharacter* OverdriveCharacter, const FVector& ClientAimStart, const FVector& ClientAimDirection) const
{
	if (!OverdriveCharacter || !OverdriveCharacter->HasAuthority())
	{
		return;
	}

	AController* Controller = OverdriveCharacter->GetController();
	UWorld* World = OverdriveCharacter->GetWorld();
	if (!Controller || !World)
	{
		return;
	}

	FVector ServerViewLocation;
	FRotator ServerViewRotation;
	Controller->GetPlayerViewPoint(ServerViewLocation, ServerViewRotation);

	const FVector SafeAimDirection = ClientAimDirection.GetSafeNormal();
	if (SafeAimDirection.IsNearlyZero())
	{
		return;
	}

	constexpr float MaxAimStartDistanceFromServerView = 300.0f;
	constexpr float MaxAimAngleDeltaDegrees = 15.0f;
	constexpr float ServerTraceDistance = 10000.0f;

	if (FVector::DistSquared(ClientAimStart, ServerViewLocation) > FMath::Square(MaxAimStartDistanceFromServerView))
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_WeaponFire server rejected fire: aim start too far. Client=%s Server=%s"),
			*ClientAimStart.ToCompactString(),
			*ServerViewLocation.ToCompactString());
		return;
	}

	const float AimDot = FVector::DotProduct(ServerViewRotation.Vector(), SafeAimDirection);
	const float MinAllowedAimDot = FMath::Cos(FMath::DegreesToRadians(MaxAimAngleDeltaDegrees));
	if (AimDot < MinAllowedAimDot)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_WeaponFire server rejected fire: aim angle too wide. Dot=%f"), AimDot);
		return;
	}

	AOverdriveWeaponBase* EquippedWeapon = OverdriveCharacter->GetEquippedWeapon();
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(OverdriveGAWeaponFireServer), false, OverdriveCharacter);
	QueryParams.AddIgnoredActor(OverdriveCharacter);
	if (EquippedWeapon)
	{
		QueryParams.AddIgnoredActor(EquippedWeapon);
	}

	const FVector TraceStart = ServerViewLocation;
	const FVector TraceEnd = TraceStart + SafeAimDirection * ServerTraceDistance;

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	const FVector DebugEnd = bHit ? HitResult.ImpactPoint : TraceEnd;

	DrawDebugLine(World, TraceStart, DebugEnd, FColor::Red, false, 2.0f, 0, 1.5f);
	if (bHit)
	{
		DrawDebugSphere(World, HitResult.ImpactPoint, 12.0f, 12, FColor::Red, false, 2.0f);
		ExecuteImpactGameplayCue(HitResult);
		ApplyDamageEffectToHitActor(HitResult.GetActor());
	}

	UE_LOG(LogTemp, Log, TEXT("GA_WeaponFire server validated trace. Hit=%s Actor=%s Start=%s End=%s"),
		bHit ? TEXT("true") : TEXT("false"),
		*GetNameSafe(HitResult.GetActor()),
		*TraceStart.ToCompactString(),
		*DebugEnd.ToCompactString());
}

void UOverdriveGameplayAbility_WeaponFire::ExecuteImpactGameplayCue(const FHitResult& HitResult) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!SourceAbilitySystemComponent || !ImpactGameplayCueTag.IsValid())
	{
		return;
	}

	FGameplayCueParameters CueParameters;
	CueParameters.Location = HitResult.ImpactPoint;
	CueParameters.Normal = HitResult.ImpactNormal;
	CueParameters.Instigator = GetAvatarActorFromActorInfo();
	CueParameters.EffectCauser = GetAvatarActorFromActorInfo();
	CueParameters.SourceObject = this;
	CueParameters.PhysicalMaterial = HitResult.PhysMaterial;

	SourceAbilitySystemComponent->ExecuteGameplayCue(ImpactGameplayCueTag, CueParameters);

	if (const UWorld* World = GetWorld())
	{
		DrawDebugSphere(World, HitResult.ImpactPoint, 18.0f, 16, FColor::Orange, false, 2.0f, 0, 2.0f);
	}

	UE_LOG(LogTemp, Log, TEXT("GA_WeaponFire executed impact GameplayCue. Tag=%s Actor=%s Location=%s Normal=%s"),
		*ImpactGameplayCueTag.ToString(),
		*GetNameSafe(HitResult.GetActor()),
		*HitResult.ImpactPoint.ToCompactString(),
		*HitResult.ImpactNormal.ToCompactString());
}

void UOverdriveGameplayAbility_WeaponFire::ApplyDamageEffectToHitActor(AActor* HitActor) const
{
	if (!DamageEffectClass || !HitActor)
	{
		return;
	}

	UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
	if (!SourceAbilitySystemComponent || !TargetAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Verbose, TEXT("GA_WeaponFire skipped damage GE. Target has no ASC. Target=%s"), *GetNameSafe(HitActor));
		return;
	}

	const UOverdriveAttributeSet* TargetAttributeSet = TargetAbilitySystemComponent->GetSet<UOverdriveAttributeSet>();
	if (TargetAttributeSet && TargetAttributeSet->GetHealth() <= 0.0f)
	{
		UE_LOG(LogTemp, Verbose, TEXT("GA_WeaponFire skipped damage GE. Target is already dead. Target=%s"), *GetNameSafe(HitActor));
		return;
	}

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	if (!DamageSpecHandle.IsValid())
	{
		return;
	}

	SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetAbilitySystemComponent);
	UE_LOG(LogTemp, Log, TEXT("GA_WeaponFire applied damage GE. Target=%s Effect=%s"), *GetNameSafe(HitActor), *GetNameSafe(DamageEffectClass));
}
