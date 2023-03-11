#pragma once

#include "NativeGameplayTags.h"
#include "Animation/AnimMontage.h"
#include "Engine/EngineTypes.h"
#include "Components/AudioComponent.h"
#include "ALSXTStructs.generated.h"

class UAnimMontage;
class UCurveFloat;
class UCurveVector;

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
	AActor* Target;

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
		return (other.Montage == Montage) && (other.BlendInCurve == BlendInCurve) && (other.InterpolationAndCorrectionAmountsCurve == InterpolationAndCorrectionAmountsCurve);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSyncedActionMontageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo AttackerSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetAnticipationPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetSyncedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetFallenPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo TargetGetUpMontage;

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
	TArray<FGameplayTag> AttackStrengths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", TitleProperty = "{AttackStances}", AllowPrivateAccess))
	TArray<FGameplayTag> AttackStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montage;

	bool operator==(const FAttackAnimation& other) const
	{
		return (other.AttackStrengths == AttackStrengths);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FSyncedAttackAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	TArray<FGameplayTag> AttackStrengths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Stance", AllowPrivateAccess))
	TArray<FGameplayTag> AttackStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FSyncedActionMontageInfo SyncedMontage;

	bool operator==(const FSyncedAttackAnimation& other) const
	{
		return (other.AttackStrengths == AttackStrengths);
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
	TArray<FSyncedActionMontageInfo> SyncedMontages;

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
struct ALSXT_API FFallenAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTag ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag ImpactForm;

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
	FGameplayTag ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	TObjectPtr<UAnimMontage> Pose;

	bool operator==(const FFallenAnimation& other) const
	{
		return (other.ImpactStrength == ImpactStrength);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FResponseAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Action Strength", AllowPrivateAccess))
	FGameplayTag ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag ImpactSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTag ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Montage}", AllowPrivateAccess))
	FActionMontageInfo Montages;

	bool operator==(const FFallenAnimation& other) const
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
	TObjectPtr<USoundBase> Sound;

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
	TArray<FGameplayTag> Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina", AllowPrivateAccess))
	TArray<FGameplayTag> Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FALSXTCharacterSound Sound;

	bool operator==(const FALSXTCharacterActionSound& other) const
	{
		return (other.Strength == Strength) && (other.Stamina == Stamina) && (other.Sound == Sound);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterDamageSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Form", AllowPrivateAccess))
	TArray<FGameplayTag> Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Strength", AllowPrivateAccess))
	TArray<FGameplayTag> Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FALSXTCharacterSound Sound;

	bool operator==(const FALSXTCharacterDamageSound& other) const
	{
		return (other.Form == Form) && (other.Damage == Damage) && (other.Sound == Sound);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBloodSpatterType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Form", AllowPrivateAccess))
	TArray<FGameplayTag> Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Attack Form", AllowPrivateAccess))
	TArray<FGameplayTag> Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Blood Spatter Distance", AllowPrivateAccess))
	TArray<FGameplayTag> Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Albedo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* Packed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float MaxDripSpeed;

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
	TArray<FGameplayTag> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<UAudioComponent> Sound;

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