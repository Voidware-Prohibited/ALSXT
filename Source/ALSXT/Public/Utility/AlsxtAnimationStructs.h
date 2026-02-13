#pragma once

#include "AlsxtOverlayStructs.h"
#include "NativeGameplayTags.h"
#include "Animation/AnimMontage.h"
#include "Engine/EngineTypes.h"
#include "Settings/AlsMantlingSettings.h"
#include "AlsxtAnimationStructs.generated.h"

class UAlsTurnInPlaceSettings;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionVariantInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.LocomotionSettings", AllowPrivateAccess))
	FGameplayTagContainer LocomotionSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralTurnInPlaceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	float ViewYawAngleThreshold{45.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float ViewYawSpeedThreshold{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
	FVector2f ViewYawAngleToActivationDelay{0.0f, 0.75f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		DisplayName = "Turn 180 Angle Threshold", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	float Turn180AngleThreshold{130.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "s"))
	float BlendDuration{0.2f};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtTransitionAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UAnimSequenceBase> LeftSequence {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UAnimSequenceBase> RightSequence {nullptr};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtTurnInPlaceStanceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 90 Left")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn90Left {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 90 Right")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn90Right {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 180 Left")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn180Left {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 180 Right")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn180Right {nullptr};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtPoseSingleFrame
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimSequenceBase> Pose {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float ExplicitTime {0.0f};

	bool operator==(const FAlsxtPoseSingleFrame& other) const
	{
		return (other.Pose == Pose) && (other.ExplicitTime == ExplicitTime);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceBlendSpaceAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Backward {nullptr};

	bool operator==(const FAlsxtAnimationInstanceBlendSpaceAssets& other) const
	{
		return (other.WalkRun_Forward == WalkRun_Forward) && (other.WalkRun_Left_Forward == WalkRun_Left_Forward);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxt1DLocomotionBlendSpaceAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Pose {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> ForwardToBackwardTransition {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> BackwardToForwardTransition {nullptr};
 
	bool operator==(const FAlsxt1DLocomotionBlendSpaceAssets& other) const
	{
		return (other.Forward == Forward) && (other.Backward == Backward) && (other.Pose == Pose) && (other.ForwardToBackwardTransition == ForwardToBackwardTransition) && (other.BackwardToForwardTransition == BackwardToForwardTransition);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtSidlingAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsxt1DLocomotionBlendSpaceAssets Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsxt1DLocomotionBlendSpaceAssets Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool MirrorLeftForRight {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> LeftToRightTransition {nullptr};
 
	bool operator==(const FAlsxtSidlingAssets& other) const
	{
		return (other.Left == Left) && (other.Right == Right) && (other.MirrorLeftForRight == MirrorLeftForRight) && (other.LeftToRightTransition == LeftToRightTransition);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCustomMovementAnimationInstanceBlendSpaceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	TObjectPtr<UAnimSequence> IdlePose {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Sprint {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Up {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Down {nullptr};

	bool operator==(const FAlsxtCustomMovementAnimationInstanceBlendSpaceAssets& other) const
	{
		return (other.WalkRun_Forward == WalkRun_Forward) && (other.WalkRun_Left_Forward == WalkRun_Left_Forward);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceAccelerationAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Left {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Right {nullptr};

	bool operator==(const FAlsxtAnimationInstanceAccelerationAssets& other) const
	{
		return (other.Acceleration_Forward == Acceleration_Forward) && (other.Acceleration_Backward == Acceleration_Backward);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceMovementAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Left_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Left_Backward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Right_Forward {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Right_Backward {nullptr};

	bool operator==(const FAlsxtAnimationInstanceMovementAssets& other) const
	{
		return (other.Walk_Forward == Walk_Forward) && (other.Walk_Backward == Walk_Backward);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceRotateInPlaceAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Rotate_90_Left {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Rotate_90_Right {nullptr};

	bool operator==(const FAlsxtAnimationInstanceRotateInPlaceAssets& other) const
	{
		return (other.Rotate_90_Left == Rotate_90_Left) && (other.Rotate_90_Right == Rotate_90_Right);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceStopTransitionAssets
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Stop_Left {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Stop_Right {nullptr};

	bool operator==(const FAlsxtAnimationInstanceStopTransitionAssets& other) const
	{
		return (other.Stop_Left == Stop_Left) && (other.Stop_Right == Stop_Right);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionAnimationInstanceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UAnimSequence> IdlePose {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandIdlePose {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandIdlePose", EditConditionHides=true))
	TObjectPtr<UAnimSequence> IdlePose_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UAnimSequence> Sprint {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSprint {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSprint", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Sprint_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UAnimSequence> SprintAcceleration {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSprintAcceleration {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSprintAcceleration", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintAcceleration_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceBlendSpaceAssets BlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandBlendSpaceAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandBlendSpaceAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceBlendSpaceAssets BlendSpaceAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UBlendSpace> MovementLean {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandMovementLean {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandMovementLean", EditConditionHides=true))
	TObjectPtr<UBlendSpace> MovementLean_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sidling")
	FAlsxtSidlingAssets SidlingAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSidlingAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSidlingAssets", EditConditionHides=true))
	FAlsxtSidlingAssets SidlingAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceAccelerationAssets AccelerationAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandAccelerationAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandAccelerationAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceAccelerationAssets AccelerationAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceMovementAssets PlantFootAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandPlantFootAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandPlantFootAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceMovementAssets PlantFootAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtTurnInPlaceStanceAssets TurnInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandTurnInPlaceAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandTurnInPlaceAssets", EditConditionHides=true))
	FAlsxtTurnInPlaceStanceAssets TurnInPlaceAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtTurnInPlaceStanceAssets RotateInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandRotateInPlaceAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandRotateInPlaceAssets", EditConditionHides=true))
	FAlsxtTurnInPlaceStanceAssets RotateInPlaceAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceStopTransitionAssets StopTransitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandStopTransitions {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandStopTransitions", EditConditionHides=true))
	FAlsxtAnimationInstanceStopTransitionAssets StopTransitions_RightHand;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FAlsxtTransitionAssets TransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandTransitionAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandTransitionAssets", EditConditionHides=true))
	FAlsxtTransitionAssets TransitionAssets_RightHand;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FAlsxtTransitionAssets DynamicTransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandDynamicTransitionAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandDynamicTransitionAssets", EditConditionHides=true))
	FAlsxtTransitionAssets DynamicTransitionAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	TObjectPtr<UAnimSequence> CustomMovementWalkPose {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementWalkPose {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta=(EditCondition="bUseUniqueRightHandCustomMovementWalkPose", EditConditionHides=true))
	TObjectPtr<UAnimSequence> CustomMovementWalkPose_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	FAlsxtAnimationInstanceMovementAssets CustomMovementAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta=(EditCondition="bUseUniqueRightHandCustomMovementAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceMovementAssets CustomMovementAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	FAlsxtCustomMovementAnimationInstanceBlendSpaceAssets CustomMovementBlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementBlendSpaceAssets {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta=(EditCondition="bUseUniqueRightHandCustomMovementBlendSpaceAssets", EditConditionHides=true))
	FAlsxtCustomMovementAnimationInstanceBlendSpaceAssets CustomMovementBlendSpaceAssets_RightHand;

	bool operator==(const FAlsxtLocomotionAnimationInstanceAssets& other) const
	{
		return (other.IdlePose == IdlePose) && (other.Sprint == Sprint) && (other.SprintAcceleration == SprintAcceleration);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayAnimationInstanceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationInstance {nullptr};

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> OverlayPoses {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverlayPosesContainRightHandPoses {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bOverlayPosesContainRightHandPoses", EditConditionHides=true))
	bool bUseUniqueRightHandOverlayPoses {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseUniqueRightHandOverlayPoses", EditConditionHides=true))
	TObjectPtr<UAnimSequence> OverlayPoses_RightHand {nullptr};

	// Arms
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> RunArms {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandRunArms {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandRunArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> RunArms_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> SprintArms {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandSprintArms {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandSprintArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintArms_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> SprintImpulseArms {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandSprintImpulseArms {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandSprintImpulseArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintImpulseArms_RightHand {nullptr};

	// Aim
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> Aim {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAim {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAim", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Aim_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|")
	TObjectPtr<UAnimSequence> AimWading {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimWading {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|", meta=(EditCondition="bUseUniqueRightHandAimWading", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimWading_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> AimCrouch {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimCrouch {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAimCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCrouch_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> AimProne {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimProne {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAimProne", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimProne_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	TObjectPtr<UAnimSequence> AimCantedLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	bool bUseUniqueRightHandAimCantedLeft {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted", meta=(EditCondition="bUseUniqueRightHandAimCantedLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCantedLeft_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	TObjectPtr<UAnimSequence> AimCantedRight {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	bool bUseUniqueRightHandAimCantedRight {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted", meta=(EditCondition="bUseUniqueRightHandAimCantedRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCantedRight_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverLeft {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverLeft_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverRight {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverRight {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverRight_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverUpLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverUpLeft {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverUpLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverUpLeft_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverUpRight {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverUpRight {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverUpRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverUpRight_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	TObjectPtr<UAnimSequence> AimLadder {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	bool bUseUniqueRightHandAimLadder {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special", meta=(EditCondition="bUseUniqueRightHandAimLadder", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimLadder_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	TObjectPtr<UAnimSequence> AimSwimming {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	bool bUseUniqueRightHandAimSwimming {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special", meta=(EditCondition="bUseUniqueRightHandAimSwimming", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimSwimming_RightHand {nullptr};

	// Lean
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> Lean {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLean {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLean", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Lean_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> LeanCrouch {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLeanCrouch {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLeanCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> LeanCrouch_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> LeanProne {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLeanProne {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLeanProne", EditConditionHides=true))
	TObjectPtr<UAnimSequence> LeanProne_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsHigh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsHigh {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsHigh", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsHigh_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsLow {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsLow {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsLow", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsLow_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsInAir {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsInAir {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsInAir", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsInAir_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting")
	TSoftObjectPtr<UAlsMantlingSettings> VaultingSettings {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting")
	bool bUseUniqueRightHandVaulting {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting", meta=(EditCondition="bUseUniqueRightHandVaulting", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> VaultingSettings_RightHand {nullptr};

	// Slope Slide
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	TObjectPtr<UAnimSequence> SlopeSlide {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	bool bUseUniqueRightHandSlopeSlide {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide", meta=(EditCondition="bUseUniqueRightHandSlopeSlide", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SlopeSlide_RightHand {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	TObjectPtr<UAnimSequence> SlopeSlideCrouch {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	bool bUseUniqueRightHandSlopeSlideCrouch {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide", meta=(EditCondition="bUseUniqueRightHandSlopeSlideCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SlopeSlideCrouch_RightHand {nullptr};
	

	bool operator==(const FAlsxtOverlayAnimationInstanceAssets& other) const
	{
		return (other.OverlayPoses == OverlayPoses) && (other.RunArms == RunArms) && (other.SprintArms == SprintArms) && (other.SprintImpulseArms == SprintImpulseArms) && (other.Aim == Aim) && (other.AimCrouch == AimCrouch) && (other.AimProne == AimProne) && (other.AimAroundCoverLeft == AimAroundCoverLeft) && (other.AimAroundCoverRight == AimAroundCoverRight) && (other.AimAroundCoverUpLeft == AimAroundCoverUpLeft) && (other.AimAroundCoverUpRight == AimAroundCoverUpRight) && (other.AimLadder == AimLadder) && (other.AimSwimming == AimSwimming);
	}

};