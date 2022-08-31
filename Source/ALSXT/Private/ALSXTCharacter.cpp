#include "ALSXTCharacter.h"

#include "AlsCharacter.h"
#include "AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

AALSXTCharacter::AALSXTCharacter()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});
}

void AALSXTCharacter::NotifyControllerChanged()
{
	const auto* PreviousPlayer{Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		auto* EnhancedInputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())};
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	auto* NewPlayer{Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;

		auto* EnhancedInputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())};
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	Super::NotifyControllerChanged();
}

void AALSXTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FootprintsState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFreelooking, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredSex, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLocomotionVariant, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredInjury, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredCombatStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponFirearmStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponReadyPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredBlocking, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStationaryMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStatus, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFocus, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredHitReaction, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredHitSurface, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredBumpReaction, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredBumpSurface, Parameters)

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MeshRotation, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MeshRotationYaw, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MovementInput, Parameters)
}

void AALSXTCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AALSXTCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::InputLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::InputSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::InputWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::InputCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::InputJump);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::InputAim);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchShoulder);
		EnhancedInput->BindAction(FreelookAction, ETriggerEvent::Triggered, this, &ThisClass::InputFreelook);
		EnhancedInput->BindAction(ToggleCombatReadyAction, ETriggerEvent::Triggered, this, &ThisClass::InputToggleCombatReady);
		EnhancedInput->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ThisClass::InputBlock);
	}
}

void AALSXTCharacter::InputLookMouse(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void AALSXTCharacter::InputLook(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.X * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void AALSXTCharacter::InputMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsMath::ClampMagnitude012D(ActionValue.Get<FVector2D>())};

	FRotator CharRotation = GetMesh()->GetComponentRotation();
	MeshRotation = CharRotation;

	const auto ForwardDirection{UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw))};
	const auto RightDirection{UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection)};

	const auto CharForwardDirection{ UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(CharRotation.Pitch)) };
	const auto CharRightDirection{ UAlsMath::PerpendicularCounterClockwiseXY(CharForwardDirection) };

	MeshRotationYaw = FMath::GetMappedRangeValueClamped(FVector2D(-180.f, 180.f), FVector2D(0, 360.0f), MeshRotation.Yaw);

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
	if (GetDesiredFreelooking() == ALSXTFreelookingTags::True)
	{
		AddMovementInput(CharForwardDirection * Value.Y + CharRightDirection * Value.X);
		MovementInput = CharForwardDirection * Value.Y + CharRightDirection * Value.X;
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CharRotation.ToString());
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(MeshRotationYaw));
	}
	else
	{
		AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
		MovementInput = CharForwardDirection * Value.Y + CharRightDirection * Value.X;
	}
}

void AALSXTCharacter::InputSprint(const FInputActionValue& ActionValue)
{
	if (CanSprint())
	{
		SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
	}
}

void AALSXTCharacter::InputWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		SetDesiredGait(AlsGaitTags::Running);
	}
	else if (GetDesiredGait() == AlsGaitTags::Running)
	{
		SetDesiredGait(AlsGaitTags::Walking);
	}
}

void AALSXTCharacter::InputCrouch()
{
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		if (CanSlide())
		{
			// StartSlideInternal();
			TryStartSliding(1.3f);
		}
		else {
			SetDesiredStance(AlsStanceTags::Crouching);
		}
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
	}
}

void AALSXTCharacter::InputJump(const FInputActionValue& ActionValue)
{
	if (CanJump())
	{
		if (ActionValue.Get<bool>())
		{
			if (TryStopRagdolling())
			{
				return;
			}
			if (TryVault())
			{
				return;
			}
			if (TryStartMantlingGrounded())
			{
				return;
			}
			if (GetStance() == AlsStanceTags::Crouching)
			{
				SetDesiredStance(AlsStanceTags::Standing);
				return;
			}

			Jump();
		}
		else
		{
			StopJumping();
		}
	}
	else
	{
		StopJumping();
	}
}

void AALSXTCharacter::InputAim(const FInputActionValue& ActionValue)
{
	if (CanAim())
	{
		SetDesiredAiming(ActionValue.Get<bool>());
	}
}

void AALSXTCharacter::InputRagdoll()
{
	if (!TryStopRagdolling())
	{
		StartRagdolling();
	}
}

void AALSXTCharacter::InputRoll()
{
	static constexpr auto PlayRate{ 1.3f };
	if(CanRoll())
	{
		TryStartRolling(PlayRate);
	}
}

void AALSXTCharacter::InputRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
		                       ? AlsRotationModeTags::LookingDirection
		                       : AlsRotationModeTags::VelocityDirection);
}

void AALSXTCharacter::InputViewMode()
{
	const auto PreviousViewMode{ GetViewMode() };
	auto DesiredViewMode{ FGameplayTag::EmptyTag };
	DesiredViewMode == (GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
	if (CanSetToViewMode(DesiredViewMode)) 
	{
		SetViewMode(GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
		OnViewModeChanged(PreviousViewMode);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AALSXTCharacter::InputSwitchShoulder()
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AALSXTCharacter::InputFreelook(const FInputActionValue& ActionValue)
{
	if (CanFreelook())
	{
		if (ActionValue.Get<bool>())
		{
			SetDesiredFreelooking(ALSXTFreelookingTags::True);
			// LockRotation(MeshRotationYaw);
		}
		else
		{
			SetDesiredFreelooking(ALSXTFreelookingTags::False);
			// UnLockRotation();
		}
	}
}

void AALSXTCharacter::ApplyDesiredStance()
{
	if (!GetLocomotionAction().IsValid())
	{
		if (GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
		{
			if (GetDesiredStance() == AlsStanceTags::Standing)
			{
				UnCrouch();
			}
			else if (GetDesiredStance() == AlsStanceTags::Crouching)
			{
				Crouch();
			}
		}
		else if (GetLocomotionMode() == AlsLocomotionModeTags::InAir)
		{
			UnCrouch();
		}
	}
	else if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling && ALSXTSettings->Rolling.bCrouchOnStart) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding && ALSXTSettings->Sliding.bCrouchOnStart))
	{
		Crouch();
	}
}

void AALSXTCharacter::Crouch(const bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);

	// Change stance instantly without waiting for ACharacter::OnStartCrouch().

	if (!GetCharacterMovement()->bWantsToCrouch)
	{
		return;
	}

	if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding))
	{
		SetDesiredStance(GetDesiredStance()); // Keep desired stance when rolling.
		return;
	}

	SetDesiredStance(AlsStanceTags::Crouching);
}

bool AALSXTCharacter::IsAimingDownSights_Implementation() const
{
	return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson));
}

void AALSXTCharacter::SetFootprintsState(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	const auto PreviousFootprintsState{ FootprintsState };

	FootprintsState = NewFootprintsState;

	OnFootprintsStateChanged(PreviousFootprintsState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetFootprintsState(Foot, NewFootprintsState);
	}
}

void AALSXTCharacter::ServerSetFootprintsState_Implementation(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	SetFootprintsState(Foot, NewFootprintsState);
}


void AALSXTCharacter::ServerProcessNewFootprintsState_Implementation(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	ProcessNewFootprintsState(Foot, NewFootprintsState);
}

void AALSXTCharacter::OnReplicate_FootprintsState(const FALSXTFootprintsState& PreviousFootprintsState)
{
	OnFootprintsStateChanged(PreviousFootprintsState);
}

void AALSXTCharacter::OnFootprintsStateChanged_Implementation(const FALSXTFootprintsState& PreviousFootprintsState) {}

void AALSXTCharacter::InputToggleCombatReady()
{
	if (CanToggleCombatReady())
	{
		if ((GetDesiredCombatStance() == FGameplayTag::EmptyTag) | (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral))
		{
			if (CanBecomeCombatReady())
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
			}
		}
		else
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Neutral);
		}
	}
}

void AALSXTCharacter::InputBlock(const FInputActionValue& ActionValue)
{
	if (CanBlock())
	{
		SetDesiredBlocking(ActionValue.Get<bool>() ? ALSXTBlockingTags::Blocking : ALSXTBlockingTags::NotBlocking);
	}
	else if ((DesiredBlocking == ALSXTBlockingTags::Blocking) && (ActionValue.Get<bool>()  == false))
	{
		SetDesiredBlocking(ALSXTBlockingTags::NotBlocking);
	}
}

void AALSXTCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DebugDisplay, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
}

// Freelooking

void AALSXTCharacter::SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag)
{
	if (DesiredFreelooking != NewFreelookingTag)
	{
		DesiredFreelooking = NewFreelookingTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFreelooking, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFreelooking(NewFreelookingTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredFreelooking_Implementation(const FGameplayTag& NewFreelookingTag)
{
	SetDesiredFreelooking(NewFreelookingTag);
}

void AALSXTCharacter::SetFreelooking(const FGameplayTag& NewFreelookingTag)
{

	if (Freelooking != NewFreelookingTag)
	{
		const auto PreviousFreelooking{ Freelooking };

		Freelooking = NewFreelookingTag;

		OnFreelookingChanged(PreviousFreelooking);
	}
}

void AALSXTCharacter::OnFreelookingChanged_Implementation(const FGameplayTag& PreviousFreelookingTag) {}

// Sex

void AALSXTCharacter::SetDesiredSex(const FGameplayTag& NewSexTag)
{
	if (DesiredSex != NewSexTag)
	{
		DesiredSex = NewSexTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredSex, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredSex(NewSexTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredSex_Implementation(const FGameplayTag& NewSexTag)
{
	SetDesiredSex(NewSexTag);
}

void AALSXTCharacter::SetSex(const FGameplayTag& NewSexTag)
{

	if (Sex != NewSexTag)
	{
		const auto PreviousSex{ Sex };

		Sex = NewSexTag;

		OnSexChanged(PreviousSex);
	}
}

void AALSXTCharacter::OnSexChanged_Implementation(const FGameplayTag& PreviousSexTag) {}

// LocomotionVariant

void AALSXTCharacter::SetDesiredLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
{
	if (DesiredLocomotionVariant != NewLocomotionVariantTag)
	{
		DesiredLocomotionVariant = NewLocomotionVariantTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredLocomotionVariant, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredLocomotionVariant(NewLocomotionVariantTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariantTag)
{
	SetDesiredLocomotionVariant(NewLocomotionVariantTag);
}

void AALSXTCharacter::SetLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
{

	if (LocomotionVariant != NewLocomotionVariantTag)
	{
		const auto PreviousLocomotionVariant{ LocomotionVariant };

		LocomotionVariant = NewLocomotionVariantTag;

		OnLocomotionVariantChanged(PreviousLocomotionVariant);
	}
}

void AALSXTCharacter::OnLocomotionVariantChanged_Implementation(const FGameplayTag& PreviousLocomotionVariantTag) {}

// Injury

void AALSXTCharacter::SetDesiredInjury(const FGameplayTag& NewInjuryTag)
{
	if (DesiredInjury != NewInjuryTag)
	{
		DesiredInjury = NewInjuryTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredInjury, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredInjury(NewInjuryTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredInjury_Implementation(const FGameplayTag& NewInjuryTag)
{
	SetDesiredInjury(NewInjuryTag);
}

void AALSXTCharacter::SetInjury(const FGameplayTag& NewInjuryTag)
{

	if (Injury != NewInjuryTag)
	{
		const auto PreviousInjury{ Injury };

		Injury = NewInjuryTag;

		OnInjuryChanged(PreviousInjury);
	}
}

void AALSXTCharacter::OnInjuryChanged_Implementation(const FGameplayTag& PreviousInjuryTag) {}

// CombatStance

void AALSXTCharacter::SetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag)
{
	if (DesiredCombatStance != NewCombatStanceTag)
	{
		DesiredCombatStance = NewCombatStanceTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredCombatStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredCombatStance(NewCombatStanceTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredCombatStance_Implementation(const FGameplayTag& NewCombatStanceTag)
{
	SetDesiredCombatStance(NewCombatStanceTag);
}

void AALSXTCharacter::SetCombatStance(const FGameplayTag& NewCombatStanceTag)
{

	if (CombatStance != NewCombatStanceTag)
	{
		const auto PreviousCombatStance{ CombatStance };

		CombatStance = NewCombatStanceTag;

		OnCombatStanceChanged(PreviousCombatStance);
	}
}

void AALSXTCharacter::OnCombatStanceChanged_Implementation(const FGameplayTag& PreviousCombatStanceTag) {}

// WeaponFirearmStance

void AALSXTCharacter::SetDesiredWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
{
	if (DesiredWeaponFirearmStance != NewWeaponFirearmStanceTag)
	{
		DesiredWeaponFirearmStance = NewWeaponFirearmStanceTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponFirearmStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponFirearmStance(NewWeaponFirearmStanceTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredWeaponFirearmStance_Implementation(const FGameplayTag& NewWeaponFirearmStanceTag)
{
	SetDesiredWeaponFirearmStance(NewWeaponFirearmStanceTag);
}

void AALSXTCharacter::SetWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
{

	if (WeaponFirearmStance != NewWeaponFirearmStanceTag)
	{
		const auto PreviousWeaponFirearmStance{ WeaponFirearmStance };

		WeaponFirearmStance = NewWeaponFirearmStanceTag;

		OnWeaponFirearmStanceChanged(PreviousWeaponFirearmStance);
	}
}

void AALSXTCharacter::OnWeaponFirearmStanceChanged_Implementation(const FGameplayTag& PreviousWeaponFirearmStanceTag) {}

// WeaponReadyPosition

void AALSXTCharacter::SetDesiredWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
{
	if (DesiredWeaponReadyPosition != NewWeaponReadyPositionTag)
	{
		DesiredWeaponReadyPosition = NewWeaponReadyPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponReadyPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponReadyPosition(NewWeaponReadyPositionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredWeaponReadyPosition_Implementation(const FGameplayTag& NewWeaponReadyPositionTag)
{
	SetDesiredWeaponReadyPosition(NewWeaponReadyPositionTag);
}

void AALSXTCharacter::SetWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
{

	if (WeaponReadyPosition != NewWeaponReadyPositionTag)
	{
		const auto PreviousWeaponReadyPosition{ WeaponReadyPosition };

		WeaponReadyPosition = NewWeaponReadyPositionTag;

		OnWeaponReadyPositionChanged(PreviousWeaponReadyPosition);
	}
}

// Blocking

bool AALSXTCharacter::IsBlocking() const
{
	if (GetBlocking() == ALSXTBlockingTags::Blocking)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void AALSXTCharacter::SetDesiredBlocking(const FGameplayTag& NewBlockingTag)
{
	if (DesiredBlocking != NewBlockingTag)
	{
		DesiredBlocking = NewBlockingTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredBlocking, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredBlocking(NewBlockingTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredBlocking_Implementation(const FGameplayTag& NewBlockingTag)
{
	SetDesiredBlocking(NewBlockingTag);
}

void AALSXTCharacter::SetBlocking(const FGameplayTag& NewBlockingTag)
{
	if (Blocking != NewBlockingTag)
	{
		const auto PreviousBlocking{ Blocking };

		Blocking = NewBlockingTag;

		OnBlockingChanged(PreviousBlocking);
	}
}

void AALSXTCharacter::OnBlockingChanged_Implementation(const FGameplayTag& PreviousBlockingTag) {}

void AALSXTCharacter::SetDesiredStationaryMode(const FGameplayTag& NewStationaryModeTag)
{
	if (DesiredStationaryMode != NewStationaryModeTag)
	{
		DesiredStationaryMode = NewStationaryModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredStationaryMode, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredStationaryMode(NewStationaryModeTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredStationaryMode_Implementation(const FGameplayTag& NewStationaryModeTag)
{
	SetDesiredStationaryMode(NewStationaryModeTag);
}

void AALSXTCharacter::SetStationaryMode(const FGameplayTag& NewStationaryModeTag)
{

	if (StationaryMode != NewStationaryModeTag)
	{
		const auto PreviousStationaryMode{ StationaryMode };

		StationaryMode = NewStationaryModeTag;

		OnStationaryModeChanged(PreviousStationaryMode);
	}
}

void AALSXTCharacter::OnStationaryModeChanged_Implementation(const FGameplayTag& PreviousStationaryModeTag) {}

// Status

void AALSXTCharacter::SetDesiredStatus(const FGameplayTag& NewStatusTag)
{
	if (DesiredStatus != NewStatusTag)
	{
		DesiredStatus = NewStatusTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredStatus, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredStatus(NewStatusTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	SetDesiredStatus(NewStatusTag);
}

void AALSXTCharacter::SetStatus(const FGameplayTag& NewStatusTag)
{

	if (Status != NewStatusTag)
	{
		const auto PreviousStatus{ Status };

		Status = NewStatusTag;

		OnStatusChanged(PreviousStatus);
	}
}

void AALSXTCharacter::OnStatusChanged_Implementation(const FGameplayTag& PreviousStatusTag) {}

// Focus

void AALSXTCharacter::SetDesiredFocus(const FGameplayTag& NewFocusTag)
{
	if (DesiredFocus != NewFocusTag)
	{
		DesiredFocus = NewFocusTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFocus, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFocus(NewFocusTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredFocus_Implementation(const FGameplayTag& NewFocusTag)
{
	SetDesiredFocus(NewFocusTag);
}

void AALSXTCharacter::SetFocus(const FGameplayTag& NewFocusTag)
{

	if (Focus != NewFocusTag)
	{
		const auto PreviousFocus{ Focus };

		Focus = NewFocusTag;

		OnFocusChanged(PreviousFocus);
	}
}

void AALSXTCharacter::OnFocusChanged_Implementation(const FGameplayTag& PreviousFocusTag) {}

// HitReaction

void AALSXTCharacter::SetDesiredHitReaction(const FGameplayTag& NewHitReactionTag)
{
	if (DesiredHitReaction != NewHitReactionTag)
	{
		DesiredHitReaction = NewHitReactionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredHitReaction, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredHitReaction(NewHitReactionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredHitReaction_Implementation(const FGameplayTag& NewHitReactionTag)
{
	SetDesiredHitReaction(NewHitReactionTag);
}

void AALSXTCharacter::SetHitReaction(const FGameplayTag& NewHitReactionTag)
{

	if (HitReaction != NewHitReactionTag)
	{
		const auto PreviousHitReaction{ HitReaction };

		HitReaction = NewHitReactionTag;

		OnHitReactionChanged(PreviousHitReaction);
	}
}

void AALSXTCharacter::OnHitReactionChanged_Implementation(const FGameplayTag& PreviousHitReactionTag) {}

// HitSurface

void AALSXTCharacter::SetDesiredHitSurface(const FGameplayTag& NewHitSurfaceTag)
{
	if (DesiredHitSurface != NewHitSurfaceTag)
	{
		DesiredHitSurface = NewHitSurfaceTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredHitSurface, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredHitSurface(NewHitSurfaceTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredHitSurface_Implementation(const FGameplayTag& NewHitSurfaceTag)
{
	SetDesiredHitSurface(NewHitSurfaceTag);
}

void AALSXTCharacter::SetHitSurface(const FGameplayTag& NewHitSurfaceTag)
{

	if (HitSurface != NewHitSurfaceTag)
	{
		const auto PreviousHitSurface{ HitSurface };

		HitSurface = NewHitSurfaceTag;

		OnHitSurfaceChanged(PreviousHitSurface);
	}
}

void AALSXTCharacter::OnHitSurfaceChanged_Implementation(const FGameplayTag& PreviousHitSurfaceTag) {}

// BumpReaction

void AALSXTCharacter::SetDesiredBumpReaction(const FGameplayTag& NewBumpReactionTag)
{
	if (DesiredBumpReaction != NewBumpReactionTag)
	{
		DesiredBumpReaction = NewBumpReactionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredBumpReaction, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredBumpReaction(NewBumpReactionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredBumpReaction_Implementation(const FGameplayTag& NewBumpReactionTag)
{
	SetDesiredBumpReaction(NewBumpReactionTag);
}

void AALSXTCharacter::SetBumpReaction(const FGameplayTag& NewBumpReactionTag)
{

	if (BumpReaction != NewBumpReactionTag)
	{
		const auto PreviousBumpReaction{ BumpReaction };

		BumpReaction = NewBumpReactionTag;

		OnBumpReactionChanged(PreviousBumpReaction);
	}
}

void AALSXTCharacter::OnBumpReactionChanged_Implementation(const FGameplayTag& PreviousBumpReactionTag) {}

// BumpSurface

void AALSXTCharacter::SetDesiredBumpSurface(const FGameplayTag& NewBumpSurfaceTag)
{
	if (DesiredBumpSurface != NewBumpSurfaceTag)
	{
		DesiredBumpSurface = NewBumpSurfaceTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredBumpSurface, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredBumpSurface(NewBumpSurfaceTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredBumpSurface_Implementation(const FGameplayTag& NewBumpSurfaceTag)
{
	SetDesiredBumpSurface(NewBumpSurfaceTag);
}

void AALSXTCharacter::SetBumpSurface(const FGameplayTag& NewBumpSurfaceTag)
{

	if (BumpSurface != NewBumpSurfaceTag)
	{
		const auto PreviousBumpSurface{ BumpSurface };

		BumpSurface = NewBumpSurfaceTag;

		OnBumpSurfaceChanged(PreviousBumpSurface);
	}
}

void AALSXTCharacter::OnBumpSurfaceChanged_Implementation(const FGameplayTag& PreviousBumpSurfaceTag) {}

void AALSXTCharacter::OnAIJumpObstacle_Implementation()
{
	// if (TryVault())
	// {
	// 	StopJumping();
	// 	StartVault();
	// }
	// else
	// {
	// 	Jump();
	// }
	Jump();
}
void AALSXTCharacter::CanSprint_Implementation() {}
void AALSXTCharacter::AIObstacleTrace_Implementation() {}
void AALSXTCharacter::StartVault_Implementation() {}
void AALSXTCharacter::StartWallrun_Implementation() {}
void AALSXTCharacter::OnWeaponReadyPositionChanged_Implementation(const FGameplayTag& PreviousWeaponReadyPositionTag) {}