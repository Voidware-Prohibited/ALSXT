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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayThumbnail = "true", AllowPrivateAccess))
	USoundBase* Sound{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector2D PitchRange {1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSubtitles Subtitles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Mature{ false };

	bool operator==(const FSound& other) const
	{
		return (other.Sound == Sound) && (other.PitchRange == PitchRange) && (other.Subtitles == Subtitles) && (other.Mature == Mature);
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
struct ALSXT_API FImpactHistoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	float Distance{ 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	double Time {0.0f};

	bool operator==(const FImpactHistoryEntry& other) const
	{
		return (other.Actor == Actor);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FExtendedHitResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bHit {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Mass{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Velocity{FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Direction{FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Impulse{FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactLocation; // Calculated from Get Location from Bone Name

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactSide; // Calculated from Get Side from Hit

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactStrength; // Calculated by Mass and Volume

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactGait; // Used for Bump and Crowd Navigation Reactions

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
	FDateTime DateTime;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage { nullptr };

	// UnarmedAttack time to blend in amount curve.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UCurveFloat> BlendInCurve { nullptr };

	// UnarmedAttack time to interpolation, horizontal and vertical correction amounts curve.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UCurveVector> InterpolationAndCorrectionAmountsCurve { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector StartRelativeLocation{-65.0f, 0.0f, -100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D ReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = 0))
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
	FGameplayTagContainer StaticStationaryMode;

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
	FGameplayTagContainer StationarySeatSide;

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
struct ALSXT_API FMantleAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Mantle Type", AllowPrivateAccess))
	FGameplayTagContainer MantleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Hand", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FMantleAnimation& other) const
	{
		return (other.MantleType == MantleType) && (other.Gait == Gait) && (other.Side == Side) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FVaultAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Vault Type", AllowPrivateAccess))
	FGameplayTagContainer VaultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float VerticalOffset {0.0f};

	bool operator==(const FVaultAnimation& other) const
	{
		return (other.VaultType == VaultType) && (other.Gait == Gait) && (other.Montage == Montage) && (other.VerticalOffset == VerticalOffset);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FInspectItemAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> EnterMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> IdleMontage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> ExitMontage{ nullptr };

	bool operator==(const FInspectItemAnimation& other) const
	{
		return (other.Montage == Montage) && (other.EnterMontage == EnterMontage) && (other.IdleMontage == IdleMontage) && (other.ExitMontage == ExitMontage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSyncedActionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo InstigatorSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveVector> InstigatorCameraCurve { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetAnticipationPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveVector> TargetCameraCurve { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> TargetFallingMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimMontage> TargetFallenPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> TargetGetUpMontages;

	bool operator==(const FSyncedActionAnimation& other) const
	{
		return (other.InstigatorSyncedMontage == InstigatorSyncedMontage) && (other.TargetAnticipationPose == TargetAnticipationPose) && (other.TargetSyncedMontage == TargetSyncedMontage) && (other.TargetFallingMontages == TargetFallingMontages) && (other.TargetFallenPose == TargetFallenPose) && (other.TargetGetUpMontages == TargetGetUpMontages);
	}

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
	FGameplayTagContainer AttackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", AllowPrivateAccess))
	FGameplayTagContainer AttackStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Type", TitleProperty = "{AttackType}", AllowPrivateAccess))
	FGameplayTagContainer AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSyncedActionAnimation SyncedMontage;

	bool operator==(const FSyncedAttackAnimation& other) const
	{
		return (other.AttackStrength == AttackStrength) && (other.AttackStance == AttackStance) && (other.AttackType == AttackType) && (other.SyncedMontage == SyncedMontage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FMoveToTargetAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", TitleProperty = "{AttackStances}", AllowPrivateAccess))
	FGameplayTagContainer Stances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FMoveToTargetAnimation& other) const
	{
		return (other.Stances == Stances) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FBumpReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FBumpReactionAnimation& other) const
	{
		return (other.Velocity == Velocity) && (other.Side == Side) && (other.Form == Form) && (other.Montage == Montage);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FCrowdBumpReactionAnimation& other) const
	{
		return (other.Gait == Gait) && (other.Side == Side) && (other.Form == Form) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FBumpPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UAnimSequenceBase> Pose;

	bool operator==(const FBumpPose& other) const
	{
		return (other.Gait == Gait) && (other.Side == Side) && (other.Form == Form) && (other.Pose == Pose);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FImpactReactionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Impact Velocity}", AllowPrivateAccess))
	FGameplayTagContainer ImpactVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", TitleProperty = "{Impact Side}", AllowPrivateAccess))
	FGameplayTagContainer ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", TitleProperty = "{Impact Form}", AllowPrivateAccess))
	FGameplayTagContainer ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FImpactReactionAnimation& other) const
	{
		return (other.ImpactVelocity == ImpactVelocity) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Montage == Montage);
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
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Montage == Montage);
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
	TArray<FActionMontageInfo> FallingMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FallenPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TArray<FActionMontageInfo> GetUpMontages;

	bool operator==(const FFallenAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.FallingMontages == FallingMontages) && (other.FallenPose == FallenPose) && (other.GetUpMontages == GetUpMontages);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FDeathAnimation
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

	bool operator==(const FDeathAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAnticipationPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer Stance;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Health", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimSequenceBase> Pose;

	bool operator==(const FAnticipationPose& other) const
	{
		return (other.Velocity == Velocity) && (other.Stance == Stance) && (other.Side == Side) && (other.Form == Form) && (other.Health == Health) && (other.Pose == Pose);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FClutchImpactLocationAnimation
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
	TObjectPtr<UAnimSequenceBase> Pose;

	bool operator==(const FClutchImpactLocationAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Health == Health) && (other.Pose == Pose);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FStabilizationMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer Stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Health", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FStabilizationMontage& other) const
	{
		return (other.Velocity == Velocity) && (other.Stance == Stance) && (other.Side == Side) && (other.Form == Form) && (other.Health == Health) && (other.Montage == Montage);
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
	FActionMontageInfo Montage;

	bool operator==(const FResponseAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength) && (other.ImpactSide == ImpactSide) && (other.ImpactForm == ImpactForm) && (other.Health == Health) && (other.Montage == Montage);
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
struct ALSXT_API FALSXTCharacterVoiceParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTag Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTag Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (ClampMin=0.5f, ClampMax=1.5f, AllowPrivateAccess))
	float Speed {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (ClampMin = 0.5f, ClampMax = 1.5f, AllowPrivateAccess))
	float Pitch {1.0f};
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterEyeParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector EyeColor {FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector EyeColor2 {FVector(ForceInit)};
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterSkinParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector SkinColor {FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector SkinColor2 {FVector(ForceInit)};
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterHairParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FString HairStyleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector HairColor1 {FVector(ForceInit)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FVector HairColor2 {FVector(ForceInit)};
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterMorphParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FName MorphName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	float Value {0.0f};

	bool operator==(const FALSXTCharacterMorphParameter& other) const
	{
		return (other.MorphName == MorphName) && (other.Value == Value);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterMorphParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	TArray<FALSXTCharacterMorphParameter> Parameters;

	bool operator==(const FALSXTCharacterMorphParameters& other) const
	{
		return (other.Parameters == Parameters);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterClothingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTagContainer ClothingSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FString ClothingItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Item Condition", AllowPrivateAccess))
	FGameplayTag ClothingItemCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Clothing State", AllowPrivateAccess))
	FGameplayTagContainer ClothingItemStates;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterIdentity
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	FALSXTCharacterVoiceParameters Voice;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBreathSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Stamina", AllowPrivateAccess))
	FGameplayTagContainer Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTBreathSound& other) const
	{
		return (other.Sex == Sex) && (other.Variant == Variant) && (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHoldingBreathSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Holding Breath", AllowPrivateAccess))
	FGameplayTagContainer HoldingBreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTHoldingBreathSound& other) const
	{
		return (other.Sex == Sex) && (other.Variant == Variant) && (other.HoldingBreathType == HoldingBreathType) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBreathParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina", TitleProperty = "{Stamina}", AllowPrivateAccess))
	FGameplayTagContainer Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Breath Type", TitleProperty = "{BreathType}", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<UNiagaraSystem*> Particles;

	bool operator==(const FALSXTBreathParticle& other) const
	{
		return (other.Stamina == Stamina) && (other.Particles == Particles);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterActionSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Stamina", AllowPrivateAccess))
	FGameplayTagContainer Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", ShowOnlyInnerProperties, AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTCharacterActionSound& other) const
	{
		return (other.Sex == Sex) && (other.Overlay == Overlay) && (other.Strength == Strength) && (other.Stamina == Stamina) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterMovementSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Character Movement Sound", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Object Weight", AllowPrivateAccess))
	FGameplayTagContainer Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTCharacterMovementSound& other) const
	{
		return (other.Overlay == Overlay) && (other.Type == Type) && (other.Weight == Weight) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterMovementSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", ShowOnlyInnerProperties, AllowPrivateAccess))
	TArray<FALSXTCharacterMovementSound> Sounds;

	bool operator==(const FALSXTCharacterMovementSounds& other) const
	{
		return (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTWeaponMovementSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Weapon", AllowPrivateAccess))
	FGameplayTagContainer Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Character Movement Sound", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTWeaponMovementSound& other) const
	{
		return (other.Weapon == Weapon) && (other.Type == Type) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTWeaponMovementSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTWeaponMovementSound> Sounds;

	bool operator==(const FALSXTWeaponMovementSounds& other) const
	{
		return (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTWeaponActionSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Weapon", AllowPrivateAccess))
	FGameplayTagContainer Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Weapon Action", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTWeaponActionSound& other) const
	{
		return (other.Weapon == Weapon) && (other.Type == Type) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTWeaponActionSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTWeaponActionSound> Sounds;

	bool operator==(const FALSXTWeaponActionSounds& other) const
	{
		return (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterDamageSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Attack Method", AllowPrivateAccess))
	FGameplayTagContainer AttackMethod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Game.Damage Amount", AllowPrivateAccess))
	FGameplayTagContainer Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (TitleProperty = "{Sound}", AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTCharacterDamageSound& other) const
	{
		return (other.Sex == Sex) && (other.Variant == Variant) && (other.Form == Form) && (other.AttackMethod == AttackMethod) && (other.Damage == Damage) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> PhysicalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FSound> Sounds;

	bool operator==(const FALSXTImpactSound& other) const
	{
		return (other.PhysicalMaterials == PhysicalMaterials) && (other.Velocity == Velocity) && (other.Form == Form) && (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactParticleActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> PhysicalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ParticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> BackParticleActor;

	bool operator==(const FALSXTImpactParticleActor& other) const
	{
		return (other.Velocity == Velocity) && (other.Form == Form) && (other.ParticleActor == ParticleActor) && (other.BackParticleActor == BackParticleActor);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactParticleActorMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTImpactParticleActor> ParticleActors;

	// bool operator==(const FALSXTImpactParticleActorMap& other) const
	// {
	// 	return (other.ParticleActors == ParticleActors);
	// }
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactPointParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UNiagaraSystem* Particle{ nullptr };

	bool operator==(const FALSXTImpactPointParticle& other) const
	{
		return (other.Velocity == Velocity) && (other.Form == Form) && (other.Particle == Particle);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> PhysicalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Mature {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UNiagaraSystem* ImpactParticle{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UNiagaraSystem* ImpactBackParticle{ nullptr };

	bool operator==(const FALSXTImpactParticle& other) const
	{
		return (other.PhysicalMaterials == PhysicalMaterials) && (other.Velocity == Velocity) && (other.Form == Form) && (other.ImpactParticle == ImpactParticle) && (other.ImpactBackParticle == ImpactBackParticle);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTImpactSound> Sounds;

	bool operator==(const FALSXTImpactSounds& other) const
	{
		return (other.Sounds == Sounds);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactParticles
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTImpactParticle> Particles;

	bool operator==(const FALSXTImpactParticles& other) const
	{
		return (other.Particles == Particles);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactParticleMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTImpactParticles> Particles;

	// bool operator==(const FALSXTImpactParticleMap& other) const
	// {
	// 	return (other.Particles == Particles);
	// }
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactDecal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> PhysicalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Size", AllowPrivateAccess))
	FGameplayTagContainer Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UMaterialInterface* ImpactDecal{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UMaterialInterface* ImpactBackDecal{ nullptr };

	bool operator==(const FALSXTImpactDecal& other) const
	{
		return (other.PhysicalMaterials == PhysicalMaterials) && (other.Velocity == Velocity) && (other.Form == Form) && (other.ImpactDecal == ImpactDecal) && (other.ImpactBackDecal == ImpactBackDecal);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactDecals
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTImpactDecal> Decals;

	bool operator==(const FALSXTImpactDecals& other) const
	{
		return (other.Decals == Decals);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactDecalMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTImpactDecals> Decals;

	// bool operator==(const FALSXTImpactParticleMap& other) const
	// {
	// 	return (other.Particles == Particles);
	// }
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBallisticDecalMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTImpactParticles> Particles;

	// bool operator==(const FALSXTImpactParticleMap& other) const
	// {
	// 	return (other.Particles == Particles);
	// }
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactSoundMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTImpactSounds> Sounds;

	// bool operator==(const FALSXTImpactSoundMap& other) const
	// {
	// 	return (other.Sounds == Sounds);
	// }
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTResponseVocalization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Sex", TitleProperty = "{Sex}", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Variant", TitleProperty = "{Variant}", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool Mature {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FALSXTCharacterSound ResponseVocalization;

	bool operator==(const FALSXTResponseVocalization& other) const
	{
		return (other.Sex == Sex) && (other.Variant == Variant) && (other.Velocity == Velocity) && (other.Form == Form) && (other.Health == Health) && (other.Mature == Mature) && (other.ResponseVocalization == ResponseVocalization);
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
struct ALSXT_API FALSXTElementalDecal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Volume", TitleProperty = "{Size}", AllowPrivateAccess))
	FGameplayTagContainer Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UMaterialInterface* Decal {nullptr};

	bool operator==(const FALSXTElementalDecal& other) const
	{
		return (other.Size == Size) && (other.Decal == Decal);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTElementalParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Volume", TitleProperty = "{Volume}", AllowPrivateAccess))
	FGameplayTagContainer Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UNiagaraSystem* Particle {nullptr};

	bool operator==(const FALSXTElementalParticle& other) const
	{
		return (other.Volume == Volume) && (other.Velocity == Velocity) && (other.Particle == Particle);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTElementalSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Velocity", TitleProperty = "{Velocity}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Rebound", TitleProperty = "{Rebound}", AllowPrivateAccess))
	FGameplayTagContainer Rebound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	USoundBase* Sound{nullptr};

	bool operator==(const FALSXTElementalSound& other) const
	{
		return (other.Velocity == Velocity) && (other.Rebound == Rebound) && (other.Sound == Sound);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTElementalParticleActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Volume", TitleProperty = "{Conditions}", AllowPrivateAccess))
	FGameplayTagContainer Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Velocity", TitleProperty = "{Conditions}", AllowPrivateAccess))
	FGameplayTagContainer Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> Particle {nullptr};

	bool operator==(const FALSXTElementalParticleActor& other) const
	{
		return (other.Volume == Volume) && (other.Velocity == Velocity) && (other.Particle == Particle);
	}

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTMeshPaintCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces}", AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Mesh Paint Type", TitleProperty = "{PaintType}", AllowPrivateAccess))
	FGameplayTag PaintType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Elemental Condition", TitleProperty = "{Conditions}", AllowPrivateAccess))
	FGameplayTagContainer Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool FadeOut{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FadeOutDelay{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FadeOutSpeed{ 0.0f };

	bool operator==(const FALSXTMeshPaintCriteria& other) const
	{
		return (other.Surfaces == Surfaces) && (other.PaintType == PaintType) && (other.Conditions == Conditions) && (other.FadeOut == FadeOut) && (other.FadeOutDelay == FadeOutDelay) && (other.FadeOutSpeed == FadeOutSpeed);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTMeshPaintCriteriaMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces}", AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> MeshPaintCriteriaMap;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTElementalInteraction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces}", AllowPrivateAccess))
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Elemental Condition", TitleProperty = "{Conditions}", AllowPrivateAccess))
	FGameplayTagContainer Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTElementalSound> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTElementalParticle> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTElementalDecal> Decals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTElementalParticleActor> ParticleActors;

	bool operator==(const FALSXTElementalInteraction& other) const
	{
		return (other.Surfaces == Surfaces) && (other.Conditions == Conditions) && (other.Sounds == Sounds) && (other.Particles == Particles) && (other.Decals == Decals) && (other.ParticleActors == ParticleActors);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTElementalInteractionSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces} {Conditions} {Sounds}", AllowPrivateAccess))
	TArray<FALSXTElementalInteraction> ElementalInteractions;

	bool operator==(const FALSXTElementalInteractionSet& other) const
	{
		return (other.ElementalInteractions == ElementalInteractions);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FIdleAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Sex", TitleProperty = "{Sex}", AllowPrivateAccess))
	FGameplayTagContainer Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", TitleProperty = "{Overlay}", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", TitleProperty = "{Stance}", AllowPrivateAccess))
	FGameplayTagContainer Stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Injury", TitleProperty = "{Injury}", AllowPrivateAccess))
	FGameplayTagContainer Injury;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locomotion Variant", TitleProperty = "{Variant}", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Combat Stance", TitleProperty = "{CombatStance}", AllowPrivateAccess))
	FGameplayTagContainer CombatStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Idle Animation Activity", TitleProperty = "{Activity}", AllowPrivateAccess))
	FGameplayTagContainer Activity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool Loops{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage { nullptr };

	bool operator==(const FIdleAnimation& other) const
	{
		return (other.Sex == Sex) && (other.Overlay == Overlay) && (other.Stance == Stance) && (other.Injury == Injury) && (other.Variant == Variant) && (other.CombatStance == CombatStance) && (other.Activity == Activity) && (other.Loops == Loops) && (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FForegripPositionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FGameplayTag Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = ( AllowPrivateAccess))
	FName SocketName;

	bool operator==(const FForegripPositionEntry& other) const
	{
		return (other.Position == Position) && (other.SocketName == SocketName);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FForegripPositions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FForegripPositionEntry Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FForegripPositionEntry MagazineWell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FForegripPositionEntry Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FForegripPositionEntry Angled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess))
	FForegripPositionEntry Top;

	bool operator==(const FForegripPositions& other) const
	{
		return (other.Default == Default) && (other.MagazineWell == MagazineWell) && (other.Vertical == Vertical) && (other.Angled == Angled) && (other.Top == Top);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FMotionSoundAreaMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Character Movement Sound", TitleProperty = "{Motions}", AllowPrivateAccess))
	FGameplayTagContainer Motions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (Categories = "Als.Motion Sound Area", TitleProperty = "{MotionSoundArea}", AllowPrivateAccess))
	FGameplayTag MotionSoundArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (TitleProperty = "{MotionSoundArea}", AllowPrivateAccess))
	FName MotionSoundBone;

	bool operator==(const FMotionSoundAreaMap& other) const
	{
		return (other.Motions == Motions) && (other.MotionSoundArea == MotionSoundArea) && (other.MotionSoundBone == MotionSoundBone);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FMotionSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	TArray<FSound> BreathSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	TArray<FSound> VocalSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	float VocalSoundsPitch{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	TArray<FSound> CharacterMovementSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	float CharacterMovementSoundsPitch{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	TArray<FSound> CharacterMovementAccentSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	float CharacterMovementAccentPitch{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{CharacterMovementSocketName}", AllowPrivateAccess))
	FName CharacterMovementSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Sounds}", AllowPrivateAccess))
	TArray<FSound> WeaponMovementSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ActionSounds}", AllowPrivateAccess))
	float WeaponMovementSoundsPitch{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{WeaponMovementSocketName}", AllowPrivateAccess))
	FName WeaponMovementSocketName;

	bool operator==(const FMotionSounds& other) const
	{
		return (other.BreathSounds == BreathSounds) && (other.VocalSounds == VocalSounds) && (other.VocalSoundsPitch == VocalSoundsPitch) && (other.CharacterMovementSounds == CharacterMovementSounds) && (other.CharacterMovementSoundsPitch == CharacterMovementSoundsPitch) && (other.CharacterMovementAccentSounds == CharacterMovementAccentSounds) && (other.CharacterMovementAccentPitch == CharacterMovementAccentPitch) && (other.CharacterMovementSocketName == CharacterMovementSocketName) && (other.WeaponMovementSounds == WeaponMovementSounds) && (other.WeaponMovementSoundsPitch == WeaponMovementSoundsPitch) && (other.WeaponMovementSocketName == WeaponMovementSocketName);
	}
};