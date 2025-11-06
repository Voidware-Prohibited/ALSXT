#pragma once

#include "NativeGameplayTags.h"
#include "Animation/AnimMontage.h"
#include "Engine/EngineTypes.h"
#include "AlsxtAnimationStructs.generated.h"

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
struct ALSXT_API FAlsxtSwimmingAnimationInstanceBlendSpaceAssets
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

	bool operator==(const FAlsxtSwimmingAnimationInstanceBlendSpaceAssets& other) const
	{
		return (other.WalkRun_Forward == WalkRun_Forward) && (other.WalkRun_Left_Forward == WalkRun_Left_Forward);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationInstanceRunStartAssets
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

	bool operator==(const FAlsxtAnimationInstanceRunStartAssets& other) const
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> IdlePose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	TObjectPtr<UBlendSpace> SprintLean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAlsxtAnimationInstanceRotateInPlaceAssets RotateInPlaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxtAnimationInstanceBlendSpaceAssets BlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxt1DLocomotionBlendSpaceAssets SidlingRightBlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxt1DLocomotionBlendSpaceAssets SidlingLeftBlendSpaceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxtAnimationInstanceRunStartAssets RunStartAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxtAnimationInstanceMovementAssets PlantFootAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standing")
	FAlsxtAnimationInstanceStopTransitionAssets StopTransitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch, Prone and Ladder")
	TObjectPtr<UAnimSequence> WalkPose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch, Prone and Ladder")
	FAlsxtAnimationInstanceMovementAssets MovementAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch, Prone and Ladder")
	FAlsxtSwimmingAnimationInstanceBlendSpaceAssets SwimmingBlendSpaceAssets;

	bool operator==(const FAlsxtLocomotionAnimationInstanceAssets& other) const
	{
		return (other.IdlePose == IdlePose) && (other.Sprint == Sprint) && (other.RotateInPlaceAssets == RotateInPlaceAssets);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayAnimationInstanceAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> OverlayPoses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Run_Arms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Sprint_Arms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Sprint_Impulse_Arms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> SlopeSlide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> SlopeSlide_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Prone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Lean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Lean_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Lean_Prone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_AroundCover;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Crouch_AroundCover;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Stationary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Ladder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Aim_Swimming;
	

	bool operator==(const FAlsxtOverlayAnimationInstanceAssets& other) const
	{
		return (other.OverlayPoses == OverlayPoses) && (other.Run_Arms == Run_Arms) && (other.Sprint_Arms == Sprint_Arms) && (other.Sprint_Impulse_Arms == Sprint_Impulse_Arms) && (other.Aim == Aim) && (other.Aim_Crouch == Aim_Crouch) && (other.Aim_Prone == Aim_Prone) && (other.Aim_AroundCover == Aim_AroundCover) && (other.Aim_Crouch_AroundCover == Aim_Crouch_AroundCover) && (other.Aim_Stationary == Aim_Stationary) && (other.Aim_Ladder == Aim_Ladder) && (other.Aim_Swimming == Aim_Swimming);
	}

};