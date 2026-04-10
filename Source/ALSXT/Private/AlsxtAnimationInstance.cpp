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
#include "Settings/AlsAnimationInstanceSettings.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsUtility.h"
#include "Utility/AlsVector.h"
#include "Utility/AlsPrivateMemberAccessor.h"

ALS_DEFINE_PRIVATE_MEMBER_ACCESSOR(AlsGetAnimationCurvesAccessor, &FAnimInstanceProxy::GetAnimationCurves,
								   const TMap<FName, float>& (FAnimInstanceProxy::*)(EAnimCurveType) const)

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

	RefreshAlsxtPose();
	
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
		Readiness = IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwningActor());
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

void UAlsxtAnimationInstance::AlsxtPlayQueuedTransitionAnimation()
{
	check(IsInGameThread())

	if (TransitionsState.bStopTransitionsQueued || !IsValid(TransitionsState.QueuedTransitionSequence))
	{
		return;
	}

	PlaySlotAnimationAsDynamicMontage(TransitionsState.QueuedTransitionSequence, UAlsConstants::TransitionSlotName(),
									  TransitionsState.QueuedTransitionBlendInDuration, TransitionsState.QueuedTransitionBlendOutDuration,
									  TransitionsState.QueuedTransitionPlayRate, 1, 0.0f, TransitionsState.QueuedTransitionStartTime);

	TransitionsState.QueuedTransitionSequence = nullptr;
	TransitionsState.QueuedTransitionBlendInDuration = 0.0f;
	TransitionsState.QueuedTransitionBlendOutDuration = 0.0f;
	TransitionsState.QueuedTransitionPlayRate = 1.0f;
	TransitionsState.QueuedTransitionStartTime = 0.0f;
}

void UAlsxtAnimationInstance::AlsxtRefreshCrouchingMovement()
{
#if WITH_EDITOR
	if (!IsValid(GetWorld()) || !GetWorld()->IsGameWorld())
	{
		return;
	}
#endif

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UAlsxtAnimationInstance::RefreshCrouchingMovement"),
								STAT_UAlsxtAnimationInstance_RefreshCrouchingMovement, STATGROUP_Als)
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__)

	if (!IsValid(ALSXTSettings))
	{
		return;
	}

	// const auto Speed{LocomotionState.Speed / LocomotionState.Scale};
 
	// CrouchingState.StrideBlendAmount = Settings->Crouching.StrideBlendAmountCurve->GetFloatValue(Speed);
 
	// CrouchingState.PlayRate = FMath::Clamp(
	// 	Speed / (Settings->Crouching.AnimatedCrouchSpeed * CrouchingState.StrideBlendAmount),
	// 	UE_KINDA_SMALL_NUMBER, 2.0f);

	////

	const auto Speed{LocomotionState.Speed / LocomotionState.Scale};

	// Calculate the stride blend amount. This value is used within the blend spaces to scale the stride (distance feet travel)
	// so that the character can walk or run at different movement speeds. It also allows the walk or run gait animations to
	// blend independently while still matching the animation speed to the movement speed, preventing the character from needing
	// to play a half walk + half run blend. The curves are used to map the stride amount to the speed for maximum control.

	AlsxtCrouchingState.StrideBlendAmount = FMath::Lerp(ALSXTSettings->Crouching.StrideBlendAmountWalkCurve->GetFloatValue(Speed),
	                                              ALSXTSettings->Crouching.StrideBlendAmountRunCurve->GetFloatValue(Speed),
	                                              PoseState.UnweightedGaitRunningAmount);

	// Calculate the walk run blend amount. This value is used within the blend spaces to blend between walking and running.

	AlsxtCrouchingState.WalkRunBlendAmount = Gait == AlsGaitTags::Walking ? 0.0f : 1.0f;

	// Calculate the standing play rate by dividing the character's speed by the animated speed for each gait.
	// The interpolation is determined by the gait amount curve that exists on every locomotion cycle so that
	// the play rate is always in sync with the currently blended animation. The value is also divided by the
	// stride blend and the capsule scale so that the play rate increases as the stride or scale gets smaller.

	// TODO Automatically calculate the play rate, such as is done in the UAnimDistanceMatchingLibrary::SetPlayrateToMatchSpeed() function.

	const auto WalkRunSpeedAmount{
		FMath::Lerp(Speed / ALSXTSettings->Crouching.AnimatedWalkSpeed,
		            Speed / ALSXTSettings->Crouching.AnimatedRunSpeed,
		            PoseState.UnweightedGaitRunningAmount)
	};

	const auto WalkRunSprintSpeedAmount{
		FMath::Lerp(WalkRunSpeedAmount,
		            Speed / ALSXTSettings->Crouching.AnimatedSprintSpeed,
		            PoseState.UnweightedGaitSprintingAmount)
	};

	// Do not let the play rate be exactly zero, otherwise animation notifies
	// may start to be triggered every frame until the play rate is changed.
	// TODO Check the need for this hack in future engine versions.

	AlsxtCrouchingState.PlayRate = FMath::Clamp(WalkRunSprintSpeedAmount / AlsxtCrouchingState.StrideBlendAmount, UE_KINDA_SMALL_NUMBER, 3.0f);

	AlsxtCrouchingState.SprintBlockAmount = GetCurveValueClamped01(UAlsConstants::SprintBlockCurveName());

	if (Gait != AlsGaitTags::Sprinting)
	{
		AlsxtCrouchingState.SprintTime = 0.0f;
		AlsxtCrouchingState.SprintAccelerationAmount = 0.0f;
		return;
	}

	// Use the relative acceleration as the sprint relative acceleration if less than 0.5 seconds has
	// elapsed since the start of the sprint, otherwise set the sprint relative acceleration to zero.
	// This is necessary to apply the acceleration animation only at the beginning of the sprint.

	static constexpr auto SprintTimeThreshold{0.5f};

	AlsxtCrouchingState.SprintTime = bPendingUpdate
		                           ? SprintTimeThreshold
		                           : AlsxtCrouchingState.SprintTime + GetDeltaSeconds();

	AlsxtCrouchingState.SprintAccelerationAmount = AlsxtCrouchingState.SprintTime >= SprintTimeThreshold
		                                         ? 0.0f
		                                         : GetAlsxtRelativeAccelerationAmount().X;
}

void UAlsxtAnimationInstance::AlsxtRefreshStandingMovement()
{
	#if WITH_EDITOR
	if (!IsValid(GetWorld()) || !GetWorld()->IsGameWorld())
	{
		return;
	}
#endif

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UAlsxtAnimationInstance::AlsxtRefreshStandingMovement"),
	                            STAT_UAlsxtAnimationInstance_AlsxtRefreshStandingMovement, STATGROUP_Als)
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__)

	if (!IsValid(Settings))
	{
		return;
	}

	const auto Speed{LocomotionState.Speed / LocomotionState.Scale};

	// Calculate the stride blend amount. This value is used within the blend spaces to scale the stride (distance feet travel)
	// so that the character can walk or run at different movement speeds. It also allows the walk or run gait animations to
	// blend independently while still matching the animation speed to the movement speed, preventing the character from needing
	// to play a half walk + half run blend. The curves are used to map the stride amount to the speed for maximum control.

	StandingState.StrideBlendAmount = FMath::Lerp(Settings->Standing.StrideBlendAmountWalkCurve->GetFloatValue(Speed),
	                                              Settings->Standing.StrideBlendAmountRunCurve->GetFloatValue(Speed),
	                                              PoseState.UnweightedGaitRunningAmount);

	// Calculate the walk run blend amount. This value is used within the blend spaces to blend between walking and running.

	StandingState.WalkRunBlendAmount = Gait == AlsGaitTags::Walking ? 0.0f : 1.0f;

	// Calculate the standing play rate by dividing the character's speed by the animated speed for each gait.
	// The interpolation is determined by the gait amount curve that exists on every locomotion cycle so that
	// the play rate is always in sync with the currently blended animation. The value is also divided by the
	// stride blend and the capsule scale so that the play rate increases as the stride or scale gets smaller.

	// TODO Automatically calculate the play rate, such as is done in the UAnimDistanceMatchingLibrary::SetPlayrateToMatchSpeed() function.

	const auto WalkRunSpeedAmount{
		FMath::Lerp(Speed / Settings->Standing.AnimatedWalkSpeed,
		            Speed / Settings->Standing.AnimatedRunSpeed,
		            PoseState.UnweightedGaitRunningAmount)
	};

	const auto WalkRunSprintSpeedAmount{
		FMath::Lerp(WalkRunSpeedAmount,
		            Speed / Settings->Standing.AnimatedSprintSpeed,
		            PoseState.UnweightedGaitSprintingAmount)
	};

	// Do not let the play rate be exactly zero, otherwise animation notifies
	// may start to be triggered every frame until the play rate is changed.
	// TODO Check the need for this hack in future engine versions.

	StandingState.PlayRate = FMath::Clamp(WalkRunSprintSpeedAmount / StandingState.StrideBlendAmount, UE_KINDA_SMALL_NUMBER, 3.0f);

	StandingState.SprintBlockAmount = GetCurveValueClamped01(UAlsConstants::SprintBlockCurveName());

	if (Gait != AlsGaitTags::Sprinting)
	{
		StandingState.SprintTime = 0.0f;
		StandingState.SprintAccelerationAmount = 0.0f;
		return;
	}

	// Use the relative acceleration as the sprint relative acceleration if less than 0.5 seconds has
	// elapsed since the start of the sprint, otherwise set the sprint relative acceleration to zero.
	// This is necessary to apply the acceleration animation only at the beginning of the sprint.

	static constexpr auto SprintTimeThreshold{0.5f};

	StandingState.SprintTime = bPendingUpdate
		                           ? SprintTimeThreshold
		                           : StandingState.SprintTime + GetDeltaSeconds();

	StandingState.SprintAccelerationAmount = StandingState.SprintTime >= SprintTimeThreshold
		                                         ? 0.0f
		                                         : GetAlsxtRelativeAccelerationAmount().X;
}

void UAlsxtAnimationInstance::AlsxtRefreshDynamicTransitions()
{
	#if WITH_EDITOR
	if (!IsValid(GetWorld()) || !GetWorld()->IsGameWorld())
	{
		return;
	}
#endif

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UAlsAnimationInstance::RefreshDynamicTransitions"),
	                            STAT_UAlsAnimationInstance_RefreshDynamicTransitions, STATGROUP_Als)
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(__FUNCTION__)

	if (DynamicTransitionsState.bUpdatedThisFrame || !IsValid(Settings))
	{
		return;
	}

	DynamicTransitionsState.bUpdatedThisFrame = true;

	if (DynamicTransitionsState.FrameDelay > 0)
	{
		DynamicTransitionsState.FrameDelay -= 1;
		return;
	}

	if (!TransitionsState.bTransitionsAllowed)
	{
		return;
	}

	// Check each foot to see if the location difference between the foot look and its desired / target location
	// exceeds a threshold. If it does, play an additive transition animation on that foot. The currently set
	// transition plays the second half of a 2 foot transition animation, so that only a single foot moves.

	const auto FootLockDistanceThresholdSquared{
		FMath::Square(Settings->DynamicTransitions.FootLockDistanceThreshold * LocomotionState.Scale)
	};

	const auto FootLockLeftDistanceSquared{FVector::DistSquared(FeetState.Left.TargetLocation, FeetState.Left.LockLocation)};
	const auto FootLockRightDistanceSquared{FVector::DistSquared(FeetState.Right.TargetLocation, FeetState.Right.LockLocation)};

	const auto bTransitionLeftAllowed{
		FAnimWeight::IsRelevant(FeetState.Left.LockAmount) && FootLockLeftDistanceSquared > FootLockDistanceThresholdSquared
	};

	const auto bTransitionRightAllowed{
		FAnimWeight::IsRelevant(FeetState.Right.LockAmount) && FootLockRightDistanceSquared > FootLockDistanceThresholdSquared
	};

	if (!bTransitionLeftAllowed && !bTransitionRightAllowed)
	{
		return;
	}

	TObjectPtr<UAnimSequenceBase> DynamicTransitionSequence;

	// If both transitions are allowed, choose the one with a greater lock distance.

	if (!bTransitionLeftAllowed || (bTransitionRightAllowed && FootLockLeftDistanceSquared < FootLockRightDistanceSquared))
	{
		DynamicTransitionSequence = Stance == AlsStanceTags::Crouching
			                            ? Settings->DynamicTransitions.CrouchingRightSequence
			                            : Settings->DynamicTransitions.StandingRightSequence;
	}
	else if (!bTransitionRightAllowed || FootLockLeftDistanceSquared >= FootLockRightDistanceSquared)
	{
		DynamicTransitionSequence = Stance == AlsStanceTags::Crouching
			                            ? Settings->DynamicTransitions.CrouchingLeftSequence
			                            : Settings->DynamicTransitions.StandingLeftSequence;
	}

	if (IsValid(DynamicTransitionSequence))
	{
		// Block next dynamic transitions for about 2 frames to give the animation blueprint some time to properly react to the animation.

		DynamicTransitionsState.FrameDelay = 2;

		// Animation montages can't be played in the worker thread, so queue them up to play later in the game thread.

		TransitionsState.QueuedTransitionSequence = DynamicTransitionSequence;
		TransitionsState.QueuedTransitionBlendInDuration = Settings->DynamicTransitions.BlendDuration;
		TransitionsState.QueuedTransitionBlendOutDuration = Settings->DynamicTransitions.BlendDuration;
		TransitionsState.QueuedTransitionPlayRate = Settings->DynamicTransitions.PlayRate;
		TransitionsState.QueuedTransitionStartTime = 0.0f;

		if (IsInGameThread())
		{
			AlsxtPlayQueuedTransitionAnimation();
		}
	}
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

void UAlsxtAnimationInstance::RefreshAlsxtPose()
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

FVector2f UAlsxtAnimationInstance::GetAlsxtRelativeAccelerationAmount() const
{
	// This value represents the current amount of acceleration / deceleration relative to the
	// character rotation. It is normalized to a range of -1 to 1 so that -1 equals the max
	// braking deceleration and 1 equals the max acceleration of the character movement component.

	const auto MaxAcceleration{
		(LocomotionState.Acceleration | LocomotionState.Velocity) >= 0.0f
			? LocomotionState.MaxAcceleration
			: LocomotionState.MaxBrakingDeceleration
	};

	if (MaxAcceleration <= UE_KINDA_SMALL_NUMBER)
	{
		return FVector2f::ZeroVector;
	}

	const FVector3f RelativeAcceleration{LocomotionState.RotationQuaternion.UnrotateVector(LocomotionState.Acceleration)};

	return FVector2f{UAlsVector::ClampMagnitude01(RelativeAcceleration / MaxAcceleration)};
}
