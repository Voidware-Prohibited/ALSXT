// Fill out your copyright notice in the Description page of Project Settings.

#include "AlsxtAnimationInstance.h"
#include "AlsxtAnimationInstanceProxy.h"
#include "AlsxtCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtHeldItemInterface.h"
#include "Utility/AlsxtConstants.h"
#include "ALS/Public/Utility/AlsMacros.h"
#include "Math/UnrealMathUtility.h"
#include "Stats/Stats.h"
#include "Interfaces/AlsxtFirearmInterface.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtBreathAttributeSet.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"

UAlsxtAnimationInstance::UAlsxtAnimationInstance()
{
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UAlsxtAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ALSXTCharacter = Cast<AAlsxtCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacter))
	{
		// Use default objects for editor preview.

		ALSXTCharacter = GetMutableDefault<AAlsxtCharacter>();
	}
#endif
}

void UAlsxtAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (IsValid(ALSXTSettings))
	{
		ALS_ENSURE(IsValid(ALSXTCharacter));
		if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
		{
			StaminaThresholdSettings = IAlsxtCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->StatusSettings.StaminaThresholdSettings;
			CharacterBreathEffectsSettings = IAlsxtCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->BreathEffects;
		}
	}
	
}

void UAlsxtAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTCharacter))
	{
		return;
	}

	if (!GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		return;
	}

	RefreshALSXTPose();
	
	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		AnimationParametersState = IAlsxtCharacterInterface::Execute_GetCharacterAnimationParametersState(GetOwningActor());
		JigglePhysicsSettings = IAlsxtCharacterInterface::Execute_GetCharacterJigglePhysicsSettings(GetOwningActor());
		Freelooking = IAlsxtCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor());
		Sex = IAlsxtCharacterInterface::Execute_GetCharacterSex(GetOwningActor());
		DefensiveMode = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwningActor());
		Lean = IAlsxtCharacterInterface::Execute_GetCharacterLean(GetOwningActor());
		LocomotionVariant = IAlsxtCharacterInterface::Execute_GetCharacterLocomotionVariant(GetOwningActor());
		Injury = IAlsxtCharacterInterface::Execute_GetCharacterInjury(GetOwningActor());
		CombatStance = IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwningActor());
		WeaponFirearmStance = IAlsxtCharacterInterface::Execute_GetCharacterWeaponFirearmStance(GetOwningActor());
		WeaponReadyPosition = IAlsxtCharacterInterface::Execute_GetCharacterWeaponReadyPosition(GetOwningActor());
		StationaryMode = IAlsxtCharacterInterface::Execute_GetCharacterStationaryMode(GetOwningActor());
		HoldingBreath = IAlsxtCharacterInterface::Execute_GetCharacterHoldingBreath(GetOwningActor());
		IAlsxtCharacterInterface::Execute_GetCharacterGesture(GetOwningActor(), Gesture, GestureHand);
		ForegripPosition = IAlsxtCharacterInterface::Execute_GetCharacterForegripPosition(GetOwningActor());
		FirearmFingerAction = IAlsxtCharacterInterface::Execute_GetCharacterFirearmFingerAction(GetOwningActor());
		FirearmFingerActionHand = IAlsxtCharacterInterface::Execute_GetCharacterFirearmFingerActionHand(GetOwningActor());
		WeaponCarryPosition = IAlsxtCharacterInterface::Execute_GetCharacterWeaponCarryPosition(GetOwningActor());
		FirearmSightLocation = IAlsxtCharacterInterface::Execute_GetCharacterFirearmSightLocation(GetOwningActor());
		ForegripTransform = IAlsxtCharacterInterface::Execute_GetCharacterCurrentForegripTransform(GetOwningActor());
		VaultType = IAlsxtCharacterInterface::Execute_GetCharacterVaultType(GetOwningActor());		
		AimState = IAlsxtCharacterInterface::Execute_GetCharacterAimState(GetOwningActor());
		FreelookState = IAlsxtCharacterInterface::Execute_GetCharacterFreelookState(GetOwningActor());
		HeadLookAtState = IAlsxtCharacterInterface::Execute_GetCharacterHeadLookAtState(GetOwningActor());
		StatusState = IAlsxtCharacterInterface::Execute_GetCharacterStatusState(GetOwningActor());
		DefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwningActor());
		DefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwningActor());
		StationaryModeState = IAlsxtCharacterInterface::Execute_GetCharacterStationaryModeState(GetOwningActor());
		WeaponObstruction = IAlsxtCharacterInterface::Execute_GetCharacterWeaponObstruction(GetOwningActor());
		BreathState = IAlsxtCharacterInterface::Execute_GetCharacterBreathState(GetOwningActor());
		// ReloadingType = IALSXTCharacterInterface::Execute_GetCharacterReloadingType(GetOwningActor());
		
	}

	if (GetOwningActor()->Implements<UAlsxtCollisionInterface>())
	{
		PhysicalAnimationMode = IAlsxtCollisionInterface::Execute_GetCharacterPhysicalAnimationMode(GetOwningActor());
		CrowdNavigationPoseState = IAlsxtCollisionInterface::Execute_GetCrowdNavigationPoseState(GetOwningActor());
		BumpPoseState = IAlsxtCollisionInterface::Execute_GetBumpPoseState(GetOwningActor());
	}

	if (GetOwningActor()->Implements<UAlsxtHeldItemInterface>())
	{
		HeldItemSettings = IAlsxtHeldItemInterface::Execute_GetHeldItemSettings(GetOwningActor());
		HeldItemState = IAlsxtHeldItemInterface::Execute_GetCharacterHeldItemState(GetOwningActor());
		DoesOverlayObjectUseLeftHandIK = IAlsxtHeldItemInterface::Execute_GetHeldItemSettings(GetOwningActor()).UsesLeftHandIK;
	}

	if (GetOwningActor()->Implements<UAlsxtFirearmInterface>())
	{
		if (IsValid(IAlsxtFirearmInterface::Execute_GetFirearmDischargeEffectsState(GetOwningActor()).RecoilAsset))
		{
			RecoilState = IAlsxtFirearmInterface::Execute_GetRecoilState(GetOwningActor());
		}


	}

	if (IAlsxtCharacterInterface::Execute_GetCharacterStationaryModeState(GetOwningActor()).Mode != StationaryModeState.Mode && (IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwningActor()) != Stance || IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwningActor()) != CombatStance))
	{

	}

	// for (FALSXTStationaryModeEntry StationaryModeAnimations : ALSXTSettings->StationaryMode.Animations)
	// {
	// 	FGameplayTagContainer CurrentStateContainer;
	// 	CurrentStateContainer.AddTag(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwningActor()));
	// 	CurrentStateContainer.AddTag(IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwningActor()));
	// 	FGameplayTagContainer EntryContainer;
	// 	if (StationaryModeAnimations.Overlays.HasTag(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwningActor())))
	// 	{
	// 
	// 	}
	// }
}

void UAlsxtAnimationInstance::NativeThreadSafeUpdateAnimation(const float DeltaTime)
{

	Super::NativeThreadSafeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}
}

void UAlsxtAnimationInstance::NativePostEvaluateAnimation()
{

	Super::NativePostEvaluateAnimation();

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}

	bPendingUpdate = false;
}

FAnimInstanceProxy* UAlsxtAnimationInstance::CreateAnimInstanceProxy()
{
	return new FAlsxtAnimationInstanceProxy{ this };
}

bool UAlsxtAnimationInstance::IsSpineRotationAllowed()
{
	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		return Super::IsSpineRotationAllowed() && IAlsxtCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
		//return ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming && ALSXTCharacter->GetLocomotionState().bRotationLocked == false;
	}
	else
	{
		return false;
	}
}

bool UAlsxtAnimationInstance::IsRotateInPlaceAllowed()
{
	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		return Super::IsRotateInPlaceAllowed() && IAlsxtCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
	}
	else
	{
		return false;
	}
}

bool UAlsxtAnimationInstance::IsTurnInPlaceAllowed()
{
	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		return Super::IsTurnInPlaceAllowed() && IAlsxtCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor()) != ALSXTFreelookingTags::True;
	}
	else
	{
		return false;
	}
}

void UAlsxtAnimationInstance::RefreshALSXTPose()
{
	const auto& Curves{ GetProxyOnAnyThread<FAlsxtAnimationInstanceProxy>().GetAnimationCurves(EAnimCurveType::AttributeCurve) };

	static const auto GetCurveValue{
		[](const TMap<FName, float>& Curves, const FName& CurveName) -> float
		{
			const auto* Value{Curves.Find(CurveName)};

			return Value != nullptr ? *Value : 0.0f;
		}
	};

	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		ALSXTPoseState.LeanDirection = IAlsxtCharacterInterface::Execute_GetCharacterPoseState(GetOwningActor()).LeanDirection;
	}

	ALSXTPoseState.LeanLeftAmount = GetCurveValue(Curves, UAlsxtConstants::PoseLeanLeftCurveName());
	ALSXTPoseState.LeanRightAmount = GetCurveValue(Curves, UAlsxtConstants::PoseLeanRightCurveName());

	ALSXTPoseState.CantedAmount = GetCurveValue(Curves, UAlsxtConstants::PoseCantedCurveName());

	ALSXTPoseState.VaultingAmount = GetCurveValue(Curves, UAlsxtConstants::PoseVaultingCurveName());
	ALSXTPoseState.SlidingAmount = GetCurveValue(Curves, UAlsxtConstants::PoseSlidingCurveName());

	ALSXTPoseState.WallJumpAmount = GetCurveValue(Curves, UAlsxtConstants::PoseWallJumpCurveName());
	ALSXTPoseState.WallRunAmount = GetCurveValue(Curves, UAlsxtConstants::PoseWallRunCurveName());

	ALSXTPoseState.StationaryAmount = GetCurveValue(Curves, UAlsxtConstants::PoseStationaryCurveName());
	ALSXTPoseState.SeatedAmount = GetCurveValue(Curves, UAlsxtConstants::PoseSeatedCurveName());

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

void UAlsxtAnimationInstance::UpdateStatusState()
{
	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		FAlsxtStatusState NewStatusState{ IAlsxtCharacterInterface::Execute_GetCharacterStatusState(GetOwningActor()) };

		if (NewStatusState != StatusState)
		{
			StatusState = NewStatusState;
		}
	}
}

TSoftObjectPtr<UAlsxtAbilitySystemComponent> UAlsxtAnimationInstance::GetAlsxtAbilitySystemComponent() const
{
	// Set outbound pointer
	TSoftObjectPtr<UAlsxtAbilitySystemComponent> ASC;
	
	// First check if AlsxtCharacter pointer is set, otherwise return empty pointer
	if (ALSXTCharacter)
	{		
		// Double check AlsxtCharacter implements UAlsxtAbilitySystemInterface
		if (ALSXTCharacter->Implements<UAlsxtAbilitySystemInterface>())
		{
			if (IAlsxtAbilitySystemInterface* ASCInterface = Cast<IAlsxtAbilitySystemInterface>(ALSXTCharacter))
			{
				// Create the FSoftObjectPath from the raw pointer
				FSoftObjectPath AscObjectPath(ASCInterface->GetAbilitySystemComponent());
				TSoftObjectPtr<UAlsxtAbilitySystemComponent> AscSoftPointer(AscObjectPath);
				return AscSoftPointer;
			}
		}
	}
	return ASC;
}

float UAlsxtAnimationInstance::GetBreathingRate() const
{
	// GetAlsxtAbilitySystemComponent();

	if (AlsxtAbilitySystemComponent)
	{
		// 4. Get the breathing rate attribute value from the attribute set.
		const FGameplayAttribute BreathingRateAttribute = UAlsxtBreathAttributeSet::GetCurrentBreathRateAttribute();
		if (BreathingRateAttribute.IsValid())
		{
			// You can use the GetNumericAttribute function to get the current value.
			return AlsxtAbilitySystemComponent->GetNumericAttribute(BreathingRateAttribute);
		}
	}

	return 0.0f;
}

FALSXTControlRigInput UAlsxtAnimationInstance::GetALSXTControlRigInput() const {

	return {
		.bObstacleNavigation = DefensiveModeState.ObstacleMode == ALSXTDefensiveModeTags::CrowdNavigation || DefensiveModeState.ObstacleMode == ALSXTDefensiveModeTags::ObstacleNavigation,
		.bCombatStance = CombatStance != ALSXTCombatStanceTags::Neutral,
		.bEnableLeftHandIK = DoesOverlayObjectUseLeftHandIK
	};
}
