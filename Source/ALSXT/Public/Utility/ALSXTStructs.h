#pragma once

#include "NativeGameplayTags.h"
#include "Animation/AnimMontage.h"
#include "Engine/EngineTypes.h"
#include "Sound/SoundBase.h"
#include "NiagaraSystem.h"
#include "ALSXTStructs.generated.h"

class UAnimMontage;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFacialExpression
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Facial Expression", TitleProperty = "{FacialExpression}", AllowPrivateAccess))
	FGameplayTagContainer Expression;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<FName, float> Parameters;

	bool operator==(const FALSXTFacialExpression& other) const
	{
		return (other.Expression == Expression);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSubtitleEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FString SpeakerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Time{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float MinimumDisplayDuration{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool ForceDisplayAtTop{ false };

	bool operator==(const FSubtitleEntry& other) const
	{
		return (other.SpeakerCharacter == SpeakerCharacter) && (Text.EqualTo(other.Text)) && (other.Time == Time) && (other.MinimumDisplayDuration == MinimumDisplayDuration) && (other.ForceDisplayAtTop == ForceDisplayAtTop);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FSubtitles
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Mature{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool ManualWordWrap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool SingleLine{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FString SpokenText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float SubtitlePriority{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FSubtitleEntry> Subtitles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FString Comment;

	bool operator==(const FSubtitles& other) const
	{
		return (other.Mature == Mature) && (other.ManualWordWrap == ManualWordWrap) && (other.SingleLine == SingleLine) && (other.SpokenText == SpokenText) && (other.SubtitlePriority == SubtitlePriority) && (other.Subtitles == Subtitles) && (other.Comment == Comment);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	USoundBase* Sound{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Mature{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSubtitles Subtitles;

	bool operator==(const FSound& other) const
	{
		return (other.Sound == Sound) && (other.Mature == Mature) && (other.Subtitles == Subtitles);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FBoneLocationEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Location", AllowPrivateAccess))
	FGameplayTag Location;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FString Comment;

};

USTRUCT(BlueprintType)
struct ALSXT_API FExtendedHitResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Hit {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Mass{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Velocity{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Direction{FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Impulse{FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FHitResult HitResult;

};

USTRUCT(BlueprintType)
struct ALSXT_API FDoubleHitResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag CollisionType;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactType;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactLocation; // Calculated from Get Location from Bone Name

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactSide; // Calculated from Get Side from Hit

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Strength; // Calculated by Mass and Volume

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FExtendedHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FExtendedHitResult OriginHitResult;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackDoubleHitResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Overlay;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Strength; // Explicitly set in AnimNotify or Attack code

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float BaseDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FDoubleHitResult DoubleHitResult;

};

USTRUCT(BlueprintType)
struct ALSXT_API FTargetHitResultEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Valid { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float DistanceFromPlayer{ 340282346638528859811704183484516925440.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float AngleFromCenter {361.0f};

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FHitResult HitResult;

};

USTRUCT(BlueprintType)
struct ALSXT_API FLastTargetEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LastInteraction { 99.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LastHitTo { 99.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int ConsecutiveHits { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LastBlockedAttack { 99.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	AActor* Target {nullptr};

};

USTRUCT(BlueprintType)
struct ALSXT_API FActionMontageInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage { nullptr };

	// UnarmedAttack time to blend in amount curve.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveFloat> BlendInCurve { nullptr };

	// UnarmedAttack time to interpolation, horizontal and vertical correction amounts curve.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveVector> InterpolationAndCorrectionAmountsCurve { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector StartRelativeLocation{-65.0f, 0.0f, -100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D ReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D PlayRate{1.0f, 1.0f};

	bool operator==(const FActionMontageInfo& other) const
	{
		return (other.Montage == Montage) && (other.BlendInCurve == BlendInCurve) && (other.InterpolationAndCorrectionAmountsCurve == InterpolationAndCorrectionAmountsCurve) && (other.StartRelativeLocation == StartRelativeLocation) && (other.ReferenceHeight == ReferenceHeight) && (other.StartTime == StartTime) && (other.PlayRate == PlayRate);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FStaticStationaryModeAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", TitleProperty = "{AttackStrengths}", AllowPrivateAccess))
	TArray<FGameplayTag> StaticStationaryMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo EnterMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> IdlePose { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo ExitMontage;

	bool operator==(const FStaticStationaryModeAnimation& other) const
	{
		return (other.StaticStationaryMode == StaticStationaryMode);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FVehicleStationaryModeAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", TitleProperty = "{AttackStrengths}", AllowPrivateAccess))
	TArray<FGameplayTag> StationarySeatSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo EnterMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> IdlePose { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo ExitMontage;

	bool operator==(const FVehicleStationaryModeAnimation& other) const
	{
		return (other.StationarySeatSide == StationarySeatSide);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FVaultAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.LocomotionMode", AllowPrivateAccess))
	FGameplayTagContainer LocomotionMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer Stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Vault Type", AllowPrivateAccess))
	FGameplayTagContainer VaultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FVaultAnimation& other) const
	{
		return (other.LocomotionMode == LocomotionMode) && (other.Stance == Stance) && (other.Gait == Gait) && (other.VaultType == VaultType) && (other.Montage == Montage);
	}
};



USTRUCT(BlueprintType)
struct ALSXT_API FSyncedActionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo InstigatorSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetAnticipationPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetFallenPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetGetUpMontage;

	bool operator==(const FSyncedActionAnimation& other) const
	{
		return (other.InstigatorSyncedMontage == InstigatorSyncedMontage) && (other.TargetAnticipationPose == TargetAnticipationPose) && (other.TargetSyncedMontage == TargetSyncedMontage) && (other.TargetFallenPose == TargetFallenPose) && (other.TargetGetUpMontage == TargetGetUpMontage);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactForm
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag ImpactReactionForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FActionMontageInfo> BlockingMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FActionMontageInfo> RegularMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FActionMontageInfo> RecoveryMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FActionMontageInfo> DeathMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FActionMontageInfo DefaultFallbackMontage;
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionSide
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ImpactReactionSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray <FImpactForm> ImpactReactionForms;
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionStrength
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTag ImpactReactionStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray <FImpactReactionSide> ImpactReactionSides;
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionLocation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Location", AllowPrivateAccess))
	FGameplayTag ImpactReactionLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray <FImpactReactionStrength> ImpactReactionStrengths;
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionOverlayMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTag OverlayMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray <FImpactReactionLocation> ImpactReactionLocations;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", TitleProperty = "{AttackStrengths}", AllowPrivateAccess))
	FGameplayTagContainer AttackStrengths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", TitleProperty = "{AttackStances}", AllowPrivateAccess))
	FGameplayTagContainer AttackStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Type", TitleProperty = "{AttackType}", AllowPrivateAccess))
	FGameplayTagContainer AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FAttackAnimation& other) const
	{
		return (other.AttackStrengths == AttackStrengths) && (other.AttackStances == AttackStances) && (other.AttackType == AttackType) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSyncedAttackAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer AttackStrengths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", AllowPrivateAccess))
	FGameplayTagContainer AttackStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Type", TitleProperty = "{AttackType}", AllowPrivateAccess))
	FGameplayTagContainer AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSyncedActionAnimation SyncedMontage;

	bool operator==(const FSyncedAttackAnimation& other) const
	{
		return (other.AttackStrengths == AttackStrengths) && (other.AttackStances == AttackStances) && (other.AttackType == AttackType) && (other.SyncedMontage == SyncedMontage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackStance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", AllowPrivateAccess))
	FGameplayTag AttackStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag AnimationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> MontageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> RegularMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FSyncedActionAnimation> SyncedMontages;

	bool operator==(const FAttackStance& other) const
	{
		return (other.AttackStance == AttackStance);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackStrength
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTag AttackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackStance}", AllowPrivateAccess))
	TArray <FAttackStance> AttackStances;

	bool operator==(const FAttackStrength& other) const
	{
		return (other.AttackStrength == AttackStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FObstacleBumpReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FObstacleBumpReactionAnimation& other) const
	{
		return (other.Gait == Gait);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FCrowdBumpReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FCrowdBumpReactionAnimation& other) const
	{
		return (other.Gait == Gait);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", TitleProperty = "{AttackStrengths}", AllowPrivateAccess))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", TitleProperty = "{AttackStances}", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", TitleProperty = "{AttackStances}", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FImpactReactionAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FAttackReactionAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FFallenAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FallenPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> GetUpMontages;

	bool operator==(const FFallenAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAnticipationPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Health", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimMontage> Pose;

	bool operator==(const FAnticipationPose& other) const
	{
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Health == Health);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FResponseAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Health", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montages;

	bool operator==(const FResponseAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAttackType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Type", AllowPrivateAccess))
	FGameplayTag AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackStrengths}, {AttackStances}, {Montage}", AllowPrivateAccess))
	TArray<FAttackAnimation> RegularMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackStrengths}, {AttackStances}", AllowPrivateAccess))
	TArray<FSyncedAttackAnimation> SyncedMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ImpactStrength}, {ImpactSide}, {ImpactForm}", AllowPrivateAccess))
	TArray<FFallenAnimation> FallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ImpactStrength}, {ImpactSide}, {ImpactForm}, {Health}", AllowPrivateAccess))
	TArray<FResponseAnimation> ResponseAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackStrength}", AllowPrivateAccess))
	TArray<FAttackStrength> AttackStrengths;

	bool operator==(const FAttackType& other) const
	{
		return (other.AttackType == AttackType);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSound Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector2D PitchRange {1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector2D PlayRateRange{ 1.0f, 1.0f };

	bool operator==(const FALSXTCharacterSound& other) const
	{
		return (other.Sound == Sound) && (other.PitchRange == PitchRange) && (other.PlayRateRange == PlayRateRange);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterActionSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina", AllowPrivateAccess))
	FGameplayTagContainer Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FALSXTCharacterSound CharacterSound;

	bool operator==(const FALSXTCharacterActionSound& other) const
	{
		return (other.Strength == Strength) && (other.Stamina == Stamina) && (other.CharacterSound == CharacterSound);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterDamageSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Damage Amount", AllowPrivateAccess))
	FGameplayTagContainer Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FALSXTCharacterSound CharacterSound;

	bool operator==(const FALSXTCharacterDamageSound& other) const
	{
		return (other.Form == Form) && (other.Damage == Damage) && (other.CharacterSound == CharacterSound);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBloodSpatterType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Velocity", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Volume", AllowPrivateAccess))
	FGameplayTagContainer Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Distance", AllowPrivateAccess))
	FGameplayTagContainer Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Albedo {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Normal {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Packed {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float MaxDripSpeed {0.0f};

	bool operator==(const FALSXTBloodSpatterType& other) const
	{
		return (other.Velocity == Velocity) && (other.Volume == Volume) && (other.Distance == Distance) && (other.Albedo == Albedo) && (other.Normal == Normal) && (other.Packed == Packed) && (other.MaxDripSpeed == MaxDripSpeed);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTSurfaceInteraction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces}", AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Form", TitleProperty = "{Conditions}", AllowPrivateAccess))
	FGameplayTagContainer Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	USoundBase* Sound {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ParticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UNiagaraSystem* Particle {nullptr};

	bool operator==(const FALSXTSurfaceInteraction& other) const
	{
		return (other.Surfaces == Surfaces) && (other.Conditions == Conditions) && (other.Sound == Sound);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTSurfaceInteractionSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces} {Conditions} {Sound}", AllowPrivateAccess))
	TArray<FALSXTSurfaceInteraction> SurfaceInteractions;

	bool operator==(const FALSXTSurfaceInteractionSet& other) const
	{
		return (other.SurfaceInteractions == SurfaceInteractions);
	}
};