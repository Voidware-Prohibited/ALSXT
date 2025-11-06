// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterMovementComponent.h"
#include "Settings/AlsxtMovementSettings.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtCharacterMovementComponent.generated.h"

class UAlsxtMovementSettings;
class AAlsxtCharacter;


UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_Ladder		UMETA(DisplayName = "Ladder"),
	CMOVE_MAX			UMETA(Hidden),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterSlideSlopeAngleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitSlideSlopeAngleDelegate);

/**
 * 
 */
UCLASS(ClassGroup=(ALSXT), meta=(BlueprintSpawnableComponent))
class ALSXT_API UAlsxtCharacterMovementComponent : public UAlsCharacterMovementComponent
{
	GENERATED_BODY()

	friend AAlsxtCharacter;

public:
	virtual void BeginPlay() override;

	virtual FVector ConsumeInputVector() override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

	void SetMovementSpeedMultiplier(float Multiplier);

	float GetCharacterWeight() const;

	float GetStandingWalkSpeedMultiplier() const;
	float GetStandingAimingSpeedMultiplier() const;
	float GetStandingCombatSpeedMultiplier() const;
	float GetStandingRunSpeedMultiplier() const;
	float GetStandingSprintSpeedMultiplier() const;
	float GetCrouchedWalkSpeedMultiplier() const;
	float GetCrouchedAimingSpeedMultiplier() const;
	float GetCrouchedCombatSpeedMultiplier() const;
	float GetCrouchedRunSpeedMultiplier() const;
	float GetCrouchedSprintSpeedMultiplier() const;
	float GetProneWalkSpeedMultiplier() const;
	float GetProneAimingSpeedMultiplier() const;
	float GetProneCombatSpeedMultiplier() const;
	float GetProneRunSpeedMultiplier() const;
	float GetProneSprintSpeedMultiplier() const;

	float GetClimbingWalkSpeedMultiplier() const;
	float GetClimbingAimingSpeedMultiplier() const;
	float GetClimbingCombatSpeedMultiplier() const;
	float GetClimbingRunSpeedMultiplier() const;
	float GetClimbingSprintSpeedMultiplier() const;
	float GetSwimmingWalkSpeedMultiplier() const;
	float GetSwimmingAimingSpeedMultiplier() const;
	float GetSwimmingCombatSpeedMultiplier() const;
	float GetSwimmingRunSpeedMultiplier() const;
	float GetSwimmingSprintSpeedMultiplier() const;

	float GetMaxStandingWalkSpeed() const;
	float GetMaxStandingAimingSpeed() const;
	float GetMaxStandingCombatSpeed() const;
	float GetMaxStandingRunSpeed() const;
	float GetMaxStandingSprintSpeed() const;
	float GetMaxCrouchedWalkSpeed() const;
	float GetMaxCrouchedAimingSpeed() const;
	float GetMaxCrouchedCombatSpeed() const;
	float GetMaxCrouchedRunSpeed() const;
	float GetMaxCrouchedSprintSpeed() const;
	float GetMaxProneWalkSpeed() const;
	float GetMaxProneAimingSpeed() const;
	float GetMaxProneCombatSpeed() const;
	float GetMaxProneRunSpeed() const;
	float GetMaxProneSprintSpeed() const;

	float GetMaxClimbingWalkSpeed() const;
	float GetMaxClimbingAimingSpeed() const;
	float GetMaxClimbingCombatSpeed() const;
	float GetMaxClimbingRunSpeed() const;
	float GetMaxClimbingSprintSpeed() const;
	float GetMaxSwimmingWalkSpeed() const;
	float GetMaxSwimmingAimingSpeed() const;
	float GetMaxSwimmingCombatSpeed() const;
	float GetMaxSwimmingRunSpeed() const;
	float GetMaxSwimmingSprintSpeed() const;

	void SetStandingWalkSpeedMultiplier(float Multiplier);
	void SetStandingAimingSpeedMultiplier(float Multiplier);
	void SetStandingCombatSpeedMultiplier(float Multiplier);
	void SetStandingRunSpeedMultiplier(float Multiplier);
	void SetStandingSprintSpeedMultiplier(float Multiplier);
	void SetCrouchedWalkSpeedMultiplier(float Multiplier);
	void SetCrouchedAimingSpeedMultiplier(float Multiplier);
	void SetCrouchedCombatSpeedMultiplier(float Multiplier);
	void SetCrouchedRunSpeedMultiplier(float Multiplier);
	void SetCrouchedSprintSpeedMultiplier(float Multiplier);
	void SetProneWalkSpeedMultiplier(float Multiplier);
	void SetProneAimingSpeedMultiplier(float Multiplier);
	void SetProneCombatSpeedMultiplier(float Multiplier);
	void SetProneRunSpeedMultiplier(float Multiplier);
	void SetProneSprintSpeedMultiplier(float Multiplier);

	void SetClimbingWalkSpeedMultiplier(float Multiplier);
	void SetClimbingAimingSpeedMultiplier(float Multiplier);
	void SetClimbingCombatSpeedMultiplier(float Multiplier);
	void SetClimbingRunSpeedMultiplier(float Multiplier);
	void SetClimbingSprintSpeedMultiplier(float Multiplier);

	void SetSwimmingWalkSpeedMultiplier(float Multiplier);
	void SetSwimmingAimingSpeedMultiplier(float Multiplier);
	void SetSwimmingCombatSpeedMultiplier(float Multiplier);
	void SetSwimmingRunSpeedMultiplier(float Multiplier);
	void SetSwimmingSprintSpeedMultiplier(float Multiplier);

	FOnEnterSlideSlopeAngleDelegate OnEnterSlideSlopeAngle;

	UPROPERTY(BlueprintAssignable, Category="Movement")
	FOnExitSlideSlopeAngleDelegate OnExitSlideSlopeAngle;
	
protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<UAlsxtMovementSettings> AlsxtMovementSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsxtMovementGaitSettings AlsxtGaitSettings;

	/**
	 * @todo Add the following into settings
	 */
	// The angle when character begins sliding
	float SlideSlopeAngle = 45.0f;

	// Amount of degrees wetness can affect the sliding angle, to make it appear slippery
	float WetnessSlideSlopeAngleModifier = 3.0f;
	
	float LastSlopeAngle = 0.0f;

	void CheckSlopeAngle();
	
	float MovementSpeedMultiplier = 1.f;

	float StandingWalkSpeedMultiplier = 1.f;
	float StandingAimingSpeedMultiplier = 1.f;
	float StandingCombatSpeedMultiplier = 1.f;
	float StandingRunSpeedMultiplier = 1.f;
	float StandingSprintSpeedMultiplier = 1.f;
	
	float CrouchedWalkSpeedMultiplier = 1.f;
	float CrouchedAimingSpeedMultiplier = 1.f;
	float CrouchedCombatSpeedMultiplier = 1.f;
	float CrouchedRunSpeedMultiplier = 1.f;
	float CrouchedSprintSpeedMultiplier = 1.f;
	
	float ProneWalkSpeedMultiplier = 1.f;
	float ProneAimingSpeedMultiplier = 1.f;
	float ProneCombatSpeedMultiplier = 1.f;
	float ProneRunSpeedMultiplier = 1.f;
	float ProneSprintSpeedMultiplier = 1.f;
	
	float ClimbingWalkSpeedMultiplier = 1.f;
	float ClimbingAimingSpeedMultiplier = 1.f;
	float ClimbingCombatSpeedMultiplier = 1.f;
	float ClimbingRunSpeedMultiplier = 1.f;
	float ClimbingSprintSpeedMultiplier = 1.f;

	float SwimmingWalkSpeedMultiplier = 1.f;
	float SwimmingAimingSpeedMultiplier = 1.f;
	float SwimmingCombatSpeedMultiplier = 1.f;
	float SwimmingRunSpeedMultiplier = 1.f;
	float SwimmingSprintSpeedMultiplier = 1.f;

	//

	float StandingWalkBackwardSpeedMultiplier = 1.f;
	float StandingAimingBackwardSpeedMultiplier = 1.f;
	float StandingCombatBackwardSpeedMultiplier = 1.f;
	float StandingRunBackwardSpeedMultiplier = 1.f;
	float StandingSprintBackwardSpeedMultiplier = 1.f;
	
	float CrouchedWalkBackwardSpeedMultiplier = 1.f;
	float CrouchedAimingBackwardSpeedMultiplier = 1.f;
	float CrouchedCombatBackwardSpeedMultiplier = 1.f;
	float CrouchedRunBackwardSpeedMultiplier = 1.f;
	float CrouchedSprintBackwardSpeedMultiplier = 1.f;
	
	float ProneWalkBackwardSpeedMultiplier = 1.f;
	float ProneAimingBackwardSpeedMultiplier = 1.f;
	float ProneCombatBackwardSpeedMultiplier = 1.f;
	float ProneRunBackwardSpeedMultiplier = 1.f;
	float ProneSprintBackwardSpeedMultiplier = 1.f;
	
	float ClimbingWalkBackwardSpeedMultiplier = 1.f;
	float ClimbingAimingBackwardSpeedMultiplier = 1.f;
	float ClimbingCombatBackwardSpeedMultiplier = 1.f;
	float ClimbingRunBackwardSpeedMultiplier = 1.f;
	float ClimbingSprintBackwardSpeedMultiplier = 1.f;

	float SwimmingWalkBackwardSpeedMultiplier = 1.f;
	float SwimmingAimingBackwardSpeedMultiplier = 1.f;
	float SwimmingCombatBackwardSpeedMultiplier = 1.f;
	float SwimmingRunBackwardSpeedMultiplier = 1.f;
	float SwimmingSprintBackwardSpeedMultiplier = 1.f;
	
};
