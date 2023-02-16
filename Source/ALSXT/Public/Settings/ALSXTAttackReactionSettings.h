#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTAttackReactionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTAttackReactionParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector_NetQuantize100 TargetRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "cm"))
	float ImpactHeight{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FDoubleHitResult Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ImpactLocation{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTAttackReactionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactReactionLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector StartRelativeLocation{-65.0f, 0.0f, -100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D ReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D PlayRate{1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray <FImpactReactionStrength> ImpactReactionStrengths;

public:
	float CalculateStartTime(float AttackHeight) const;

	float CalculatePlayRate(float AttackHeight) const;
};

inline float UALSXTAttackReactionSettings::CalculateStartTime(const float AttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, StartTime, AttackHeight);
}

inline float UALSXTAttackReactionSettings::CalculatePlayRate(const float AttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, PlayRate, AttackHeight);
}

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralAttackReactionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowAttackReaction{ true };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateToInputOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float RotationInterpolationSpeed{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180))
	float RotationOffset{ 45.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FActionMontageInfo DefaultMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FImpactReactionLocation> AttackReactionLocations;
};