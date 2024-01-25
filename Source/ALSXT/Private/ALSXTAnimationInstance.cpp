// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "ALS/Public/Utility/AlsMacros.h"
#include "Math/UnrealMathUtility.h"
#include "Stats/Stats.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimationInstance)

UALSXTAnimationInstance::UALSXTAnimationInstance()
{
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UALSXTAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ALSXTCharacter = Cast<AALSXTCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacter))
	{
		// Use default objects for editor preview.

		ALSXTCharacter = GetMutableDefault<AALSXTCharacter>();
	}
#endif
}

void UALSXTAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ALS_ENSURE(IsValid(ALSXTSettings));
	ALS_ENSURE(IsValid(ALSXTCharacter));
	StaminaThresholdSettings = ALSXTCharacter->ALSXTSettings->StatusSettings.StaminaThresholdSettings;
	CharacterBreathEffectsSettings = ALSXTCharacter->ALSXTSettings->BreathEffects;
}

void UALSXTAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTCharacter))
	{
		return;
	}

	Freelooking = ALSXTCharacter->GetDesiredFreelooking();
	Sex = ALSXTCharacter->GetDesiredSex();
	DefensiveMode = ALSXTCharacter->GetDesiredDefensiveMode();
	LocomotionVariant = ALSXTCharacter->GetDesiredLocomotionVariant();
	Injury = ALSXTCharacter->GetDesiredInjury();
	CombatStance = ALSXTCharacter->GetDesiredCombatStance();
	WeaponFirearmStance = ALSXTCharacter->GetDesiredWeaponFirearmStance();
	WeaponReadyPosition = ALSXTCharacter->GetDesiredWeaponReadyPosition();
	StationaryMode = ALSXTCharacter->GetStationaryMode();
	HoldingBreath = ALSXTCharacter->GetHoldingBreath();
	PhysicalAnimationMode = ALSXTCharacter->GetPhysicalAnimationMode();
	Gesture = ALSXTCharacter->GetGesture();
	GestureHand = ALSXTCharacter->GetGestureHand();
	ReloadingType = ALSXTCharacter->GetReloadingType();
	ForegripPosition = ALSXTCharacter->GetDesiredForegripPosition();
	FirearmFingerAction = ALSXTCharacter->GetFirearmFingerAction();
	FirearmFingerActionHand = ALSXTCharacter->GetFirearmFingerActionHand();
	WeaponCarryPosition = ALSXTCharacter->GetWeaponCarryPosition();
	FirearmSightLocation = ALSXTCharacter->GetFirearmSightLocation();
	ForegripTransform = ALSXTCharacter->GetCurrentForegripTransform();
	VaultType = ALSXTCharacter->GetVaultType();
	AimState = ALSXTCharacter->GetAimState();
	FreelookState = ALSXTCharacter->GetFreelookState();
	HeadLookAtState = ALSXTCharacter->GetHeadLookAtState();
	DoesOverlayObjectUseLeftHandIK = ALSXTCharacter->DoesOverlayObjectUseLeftHandIK();

	if (UKismetSystemLibrary::DoesImplementInterface(ALSXTCharacter, UALSXTCharacterInterface::StaticClass()))
	{
		// FALSXTStatusState NewStatusState;
		// NewStatusState.CurrentStatus = IALSXTCharacterInterface::Execute_GetStatus(ALSXTCharacter);
		// NewStatusState.CurrentHealth = IALSXTCharacterInterface::Execute_GetHealth(ALSXTCharacter);
		// NewStatusState.CurrentStamina = IALSXTCharacterInterface::Execute_GetStamina(ALSXTCharacter);
		// BreathState.BreathType = IALSXTCharacterInterface::Execute_GetBreathType(ALSXTCharacter);
		StatusState = IALSXTCharacterInterface::Execute_GetStatusState(ALSXTCharacter);
		CrowdNavigationPoseState = IALSXTCharacterInterface::Execute_GetCrowdNavigationPoseState(ALSXTCharacter);
		BumpPoseState = IALSXTCharacterInterface::Execute_GetBumpPoseState(ALSXTCharacter);
	}

	DefensiveModeState = ALSXTCharacter->GetDefensiveModeState();
	WeaponObstruction = ALSXTCharacter->GetWeaponObstruction();
	BreathState.HoldingBreath = ALSXTCharacter->GetDesiredHoldingBreath();
}

void UALSXTAnimationInstance::NativeThreadSafeUpdateAnimation(const float DeltaTime)
{

	Super::NativeThreadSafeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}

	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		BreathState.TargetState = CalculateTargetBreathState();

		if (ShouldTransitionBreathState())
		{
			TransitionBreathState();
		}
	}
}

void UALSXTAnimationInstance::NativePostEvaluateAnimation()
{

	Super::NativePostEvaluateAnimation();

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}

	bPendingUpdate = false;
}



bool UALSXTAnimationInstance::IsSpineRotationAllowed()
{
	return Super::IsSpineRotationAllowed() && ALSXTCharacter->GetDesiredFreelooking() != ALSXTFreelookingTags::True;
	//return ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming && ALSXTCharacter->GetLocomotionState().bRotationLocked == false;
}

bool UALSXTAnimationInstance::IsRotateInPlaceAllowed()
{
	return Super::IsRotateInPlaceAllowed() && ALSXTCharacter->GetDesiredFreelooking() != ALSXTFreelookingTags::True;
}

bool UALSXTAnimationInstance::IsTurnInPlaceAllowed()
{
	return Super::IsTurnInPlaceAllowed() && ALSXTCharacter->GetDesiredFreelooking() != ALSXTFreelookingTags::True;
}

void UALSXTAnimationInstance::UpdateStatusState()
{
	FALSXTStatusState NewStatusState{ IALSXTCharacterInterface::Execute_GetStatusState(ALSXTCharacter) };
	
	if (NewStatusState != StatusState)
	{
		StatusState = NewStatusState;
		if (StatusState.CurrentStatus == ALSXTStatusTags::Dead)
		{
			BreathState.CurrentBreathRate = 0.0;
			BreathState.CurrentBreathAlpha = 0.0;
			BreathState.TargetState.Alpha = 0.0;
			BreathState.TargetState.Rate = 0.0;
			BreathState.TargetState.TransitionRate = 1.0;
		}
	}
}

void UALSXTAnimationInstance::UpdateBreathState()
{
	const float Stamina = StatusState.CurrentStamina;
	FGameplayTag BreathType = IALSXTCharacterInterface::Execute_GetBreathType(ALSXTCharacter);

	if (ShouldTransitionBreathState())
	{
		FALSXTTargetBreathState NewTargetState = CalculateTargetBreathState();
		BreathState.TargetState = NewTargetState;
	}
}

bool UALSXTAnimationInstance::ShouldUpdateBreathState() const
{
	return StatusState.CurrentStamina != IALSXTCharacterInterface::Execute_GetStamina(ALSXTCharacter);
}

bool UALSXTAnimationInstance::ShouldTransitionBreathState()
{
	return (BreathState.CurrentBreathAlpha != BreathState.TargetState.Alpha || BreathState.CurrentBreathRate != BreathState.TargetState.Rate);
}

FALSXTTargetBreathState UALSXTAnimationInstance::CalculateTargetBreathState()
{
	FALSXTTargetBreathState NewTargetBreathState;

	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::True)
	{
		NewTargetBreathState.Alpha = 0.0;
		NewTargetBreathState.Rate = 0.0;
		return NewTargetBreathState;
	}
	else
	{
		FVector2D ConversionRange{ 0, 1 };
		FVector2D UtilizedStaminaRange{ 0, StaminaThresholdSettings.StaminaOptimalThreshold };
		float CurrentStaminaConverted = FMath::GetMappedRangeValueClamped(UtilizedStaminaRange, ConversionRange, StatusState.CurrentStamina);
		float PlayRateConverted = FMath::GetMappedRangeValueClamped(ConversionRange, CharacterBreathEffectsSettings.BreathAnimationPlayRateRange, CurrentStaminaConverted);
		float BlendConverted = FMath::GetMappedRangeValueClamped(ConversionRange, CharacterBreathEffectsSettings.BreathAnimationBlendRange, CurrentStaminaConverted);
		NewTargetBreathState.Alpha = BlendConverted;
		NewTargetBreathState.Rate = PlayRateConverted;
		NewTargetBreathState.TransitionRate = 1.0;
		return NewTargetBreathState;
	}
}

void UALSXTAnimationInstance::TransitionBreathState()
{
	BreathState.CurrentBreathAlpha = BreathState.TargetState.Alpha;
	BreathState.CurrentBreathRate = BreathState.TargetState.Rate;
}