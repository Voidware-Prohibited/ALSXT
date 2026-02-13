#pragma once

#include "AlsxtProceduralAimSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtProceduralAimSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableProceduralAim{ true };
	
	// Left is 0, Right is 1, 3 is Ambidex
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin=0, ClampMax=1))
	int DominantHand{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin=0, ClampMax=1))
	float BaseNonDominantHandHandlingAbility{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float AimedMaxTargetTraceDistance{ 500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCombatProceduralAim{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float CombatMaxTargetTraceDistance{ 100.0 };
};

USTRUCT(BlueprintType)
struct FSKGCycleAimingPointSpringInterpSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float LocationStiffness {12.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float LocationCriticalDampingFactor {0.65f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float LocationMass {0.05f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float LocationTargetVelocityAmount {0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float RotationStiffness {18.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float RotationCriticalDampingFactor {0.6f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float RotationMass {0.05f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float RotationTargetVelocityAmount {0.0f};
};


// Settings used for when changing aiming points (such as going from one optic to another for aiming, or point aiming)
USTRUCT(BlueprintType)
struct FSKGCycleAimingPointSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	bool bUseSpringInterpForCyclingAimingPoint {true};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseSpringInterpForCyclingAimingPoint"))
	FSKGCycleAimingPointSpringInterpSettings SpringInterpSettings;
};

USTRUCT(BlueprintType)
struct FSKGProceduralAimingSettings
{
	GENERATED_BODY()
	/*
	 * The forward axis of the firearm (NOT the attachment) which is used for correcting the aiming distance
	 * and it must be set on all attachments used to aim
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	TEnumAsByte<EAxis::Type> ForwardAxis {EAxis::Type::Y};
	// The higher the value the further the aim point will be from your camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float AimingDistance {10.0f};
	/*
	 * If true, the position of the aim point is discarded and will the aim point to be exactly
	 * the AimingDistance behind the camera
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	bool bUseFixedAimingDistance {false};
	// This is the base speed used for aiming. If you have a firearm with an optic, the value for the firearm is used.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float DefaultAimingSpeed {10.0f};
	// Amount the camera will zoom in when you aim
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float CameraZoomPercentage {0.0f};
	// Amount the camera will zoom in when you aim
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	float CameraZoomInterpSpeed {10.0f};
	// If false, this aiming device will not be used when cycling aiming devices
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	bool bCanAim {true};
};

USTRUCT(BlueprintType)
struct FSKGDeadzoneSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	bool bUseDeadzone {false};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	float PitchRate {0.6f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	float YawRate {0.6f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	float PitchLimit {5.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	float YawLimit {10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	float InterpolationSpeed {10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone"))
	bool bDisableDeadzoneWhenAiming {false};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural", meta = (EditCondition = "bUseDeadzone && bDisableDeadzoneWhenAiming"))
	float DeadzoneAimingDisableInterpolationSpeed {6.0f};
};

USTRUCT(BlueprintType)
struct FSKGFirstAndThirdPersonBasePoseOffset
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	FTransform FirstPersonOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKGProcedural")
	FTransform ThirdPersonOffset;
};
