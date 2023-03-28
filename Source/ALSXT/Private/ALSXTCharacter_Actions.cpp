#include "AlsCharacter.h"

#include "AlsAnimationInstance.h"
#include "AlsCharacterMovementComponent.h"
#include "ALSXTCharacter.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Net/Core/PushModel/PushModel.h"
#include "RootMotionSources/AlsRootMotionSource_Mantling.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsMath.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Utility/AlsUtility.h"

void AALSXTCharacter::TryStartSliding(const float PlayRate)
{
	if (GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
	{
		StartSliding(PlayRate, ALSXTSettings->Sliding.bRotateToInputOnStart && GetLocomotionState().bHasInput
			? GetLocomotionState().InputYawAngle
			: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetActorRotation().Yaw)));
	}
}

bool AALSXTCharacter::IsSlidingAllowedToStart(const UAnimMontage* Montage) const
{
	return !GetLocomotionAction().IsValid() ||
		// ReSharper disable once CppRedundantParentheses
		(GetLocomotionAction() == AlsLocomotionActionTags::Sliding &&
			!GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage));
}

void AALSXTCharacter::StartSliding(const float PlayRate, const float TargetYawAngle)
{
	if (GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	auto* Montage{ SelectSlideMontage() };

	if (!ALS_ENSURE(IsValid(Montage)) || !IsSlidingAllowedToStart(Montage))
	{
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetActorRotation().Yaw)) };

	if (GetLocalRole() >= ROLE_Authority)
	{
		MulticastStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
	}
	else
	{
		GetCharacterMovement()->FlushServerMoves();

		StartSlidingImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		ServerStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		OnSlidingStarted();
	}
}

UAnimMontage* AALSXTCharacter::SelectSlideMontage_Implementation()
{
	return ALSXTSettings->Sliding.Montage;
}

void AALSXTCharacter::ServerStartSliding_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsSlidingAllowedToStart(Montage))
	{
		MulticastStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		ForceNetUpdate();
	}
}

void AALSXTCharacter::MulticastStartSliding_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	StartSlidingImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
}

void AALSXTCharacter::StartSlidingImplementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsSlidingAllowedToStart(Montage) && GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate))
	{
		SlidingState.TargetYawAngle = TargetYawAngle;

		RefreshRotationInstant(StartYawAngle);

		SetLocomotionAction(AlsLocomotionActionTags::Sliding);
		// Crouch(); //Hack
	}
}

void AALSXTCharacter::RefreshSliding(const float DeltaTime)
{
	if (GetLocalRole() <= ROLE_SimulatedProxy ||
		GetMesh()->GetAnimInstance()->RootMotionMode <= ERootMotionMode::IgnoreRootMotion)
	{
		// Refresh Sliding physics here because AALSXTCharacter::PhysicsRotation()
		// won't be called on simulated proxies or with ignored root motion.

		RefreshSlidingPhysics(DeltaTime);
	}
}

void AALSXTCharacter::RefreshSlidingPhysics(const float DeltaTime)
{
	if (GetLocomotionAction() != AlsLocomotionActionTags::Sliding)
	{
		return;
	}

	auto TargetRotation{ GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };

	if (ALSXTSettings->Sliding.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = SlidingState.TargetYawAngle;

		GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			SlidingState.TargetYawAngle, DeltaTime,
			ALSXTSettings->Sliding.RotationInterpolationSpeed);

		GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

bool AALSXTCharacter::TryStartVaultingGrounded()
{
	return LocomotionMode == AlsLocomotionModeTags::Grounded &&
	       TryStartVaulting(ALSXTSettings->Vaulting.GroundedTrace);
}

bool AALSXTCharacter::TryStartVaultingInAir()
{
	return LocomotionMode == AlsLocomotionModeTags::InAir && IsLocallyControlled() &&
	       TryStartVaulting(ALSXTSettings->Vaulting.InAirTrace);
}

bool AALSXTCharacter::IsVaultingAllowedToStart_Implementation(FVaultAnimation VaultAnimation) const
{
	// return !LocomotionAction.IsValid();

	return !GetLocomotionAction().IsValid() ||
		// ReSharper disable once CppRedundantParentheses
		(GetLocomotionAction() == AlsLocomotionActionTags::Vaulting &&
			!GetMesh()->GetAnimInstance()->Montage_IsPlaying(VaultAnimation.Montage.Montage));
}

bool AALSXTCharacter::TryStartVaulting(const FALSXTVaultingTraceSettings& TraceSettings)
{
	const auto ActorLocation{GetActorLocation()};
	const auto ActorYawAngle{UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetActorRotation().Yaw))};

	float ForwardTraceAngle;
	if (LocomotionState.bHasSpeed)
	{
		ForwardTraceAngle = LocomotionState.bHasInput
			                    ? LocomotionState.VelocityYawAngle +
			                      FMath::ClampAngle(LocomotionState.InputYawAngle - LocomotionState.VelocityYawAngle,
			                                        -ALSXTSettings->Vaulting.MaxReachAngle, ALSXTSettings->Vaulting.MaxReachAngle)
			                    : LocomotionState.VelocityYawAngle;
	}
	else
	{
		ForwardTraceAngle = LocomotionState.bHasInput ? LocomotionState.InputYawAngle : ActorYawAngle;
	}

	const auto ForwardTraceDeltaAngle{FRotator3f::NormalizeAxis(ForwardTraceAngle - ActorYawAngle)};
	if (FMath::Abs(ForwardTraceDeltaAngle) > ALSXTSettings->Vaulting.TraceAngleThreshold)
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : ALSXTSettings->Vaulting.VaultingTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}

	const auto ForwardTraceDirection{
		UAlsMath::AngleToDirectionXY(
			ActorYawAngle + FMath::ClampAngle(ForwardTraceDeltaAngle, -ALSXTSettings->Vaulting.MaxReachAngle, ALSXTSettings->Vaulting.MaxReachAngle))
	};

#if ENABLE_DRAW_DEBUG
	 const auto bDisplayDebug{UAlsUtility::ShouldDisplayDebugForActor(this, UAlsConstants::MantlingDisplayName())};
#endif

	const auto* Capsule{GetCapsuleComponent()};

	const auto CapsuleScale{Capsule->GetComponentScale().Z};
	const auto CapsuleRadius{Capsule->GetScaledCapsuleRadius()};
	const auto CapsuleHalfHeight{Capsule->GetScaledCapsuleHalfHeight()};

	const FVector CapsuleBottomLocation{ActorLocation.X, ActorLocation.Y, ActorLocation.Z - CapsuleHalfHeight};

	const auto TraceCapsuleRadius{CapsuleRadius - 1.0f};

	const auto LedgeHeightDelta{UE_REAL_TO_FLOAT((TraceSettings.LedgeHeight.GetMax() - TraceSettings.LedgeHeight.GetMin()) * CapsuleScale)};

	// Trace forward to find an object the character cannot walk on.

	static const FName ForwardTraceTag{__FUNCTION__ TEXT(" (Forward Trace)")};

	auto ForwardTraceStart{CapsuleBottomLocation - ForwardTraceDirection * CapsuleRadius};
	ForwardTraceStart.Z += (TraceSettings.LedgeHeight.X + TraceSettings.LedgeHeight.Y) *
		0.5f * CapsuleScale - UCharacterMovementComponent::MAX_FLOOR_DIST;

	auto ForwardTraceEnd{ForwardTraceStart + ForwardTraceDirection * (CapsuleRadius + (TraceSettings.ReachDistance + 1.0f) * CapsuleScale)};

	const auto ForwardTraceCapsuleHalfHeight{LedgeHeightDelta * 0.5f};

	FHitResult ForwardTraceHit;
	GetWorld()->SweepSingleByObjectType(ForwardTraceHit, ForwardTraceStart, ForwardTraceEnd, FQuat::Identity, ObjectQueryParameters,
	                                    FCollisionShape::MakeCapsule(TraceCapsuleRadius, ForwardTraceCapsuleHalfHeight),
	                                    {ForwardTraceTag, false, this});
	
	auto* TargetPrimitive{ForwardTraceHit.GetComponent()};

	if (!ForwardTraceHit.IsValidBlockingHit() ||
	    !IsValid(TargetPrimitive) ||
	    TargetPrimitive->GetComponentVelocity().SizeSquared() > FMath::Square(ALSXTSettings->Vaulting.TargetPrimitiveSpeedThreshold) ||
	    !TargetPrimitive->CanCharacterStepUp(this) ||
	    GetCharacterMovement()->IsWalkable(ForwardTraceHit))
	{
#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), ForwardTraceStart, ForwardTraceEnd, TraceCapsuleRadius,
			                                                    ForwardTraceCapsuleHalfHeight, false, ForwardTraceHit, {0.0f, 0.25f, 1.0f},
			                                                    {0.0f, 0.75f, 1.0f}, TraceSettings.bDrawFailedTraces ? 5.0f : 0.0f);
		}
#endif

		return false;
	}

	// Set Local Variables
	FHitResult DepthTraceHit;
	static const FName DepthTraceTag{ __FUNCTION__ TEXT(" (Depth Trace)") };
	FVector HitLocation = ForwardTraceHit.ImpactPoint;
	FVector HitNormal = ForwardTraceHit.ImpactNormal;

	// Depth Trace Start Location.
	FVector DepthStartLocation = HitLocation + (ForwardTraceHit.ImpactNormal * (TraceSettings.MaxDepth * -1));
	FVector DepthEndLocation = ForwardTraceHit.ImpactPoint + (ForwardTraceHit.ImpactNormal * (1 * -1));

	// Depth Trace. Check if less than max depth.
	GetWorld()->SweepSingleByObjectType(DepthTraceHit, DepthStartLocation, DepthEndLocation, FQuat::Identity, ObjectQueryParameters,
		FCollisionShape::MakeCapsule(TraceCapsuleRadius, ForwardTraceCapsuleHalfHeight),
		{ DepthTraceTag, false, this });


	// Check if object is thicker than MaxDepth
	if(!DepthTraceHit.IsValidBlockingHit())
	{

#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), DepthStartLocation, DepthEndLocation, TraceCapsuleRadius,
				ForwardTraceCapsuleHalfHeight, false, DepthTraceHit, FLinearColor::Yellow,
				{ 0.0f, 0.75f, 1.0f }, TraceSettings.bDrawFailedTraces ? 5.0f : 5.0f);
		}
#endif

		return false;
	}

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), DepthStartLocation, DepthEndLocation, TraceCapsuleRadius,
			ForwardTraceCapsuleHalfHeight, false, DepthTraceHit, FLinearColor::Yellow,
			{ 0.0f, 0.75f, 1.0f }, TraceSettings.bDrawFailedTraces ? 5.0f : 5.0f);
	}
#endif

	auto* DepthPrimitive{ DepthTraceHit.GetComponent() };

	// Trace downward from the first trace's impact point and determine if the hit location is walkable.

	static const FName DownwardTraceTag{__FUNCTION__ TEXT(" (Downward Trace)")};

	const auto TargetLocationOffset{
		FVector2D{ForwardTraceHit.ImpactNormal.GetSafeNormal2D()} * (TraceSettings.TargetLocationOffset * CapsuleScale)
	};

	const FVector DownwardTraceStart{
		ForwardTraceHit.ImpactPoint.X - TargetLocationOffset.X,
		ForwardTraceHit.ImpactPoint.Y - TargetLocationOffset.Y,
		CapsuleBottomLocation.Z + LedgeHeightDelta + 2.5f * TraceCapsuleRadius + UCharacterMovementComponent::MIN_FLOOR_DIST
	};

	const FVector DownwardTraceEnd{
		DownwardTraceStart.X,
		DownwardTraceStart.Y,
		CapsuleBottomLocation.Z +
		TraceSettings.LedgeHeight.GetMin() * CapsuleScale + TraceCapsuleRadius - UCharacterMovementComponent::MAX_FLOOR_DIST
	};

	FHitResult DownwardTraceHit;
	GetWorld()->SweepSingleByObjectType(DownwardTraceHit, DownwardTraceStart, DownwardTraceEnd, FQuat::Identity,
	                                    ObjectQueryParameters, FCollisionShape::MakeSphere(TraceCapsuleRadius),
	                                    {DownwardTraceTag, false, this});

	if (!GetCharacterMovement()->IsWalkable(DownwardTraceHit))
	{
#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), ForwardTraceStart, ForwardTraceEnd, TraceCapsuleRadius,
			                                                    ForwardTraceCapsuleHalfHeight, true, ForwardTraceHit, {0.0f, 0.25f, 1.0f},
			                                                    {0.0f, 0.75f, 1.0f}, TraceSettings.bDrawFailedTraces ? 5.0f : 0.0f);

			UAlsUtility::DrawDebugSweepSingleSphere(GetWorld(), DownwardTraceStart, DownwardTraceEnd, TraceCapsuleRadius,
			                                        false, DownwardTraceHit, {0.25f, 0.0f, 1.0f}, {0.75f, 0.0f, 1.0f},
			                                        TraceSettings.bDrawFailedTraces ? 7.5f : 0.0f);
		}
#endif

		return false;
	}

	// Check if the capsule has room to stand at the downward trace's location. If so,
	// set that location as the target transform and calculate the Vaulting height.

	static const FName FreeSpaceTraceTag{__FUNCTION__ TEXT(" (Free Space Overlap)")};

	const FVector TargetLocation{
		DownwardTraceHit.ImpactPoint.X,
		DownwardTraceHit.ImpactPoint.Y,
		DownwardTraceHit.ImpactPoint.Z + UCharacterMovementComponent::MIN_FLOOR_DIST
	};

	const FVector TargetCapsuleLocation{TargetLocation.X, TargetLocation.Y, TargetLocation.Z + CapsuleHalfHeight};

	if (GetWorld()->OverlapAnyTestByObjectType(TargetCapsuleLocation, FQuat::Identity, ObjectQueryParameters,
	                                           FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
	                                           {FreeSpaceTraceTag, false, this}))
	{
#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), ForwardTraceStart, ForwardTraceEnd, TraceCapsuleRadius,
			                                                    ForwardTraceCapsuleHalfHeight, true, ForwardTraceHit, {0.0f, 0.25f, 1.0f},
			                                                    {0.0f, 0.75f, 1.0f}, TraceSettings.bDrawFailedTraces ? 5.0f : 0.0f);

			UAlsUtility::DrawDebugSweepSingleSphere(GetWorld(), DownwardTraceStart, DownwardTraceEnd, TraceCapsuleRadius,
			                                        false, DownwardTraceHit, {0.25f, 0.0f, 1.0f}, {0.75f, 0.0f, 1.0f},
			                                        TraceSettings.bDrawFailedTraces ? 7.5f : 0.0f);

			DrawDebugCapsule(GetWorld(), TargetCapsuleLocation, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity,
			                 FColor::Red, false, TraceSettings.bDrawFailedTraces ? 10.0f : 0.0f);
		}
#endif

		return false;
	}

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), ForwardTraceStart, ForwardTraceEnd, TraceCapsuleRadius,
		                                                    ForwardTraceCapsuleHalfHeight, true, ForwardTraceHit,
		                                                    {0.0f, 0.25f, 1.0f}, {0.0f, 0.75f, 1.0f}, 5.0f);

		UAlsUtility::DrawDebugSweepSingleSphere(GetWorld(), DownwardTraceStart, DownwardTraceEnd,
		                                        TraceCapsuleRadius, true, DownwardTraceHit,
		                                        {0.25f, 0.0f, 1.0f}, {0.75f, 0.0f, 1.0f}, 7.5f);
	}
#endif
	
	//
	// Vaulting Room Trace

	// Vaulting Room Trace Hit Result
	FHitResult LandingTraceHit;
	static const FName LandingTraceTag{ __FUNCTION__ TEXT(" (Landing Trace)") };

	// Set Local Variables
	//const FVector LandingStartLocation{	TargetCapsuleLocation + (DepthTraceHit.Normal * 60) + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1)) };
	const FVector LandingStartLocation{ TargetCapsuleLocation + (DepthTraceHit.Normal * 60) };
	const FVector LandingEndLocation{ LandingStartLocation.X, LandingStartLocation.Y, GetActorLocation().Z };
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	// EDrawDebugTrace VaultRoomDebugType;
	// TEnumAsByte<EDrawDebugTrace::None>* DebugTrace;

	// Trace for room for Vaulting action
	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), LandingStartLocation, LandingEndLocation, CapsuleRadius, CapsuleHalfHeight, ALSXTSettings->Vaulting.VaultingTraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		FString LandingHit = HitResults[0].GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, LandingHit);
		return false;
	}

	//
	// Landing Location Trace

	const FVector LandingLocation = LandingEndLocation - (DownwardTraceHit.Normal * 10);
	// const FVector LandingLocation = LandingEndLocation;
	// Landing Location Trace Hit Result
	FHitResult LandingLocationTraceHit;
	TArray <FHitResult> LandingLocationTraceHits;
	TArray <FHitResult> LandingPoints;
	FHitResult LandingPoint;
	FVector LandingPointLocation;
	static const FName LandingLocationTraceTag{ __FUNCTION__ TEXT(" (Landing Location Trace)") };

	// Trace for Landing Location

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), LandingEndLocation, LandingLocation, CapsuleRadius, CapsuleHalfHeight, ALSXTSettings->Vaulting.VaultingTraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, LandingLocationTraceHits, true, FLinearColor::Green, FLinearColor::Red, 5.0f);

	for (auto LandingLocHit : LandingLocationTraceHits)
	{
		if (!GetCharacterMovement()->IsWalkable(LandingLocHit))
		{
			return false;
		}
		else
		{
			LandingPoints.Add(LandingLocHit);
		}
	}

	for (auto LandingPt : LandingPoints)
	{
		if (LandingPt.ImpactPoint.Z > LandingPoint.ImpactPoint.Z)
		{
			LandingPoint = LandingPt;
		}
	}

	// LandingPointLocation = LandingLocationTraceHits[0].ImpactPoint + (LandingLocationTraceHits[0].Normal * 1);
	LandingPointLocation = LandingPoint.ImpactPoint - (LandingPoint.Normal * CapsuleHalfHeight);
	LandingPointLocation = LandingPointLocation + (LandingPoint.Normal * 1);
	// LandingPointLocation = LandingLocation;

	const auto TargetRotation{(-ForwardTraceHit.ImpactNormal.GetSafeNormal2D()).ToOrientationQuat()};

	FALSXTVaultingParameters Parameters;

	Parameters.TargetPrimitive = TargetPrimitive;
	Parameters.VaultingHeight = UE_REAL_TO_FLOAT((TargetLocation.Z - CapsuleBottomLocation.Z) / CapsuleScale);
	// Parameters.VaultingHeight = UE_REAL_TO_FLOAT(TargetLocation.Z);

	// Determine the Vaulting type by checking the movement mode and Vaulting height.
	// TODO Expand this

	Parameters.LocomotionMode = GetLocomotionMode();
	Parameters.Gait = GetGait();
	Parameters.VaultingType = LocomotionMode != AlsLocomotionModeTags::Grounded
		                          ? ALSXTVaultTypeTags::InAir
		                          : Parameters.VaultingHeight > ALSXTSettings->Vaulting.VaultingHighHeightThreshold
		                          ? ALSXTVaultTypeTags::High
		                          : ALSXTVaultTypeTags::Low;

	FVaultAnimation VaultingAnimation{ SelectVaultingMontage(GetLocomotionMode(), Stance, GetDesiredGait(), Parameters.VaultingType) };
	Parameters.VaultAnimation = VaultingAnimation;

	if (!ALSXTSettings->Vaulting.bAllowVaulting || GetLocalRole() <= ROLE_SimulatedProxy || !IsVaultingAllowedToStart(VaultingAnimation))
	{
		return false;
	}

	// If the target primitive can't move, then use world coordinates to save
	// some performance by skipping some coordinate space transformations later.

	

	if (MovementBaseUtility::UseRelativeLocation(TargetPrimitive))
	{
		// const auto TargetRelativeTransform{
		// 	TargetPrimitive->GetComponentTransform().GetRelativeTransform({TargetRotation, TargetLocation})
		// };
		// 
		// Parameters.TargetRelativeLocation = TargetRelativeTransform.GetLocation();
		// Parameters.TargetRelativeRotation = TargetRelativeTransform.Rotator();

		// const auto TargetRelativeTransform{
		// 	TargetPrimitive->GetComponentTransform().GetRelativeTransform({TargetRotation, LandingEndLocation + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1))})
		// };

		const auto TargetRelativeTransform{
			TargetPrimitive->GetComponentTransform().GetRelativeTransform({TargetRotation, LandingPointLocation})
		};

		Parameters.TargetRelativeLocation = TargetRelativeTransform.GetLocation();
		Parameters.TargetRelativeRotation = TargetRelativeTransform.Rotator();
	}
	else
	{
		// Parameters.TargetRelativeLocation = TargetLocation;
		// Parameters.TargetRelativeRotation = TargetRotation.Rotator();
		
		// Parameters.TargetRelativeLocation = LandingEndLocation + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1));
		Parameters.TargetRelativeLocation = LandingPointLocation;
		Parameters.TargetRelativeRotation = TargetRotation.Rotator();

	}
	FALSXTVaultingState NewVaultingState;
	NewVaultingState.VaultingParameters = Parameters;
	SetVaultingState(NewVaultingState);

	if (GetLocalRole() >= ROLE_Authority)
	{
		MulticastStartVaulting(Parameters);
	}
	else
	{
		GetCharacterMovement()->FlushServerMoves();

		StartVaultingImplementation(Parameters);
		ServerStartVaulting(Parameters);
	}

	return true;
}

void AALSXTCharacter::ServerStartVaulting_Implementation(const FALSXTVaultingParameters& Parameters)
{
	if (IsVaultingAllowedToStart(Parameters.VaultAnimation))
	{
		MulticastStartVaulting(Parameters);
		ForceNetUpdate();
	}
}

void AALSXTCharacter::MulticastStartVaulting_Implementation(const FALSXTVaultingParameters& Parameters)
{
	StartVaultingImplementation(Parameters);
}

void AALSXTCharacter::StartVaultingImplementation(const FALSXTVaultingParameters& Parameters)
{
	if (!IsVaultingAllowedToStart(Parameters.VaultAnimation))
	{
		return;
	}

	auto* VaultingSettings{SelectVaultingSettings(Parameters.VaultingType)};

	// if (!ALS_ENSURE(IsValid(VaultingSettings)) ||
	//     !ALS_ENSURE(IsValid(VaultingSettings->BlendInCurve)) ||
	//     !ALS_ENSURE(IsValid(VaultingSettings->InterpolationAndCorrectionAmountsCurve)))
	// {
	// 	return;
	// }

	if (!ALS_ENSURE(IsValid(VaultingState.VaultingParameters.VaultAnimation.Montage.BlendInCurve)) ||
		!ALS_ENSURE(IsValid(VaultingState.VaultingParameters.VaultAnimation.Montage.InterpolationAndCorrectionAmountsCurve)))
	{
		return;
	}

	const auto StartTime{VaultingSettings->GetStartTimeForHeight(VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight, VaultingState.VaultingParameters.VaultAnimation.Montage.StartTime, Parameters.VaultingHeight)};
	const auto PlayRate{VaultingSettings->GetPlayRateForHeight(VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight, VaultingState.VaultingParameters.VaultAnimation.Montage.PlayRate, Parameters.VaultingHeight)};

	// Calculate Vaulting duration.

	auto MinTime{0.0f};
	auto MaxTime{0.0f};
	VaultingState.VaultingParameters.VaultAnimation.Montage.InterpolationAndCorrectionAmountsCurve->GetTimeRange(MinTime, MaxTime);

	const auto Duration{MaxTime - StartTime};

	// Calculate actor offsets (offsets between actor and target transform).

	const auto bUseRelativeLocation{MovementBaseUtility::UseRelativeLocation(Parameters.TargetPrimitive.Get())};
	const auto TargetRelativeRotation{Parameters.TargetRelativeRotation.GetNormalized()};

	const auto TargetTransform{
		bUseRelativeLocation
			? FTransform{
				TargetRelativeRotation, Parameters.TargetRelativeLocation,
				Parameters.TargetPrimitive->GetComponentScale()
			}.GetRelativeTransformReverse(Parameters.TargetPrimitive->GetComponentTransform())
			: FTransform{TargetRelativeRotation, Parameters.TargetRelativeLocation}
	};

	const auto ActorFeetLocationOffset{GetCharacterMovement()->GetActorFeetLocation() - TargetTransform.GetLocation()};
	const auto ActorRotationOffset{TargetTransform.GetRotation().Inverse() * GetActorQuat()};

	// Clear the character movement mode and set the locomotion action to Vaulting.

	GetCharacterMovement()->SetMovementMode(MOVE_Custom);
	GetCharacterMovement()->SetBase(Parameters.TargetPrimitive.Get());
	AlsCharacterMovement->SetMovementModeLocked(true);

	if (GetLocalRole() >= ROLE_Authority)
	{
		GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	}

	// Apply Vaulting root motion.

	const auto Vaulting{MakeShared<FALSXTRootMotionSource_Vaulting>()};
	Vaulting->InstanceName = __FUNCTION__;
	Vaulting->Duration = Duration / PlayRate;
	Vaulting->VaultingSettings = VaultingSettings;
	Vaulting->TargetPrimitive = bUseRelativeLocation ? Parameters.TargetPrimitive : nullptr;
	Vaulting->TargetRelativeLocation = Parameters.TargetRelativeLocation;
	Vaulting->TargetRelativeRotation = TargetRelativeRotation;
	Vaulting->ActorFeetLocationOffset = ActorFeetLocationOffset;
	Vaulting->ActorRotationOffset = ActorRotationOffset.Rotator();
	Vaulting->VaultingHeight = Parameters.VaultingHeight;

	VaultingRootMotionSourceId = GetCharacterMovement()->ApplyRootMotionSource(Vaulting);

	// Play the animation montage if valid.

	if (ALS_ENSURE(IsValid(Parameters.VaultAnimation.Montage.Montage)))
	{
		// TODO Magic. I can't explain why, but this code fixes animation and root motion source desynchronization.

		const auto MontageStartTime{
			(Parameters.VaultingType == ALSXTVaultTypeTags::High || Parameters.VaultingType == ALSXTVaultTypeTags::Fence) && IsLocallyControlled()
				? StartTime - FMath::GetMappedRangeValueClamped(
					  FVector2f{VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight}, {GetWorld()->GetDeltaSeconds(), 0.0f}, Parameters.VaultingHeight)
				: StartTime
		};

		if (GetMesh()->GetAnimInstance()->Montage_Play(Parameters.VaultAnimation.Montage.Montage, PlayRate,
		                                               EMontagePlayReturnType::MontageLength,
		                                               MontageStartTime, false))
		{
			SetLocomotionAction(AlsLocomotionActionTags::Vaulting);
		}
	}

	OnVaultingStarted(Parameters);
}

UALSXTVaultingSettings* AALSXTCharacter::SelectVaultingSettings_Implementation(const FGameplayTag& VaultingType)
{
	return nullptr;
}

FVaultAnimation AALSXTCharacter::SelectVaultingMontage_Implementation(const FGameplayTag& CurrentLocomotionMode, const FGameplayTag& CurrentStance, const FGameplayTag& CurrentGait, const FGameplayTag& VaultingType)
{
	TArray<FVaultAnimation> VaultingAnimations = SelectVaultingSettings(VaultingType)->VaultAnimations;
	TArray<FGameplayTag> TagsArray = { CurrentLocomotionMode, CurrentStance, CurrentGait, VaultingType };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
	TArray<FVaultAnimation> FilteredVaultingAnimations;
	FVaultAnimation SelectedVaultingAnimation;

	// Return is there are no sounds
	if (VaultingAnimations.Num() < 1 || !VaultingAnimations[0].Montage.Montage)
	{
		return SelectedVaultingAnimation;
	}

	// Filter sounds based on Tag parameters
	for (auto VaultingAnimation : VaultingAnimations)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(VaultingAnimation.LocomotionMode);
		CurrentTagsContainer.AppendTags(VaultingAnimation.Stance);
		CurrentTagsContainer.AppendTags(VaultingAnimation.Gait);
		CurrentTagsContainer.AppendTags(VaultingAnimation.VaultType);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredVaultingAnimations.Add(VaultingAnimation);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredVaultingAnimations.Num() < 1 || !FilteredVaultingAnimations[0].Montage.Montage)
	{
		return SelectedVaultingAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredVaultingAnimations.Num() > 1)
	{
		// If FilteredVaultingAnimations contains LastCharacterVaultingAnimation, remove it from FilteredVaultingAnimations array to avoid duplicates
		// if (FilteredVaultingAnimations.Contains(LastCharacterVaultingAnimation))
		// {
		// 	FilteredVaultingAnimations.Remove(LastCharacterVaultingAnimation);
		// }

		//Shuffle Array
		for (int m = FilteredVaultingAnimations.Max(); m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredVaultingAnimations.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = rand() % FilteredVaultingAnimations.Max();
		SelectedVaultingAnimation = FilteredVaultingAnimations[RandIndex];
		return SelectedVaultingAnimation;
	}
	else
	{
		SelectedVaultingAnimation = FilteredVaultingAnimations[0];
		return SelectedVaultingAnimation;
	}

	return SelectedVaultingAnimation;
}

void AALSXTCharacter::OnVaultingStarted_Implementation(const FALSXTVaultingParameters& Parameters) {}

void AALSXTCharacter::RefreshVaulting()
{
	if (VaultingRootMotionSourceId <= 0)
	{
		return;
	}

	const auto RootMotionSource{GetCharacterMovement()->GetRootMotionSourceByID(VaultingRootMotionSourceId)};

	if (!RootMotionSource.IsValid() ||
	    RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::Finished) ||
	    RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::MarkedForRemoval) ||
	    // ReSharper disable once CppRedundantParentheses
	    (LocomotionAction.IsValid() && LocomotionAction != AlsLocomotionActionTags::Vaulting) ||
	    GetCharacterMovement()->MovementMode != MOVE_Custom)
	{
		StopVaulting();
		ForceNetUpdate();
	}
}

void AALSXTCharacter::StopVaulting()
{
	if (VaultingRootMotionSourceId <= 0)
	{
		return;
	}

	const auto RootMotionSource{GetCharacterMovement()->GetRootMotionSourceByID(VaultingRootMotionSourceId)};

	if (RootMotionSource.IsValid() &&
	    !RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::Finished) &&
	    !RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::MarkedForRemoval))
	{
		RootMotionSource->Status.SetFlag(ERootMotionSourceStatusFlags::MarkedForRemoval);
	}

	VaultingRootMotionSourceId = 0;

	if (GetLocalRole() >= ROLE_Authority)
	{
		GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	AlsCharacterMovement->SetMovementModeLocked(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	OnVaultingEnded();
}

void AALSXTCharacter::OnVaultingEnded_Implementation() {}