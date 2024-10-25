#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Settings/ALSXTDefensiveModeSettings.h"
#include "ALSXTDefensiveModeState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeState
{
	GENERATED_BODY()

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Mode {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ImpactMode{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AnticipationMode{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ObstacleMode{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> ImpactMontage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> AnticipationPose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> AnticipationCrouchedPose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> AnticipationInAirPose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> ObstaclePose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTDefensivePoseSet ObstaclePoseSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTDefensivePoseStanceSet CrowdNavigationPoseSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTAnticipationPoseSet AnticipationPoseSet;

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> Montage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ObstacleTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ObstacleSide{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Height", AllowPrivateAccess))
	FGameplayTag ObstacleHeight{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AnticipationTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag AnticipationSide{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Height", AllowPrivateAccess))
	FGameplayTag AnticipationHeight{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag AnticipationForm{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ImpactTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ImpactSide{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Height", AllowPrivateAccess))
	FGameplayTag ImpactHeight{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag ImpactForm{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", AllowPrivateAccess))
	FGameplayTag ImpactVelocity{ FGameplayTag::EmptyTag };

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Side{ FGameplayTag::EmptyTag };

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Form{ FGameplayTag::EmptyTag };

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Velocity{ FGameplayTag::EmptyTag };

	// Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	bool operator==(const FALSXTDefensiveModeState& other) const
	{
		return (other.Mode == Mode) && (other.Montage == Montage) && (other.Side == Side) && (other.Form == Form) && (other.Velocity == Velocity) && (other.Transform.GetLocation() == Transform.GetLocation()) && (other.Transform.GetRotation() == Transform.GetRotation());
	}
};

