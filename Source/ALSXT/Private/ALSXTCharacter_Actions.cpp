#include "AlsCharacter.h"

#include "AlsAnimationInstance.h"
#include "AlsCharacterMovementComponent.h"
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
#include "Utility/AlsConstants.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsMath.h"
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
