// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtRecoilAttributeSet.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Settings/AlsxtProceduralRecoilSettingsDataAsset.h"
#include "GameFramework/Character.h"
#include "Settings/AlsxtFirearmSettings.h"
#include "Utility/AlsxtAimableOverlayObjectGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtProceduralRecoilAnimComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAlsxtProceduralRecoil, Log, All);

DECLARE_DELEGATE(FAlsxtPerformProceduralRecoilTransitionDelegate);
DECLARE_DELEGATE_RetVal(bool, FAlsxtProceduralRecoilTransitionConditionDelegate);

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilGlobalMagnitudes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Global Magnitudes", meta=(ToolTip="Out Translation is multiplied byt this value for Final Output"))
	FVector Translation = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Global Magnitudes", meta=(ToolTip="Out Rotation is multiplied byt this value for Final Output"))
	FVector Rotation = FVector::OneVector;
};

UENUM(BlueprintType)
enum EAlsxtFiringMode
{
	Semi UMETA(DisplayName = "Semi"),
	Auto UMETA(DisplayName = "Auto"),
	Burst UMETA(DisplayName = "Burst"),
};

/** Ignore axis */
USTRUCT(BlueprintType)
struct FAlsxtIgnoreAxis
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool X {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Y {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Z {false};
};

USTRUCT(Blueprintable)
struct FAlsxtProceduralRecoilRestAxisCriteria
{
	GENERATED_BODY()

	FAlsxtProceduralRecoilRestAxisCriteria() = default;

	FAlsxtProceduralRecoilRestAxisCriteria(bool X, bool Y, bool Z) : X(X), Y(Y), Z(Z) { }
	
	bool X {false};
	bool Y {false};
	bool Z {false};
};

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilAnimationState
{
	GENERATED_BODY()

	FAlsxtProceduralRecoilTransitionConditionDelegate TransitionConditionDelegate;
	
	FAlsxtPerformProceduralRecoilTransitionDelegate OnPlayDelegate;
	FAlsxtPerformProceduralRecoilTransitionDelegate OnStopDelegate;
};

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	FGameplayTag Readiness {AlsxtReadyStanceTags::Relaxed};

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(Categories="Alsxt.Firearm Fire Mode",AllowPrivateAccess=true))
	FGameplayTag FireMode {AlsxtFirearmFireModeTags::SemiAuto};

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAlsxtFiringMode> FireModeOld {0};

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	bool bIsAiming {false};

	/** Current fire rate in shots per minute */
	float FireRate {0.0f};

	/** Current Burst Fire Rounds */
	int BurstRounds {0};

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TSoftObjectPtr<UCurveVector> CurrentRotationCurveAsset{nullptr};
	
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TSoftObjectPtr<UCurveVector> CurrentTranslationCurveAsset{nullptr};

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bTimelineInitialized {false};

	/** If simulation enabled or not */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bEnableSimulation {false};

	/** True: Animation is Playing.
	* False: Animation has ended or is blending out */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bRecoilAnimationPlaying  {true};

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bDischarging {false};

	/** Enabled for Repeating Fire Modes (Burst, Auto etc.) */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bRepeatingFireSmoothingEnabled {false};

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	float TimeSincePreviousDischarge {0.0f};
	
	/** Current Burst Fire count */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	int CurrentBurstCounter {0};

	/** Simplified state machine for animation states */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TArray<FAlsxtProceduralRecoilAnimationState> StateMachine;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	uint8 CurrentStateMachineIndex  {0};	
};

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilPlayerControllerMagnitudes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FVector2D Pitch = FVector2D::UnitVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FVector4 Yaw = FVector4(1, 1, 1, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FVector4 Roll = FVector4(1, 1, 1, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kickback")
	FVector2D Kickback = FVector2D::UnitVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kickback")
	FVector2D KickbackRightward = FVector2D::UnitVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kickback")
	FVector2D KickbackUpward = FVector2D::UnitVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FVector2D Noise = FVector2D::UnitVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General", meta=(ToolTip="X: Pitch, Y,Z: Offset"))
	FVector2D Progression = FVector2D::UnitVector;

	void Reset()
	{
		Pitch = FVector2D::UnitVector;
		Kickback = FVector2D::UnitVector;
		KickbackRightward = FVector2D::UnitVector;
		KickbackUpward = FVector2D::UnitVector;
		Noise = FVector2D::UnitVector;
		Progression = FVector2D::UnitVector;
		Yaw = FVector4(1, 1, 1, 1);
		Roll = FVector4(1, 1, 1, 1);
	}
	
	FAlsxtProceduralRecoilPlayerControllerMagnitudes operator* (const FAlsxtProceduralRecoilPlayerControllerMagnitudes& other) const
	{
		FAlsxtProceduralRecoilPlayerControllerMagnitudes NewMagnitudes;

		NewMagnitudes.Pitch *= other.Pitch;
		NewMagnitudes.Yaw *= other.Yaw;
		NewMagnitudes.Roll *= other.Roll;
		NewMagnitudes.Kickback *= other.Kickback;
		NewMagnitudes.KickbackRightward *= other.KickbackRightward;
		NewMagnitudes.KickbackUpward *= other.KickbackUpward;
		NewMagnitudes.Noise *= other.Noise;
		NewMagnitudes.Progression *= other.Progression;
		return NewMagnitudes;
	}

	void operator*= (const FAlsxtProceduralRecoilPlayerControllerMagnitudes& other)
	{
		Pitch *= other.Pitch;
		Yaw *= other.Yaw;
		Roll *= other.Roll;
		Kickback *= other.Kickback;
		KickbackRightward *= other.KickbackRightward;
		KickbackUpward *= other.KickbackUpward;
		Noise *= other.Noise;
		Progression *= other.Progression;
	}

	FAlsxtProceduralRecoilPlayerControllerMagnitudes operator/ (const FAlsxtProceduralRecoilPlayerControllerMagnitudes& other) const
	{
		FAlsxtProceduralRecoilPlayerControllerMagnitudes NewMagnitudes;

		NewMagnitudes.Pitch /= other.Pitch;
		NewMagnitudes.Yaw /= other.Yaw;
		NewMagnitudes.Roll /= other.Roll;
		NewMagnitudes.Kickback /= other.Kickback;
		NewMagnitudes.KickbackRightward /= other.KickbackRightward;
		NewMagnitudes.KickbackUpward /= other.KickbackUpward;
		NewMagnitudes.Noise /= other.Noise;
		NewMagnitudes.Progression /= other.Progression;
		return NewMagnitudes;
	}

	void operator/= (const FAlsxtProceduralRecoilPlayerControllerMagnitudes& other)
	{
		Pitch /= other.Pitch;
		Yaw /= other.Yaw;
		Roll /= other.Roll;
		Kickback /= other.Kickback;
		KickbackRightward /= other.KickbackRightward;
		KickbackUpward /= other.KickbackUpward;
		Noise /= other.Noise;
		Progression /= other.Progression;
	}
};

UCLASS(ClassGroup=(RecoilAnimation), meta=(BlueprintSpawnableComponent))
class ALSXT_API UAlsxtProceduralRecoilAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAlsxtProceduralRecoilAnimComponent();

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Essentials")
	void SetProceduralRecoilSettings(UAlsxtProceduralRecoilSettingsDataAsset* NewProceduralRecoilSettings, const float Rate = 0.0f, const int Bursts = 0);
	
	UFUNCTION(BlueprintCallable, Category = "Essentials")
	void PlayProceduralRecoilAnimation();
	
	UFUNCTION(BlueprintCallable, Category = "Essentials")
	void StopProceduralRecoilAnimation();
	
	FORCEINLINE FTransform GetOutput() const { return FTransform(OutRotation, OutTranslation); };
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetFiringMode(EAlsxtFiringMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetAiming(bool bAiming);
	
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetAimingStatus() { return bIsAiming; }

	UFUNCTION(BlueprintCallable, Category = "Magnitudes")
	void SetPlayerControllerMagnitudes(FAlsxtProceduralRecoilPlayerControllerMagnitudes NewMagnitudes);

	UFUNCTION(BlueprintCallable, Category = "Magnitudes")
	void RemovePlayerControllerMagnitudes(FAlsxtProceduralRecoilPlayerControllerMagnitudes MagnitudesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Magnitudes")
	void ResetPlayerControllerMagnitudes();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State", meta=(DevelopmentOnly))
	FORCEINLINE EAlsxtFiringMode GetFireMode() const { return FireMode; };
	
	/** Returns time since the previous shot */
	UFUNCTION(BlueprintCallable, Category = "State")
	float GetDeltaSincePreviousDischarge() const;
	
	void AddEvent(float Time, FOnTimelineEvent Event);

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FAlsxtIgnoreAxis IgnoreRotationAxes;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FAlsxtIgnoreAxis IgnoreTranslationAxes;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TObjectPtr<UAlsxtProceduralRecoilSettingsDataAsset> ProceduralRecoilSettings;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TObjectPtr<UAlsxtFirearmDischargeSettingsDataAsset> FirearmDischargeSettings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Settings")
	FAlsxtProceduralRecoilPlayerControllerMagnitudes PlayerControllerMagnitudes;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FAlsxtProceduralRecoilState ProceduralRecoilAnimationState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FVector GlobalTranslationMultiplier = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FVector GlobalRotationMultiplier = FVector::OneVector;

private:
	UAbilitySystemComponent* OwnerAbilitySystemComponent;
	const UAlsxtRecoilAttributeSet* RecoilAttributeSet;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void RefreshRecoilAnimationTimeline(FVector Alpha);

	UFUNCTION(BlueprintCallable, Category = "State")
	void CalculateTargetRecoilParameters();

	/**
	 * Interpolation between values using Curve
	 * @param Origin is the A parameter of Lerp
	 * @param Target is the previous alpha value
	 * @param Current is the current alpha value
	 * @param bStartRest determines whether to start rest curve or not
	 */
	double GetCorrectOriginValue(double& Origin, double& Target, const double Current, bool& bStartRest);

	/** Lerp_FrameRateIndependent
	 * @name Lerp_FrameRateIndependent
	 * @param Origin Origin
	 * @param Target Target
	 * @param Alpha Alpha
	 * @param DeltaTime DeltaTime
	 */
	FORCEINLINE double Lerp_FrameRateIndependent(double Origin, double Target, double Alpha, double DeltaTime)
	{
		return FMath::Lerp(Origin, Target, 1 - FMath::Exp(-Alpha * DeltaTime));
	};

	FVector Lerp_FrameRateIndependent(FVector A, FVector B, FVector Alpha, float DeltaTime);
	FRotator Lerp_FrameRateIndependent(FRotator A, FRotator B, FVector Alpha, float DeltaTime);
	
	void InitializeStateMachine();

	void SetSimulationTimer(float SemiAutoFireModeDelay);
	
	/**
     * @brief Return a random Float value (Normalized) within Min and Max Ranges *
     * @param Ranges Min (X-Y (Random)) and Max (Z- W (Random)) Values
     */
	double GetRandomFloatInRanges(const FVector4 Ranges);

	/** @brief Sets current playback to the absolute position to compensate timer error */
	void ErrorCorrectTimerAlpha(const TSoftObjectPtr<UCurveVector> Rotation, const TSoftObjectPtr<UCurveVector> Location, const float TimeCorrection);

	/** @brief Sets up curves and timeline for another state */
	void SetupTransition(FRotator OriginRotation, FVector OriginTranslation, TSoftObjectPtr<UCurveVector> TargetRotation, TSoftObjectPtr<UCurveVector> TargetTranslation);
	
	void SetupComponent();

	void RefreshProceduralRecoilSolver(float DeltaTime);
	void RefreshPlayerController(float DeltaTime);

	/** @brief Resets Parameters using Spring Interpolation */
	float SpringInterpolationReset(float Current, FFloatSpringState& SpringState, float Stiffness, float Damping, float DeltaTime);

	void RefreshSmoothing(float DeltaTime);
	void RefreshNoise(FTransform& Finalized, float DeltaTime);
	void RefreshPushback(FTransform& Finalized, float DeltaTime);
	void RefreshProgression(FTransform& Finalized, float DeltaTime);
	void RefreshRecoilSway(FTransform& Finalized, float DeltaTime);

	double GetValidValue(double OldValue, double NewValue) const
	{
		if(FMath::IsNearlyZero(NewValue))
		{
			return OldValue;
		}

		return NewValue;
	}

	void SetControllerRecoilTarget();
	void RefreshControllerCompensation();
	float RefreshCompensation(float RecoilValue, float RecoilCompensation) const;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Output Data", meta=(AllowPrivateAccess=true))
	FRotator OutRotation{FRotator::ZeroRotator};
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Output Data", meta=(AllowPrivateAccess=true))
	FVector OutTranslation{FVector::ZeroVector};
	
	UPROPERTY()
	TSoftObjectPtr<UCurveVector> CurrentRotationCurveAsset{nullptr};
	
	UPROPERTY()
	TSoftObjectPtr<UCurveVector> CurrentTranslationCurveAsset{nullptr};
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	bool bIsAiming = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	float TimeSincePreviousDischarge = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	FTimerHandle ProceduralRecoilAnimationTimerHandle;
	
	FTimeline RecoilAnimationTimeline;

	/* Important
	 * 0 = X
	 * 1 = Y
	 * 2 = Z
	 */
	FAlsxtProceduralRecoilRestAxisCriteria RotationAxisRestCriteria;
	FAlsxtProceduralRecoilRestAxisCriteria TranslationAxisRestCriteria;

	FRotator CurrentOriginRotation{FRotator::ZeroRotator};
	FVector CurrentOriginTranslation{FVector::ZeroVector};

	FRotator CurrentTargetRotation{FRotator::ZeroRotator};
	FVector CurrentTargetTranslation{FVector::ZeroVector};

	/** These are current spring states used by FloatSpingInterp */
	FFloatSpringState PitchState;
	FFloatSpringState RollState;
	FFloatSpringState KickState;

	/** Binds a member function to the Timeline Tick() */
	FOnTimelineVector TimelineProgressionFunction;

	/** If simulation enabled or not */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bEnableSimulation;

	/** Must be updated whenever current fire mode is changed */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAlsxtFiringMode> FireMode;

	/** Current fire rate in shots per minute */
	float CurrentFireRate= 0.0f;

	/** Current Burst Fire Rounds */
	int CurrentBurstAmount= 0;

	/** Current Burst Fire count */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	int CurrentBurstCounter= 0;

	/** Simplified state machine for animation states */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	TArray<FAlsxtProceduralRecoilAnimationState> StateMachine;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	uint8 CurrentStateMachineIndex = 0;

	/** Raw non-interp data */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	FRotator RawRotation{FRotator::ZeroRotator};
	
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	FVector RawTranslation{FVector::ZeroVector};

	/** Enabled for Repeating Fire Modes (Burst, Auto etc.) */
	bool bRepeatingFireSmoothingEnabled {false};

	bool bTimelineInitialized{false};

	/** True: Animation is Playing.
	* False: Animation has ended or is blending out */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bRecoilAnimationPlaying = true;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta=(AllowPrivateAccess=true))
	bool bIsDischarging{false};

	FVector2D Noise2DTarget{FVector2D::ZeroVector};
	FVector2D Noise2DCurrent{FVector2D::ZeroVector};
	FVector2D NoiseSpeed{FVector2D::ZeroVector};

	double PushbackTarget = 0.0;
	double PushbackCurrent = 0.0;
	double PushbackOut = 0.0;
	double PushbackSpeed = 0.0;

	FRotator InterpolatedRotation{FRotator::ZeroRotator};
	FVector InterpolatedTranslation{FVector::ZeroVector};
	
	FVector2D RotationPitchProgression{FVector2D::ZeroVector};
	FVector2D TranslationZProgression{FVector2D::ZeroVector};
	double RotationPitchProgressionSpeed = 0.0;
	double TranslationZProgressionSpeed = 0.0;

	FVector2D PitchSway{FVector2D::ZeroVector};
	FVector2D YawSway{FVector2D::ZeroVector};

	FVector2D Compensation{FVector::ZeroVector};
	FVector2D TargetRecoil{FVector::ZeroVector};
	FVector2D Recoil{FVector::ZeroVector};
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
};