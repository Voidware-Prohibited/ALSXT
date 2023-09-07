#pragma once

#include "ALSXTAcrobaticActionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralAcrobaticActionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bAcrobaticActions { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableWallJump { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableWallRun { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bDebugMode { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float DebugTraceDuration { 2.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float ForwardTraceDistance { 70.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float DownTraceDistance { 25.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float LateralTraceDistance { 25.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> FlipMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> BackflipMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> WallJumpForwardMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> WallJumpLeftAngleMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> WallJumpRightAngleMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> WallRunLeftMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> WallRunRightMontage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float MaximumVelocityForBackflip { 245.00 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float MinimumElevationForFlip { 100.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float MinimumSpeedForWallJump { 25.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float MinimumSpeedForWallRun { 500.0 };
};
