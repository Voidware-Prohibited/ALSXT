// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#include "Components/Character/AlsxtProceduralRecoilAnimComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Curves/CurveVector.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtRecoilAttributeSet.h"
#include "AbilitySystem/Interfaces/AlsxtAbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtFirearmInterface.h"

DEFINE_LOG_CATEGORY(LogAlsxtProceduralRecoil);

using UKismetMath = UKismetMathLibrary;

UAlsxtProceduralRecoilAnimComponent::UAlsxtProceduralRecoilAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetIsReplicatedByDefault(true);
}

void UAlsxtProceduralRecoilAnimComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Bind Functions to play on Timeline Events
	TimelineProgressionFunction.BindUFunction(this, TEXT("RefreshRecoilAnimationTimeline"));
	FOnTimelineEvent Event;
	Event.BindUFunction(this, TEXT("CalculateTargetRecoilParameters"));
	RecoilAnimationTimeline.AddEvent(0.f, Event);
	PlayerControllerMagnitudes.Reset();
}

void UAlsxtProceduralRecoilAnimComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// SetupComponent();
	// Get Character (Assumes Component exists in Character class)
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// Double check AlsxtCharacter implements UAlsxtAbilitySystemInterface
	if (GetOwner()->Implements<UAlsxtAbilitySystemInterface>())
	{
		if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (ASCInterface->GetAbilitySystemComponent())
			{
				OwnerAbilitySystemComponent = ASCInterface->GetAbilitySystemComponent();
				if (OwnerAbilitySystemComponent->GetSet<UAlsxtRecoilAttributeSet>())
				{
					RecoilAttributeSet = OwnerAbilitySystemComponent->GetSet<UAlsxtRecoilAttributeSet>();
					const UAlsxtRecoilAttributeSet* RecoilAttributes = OwnerAbilitySystemComponent->GetSet<UAlsxtRecoilAttributeSet>();
				}
				else
				{
					UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("UAlsxtRecoilAttributeSet Not Found in UAlsxtAbilitySystemComponent"));
					return;
				}
			}
			else
			{
				UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("IAbilitySystemInterface Not Found in Owner"));
				return;
			}			
		}
	}
	else
	{
		UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("UAlsxtAbilitySystemInterface Not Implemented in Owner"));
		return;
	}

	if (GetOwner()->Implements<UAlsxtCharacterInterface>())
	{
		
	}
	else
	{
		UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("UAlsxtCharacterInterface Not Implemented in Owner"));
		return;
	}

	if (GetOwner()->Implements<UAlsxtFirearmInterface>())
	{
		
	}
	FGameplayTag Readiness = IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner());
	FGameplayTag FiringMode = IAlsxtFirearmInterface::Execute_GetFirearmFireMode(GetOwner());
	InitializeStateMachine();
	SetComponentTickEnabled(false);
}

void UAlsxtProceduralRecoilAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!IsValid(ProceduralRecoilSettings))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("ProceduralRecoilSettingsDataAsset Not Found"));
		return;
	}

	RefreshPlayerController(DeltaTime);
	RefreshProceduralRecoilSolver(DeltaTime);
	
	// Disable ComponentTick if Recoil Animation is playing or Player Controller Recoil is Inactive
	if(IsComponentTickEnabled() && !RecoilAnimationTimeline.IsPlaying())
	{
		bool bRecoilAnimationNotPlaying = InterpolatedRotation.Equals(OutRotation, 0.001) && InterpolatedTranslation.Equals(OutTranslation, 0.001);
		
		if(bRecoilAnimationNotPlaying && Recoil.IsNearlyZero())
		{
			SetComponentTickEnabled(false);
		}
	}
}

void UAlsxtProceduralRecoilAnimComponent::RefreshProceduralRecoilSolver(float DeltaTime)
{
	if(!(CurrentRotationCurveAsset && CurrentTranslationCurveAsset))
	{
		return;
	}

	RecoilAnimationTimeline.TickTimeline(DeltaTime);
	FTransform Finalized = FTransform(InterpolatedRotation, InterpolatedTranslation);
		
	RefreshNoise(Finalized, DeltaTime);
	RefreshPushback(Finalized, DeltaTime);
	RefreshProgression(Finalized, DeltaTime);
	
	// FVector PivotOffset = bIsAiming ? ProceduralRecoilSettings->AdsPivotOffset : ProceduralRecoilSettings->PivotOffset;
	FVector PivotOffset = IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming ? ProceduralRecoilSettings->AdsPivotOffset : ProceduralRecoilSettings->PivotOffset;
	PivotOffset = Finalized.Rotator().RotateVector(PivotOffset) - PivotOffset;
	Finalized.AddToTranslation(PivotOffset);

	RefreshRecoilSway(Finalized, DeltaTime);

	OutTranslation = Finalized.GetLocation();
	OutRotation = Finalized.Rotator();

	// Apply Global Magnitudes for Final Calculation
	OutTranslation *= GlobalTranslationMultiplier;
	OutRotation.Pitch *= GlobalRotationMultiplier.Y;
	OutRotation.Yaw *= GlobalRotationMultiplier.Z;
	OutRotation.Roll *= GlobalRotationMultiplier.X;
}

void UAlsxtProceduralRecoilAnimComponent::RefreshPlayerController(float DeltaTime)
{
	if(!IsValid(OwnerCharacter))
	{
		return;
	}
	
	if(bIsDischarging)
	{
		FVector2D PlayerInput{FVector2D::ZeroVector};
		if(const auto PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
		{
			PlayerController->GetInputMouseDelta(PlayerInput.X, PlayerInput.Y);
		}
		
		Compensation += PlayerInput;
	}

	FVector2D CachedRecoil{Recoil};
	
	Recoil.X = FMath::FInterpTo(Recoil.X, TargetRecoil.X, DeltaTime,
		ProceduralRecoilSettings->ControllerRecoil.RecoilSmoothing.X);

	Recoil.Y = FMath::FInterpTo(Recoil.Y, TargetRecoil.Y, DeltaTime,
		ProceduralRecoilSettings->ControllerRecoil.RecoilSmoothing.Y);

	if(!bIsDischarging)
	{
		TargetRecoil = FMath::Vector2DInterpTo(TargetRecoil, FVector2D::ZeroVector, DeltaTime,
			ProceduralRecoilSettings->ControllerRecoil.Damping);
	}

	OwnerCharacter->AddControllerPitchInput(Recoil.Y - CachedRecoil.Y);
	OwnerCharacter->AddControllerYawInput(Recoil.X - CachedRecoil.X);
}

void UAlsxtProceduralRecoilAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UAlsxtProceduralRecoilAnimComponent, OutRotation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UAlsxtProceduralRecoilAnimComponent, OutTranslation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UAlsxtProceduralRecoilAnimComponent, PlayerControllerMagnitudes, COND_SkipOwner);
}

void UAlsxtProceduralRecoilAnimComponent::RefreshSmoothing(float DeltaTime)
{
	if(bRepeatingFireSmoothingEnabled)
	{
		auto ApplyInterp = [&](float Raw, float Out, float Smooth) -> float
		{
			if(FMath::IsNearlyZero(Smooth, 0.01f))
			{
				return Raw;
			}

			Raw = RecoilAnimationTimeline.IsPlaying() ? Raw : 0.f;
			return Lerp_FrameRateIndependent(Out, Raw, Smooth * 2.f, DeltaTime);
		};

		InterpolatedRotation.Pitch = ApplyInterp(RawRotation.Pitch, InterpolatedRotation.Pitch, ProceduralRecoilSettings->AutoFireModeRotationSmoothingMagnitudes.Pitch);
		InterpolatedRotation.Yaw = ApplyInterp(RawRotation.Yaw, InterpolatedRotation.Yaw, ProceduralRecoilSettings->AutoFireModeRotationSmoothingMagnitudes.Yaw);
		InterpolatedRotation.Roll = ApplyInterp(RawRotation.Roll, InterpolatedRotation.Roll, ProceduralRecoilSettings->AutoFireModeRotationSmoothingMagnitudes.Roll);

		InterpolatedTranslation.X = ApplyInterp(RawTranslation.X, InterpolatedTranslation.X, ProceduralRecoilSettings->AutoFireModeLocationSmoothingMagnitudes.X);
		InterpolatedTranslation.Y = ApplyInterp(RawTranslation.Y, InterpolatedTranslation.Y, ProceduralRecoilSettings->AutoFireModeLocationSmoothingMagnitudes.Y);
		InterpolatedTranslation.Z = ApplyInterp(RawTranslation.Z, InterpolatedTranslation.Z, ProceduralRecoilSettings->AutoFireModeLocationSmoothingMagnitudes.Z);
	}
	else
	{
		InterpolatedRotation = RawRotation;
		InterpolatedTranslation = RawTranslation;
	}
}

void UAlsxtProceduralRecoilAnimComponent::RefreshNoise(FTransform& Finalized, float DeltaTime)
{
	Noise2DTarget.X = Lerp_FrameRateIndependent(Noise2DTarget.X, 0.f, ProceduralRecoilSettings->Noise2DZero.X, DeltaTime);
	Noise2DTarget.Y = Lerp_FrameRateIndependent(Noise2DTarget.Y, 0.f, ProceduralRecoilSettings->Noise2DZero.Y, DeltaTime);
	
	Noise2DCurrent.X = Lerp_FrameRateIndependent(Noise2DCurrent.X, Noise2DTarget.X, NoiseSpeed.X, DeltaTime);
	Noise2DCurrent.Y = Lerp_FrameRateIndependent(Noise2DCurrent.Y, Noise2DTarget.Y, NoiseSpeed.Y, DeltaTime);

	const float NoiseScalar = IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming ? ProceduralRecoilSettings->AimingNoiseMagnitude : 1.f;

	Noise2DCurrent.X = Noise2DCurrent.X * NoiseScalar;
	Noise2DCurrent.Y = Noise2DCurrent.Y * NoiseScalar;
	
	const FVector Temp = Finalized.GetLocation() + FVector(0.0, Noise2DCurrent.X, Noise2DCurrent.Y);
	
	Finalized.SetLocation(Temp);
}

void UAlsxtProceduralRecoilAnimComponent::RefreshPushback(FTransform& Finalized, float DeltaTime)
{
	PushbackTarget = Lerp_FrameRateIndependent(PushbackTarget, 0.0, ProceduralRecoilSettings->PushbackZero, DeltaTime);
	PushbackCurrent = Lerp_FrameRateIndependent(PushbackCurrent, PushbackTarget, PushbackSpeed, DeltaTime);
	
	const FVector Temp = Finalized.GetLocation() + FVector(PushbackCurrent, 0.0, 0.0);
	Finalized.SetLocation(Temp);
}

void UAlsxtProceduralRecoilAnimComponent::RefreshProgression(FTransform& Finalized, float DeltaTime)
{
	RotationPitchProgression.X = Lerp_FrameRateIndependent(RotationPitchProgression.X, RotationPitchProgression.Y, RotationPitchProgressionSpeed, DeltaTime);
	RotationPitchProgression.Y = Lerp_FrameRateIndependent(RotationPitchProgression.Y, 0.f, ProceduralRecoilSettings->PitchProgress.Damping, DeltaTime);
	
	TranslationZProgression.X = Lerp_FrameRateIndependent(TranslationZProgression.X, TranslationZProgression.Y, TranslationZProgressionSpeed, DeltaTime);
	TranslationZProgression.Y = Lerp_FrameRateIndependent(TranslationZProgression.Y, 0.f, ProceduralRecoilSettings->ZProgress.Damping, DeltaTime);

	const FVector Loc = Finalized.GetLocation() + FVector(0.0, 0.0, TranslationZProgression.X);
	const FQuat Rot = Finalized.GetRotation() * FRotator(RotationPitchProgression.X, 0.0, 0.0).Quaternion();

	Finalized.SetLocation(Loc);
	Finalized.SetRotation(Rot);
}

void UAlsxtProceduralRecoilAnimComponent::RefreshRecoilSway(FTransform& Finalized, float DeltaTime)
{
	PitchSway.X = Lerp_FrameRateIndependent(PitchSway.X, PitchSway.Y, ProceduralRecoilSettings->RecoilSway.Acceleration, DeltaTime);
	YawSway.X = Lerp_FrameRateIndependent(YawSway.X, YawSway.Y, ProceduralRecoilSettings->RecoilSway.Acceleration, DeltaTime);

	PitchSway.Y = Lerp_FrameRateIndependent(PitchSway.Y, 0.f, ProceduralRecoilSettings->RecoilSway.Damping, DeltaTime);
	YawSway.Y = Lerp_FrameRateIndependent(YawSway.Y, 0.f, ProceduralRecoilSettings->RecoilSway.Damping, DeltaTime);

	FRotator SwayRotation = FRotator(PitchSway.X, YawSway.X,
		YawSway.X * ProceduralRecoilSettings->RecoilSway.RollSwayMultiplier);
	FVector SwayPosition = SwayRotation.RotateVector(ProceduralRecoilSettings->RecoilSway.PivotOffset)
		- ProceduralRecoilSettings->RecoilSway.PivotOffset;

	Finalized.SetRotation(Finalized.GetRotation() * SwayRotation.Quaternion());
	Finalized.AddToTranslation(SwayPosition);
}

void UAlsxtProceduralRecoilAnimComponent::SetControllerRecoilTarget()
{
	if(!bIsDischarging)
	{
		Compensation = FVector2D::ZeroVector;
	}

	bIsDischarging = true;
	
	TargetRecoil.X += FMath::FRandRange(ProceduralRecoilSettings->ControllerRecoil.HorizontalRecoilStep.X,
		ProceduralRecoilSettings->ControllerRecoil.HorizontalRecoilStep.Y);
	TargetRecoil.Y -= FMath::FRandRange(ProceduralRecoilSettings->ControllerRecoil.VerticalRecoilStep.X,
		ProceduralRecoilSettings->ControllerRecoil.VerticalRecoilStep.Y);

	TargetRecoil.X = FMath::Clamp(TargetRecoil.X, -90.f, 90.f);
	TargetRecoil.Y = FMath::Clamp(TargetRecoil.Y, -90.f, 90.f);
}

void UAlsxtProceduralRecoilAnimComponent::RefreshControllerCompensation()
{
	bIsDischarging = false;

	float CompensationWeight = ProceduralRecoilSettings->ControllerRecoil.Compensation;

	float CompensationX = RefreshCompensation(Recoil.X, Compensation.X * CompensationWeight);
	float CompensationY = RefreshCompensation(Recoil.Y, -Compensation.Y * CompensationWeight);

	Recoil.X *= CompensationX;
	Recoil.Y *= CompensationY;
	
	TargetRecoil = Recoil;
}

float UAlsxtProceduralRecoilAnimComponent::RefreshCompensation(float RecoilValue, float RecoilCompensation) const
{
	float Multiplier = 1.f;
	bool IsOpposite = RecoilValue * RecoilCompensation <= 0.f;
	
	if(!FMath::IsNearlyZero(RecoilCompensation) && IsOpposite)
	{
		Multiplier -= FMath::Clamp(FMath::Abs(RecoilCompensation / RecoilValue), 0.f, 1.f);
	}
            
	return Multiplier;
}

void UAlsxtProceduralRecoilAnimComponent::SetupComponent()
{
	SetIsReplicated(true);
	TimelineProgressionFunction.BindUFunction(this, TEXT("RefreshRecoilAnimationTimeline"));

	FOnTimelineEvent Event;
	Event.BindUFunction(this, TEXT("CalculateTargetRecoilParameters"));
	RecoilAnimationTimeline.AddEvent(0.f, Event);

	PlayerControllerMagnitudes.Reset();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UAlsxtProceduralRecoilAnimComponent::AddEvent(float Time, FOnTimelineEvent Event)
{
	if(Event.IsBound())
	{
		Time = FMath::Max(0.f, Time);
		RecoilAnimationTimeline.AddEvent(Time, Event);
	}
	else
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("AddEvent: Event not bound"))
	}
}

float UAlsxtProceduralRecoilAnimComponent::SpringInterpolationReset(float Current, FFloatSpringState& SpringState, float Stiffness,
                                            float Damping, float DeltaTime)
{
	DeltaTime = FMath::Min(DeltaTime, 1.f);
	return UKismetMath::FloatSpringInterp(Current, 0.f, SpringState, Stiffness, Damping, DeltaTime);
}

void UAlsxtProceduralRecoilAnimComponent::SetProceduralRecoilSettings(UAlsxtProceduralRecoilSettingsDataAsset* NewProceduralRecoilSettings, const float Rate, const int Bursts)
{
	if (!IsValid(NewProceduralRecoilSettings))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("ProceduralRecoilSettingsDataAsset Not Found"));
		return;
	}
	if (!(ProceduralRecoilSettings->SingleFireLocationCurve))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("SingleFireLocationCurve Not Found"));
		return;
	}
	if (!(ProceduralRecoilSettings->SingleFireRotationCurve))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("SingleFireRotationCurve Not Found"));
		return;
	}
	
	ProceduralRecoilSettings = NewProceduralRecoilSettings;
	Compensation = TargetRecoil = Recoil = FVector2D::ZeroVector;

	CurrentBurstAmount = CurrentBurstCounter = Bursts;
	CurrentFireRate = FMath::Max(Rate, 0.001f);

	CurrentTargetRotation = FRotator::ZeroRotator;
	CurrentTargetTranslation = FVector::ZeroVector;
	Noise2DTarget = FVector2D::ZeroVector;
	RotationPitchProgression.Y = TranslationZProgression.Y = PushbackTarget = 0.0;

	NoiseSpeed.X = GetValidValue(NoiseSpeed.X, ProceduralRecoilSettings->Noise2DAccel.X);
	NoiseSpeed.Y = GetValidValue(NoiseSpeed.Y, ProceduralRecoilSettings->Noise2DAccel.Y);

	RotationPitchProgressionSpeed = GetValidValue(RotationPitchProgressionSpeed, ProceduralRecoilSettings->PitchProgress.Acceleration);
	TranslationZProgressionSpeed = GetValidValue(TranslationZProgressionSpeed, ProceduralRecoilSettings->ZProgress.Acceleration);
	PushbackSpeed =  GetValidValue(PushbackSpeed, ProceduralRecoilSettings->PushbackAccel);

	if (bTimelineInitialized)
	{
		RecoilAnimationTimeline.SetVectorCurve(ProceduralRecoilSettings->SingleFireRotationCurve, TEXT("Rotation"));
	}
	else
	{
		RecoilAnimationTimeline.AddInterpVector(ProceduralRecoilSettings->SingleFireRotationCurve, TimelineProgressionFunction, NAME_None, TEXT("Rotation"));
		bTimelineInitialized = true;
	}
}

void UAlsxtProceduralRecoilAnimComponent::PlayProceduralRecoilAnimation()
{
	if(!IsValid(ProceduralRecoilSettings))
	{
		return;
	}

	if(!IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
	}

	SetControllerRecoilTarget();
	
	RecoilAnimationTimeline.SetPlayRate(ProceduralRecoilSettings->PlayRate);
	bRecoilAnimationPlaying = true;
	
	for (uint16 i = 0; i < StateMachine.Num(); i++)
	{
		FAlsxtProceduralRecoilTransitionConditionDelegate Condition = StateMachine[i].TransitionConditionDelegate;
		if (Condition.Execute())
		{
			CurrentStateMachineIndex = i;
			break;
		}
	}

	if (!StateMachine[CurrentStateMachineIndex].OnPlayDelegate.ExecuteIfBound())
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("OnPlayDelegate for State Machine %2.i not bound"), CurrentStateMachineIndex);
	}

	if (CurrentTranslationCurveAsset && CurrentRotationCurveAsset)
	{
		SetSimulationTimer(RecoilAnimationTimeline.GetTimelineLength());
	}
	
	TimeSincePreviousDischarge = UKismetSystemLibrary::GetGameTimeInSeconds(this);
}

void UAlsxtProceduralRecoilAnimComponent::StopProceduralRecoilAnimation()
{
	if(!IsValid(ProceduralRecoilSettings))
	{
		return;
	}
	
	RecoilAnimationTimeline.SetLooping(false);
	
	if (!StateMachine[CurrentStateMachineIndex].OnStopDelegate.ExecuteIfBound())
	{
		UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("OnStopDelegate for State Machine %2i not bound"), CurrentStateMachineIndex)
	}

	bRecoilAnimationPlaying = false;

	RefreshControllerCompensation();
}

void UAlsxtProceduralRecoilAnimComponent::SetPlayerControllerMagnitudes(FAlsxtProceduralRecoilPlayerControllerMagnitudes NewMagnitudes)
{
	PlayerControllerMagnitudes *= NewMagnitudes;
}

void UAlsxtProceduralRecoilAnimComponent::RemovePlayerControllerMagnitudes(FAlsxtProceduralRecoilPlayerControllerMagnitudes MagnitudesToRemove)
{
	PlayerControllerMagnitudes /= MagnitudesToRemove;
}

void UAlsxtProceduralRecoilAnimComponent::ResetPlayerControllerMagnitudes()
{
	PlayerControllerMagnitudes.Reset();
}

void UAlsxtProceduralRecoilAnimComponent::SetFiringMode(const EAlsxtFiringMode NewMode)
{
	FireMode = NewMode;
}

void UAlsxtProceduralRecoilAnimComponent::SetAiming(bool bAiming)
{
	bIsAiming = bAiming;
}

double UAlsxtProceduralRecoilAnimComponent::GetRandomFloatInRanges(const FVector4 Ranges)
{
	const bool RandomBool = UKismetMath::RandomBool();

	const double Max = FMath::FRandRange(Ranges.Z, Ranges.W);
	const double Min = FMath::FRandRange(Ranges.X, Ranges.Y);

	return RandomBool ? Max : Min;
}

void UAlsxtProceduralRecoilAnimComponent::ErrorCorrectTimerAlpha(const TSoftObjectPtr<UCurveVector> Rotation, const TSoftObjectPtr<UCurveVector> Location,
                                             const float TimeCorrection)
{
	/** Alpha (value of a semi curve) at the corrected time */
	const FVector RotationAlpha = Rotation.Get()->GetVectorValue(TimeCorrection);
	
	CurrentOriginRotation.Pitch = FMath::Lerp(CurrentOriginRotation.Pitch, CurrentTargetRotation.Pitch, RotationAlpha.Y);
	CurrentOriginRotation.Yaw = FMath::Lerp(CurrentOriginRotation.Yaw, CurrentTargetRotation.Yaw, RotationAlpha.Z);
	CurrentOriginRotation.Roll = FMath::Lerp(CurrentOriginRotation.Roll, CurrentTargetRotation.Roll, RotationAlpha.X);
	
	CurrentOriginTranslation = FMath::Lerp(CurrentOriginTranslation, CurrentTargetTranslation, Location.Get()->GetVectorValue(TimeCorrection));
}

void UAlsxtProceduralRecoilAnimComponent::SetupTransition(FRotator OriginRotation, FVector OriginTranslation, TSoftObjectPtr<UCurveVector> TargetRotation,
                                                TSoftObjectPtr<UCurveVector> TargetTranslation)
{
	if(!(TargetRotation))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("Invalid TargetRotation Curve Asset"))
		return;
	}
	if(!(TargetTranslation))
	{
		UE_LOG(LogAlsxtProceduralRecoil, Error, TEXT("Invalid TargetTranslation Curve Asset"))
		return;
	}
	
	CurrentOriginRotation = OriginRotation;
	CurrentOriginTranslation = OriginTranslation;
	
	RotationAxisRestCriteria = FAlsxtProceduralRecoilRestAxisCriteria(true, true, true);
	TranslationAxisRestCriteria = FAlsxtProceduralRecoilRestAxisCriteria(true, true, true);

	CurrentRotationCurveAsset = TargetRotation.Get();
	CurrentTranslationCurveAsset = TargetTranslation.Get();

	RecoilAnimationTimeline.SetVectorCurve(CurrentRotationCurveAsset.Get(), TEXT("Rotation"));
	RecoilAnimationTimeline.PlayFromStart();
}

float UAlsxtProceduralRecoilAnimComponent::GetDeltaSincePreviousDischarge() const
{
	return UKismetSystemLibrary::GetGameTimeInSeconds(this) - TimeSincePreviousDischarge;
}

void UAlsxtProceduralRecoilAnimComponent::RefreshRecoilAnimationTimeline(FVector Alpha)
{
	/** Current playback position */
	const float Playback = RecoilAnimationTimeline.GetPlaybackPosition();
	float PreviousPlayback = Playback - GetWorld()->GetDeltaSeconds() * RecoilAnimationTimeline.GetPlayRate();
	PreviousPlayback = FMath::Max(PreviousPlayback, 0.f);

	FVector NewAlpha = CurrentRotationCurveAsset->GetVectorValue(Playback);
	FVector PreviousAlpha = CurrentRotationCurveAsset->GetVectorValue(PreviousPlayback);

	FRotator ExtraRot = bRepeatingFireSmoothingEnabled ? ProceduralRecoilSettings->RotationAxesMagnitudes : FRotator(1.f,1.f, 1.f);
	FVector ExtraLoc = bRepeatingFireSmoothingEnabled ? ProceduralRecoilSettings->LocationAxesMagnitudes : FVector(1.f,1.f, 1.f);
	
	RawRotation.Roll = !IgnoreRotationAxes.X ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginRotation.Roll, PreviousAlpha.X, NewAlpha.X, RotationAxisRestCriteria.X),
	                          CurrentTargetRotation.Roll * ExtraRot.Roll, NewAlpha.X) : 0.f;

	RawRotation.Pitch = !IgnoreRotationAxes.Y ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginRotation.Pitch, PreviousAlpha.Y, NewAlpha.Y, RotationAxisRestCriteria.Y),
	                           CurrentTargetRotation.Pitch * ExtraRot.Pitch, NewAlpha.Y) : 0.f;

	RawRotation.Yaw = !IgnoreRotationAxes.Z ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginRotation.Yaw, PreviousAlpha.Z, NewAlpha.Z, RotationAxisRestCriteria.Z),
	                         CurrentTargetRotation.Yaw * ExtraRot.Yaw, NewAlpha.Z) : 0.f;

	NewAlpha = CurrentTranslationCurveAsset->GetVectorValue(Playback);
	PreviousAlpha = CurrentTranslationCurveAsset->GetVectorValue(PreviousPlayback);

	RawTranslation.X = !IgnoreTranslationAxes.X ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginTranslation.X, PreviousAlpha.X, NewAlpha.X, TranslationAxisRestCriteria.X),
	                       CurrentTargetTranslation.X * ExtraLoc.X, NewAlpha.X) : 0.f;

	RawTranslation.Y = !IgnoreTranslationAxes.Y ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginTranslation.Y, PreviousAlpha.Y, NewAlpha.Y, TranslationAxisRestCriteria.Y),
	                       CurrentTargetTranslation.Y * ExtraLoc.Y, NewAlpha.Y) : 0.f;

	RawTranslation.Z = !IgnoreTranslationAxes.Z ? FMath::Lerp(GetCorrectOriginValue(CurrentOriginTranslation.Z, PreviousAlpha.Z, NewAlpha.Z, TranslationAxisRestCriteria.Z),
	                       CurrentTargetTranslation.Z * ExtraLoc.Z, NewAlpha.Z) : 0.f;

	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	RefreshSmoothing(DeltaSeconds);
}

void UAlsxtProceduralRecoilAnimComponent::CalculateTargetRecoilParameters()
{
	// Random Rotation Lambda Function
	auto RandomRotation = [&](FVector4 Roll, FVector2D Pitch, FVector4 Yaw) -> void
	{
		Roll *= PlayerControllerMagnitudes.Roll;
		Pitch *= PlayerControllerMagnitudes.Pitch;
		Yaw *= PlayerControllerMagnitudes.Yaw;
		
		const float RandomFloatRoll = GetRandomFloatInRanges(Roll);

		// Check if CurrentTargetRotation.Roll multiplied by RandomFloatRoll Greater Than Zero (Same side)
		const bool bPositiveAxisRoll = CurrentTargetRotation.Roll * RandomFloatRoll > 0.f;

		CurrentTargetRotation.Roll = ProceduralRecoilSettings->bEnableRollSmoothing && bPositiveAxisRoll ? RandomFloatRoll * -1.f : RandomFloatRoll;
		CurrentTargetRotation.Pitch = FMath::FRandRange(Pitch.X, Pitch.Y);
		CurrentTargetRotation.Yaw = GetRandomFloatInRanges(Yaw);
	};

	// Random Translation Lambda Function
	auto RandomTranslation = [&](FVector2D Kick, FVector2D KickR, FVector2D KickUp) -> void
	{
		Kick *= PlayerControllerMagnitudes.Kickback;
		KickR *= PlayerControllerMagnitudes.KickbackRightward;
		KickUp *= PlayerControllerMagnitudes.KickbackUpward;
		
		CurrentTargetTranslation.X = FMath::FRandRange(Kick.X, Kick.Y);
		CurrentTargetTranslation.Y = FMath::FRandRange(KickR.X, KickR.Y);
		CurrentTargetTranslation.Z = FMath::FRandRange(KickUp.X, KickUp.Y);
	};

	// Random Noise Lambda Function
	auto RandomNoise = [this]() -> void
	{
		const auto Noise2DScaled = FVector2D(ProceduralRecoilSettings->Noise2DTarget.X * PlayerControllerMagnitudes.Noise.X,
			ProceduralRecoilSettings->Noise2DTarget.Y * PlayerControllerMagnitudes.Noise.Y);
		
		Noise2DTarget.X += FMath::FRandRange(-Noise2DScaled.X, Noise2DScaled.X);
		Noise2DTarget.Y += FMath::FRandRange(-Noise2DScaled.Y, Noise2DScaled.Y);
	};

	// Target Progress Lambda Function
	auto TargetProgress = [this]() -> void
	{
		const float AimingAlpha = IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming ? ProceduralRecoilSettings->ProgressAimAlpha : 1.f;

		// PlayerControllerMagnitudes.Progression: X: Pitch, Y,Z: Offset
		RotationPitchProgression.Y += ProceduralRecoilSettings->PitchProgress.Amount * PlayerControllerMagnitudes.Progression.X * AimingAlpha;
		TranslationZProgression.Y += ProceduralRecoilSettings->ZProgress.Amount * PlayerControllerMagnitudes.Progression.Y * AimingAlpha;
	};
	
	if (IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming)
	{
		RandomRotation(ProceduralRecoilSettings->AimingRollMagnitudes, ProceduralRecoilSettings->AimingPitchMagnitudes, ProceduralRecoilSettings->AimingYawMagnitudes);
		RandomTranslation(ProceduralRecoilSettings->AimingKickbackMagnitudeRange, ProceduralRecoilSettings->AimingKickbackRightwardMagnitudeRange, ProceduralRecoilSettings->AimingKickbackUpwardMagnitudeRange);
	}
	else
	{
		RandomRotation(ProceduralRecoilSettings->ReadyRollMagnitudes, ProceduralRecoilSettings->ReadyPitchMagnitudes, ProceduralRecoilSettings->ReadyYawMagnitudes);
		RandomTranslation(ProceduralRecoilSettings->ReadyKickbackMagnitudeRange, ProceduralRecoilSettings->ReadyKickbackRightwardMagnitudeRange, ProceduralRecoilSettings->ReadyKickbackUpwardMagnitudeRange);
	}

	float RandomFloatSway = FMath::FRandRange(ProceduralRecoilSettings->RecoilSway.PitchSwayRange.X, ProceduralRecoilSettings->RecoilSway.PitchSwayRange.Y);

	if(IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming)
	{
		RandomFloatSway *= ProceduralRecoilSettings->RecoilSway.AdsScale;
	}
	
	PitchSway.Y += RandomFloatSway;
	RandomFloatSway = FMath::FRandRange(ProceduralRecoilSettings->RecoilSway.YawSwayRange.X, ProceduralRecoilSettings->RecoilSway.YawSwayRange.Y);

	if(IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetOwner()) == AlsxtReadinessTags::Aiming)
	{
		RandomFloatSway *= ProceduralRecoilSettings->RecoilSway.AdsScale;
	}
	
	YawSway.Y += RandomFloatSway;

	RandomNoise();
	TargetProgress();
}

double UAlsxtProceduralRecoilAnimComponent::GetCorrectOriginValue(double& Origin, double& Target, const double Current, bool& bStartRest)
{
	if (FMath::Abs(Target) > FMath::Abs(Current) && bStartRest && !RecoilAnimationTimeline.IsLooping())
	{
		Origin = 0.0;
		bStartRest = false;
	}
	
	Target = Current;
	return Origin;
}

FVector UAlsxtProceduralRecoilAnimComponent::Lerp_FrameRateIndependent(FVector A, FVector B, FVector Alpha, float DeltaTime)
{
	FVector OutVector {Lerp_FrameRateIndependent(A.X, B.X, Alpha.X, DeltaTime), Lerp_FrameRateIndependent(A.Y, B.Y, Alpha.Y, DeltaTime), Lerp_FrameRateIndependent(A.Z, B.Z, Alpha.Z, DeltaTime)};
	return OutVector;
}

FRotator UAlsxtProceduralRecoilAnimComponent::Lerp_FrameRateIndependent(FRotator A, FRotator B, FVector Alpha, float DeltaTime)
{
	FRotator OutRotator {Lerp_FrameRateIndependent(A.Roll, B.Roll, Alpha.X, DeltaTime), Lerp_FrameRateIndependent(A.Pitch, B.Pitch, Alpha.Y, DeltaTime), Lerp_FrameRateIndependent(A.Yaw, B.Yaw, Alpha.Z, DeltaTime)};
	return OutRotator;
}

void UAlsxtProceduralRecoilAnimComponent::InitializeStateMachine()
{
	FAlsxtProceduralRecoilAnimationState SingleFireModeAnimationState;
	FAlsxtProceduralRecoilAnimationState RepeatFireModeAnimationState;

	SingleFireModeAnimationState.TransitionConditionDelegate.BindLambda([this]() -> bool
	{
		// Normalize the firing rate to the current frame's timing based on the current fire rate and the delta time.
		float AdjustedFireTiming = (60.f / CurrentFireRate) / GetWorld()->GetDeltaSeconds() + 1;
		// Calculate AdjustedFireTiming based on fire rate and delta time
		AdjustedFireTiming *= GetWorld()->GetDeltaSeconds();

		// Ensure bRepeatingFireSmoothingEnabled is not enabled until successive discharges
		// Disable smoothing for the first discharge to ensure immediate firing
		if(bRepeatingFireSmoothingEnabled)
		{
			bRepeatingFireSmoothingEnabled = false;
		}

		// Check if the firing mode is semi-automatic or if enough time has passed since the last discharge
		return FireMode == Semi || (GetDeltaSincePreviousDischarge() > AdjustedFireTiming + 0.01f && !RecoilAnimationTimeline.IsLooping());
	});

	SingleFireModeAnimationState.OnPlayDelegate.BindLambda([this]() -> void
	{
		SetupTransition(InterpolatedRotation, InterpolatedTranslation, ProceduralRecoilSettings->SingleFireRotationCurve, ProceduralRecoilSettings->SingleFireLocationCurve);
	});

	SingleFireModeAnimationState.OnStopDelegate.BindLambda([this]() -> void
	{
		GetWorld()->GetTimerManager().ClearTimer(ProceduralRecoilAnimationTimerHandle);
	});

	RepeatFireModeAnimationState.TransitionConditionDelegate.BindLambda([this]()-> bool
	{
		return true;
	});

	RepeatFireModeAnimationState.OnPlayDelegate.BindLambda([this]() -> void
	{
		bRepeatingFireSmoothingEnabled = true;

		if(RecoilAnimationTimeline.IsLooping())
		{
			return;
		}
		
		const float Correction = 60.f / CurrentFireRate + ProceduralRecoilSettings->PlaybackOffset;
		RecoilAnimationTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		RecoilAnimationTimeline.SetTimelineLength(60.f / CurrentFireRate);

		if (ProceduralRecoilSettings->AutoFireRotationCurve && ProceduralRecoilSettings->AutoFireLocationCurve)
		{
			ErrorCorrectTimerAlpha(ProceduralRecoilSettings->AutoFireRotationCurve, ProceduralRecoilSettings->AutoFireLocationCurve, Correction);
			SetupTransition(CurrentOriginRotation, CurrentOriginTranslation, ProceduralRecoilSettings->AutoFireRotationCurve, ProceduralRecoilSettings->AutoFireLocationCurve);
		}
		else
		{
			ErrorCorrectTimerAlpha(ProceduralRecoilSettings->SingleFireRotationCurve, ProceduralRecoilSettings->SingleFireLocationCurve, Correction);
			SetupTransition(CurrentOriginRotation, CurrentOriginTranslation, ProceduralRecoilSettings->SingleFireRotationCurve, ProceduralRecoilSettings->SingleFireLocationCurve);
		}

		RecoilAnimationTimeline.SetLooping(true);

		PushbackTarget = ProceduralRecoilSettings->Pushback;
	});

	RepeatFireModeAnimationState.OnStopDelegate.BindLambda([this]() -> void
	{
		if(!bRecoilAnimationPlaying)
		{
			// Invalid Transition State: Recoil is not Playing. OnStop event has already been called
			return;
		}
		
		GetWorld()->GetTimerManager().ClearTimer(ProceduralRecoilAnimationTimerHandle);
		
		if(bRepeatingFireSmoothingEnabled)
		{
			RecoilAnimationTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		}
	});
	
	//Add Transition States to the state machine
	StateMachine.Add(SingleFireModeAnimationState);
	StateMachine.Add(RepeatFireModeAnimationState);
}

void UAlsxtProceduralRecoilAnimComponent::SetSimulationTimer(float SemiAutoFireModeDelay)
{
	if (!bEnableSimulation)
	{
		return; // Exit if simulation is not enabled
	}

	// Calculate the delay based on the firing mode
	const float Delay = FireMode == Semi || (FireMode == Burst && CurrentBurstCounter == 1) ? SemiAutoFireModeDelay : 60.f / CurrentFireRate;

	// Update the burst counter for burst fire mode
	CurrentBurstCounter = CurrentBurstCounter > 1 && FireMode == Burst ? --CurrentBurstCounter : CurrentBurstAmount;

	// Set a timer to play the procedural recoil animation after the calculated delay
	GetWorld()->GetTimerManager().SetTimer(ProceduralRecoilAnimationTimerHandle, this, &UAlsxtProceduralRecoilAnimComponent::PlayProceduralRecoilAnimation, Delay);
}