#pragma once
#include "Curves/CurveVector.h"

#include "AlsxtProceduralAnimationSettings.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtGeneralProceduralAnimationSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseProceduralMovementSway {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bStaminaInfluencesMovementSway {true};
	
	// Enable Sway for Statuses like Cold and Illness
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseProceduralStatusSway {true};

	// False will use Leaning assets specified in Overlay Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseProceduralLean {true};
};

USTRUCT(BlueprintType)
struct FAlsxtMovementSwayLocationSettings
{
	GENERATED_BODY()
	// How much to add/reduce the sway provided from the movement sway curve
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float Multiplier {1.0f};
	
	// How fast it will interpolate to the new sway multiplier
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float MultiplierInterpolationRate {4.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	TObjectPtr<UCurveVector> Curve;
	
	// The speed in which it interpolates to a new value in the curve
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float InterpSpeed {10.0f};
};

USTRUCT(BlueprintType)
struct FAlsxtMovementSwaySettings
{
	GENERATED_BODY()
	
	// Enable or Disable movement sway for this Overlay
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseMovementSway {true};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementSway"))
	FAlsxtMovementSwayLocationSettings LocationSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementSway"))
	FAlsxtMovementSwayLocationSettings RotationSettings;
};

USTRUCT(BlueprintType)
struct FAlsxtMovementLagSpringInterpSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float SpringStiffness {6.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float SpringCriticalDampingFactor {0.45f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float SpringMass {0.05f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float SpringTargetVelocityAmount {1.0f};
};

USTRUCT(BlueprintType)
struct FAlsxtMovementLagSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseMovementLag {true};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	bool bDisableForwardMovementLagWhileAiming {true};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	bool bUseSpringInterpolation {true};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag && bUseSpringInterpolation"))
	FAlsxtMovementLagSpringInterpSettings SpringInterpSettings;

	// The max normalization value the procedurals will rotate up/down. Smaller = more
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	float ForwardBackwardNormalizedMax {100.0f};
	// The max normalization value the procedurals will move left/right. Smaller = more
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	float LeftRightNormalizedMax {100.0f};
	// The max normalization value the procedurals will move up/down. Smaller = more
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	float UpDownNormalizedMax {100.0f};
	// This is for the forward/backward movement which moves the muzzle down when moving forwards, and down when moving backwards
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	float ForwardBackwardMultiplier {1.0f};
	// This controls the left/right/up/down
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag"))
	FRotator Multiplier {FRotator(1.0f, 1.0f, 1.0f)};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseMovementLag && !bUseSpringInterpolation"))
	float InterpSpeed {8.0f};
};

USTRUCT(BlueprintType)
struct FAlsxtRotationLagSpringInterpSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float RotationSpringStiffness {24.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float RotationSpringCriticalDampingFactor {0.6f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float RotationSpringMass {0.1f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float RotationSpringTargetVelocityAmount {0.5f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float LocationSpringStiffness {12.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float LocationSpringCriticalDampingFactor {0.65f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float LocationSpringMass {0.05f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float LocationSpringTargetVelocityAmount {0.0f};
};

USTRUCT(BlueprintType)
struct FAlsxtRotationLagInterpSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float RotationInterpSpeed {10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float LocationInterpSpeed {10.0f};
};

USTRUCT(BlueprintType)
struct FAlsxtRotationLagSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseRotationLag {true};
	// I advise this to be false if you are using deadzone, in my opinion it gives a better result
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag"))
	bool bUseSpringInterpolation {true};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag && bUseSpringInterpolation"))
	FAlsxtRotationLagSpringInterpSettings SpringInterpSettings;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag && !bUseSpringInterpolation"))
	FAlsxtRotationLagInterpSettings InterpSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag"))
	FRotator MaxRotation {FRotator(10.0f, 10.0f, 10.0f)};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag"))
	FVector MaxLocation {FVector(10.0f, 10.0f, 10.0f)};
	// Default in BP: X = Roll, Y = Pitch, Z = Yaw
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag"))
	FRotator RotationMultiplier {FRotator(0.5f, 0.5f, 0.5f)};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseRotationLag"))
	FVector LocationMultiplier {FVector(0.25f, 0.25f, 0.25f)};
};

USTRUCT(BlueprintType)
struct FAlsxtLeanLeftRightSettings
{
	GENERATED_BODY()
	// Whether or not to use movement sway (such as idle breathing and walking sway)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	bool bUseSpringInterp {true};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseSpringInterp"))
	float Stiffness {1.5f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseSpringInterp"))
	float CriticalDampingFactor {0.55f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseSpringInterp"))
	float Mass {0.015f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "bUseSpringInterp"))
	float TargetVelocityAmount {0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural", meta = (EditCondition = "!bUseSpringInterp"))
	float InterpSpeed {10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float MaxLeanLeftAngle {45.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AlsxtProcedural")
	float MaxLeanRightAngle {45.0f};
};
