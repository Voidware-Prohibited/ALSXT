// Fill out your copyright notice in the Description page of Project Settings.

#include "ALSXTAnimationInstance.h"
#include "ALSXTAnimationInstanceProxy.h"
#include "ALSXTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Utility/ALSXTConstants.h"
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
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		StaminaThresholdSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->StatusSettings.StaminaThresholdSettings;
		CharacterBreathEffectsSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->BreathEffects;
	}
}

void UALSXTAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTCharacter))
	{
		return;
	}

	if (!GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return;
	}

	RefreshALSXTPose();
	
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		Freelooking = IALSXTCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor());
		Sex = IALSXTCharacterInterface::Execute_GetCharacterSex(GetOwningActor());
		DefensiveMode = IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwningActor());
		LocomotionVariant = IALSXTCharacterInterface::Execute_GetCharacterLocomotionVariant(GetOwningActor());
		Injury = IALSXTCharacterInterface::Execute_GetCharacterInjury(GetOwningActor());
		CombatStance = IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwningActor());
		WeaponFirearmStance = IALSXTCharacterInterface::Execute_GetCharacterWeaponFirearmStance(GetOwningActor());
		WeaponReadyPosition = IALSXTCharacterInterface::Execute_GetCharacterWeaponReadyPosition(GetOwningActor());
		StationaryMode = IALSXTCharacterInterface::Execute_GetCharacterStationaryMode(GetOwningActor());
		HoldingBreath = IALSXTCharacterInterface::Execute_GetCharacterHoldingBreath(GetOwningActor());
		PhysicalAnimationMode = IALSXTCharacterInterface::Execute_GetCharacterPhysicalAnimationMode(GetOwningActor());
		Gesture = IALSXTCharacterInterface::Execute_GetCharacterGesture(GetOwningActor());
		GestureHand = IALSXTCharacterInterface::Execute_GetCharacterGestureHand(GetOwningActor());
		ReloadingType = IALSXTCharacterInterface::Execute_GetCharacterReloadingType(GetOwningActor());
		ForegripPosition = IALSXTCharacterInterface::Execute_GetCharacterForegripPosition(GetOwningActor());
		FirearmFingerAction = IALSXTCharacterInterface::Execute_GetCharacterFirearmFingerAction(GetOwningActor());
		FirearmFingerActionHand = IALSXTCharacterInterface::Execute_GetCharacterFirearmFingerActionHand(GetOwningActor());
		WeaponCarryPosition = IALSXTCharacterInterface::Execute_GetCharacterWeaponCarryPosition(GetOwningActor());
		FirearmSightLocation = IALSXTCharacterInterface::Execute_GetCharacterFirearmSightLocation(GetOwningActor());
		ForegripTransform = IALSXTCharacterInterface::Execute_GetCharacterCurrentForegripTransform(GetOwningActor());
		VaultType = IALSXTCharacterInterface::Execute_GetCharacterVaultType(GetOwningActor());
		AimState = IALSXTCharacterInterface::Execute_GetCharacterAimState(GetOwningActor());
		FreelookState = IALSXTCharacterInterface::Execute_GetCharacterFreelookState(GetOwningActor());
		HeadLookAtState = IALSXTCharacterInterface::Execute_GetCharacterHeadLookAtState(GetOwningActor());
		DoesOverlayObjectUseLeftHandIK = IALSXTCharacterInterface::Execute_DoesCharacterOverlayObjectUseLeftHandIK(GetOwningActor());
		StatusState = IALSXTCharacterInterface::Execute_GetStatusState(GetOwningActor());
		CrowdNavigationPoseState = IALSXTCharacterInterface::Execute_GetCrowdNavigationPoseState(GetOwningActor());
		BumpPoseState = IALSXTCharacterInterface::Execute_GetBumpPoseState(GetOwningActor());
		DefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwningActor());
		WeaponObstruction = IALSXTCharacterInterface::Execute_GetCharacterWeaponObstruction(GetOwningActor());
		BreathState.HoldingBreath = IALSXTCharacterInterface::Execute_GetCharacterHoldingBreath(GetOwningActor());
	}
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

FAnimInstanceProxy* UALSXTAnimationInstance::CreateAnimInstanceProxy()
{
	return new FALSXTAnimationInstanceProxy{ this };
}

bool UALSXTAnimationInstance::IsSpineRotationAllowed()
{
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return Super::IsSpineRotationAllowed() && IALSXTCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
		//return ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming && ALSXTCharacter->GetLocomotionState().bRotationLocked == false;
	}
	else
	{
		return false;
	}
}

bool UALSXTAnimationInstance::IsRotateInPlaceAllowed()
{
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return Super::IsRotateInPlaceAllowed() && IALSXTCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
	}
	else
	{
		return false;
	}
}

bool UALSXTAnimationInstance::IsTurnInPlaceAllowed()
{
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return Super::IsTurnInPlaceAllowed() && IALSXTCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
	}
	else
	{
		return false;
	}
}

void UALSXTAnimationInstance::RefreshALSXTPose()
{
	const auto& Curves{ GetProxyOnAnyThread<FALSXTAnimationInstanceProxy>().GetAnimationCurves(EAnimCurveType::AttributeCurve) };

	static const auto GetCurveValue{
		[](const TMap<FName, float>& Curves, const FName& CurveName) -> float
		{
			const auto* Value{Curves.Find(CurveName)};

			return Value != nullptr ? *Value : 0.0f;
		}
	};

	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		ALSXTPoseState.LeanDirection = IALSXTCharacterInterface::Execute_GetCharacterPoseState(GetOwningActor()).LeanDirection;
	}

	ALSXTPoseState.LeanLeftAmount = GetCurveValue(Curves, UALSXTConstants::PoseLeanLeftCurveName());
	ALSXTPoseState.LeanRightAmount = GetCurveValue(Curves, UALSXTConstants::PoseLeanRightCurveName());

	ALSXTPoseState.CantedAmount = GetCurveValue(Curves, UALSXTConstants::PoseCantedCurveName());

	ALSXTPoseState.VaultingAmount = GetCurveValue(Curves, UALSXTConstants::PoseVaultingCurveName());
	ALSXTPoseState.SlidingAmount = GetCurveValue(Curves, UALSXTConstants::PoseSlidingCurveName());

	ALSXTPoseState.WallJumpAmount = GetCurveValue(Curves, UALSXTConstants::PoseWallJumpCurveName());
	ALSXTPoseState.WallRunAmount = GetCurveValue(Curves, UALSXTConstants::PoseWallRunCurveName());

	ALSXTPoseState.StationaryAmount = GetCurveValue(Curves, UALSXTConstants::PoseStationaryCurveName());
	ALSXTPoseState.SeatedAmount = GetCurveValue(Curves, UALSXTConstants::PoseSeatedCurveName());

	// Use the grounded pose curve value to "unweight" the gait pose curve. This is used to
	// instantly get the full gait value from the very beginning of transitions to grounded states.

	// PoseState.UnweightedGaitAmount = PoseState.GroundedAmount > 0.0f
	// 	? PoseState.GaitAmount / PoseState.GroundedAmount
	// 	: PoseState.GaitAmount;
	// 
	// PoseState.UnweightedGaitWalkingAmount = UAlsMath::Clamp01(PoseState.UnweightedGaitAmount);
	// PoseState.UnweightedGaitRunningAmount = UAlsMath::Clamp01(PoseState.UnweightedGaitAmount - 1.0f);
	// PoseState.UnweightedGaitSprintingAmount = UAlsMath::Clamp01(PoseState.UnweightedGaitAmount - 2.0f);
}

void UALSXTAnimationInstance::UpdateStatusState()
{
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		FALSXTStatusState NewStatusState{ IALSXTCharacterInterface::Execute_GetStatusState(GetOwningActor()) };

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
}

void UALSXTAnimationInstance::UpdateBreathState()
{
	const float Stamina = StatusState.CurrentStamina;
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		FGameplayTag BreathType = IALSXTCharacterInterface::Execute_GetBreathType(ALSXTCharacter);

		if (ShouldTransitionBreathState())
		{
			FALSXTTargetBreathState NewTargetState = CalculateTargetBreathState();
			BreathState.TargetState = NewTargetState;
		}
	}
}

bool UALSXTAnimationInstance::ShouldUpdateBreathState() const
{
	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return StatusState.CurrentStamina != IALSXTCharacterInterface::Execute_GetStamina(ALSXTCharacter);
	}
	else
	{
		return false;
	}
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