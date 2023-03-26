#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTCombatSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCombatParameters
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
	FGameplayTag Location{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTCombatSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackType}", AllowPrivateAccess))
	TArray<FAttackType> AttackTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackType}", AllowPrivateAccess))
	TArray<FAttackAnimation> AttackAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{AttackType}", AllowPrivateAccess))
	TArray<FSyncedAttackAnimation> SyncedAttackAnimations;

	FActionMontageInfo CurrentActionMontage;

public:
	float CalculateStartTime(FVector2D ReferenceHeight, FVector2D StartTime, float AttackHeight) const;

	float CalculatePlayRate(FVector2D ReferenceHeight, FVector2D PlayRate, float AttackHeight) const;
};

inline float UALSXTCombatSettings::CalculateStartTime(FVector2D ReferenceHeight, FVector2D StartTime, const float AttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, StartTime, AttackHeight);
}

inline float UALSXTCombatSettings::CalculatePlayRate(FVector2D ReferenceHeight, FVector2D PlayRate, const float AttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, PlayRate, AttackHeight);
}

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCombatAttackTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Start {0.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector End {0.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Radius { 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector AttackOrigin {0.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag Overlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FGameplayTag AttackStrength;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCombatSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableCombat{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableAttacks{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableSyncedAttacks{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableSpecials{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableGrapples{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableThrows{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableTakedowns{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FLinearColor HighlightColor { 1.0f, 0.0f, 1.0f, 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FName HighlightMaterialParameterName { "Highlight" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector	TraceAreaHalfSize { 400.0f, 400.0f, 150.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Target Lock", Meta = (Units = "cm", AllowPrivateAccess))
	float MaxInitialLockDistance { 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Target Lock", Meta = (Units = "cm", AllowPrivateAccess))
	float MaxLockDistance { 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Target Lock", Meta = (AllowPrivateAccess))
	bool UnlockWhenTargetIsObstructed { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstructionTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Debug", Meta = (AllowPrivateAccess))
	bool DebugMode {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Debug", Meta = (AllowPrivateAccess))
	float DebugDuration { 4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateToInputOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float RotationInterpolationSpeed{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180))
	float RotationOffset{ 45.0f };

};