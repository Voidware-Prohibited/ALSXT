// MIT


#include "Components/Character/ALSXTSlidingActionComponent.h"
#include "AlsCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Utility/AlsMacros.h"

// Sets default values for this component's properties
UALSXTSlidingActionComponent::UALSXTSlidingActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}


// Called when the game starts
void UALSXTSlidingActionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(Character) };
	if (IsValid(EnhancedInput))
	{
		//FSetupPlayerInputComponentDelegate Del = Character->OnSetupPlayerInputComponentUpdated;
		//Del.AddUniqueDynamic(this, &USlidingActionComponent::SetupInputComponent(EnhancedInput));
	}
}


// Called every frame
void UALSXTSlidingActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UALSXTSlidingActionComponent::TryStartSliding(const float PlayRate)
{
	if (Character->GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
	{
		StartSliding(PlayRate, Character->ALSXTSettings->Sliding.bRotateToInputOnStart && Character->GetLocomotionState().bHasInput
			? Character->GetLocomotionState().InputYawAngle
			: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)));
	}
}

void UALSXTSlidingActionComponent::SetupInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	if (CrouchAction)
	{
		PlayerInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &UALSXTSlidingActionComponent::InputCrouch);
	}
}

void UALSXTSlidingActionComponent::InputCrouch()
{
	if (Character->GetDesiredStance() == AlsStanceTags::Standing)
	{
		if (CanSlide())
		{
			// StartSlideInternal();
			TryStartSliding(1.3f);
		}
		else {
			Character->SetDesiredStance(AlsStanceTags::Crouching);
		}
	}
	else if (Character->GetDesiredStance() == AlsStanceTags::Crouching)
	{
		Character->SetDesiredStance(AlsStanceTags::Standing);
	}
}

bool UALSXTSlidingActionComponent::IsSlidingAllowedToStart(const UAnimMontage* Montage) const
{
	return !Character->GetLocomotionAction().IsValid() ||
		// ReSharper disable once CppRedundantParentheses
		(Character->GetLocomotionAction() == AlsLocomotionActionTags::Sliding &&
			!Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage));
}

void UALSXTSlidingActionComponent::StartSliding(const float PlayRate, const float TargetYawAngle)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	auto* Montage{ SelectSlideMontage() };

	if (!ALS_ENSURE(IsValid(Montage)) || !IsSlidingAllowedToStart(Montage))
	{
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartSlidingImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		ServerStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		OnSlidingStarted();
	}
}

UAnimMontage* UALSXTSlidingActionComponent::SelectSlideMontage_Implementation()
{
	return Character->ALSXTSettings->Sliding.Montage;
}

void UALSXTSlidingActionComponent::ServerStartSliding_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsSlidingAllowedToStart(Montage))
	{
		MulticastStartSliding(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		Character->ForceNetUpdate();
	}
}

void UALSXTSlidingActionComponent::MulticastStartSliding_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	StartSlidingImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
}

void UALSXTSlidingActionComponent::StartSlidingImplementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsSlidingAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate))
	{
		SlidingState.TargetYawAngle = TargetYawAngle;

		Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::Sliding);
		// Crouch(); //Hack
	}
}

void UALSXTSlidingActionComponent::RefreshSliding(const float DeltaTime)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy ||
		Character->GetMesh()->GetAnimInstance()->RootMotionMode <= ERootMotionMode::IgnoreRootMotion)
	{
		// Refresh Sliding physics here because USlidingActionComponent::PhysicsRotation()
		// won't be called on simulated proxies or with ignored root motion.

		RefreshSlidingPhysics(DeltaTime);
	}
}

void UALSXTSlidingActionComponent::RefreshSlidingPhysics(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::Sliding)
	{
		return;
	}

	auto TargetRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };

	if (Character->ALSXTSettings->Sliding.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = SlidingState.TargetYawAngle;

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			SlidingState.TargetYawAngle, DeltaTime,
			Character->ALSXTSettings->Sliding.RotationInterpolationSpeed);

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}
