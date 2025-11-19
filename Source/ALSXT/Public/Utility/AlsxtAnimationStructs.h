#pragma once

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
	TObjectPtr<UAnimSequenceBase> LeftSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UAnimSequenceBase> RightSequence;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtTurnInPlaceStanceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 90 Left")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn90Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 90 Right")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn90Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 180 Left")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn180Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Instanced, DisplayName = "Turn 180 Right")
	TObjectPtr<UAlsTurnInPlaceSettings> Turn180Right;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtPoseSingleFrame
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimSequenceBase> Pose;

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
	TObjectPtr<UBlendSpace> WalkRun_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Backward;

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
	TObjectPtr<UBlendSpace> Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Pose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> ForwardToBackwardTransition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> BackwardToForwardTransition;
 
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
	TObjectPtr<UAnimSequence> LeftToRightTransition;
 
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
	TObjectPtr<UAnimSequence> IdlePose;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Sprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Left_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Right_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Up;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UBlendSpace> WalkRun_Down;

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
	TObjectPtr<UAnimSequence> Acceleration_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Acceleration_Right;

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
	TObjectPtr<UAnimSequence> Walk_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Left_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Left_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Right_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Walk_Right_Backward;

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
	TObjectPtr<UAnimSequence> Rotate_90_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Rotate_90_Right;

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
	TObjectPtr<UAnimSequence> Stop_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> Stop_Right;

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
	TObjectPtr<UAnimSequence> IdlePose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandIdlePose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandIdlePose", EditConditionHides=true))
	TObjectPtr<UAnimSequence> IdlePose_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UAnimSequence> Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSprint", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Sprint_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UAnimSequence> SprintAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSprintAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSprintAcceleration", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintAcceleration_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceBlendSpaceAssets BlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandBlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandBlendSpaceAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceBlendSpaceAssets BlendSpaceAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TObjectPtr<UBlendSpace> MovementLean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandMovementLean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandMovementLean", EditConditionHides=true))
	TObjectPtr<UBlendSpace> MovementLean_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sidling")
	FAlsxtSidlingAssets SidlingAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandSidlingAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandSidlingAssets", EditConditionHides=true))
	FAlsxtSidlingAssets SidlingAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceAccelerationAssets AccelerationAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandAccelerationAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandAccelerationAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceAccelerationAssets AccelerationAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceMovementAssets PlantFootAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandPlantFootAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandPlantFootAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceMovementAssets PlantFootAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtTurnInPlaceStanceAssets TurnInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandTurnInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandTurnInPlaceAssets", EditConditionHides=true))
	FAlsxtTurnInPlaceStanceAssets TurnInPlaceAssets_RightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtTurnInPlaceStanceAssets RotateInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandRotateInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandRotateInPlaceAssets", EditConditionHides=true))
	FAlsxtTurnInPlaceStanceAssets RotateInPlaceAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FAlsxtAnimationInstanceStopTransitionAssets StopTransitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandStopTransitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandStopTransitions", EditConditionHides=true))
	FAlsxtAnimationInstanceStopTransitionAssets StopTransitions_RightHand;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FAlsxtTransitionAssets TransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandTransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandTransitionAssets", EditConditionHides=true))
	FAlsxtTransitionAssets TransitionAssets_RightHand;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FAlsxtTransitionAssets DynamicTransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandDynamicTransitionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta=(EditCondition="bUseUniqueRightHandDynamicTransitionAssets", EditConditionHides=true))
	FAlsxtTransitionAssets DynamicTransitionAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	TObjectPtr<UAnimSequence> CustomMovementWalkPose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementWalkPose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta=(EditCondition="bUseUniqueRightHandCustomMovementWalkPose", EditConditionHides=true))
	TObjectPtr<UAnimSequence> CustomMovementWalkPose_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	FAlsxtAnimationInstanceMovementAssets CustomMovementAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta=(EditCondition="bUseUniqueRightHandCustomMovementAssets", EditConditionHides=true))
	FAlsxtAnimationInstanceMovementAssets CustomMovementAssets_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Movement")
	FAlsxtCustomMovementAnimationInstanceBlendSpaceAssets CustomMovementBlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseUniqueRightHandCustomMovementBlendSpaceAssets;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> OverlayPoses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverlayPosesContainRightHandPoses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bOverlayPosesContainRightHandPoses", EditConditionHides=true))
	bool bUseUniqueRightHandOverlayPoses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bUseUniqueRightHandOverlayPoses", EditConditionHides=true))
	TObjectPtr<UAnimSequence> OverlayPoses_RightHand;

	// Arms
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> RunArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandRunArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandRunArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> RunArms_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> SprintArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandSprintArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandSprintArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintArms_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	TObjectPtr<UAnimSequence> SprintImpulseArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	bool bUseUniqueRightHandSprintImpulseArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms", meta=(EditCondition="bUseUniqueRightHandSprintImpulseArms", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SprintImpulseArms_RightHand;

	// Aim
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAim", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Aim_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|")
	TObjectPtr<UAnimSequence> AimWading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimWading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|", meta=(EditCondition="bUseUniqueRightHandAimWading", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimWading_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> AimCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAimCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCrouch_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	TObjectPtr<UAnimSequence> AimProne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bUseUniqueRightHandAimProne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta=(EditCondition="bUseUniqueRightHandAimProne", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimProne_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	TObjectPtr<UAnimSequence> AimCantedLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	bool bUseUniqueRightHandAimCantedLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted", meta=(EditCondition="bUseUniqueRightHandAimCantedLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCantedLeft_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	TObjectPtr<UAnimSequence> AimCantedRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted")
	bool bUseUniqueRightHandAimCantedRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Canted", meta=(EditCondition="bUseUniqueRightHandAimCantedRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimCantedRight_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverLeft_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverRight_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverUpLeft", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverUpLeft_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	TObjectPtr<UAnimSequence> AimAroundCoverUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover")
	bool bUseUniqueRightHandAimAroundCoverUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Cover", meta=(EditCondition="bUseUniqueRightHandAimAroundCoverUpRight", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimAroundCoverUpRight_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	TObjectPtr<UAnimSequence> AimLadder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	bool bUseUniqueRightHandAimLadder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special", meta=(EditCondition="bUseUniqueRightHandAimLadder", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimLadder_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	TObjectPtr<UAnimSequence> AimSwimming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special")
	bool bUseUniqueRightHandAimSwimming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim|Special", meta=(EditCondition="bUseUniqueRightHandAimSwimming", EditConditionHides=true))
	TObjectPtr<UAnimSequence> AimSwimming_RightHand;

	// Lean
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> Lean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLean", EditConditionHides=true))
	TObjectPtr<UAnimSequence> Lean_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> LeanCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLeanCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLeanCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> LeanCrouch_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	TObjectPtr<UAnimSequence> LeanProne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean")
	bool bUseUniqueRightHandLeanProne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lean", meta=(EditCondition="bUseUniqueRightHandLeanProne", EditConditionHides=true))
	TObjectPtr<UAnimSequence> LeanProne_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsHigh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsHigh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsHigh", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsHigh_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsLow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsLow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsLow", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsLow_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle")
	bool bUseUniqueRightHandMantleSettingsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mantle", meta=(EditCondition="bUseUniqueRightHandMantleSettingsInAir", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> MantleSettingsInAir_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting")
	TSoftObjectPtr<UAlsMantlingSettings> VaultingSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting")
	bool bUseUniqueRightHandVaulting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vaulting", meta=(EditCondition="bUseUniqueRightHandVaulting", EditConditionHides=true))
	TSoftObjectPtr<UAlsMantlingSettings> VaultingSettings_RightHand;

	// Slope Slide
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	TObjectPtr<UAnimSequence> SlopeSlide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	bool bUseUniqueRightHandSlopeSlide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide", meta=(EditCondition="bUseUniqueRightHandSlopeSlide", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SlopeSlide_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	TObjectPtr<UAnimSequence> SlopeSlideCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide")
	bool bUseUniqueRightHandSlopeSlideCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slope Slide", meta=(EditCondition="bUseUniqueRightHandSlopeSlideCrouch", EditConditionHides=true))
	TObjectPtr<UAnimSequence> SlopeSlideCrouch_RightHand;
	

	bool operator==(const FAlsxtOverlayAnimationInstanceAssets& other) const
	{
		return (other.OverlayPoses == OverlayPoses) && (other.RunArms == RunArms) && (other.SprintArms == SprintArms) && (other.SprintImpulseArms == SprintImpulseArms) && (other.Aim == Aim) && (other.AimCrouch == AimCrouch) && (other.AimProne == AimProne) && (other.AimAroundCoverLeft == AimAroundCoverLeft) && (other.AimAroundCoverRight == AimAroundCoverRight) && (other.AimAroundCoverUpLeft == AimAroundCoverUpLeft) && (other.AimAroundCoverUpRight == AimAroundCoverUpRight) && (other.AimLadder == AimLadder) && (other.AimSwimming == AimSwimming);
	}

};