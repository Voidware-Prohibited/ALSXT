#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTImpactReactionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactReactionParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactType{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactReactionStage{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FDoubleHitResult CrowdNavigationHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FDoubleHitResult BumpHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FDoubleHitResult ImpactHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FAttackDoubleHitResult AttackHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactVelocity{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactForm{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactLocation{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FAnticipationPose AnticipationPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FAnticipationPose DefensivePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FBumpReactionAnimation CrowdNavigationReactionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FBumpReactionAnimation BumpReactionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FImpactReactionAnimation ImpactReactionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FAttackReactionAnimation AttackReactionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag FallType{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector FallLandLocation {FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FFallenAnimation ImpactFallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FFallenAnimation AttackFallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float FallenMinimumTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentFallenTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FAnticipationPose BraceForImpactPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FResponseAnimation ResponseAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FFallenAnimation DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FSyncedAttackAnimation SyncedAttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float BaseDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	AActor* Target {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "cm"))
	float ImpactHeight{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector_NetQuantize100 TargetRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float TargetYawAngle{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector StartRelativeLocation{-65.0f, 0.0f, -100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D ReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D PlayRate{1.0f, 1.0f};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTImpactReactionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Defensive Modes", Meta = (TitleProperty = "{Velocity} {Stance} {Side} {Form} {Health} {Pose}", AllowPrivateAccess))
	TArray<FAnticipationPose> DefensivePoses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Defensive Modes", Meta = (TitleProperty = "{Velocity} {Stance} {Side} {Form} {Health} {Pose}", AllowPrivateAccess))
	TArray<FAnticipationPose> ImpactAnticipationPoses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Defensive Modes", Meta = (TitleProperty = "{Velocity} {Stance} {Side} {Form} {Health} {Pose}", AllowPrivateAccess))
	TArray<FAnticipationPose> AttackAnticipationPoses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Defensive Modes", Meta = (TitleProperty = "{Velocity} {Stance} {Side} {Form} {Health} {Pose}", AllowPrivateAccess))
	TArray<FAnticipationPose> StablizationPoses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Crowd Navigation", Meta = (TitleProperty = "{Velocity} {Side} {Form} {Montage}", AllowPrivateAccess))
	TArray<FBumpReactionAnimation> CrowdNavigationReactionAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Impact Reaction", Meta = (TitleProperty = "{Velocity} {Side} {Form} {Montage}", AllowPrivateAccess))
	TArray<FBumpReactionAnimation> BumpReactionAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Impact Reaction", Meta = (TitleProperty = "{ImpactVelocity} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray<FImpactReactionAnimation> ImpactReactionAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Impact Reaction", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray<FAttackReactionAnimation> AttackReactionAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Impact Reaction", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Health} {Pose}", AllowPrivateAccess))
	TArray<FClutchImpactLocationAnimation> ClutchImpactPointAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Falling", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray <FFallenAnimation> CrowdNavigationFallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Falling", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray <FFallenAnimation> ImpactFallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Falling", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray <FFallenAnimation> AttackFallenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Falling", Meta = (TitleProperty = "{Velocity} {Stance} {Side} {Form} {Health} {Pose}", AllowPrivateAccess))
	TArray<FAnticipationPose> BraceForImpactPoses;

	UPROPERTY(EditAnywhere, Category = "Animations|Falling|Settings", BlueprintReadWrite, Meta = (TitleProperty = "{FallenMinimumTime}", AllowPrivateAccess))
	float ImpactFallenMinimumTime { 0.0f };

	UPROPERTY(EditAnywhere, Category = "Animations|Falling|Settings", BlueprintReadWrite, Meta = (TitleProperty = "{AttackFallenMinimumTime}", AllowPrivateAccess))
	float AttackFallenMinimumTime { 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Response", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Health} {Montage}", AllowPrivateAccess))
	TArray<FResponseAnimation> CrowdNavigationResponseAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Response", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Health} {Montage}", AllowPrivateAccess))
	TArray<FResponseAnimation> ImpactResponseAnimations;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Response", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Health} {Montage}", AllowPrivateAccess))
	TArray<FResponseAnimation> AttackResponseAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Death", Meta = (TitleProperty = "{ImpactStrength} {ImpactSide} {ImpactForm} {Montage}", AllowPrivateAccess))
	TArray <FDeathAnimation> DeathAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Impact", Meta = (TitleProperty = "{Sounds}", AllowPrivateAccess))
	FALSXTImpactSoundMap ImpactSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Falling", Meta = (TitleProperty = "{Sounds}", AllowPrivateAccess))
	FALSXTImpactSoundMap BodyFallSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles|Impact", Meta = (TitleProperty = "{Particles}", AllowPrivateAccess))
	FALSXTImpactParticleMap ImpactParticles;

public:
	float CalculateStartTime(FVector2D ReferenceHeight, FVector2D StartTime, float ImpactHeight) const;

	float CalculatePlayRate(FVector2D ReferenceHeight, FVector2D PlayRate, float ImpactHeight) const;
};

inline float UALSXTImpactReactionSettings::CalculateStartTime(FVector2D ReferenceHeight, FVector2D StartTime, const float ImpactHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, StartTime, ImpactHeight);
}

inline float UALSXTImpactReactionSettings::CalculatePlayRate(FVector2D ReferenceHeight, FVector2D PlayRate, const float ImpactHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, PlayRate, ImpactHeight);
}

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralImpactReactionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableImpactReactions{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableAttackReactions{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableBumpReactions{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableFacialExpressions{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableCharacterSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableImpactSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableParticleActors{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableParticles{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableClutchImpactPoint{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableBraceForImpact{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableCrowdNavigationResponses{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableImpactResponses{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableAttackResponses{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableAutoGetUp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float CharacterBumpDetectionMinimumVelocity {175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float ObstacleBumpDetectionMinimumVelocity {175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float BumpDetectionRadius {30.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float MaxBumpDetectionDistance {0.125f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector	AnticipationAreaHalfSize { 20.0f, 40.0f, 50.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableSlideToCoverHook{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float MaxSlideToCoverDetectionDistance {0.25f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateToInputOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float RotationInterpolationSpeed{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180))
	float RotationOffset{ 45.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<TEnumAsByte<EObjectTypeQuery>> BumpTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<TEnumAsByte<EObjectTypeQuery>> ImpactTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", meta=(TitleProperty="{Settings}", ForceInlineRow))
	TMap<FName, FBoneLocationEntry> BoneLocationMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugMode{ false };
};