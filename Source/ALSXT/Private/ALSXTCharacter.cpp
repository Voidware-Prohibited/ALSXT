#include "ALSXTCharacter.h"

#include "AlsCharacter.h"
#include "AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Utility/ALSXTGameplayTags.h"
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredHoldingBreath, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStationaryMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredEmote, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGesture, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGestureHand, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredReloadingType, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFirearmFingerAction, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFirearmFingerActionHand, Parameters)

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
		EnhancedInput->BindAction(PrimaryActionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryAction);
		EnhancedInput->BindAction(SecondaryActionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryAction);
		EnhancedInput->BindAction(PrimaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryInteraction);
		EnhancedInput->BindAction(SecondaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryInteraction);
		EnhancedInput->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ThisClass::InputBlock);
		EnhancedInput->BindAction(HoldBreathAction, ETriggerEvent::Triggered, this, &ThisClass::InputHoldBreath);
		OnSetupPlayerInputComponentUpdated.Broadcast();
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
	FRotator CapsuleRotation = GetActorRotation();
	const auto ForwardDirection{UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw))};
	const auto RightDirection{UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection)};
	const auto CharForwardDirection{ UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(CapsuleRotation.Yaw)) };
	const auto CharRightDirection{ UAlsMath::PerpendicularCounterClockwiseXY(CharForwardDirection) };

	if (GetDesiredFreelooking() == ALSXTFreelookingTags::True)
	{
		AddMovementInput(CharForwardDirection * Value.Y + CharRightDirection * Value.X);
		MovementInput = CharForwardDirection * Value.Y + CharRightDirection * Value.X;
	}
	else
	{
		AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
		MovementInput = ForwardDirection * Value.Y + RightDirection * Value.X;
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
		if (ActionValue.Get<bool>())
		{
			SetDesiredRotationMode(AlsRotationModeTags::Aiming);
			if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Ready)
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Aiming);
			}
			if (IsHoldingAimableItem()) {
				if (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral)
				{
					SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Aiming);
				}
			}
			SetDesiredAiming(ActionValue.Get<bool>());
		}
		else 
		{
			if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Aiming)
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
			}
			if (IsHoldingAimableItem()) {
				if (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral)
				{
					SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
				} 
				else
				{
					SetDesiredRotationMode(AlsRotationModeTags::LookingDirection);
				}
			}
			else 
			{
				SetDesiredRotationMode(AlsRotationModeTags::LookingDirection);
			}
			SetDesiredAiming(ActionValue.Get<bool>());
		}
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
			ActivateFreelooking();
		}
		else
		{
			DeactivateFreelooking();
		}
	}
}

void AALSXTCharacter::InputHoldBreath(const FInputActionValue& ActionValue)
{
	if (CanHoldBreath())
	{
		SetDesiredHoldingBreath(ActionValue.Get<bool>() ? ALSXTHoldingBreathTags::True : ALSXTHoldingBreathTags::False);
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

void AALSXTCharacter::ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport)
{
	RefreshRotationInstant(TargetYawAngle, Teleport);
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
	return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson) && (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral));
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
		if ((GetDesiredCombatStance() == FGameplayTag::EmptyTag) || (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral))
		{
			if (CanBecomeCombatReady())
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
				if (IsHoldingAimableItem())
				{
					if (GetRotationMode() != AlsRotationModeTags::Aiming)
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::LowReady);
					}
					else
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
					}
				}
				else
				{
					SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
				}
			}
		}
		else
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Neutral);
			SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::PatrolReady);
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

// First Person Eye Focus

bool AALSXTCharacter::IsFirstPersonEyeFocusActive() const
{
	if (GetViewMode() == AlsViewModeTags::FirstPerson) 
	{
		if (IsDesiredAiming()) 
		{
			if (IsHoldingAimableItem()) 
			{
				if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Freelooking

void AALSXTCharacter::IsFreelooking(bool& bIsFreelooking, bool& bIsFreelookingInFirstPerson) const
{

	bIsFreelooking = (GetDesiredFreelooking() == ALSXTFreelookingTags::True) ? true : false;
	bIsFreelookingInFirstPerson = (bIsFreelooking && ((GetViewMode() == AlsViewModeTags::FirstPerson) ? true : false));
}

void AALSXTCharacter::ActivateFreelooking()
{
	//PreviousYaw = GetViewState().Rotation.Yaw;
	PreviousYaw = FMath::GetMappedRangeValueClamped(FVector2D(0, 359.998993), FVector2D(0.0, 1.0), GetControlRotation().Yaw);
	//FMath::GetMappedRangeValueClamped(FVector2D(-90,90), FVector2D(0,1), GetViewState().Rotation.Pitch)
	PreviousPitch = FMath::GetMappedRangeValueClamped(FVector2D(89.900002, -89.899994), FVector2D(0.0, 1.0), GetViewState().Rotation.Pitch);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Yaw));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), PreviousYaw));
	LockRotation(GetActorRotation().Yaw);
	SetDesiredFreelooking(ALSXTFreelookingTags::True);
}

void AALSXTCharacter::DeactivateFreelooking()
{
	UnLockRotation();
	SetDesiredFreelooking(ALSXTFreelookingTags::False);
}

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
		const auto PreviousCombatStance{ CombatStance };

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredCombatStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredCombatStance(NewCombatStanceTag);
				if (NewCombatStanceTag != ALSXTCombatStanceTags::Neutral)
				{
					if (IsHoldingAimableItem())
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::LowReady);
					}
					else
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
					}
					SetDesiredRotationMode(AlsRotationModeTags::Aiming);
				}
				else
				{
					SetDesiredRotationMode(AlsRotationModeTags::LookingDirection);
				}
			}
			else if (GetLocalRole() == ROLE_Authority)
			{
				OnCombatStanceChanged(PreviousCombatStance);
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

// HoldingBreath

void AALSXTCharacter::SetDesiredHoldingBreath(const FGameplayTag& NewHoldingBreathTag)
{
	if (DesiredHoldingBreath != NewHoldingBreathTag)
	{
		DesiredHoldingBreath = NewHoldingBreathTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredHoldingBreath, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredHoldingBreath(NewHoldingBreathTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredHoldingBreath_Implementation(const FGameplayTag& NewHoldingBreathTag)
{
	SetDesiredHoldingBreath(NewHoldingBreathTag);
}

void AALSXTCharacter::SetHoldingBreath(const FGameplayTag& NewHoldingBreathTag)
{

	if (HoldingBreath != NewHoldingBreathTag)
	{
		const auto PreviousHoldingBreath{ HoldingBreath };

		HoldingBreath = NewHoldingBreathTag;

		OnHoldingBreathChanged(PreviousHoldingBreath);
	}
}

void AALSXTCharacter::OnHoldingBreathChanged_Implementation(const FGameplayTag& PreviousHoldingBreathTag) {}

// Emote

void AALSXTCharacter::SetDesiredEmote(const FGameplayTag& NewEmoteTag)
{
	if (DesiredEmote != NewEmoteTag)
	{
		DesiredEmote = NewEmoteTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredEmote, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredEmote(NewEmoteTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredEmote_Implementation(const FGameplayTag& NewEmoteTag)
{
	SetDesiredEmote(NewEmoteTag);
}

void AALSXTCharacter::SetEmote(const FGameplayTag& NewEmoteTag)
{

	if (Emote != NewEmoteTag)
	{
		const auto PreviousEmote{ Emote };

		Emote = NewEmoteTag;

		OnEmoteChanged(PreviousEmote);
	}
}

void AALSXTCharacter::OnEmoteChanged_Implementation(const FGameplayTag& PreviousEmoteTag) {}

// Gesture

void AALSXTCharacter::SetDesiredGesture(const FGameplayTag& NewGestureTag)
{
	if (DesiredGesture != NewGestureTag)
	{
		DesiredGesture = NewGestureTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGesture, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredGesture(NewGestureTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredGesture_Implementation(const FGameplayTag& NewGestureTag)
{
	SetDesiredGesture(NewGestureTag);
}

void AALSXTCharacter::SetGesture(const FGameplayTag& NewGestureTag)
{

	if (Gesture != NewGestureTag)
	{
		const auto PreviousGesture{ Gesture };

		Gesture = NewGestureTag;

		OnGestureChanged(PreviousGesture);
	}
}

void AALSXTCharacter::OnGestureChanged_Implementation(const FGameplayTag& PreviousGestureTag) {}

// GestureHand

void AALSXTCharacter::SetDesiredGestureHand(const FGameplayTag& NewGestureHandTag)
{
	if (DesiredGestureHand != NewGestureHandTag)
	{
		DesiredGestureHand = NewGestureHandTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGestureHand, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredGestureHand(NewGestureHandTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredGestureHand_Implementation(const FGameplayTag& NewGestureHandTag)
{
	SetDesiredGestureHand(NewGestureHandTag);
}

void AALSXTCharacter::SetGestureHand(const FGameplayTag& NewGestureHandTag)
{

	if (GestureHand != NewGestureHandTag)
	{
		const auto PreviousGestureHand{ GestureHand };

		GestureHand = NewGestureHandTag;

		OnGestureHandChanged(PreviousGestureHand);
	}
}

void AALSXTCharacter::OnGestureHandChanged_Implementation(const FGameplayTag& PreviousGestureHandTag) {}

// ReloadingType

void AALSXTCharacter::SetDesiredReloadingType(const FGameplayTag& NewReloadingTypeTag)
{
	if (DesiredReloadingType != NewReloadingTypeTag)
	{
		DesiredReloadingType = NewReloadingTypeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredReloadingType, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredReloadingType(NewReloadingTypeTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredReloadingType_Implementation(const FGameplayTag& NewReloadingTypeTag)
{
	SetDesiredReloadingType(NewReloadingTypeTag);
}

void AALSXTCharacter::SetReloadingType(const FGameplayTag& NewReloadingTypeTag)
{

	if (ReloadingType != NewReloadingTypeTag)
	{
		const auto PreviousReloadingType{ ReloadingType };

		ReloadingType = NewReloadingTypeTag;

		OnReloadingTypeChanged(PreviousReloadingType);
	}
}

void AALSXTCharacter::OnReloadingTypeChanged_Implementation(const FGameplayTag& PreviousReloadingTypeTag) {}

// FirearmFingerAction

void AALSXTCharacter::SetDesiredFirearmFingerAction(const FGameplayTag& NewFirearmFingerActionTag)
{
	if (DesiredFirearmFingerAction != NewFirearmFingerActionTag)
	{
		DesiredFirearmFingerAction = NewFirearmFingerActionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFirearmFingerAction, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFirearmFingerAction(NewFirearmFingerActionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredFirearmFingerAction_Implementation(const FGameplayTag& NewFirearmFingerActionTag)
{
	SetDesiredFirearmFingerAction(NewFirearmFingerActionTag);
}

void AALSXTCharacter::SetFirearmFingerAction(const FGameplayTag& NewFirearmFingerActionTag)
{

	if (FirearmFingerAction != NewFirearmFingerActionTag)
	{
		const auto PreviousFirearmFingerAction{ FirearmFingerAction };

		FirearmFingerAction = NewFirearmFingerActionTag;

		OnFirearmFingerActionChanged(PreviousFirearmFingerAction);
	}
}

void AALSXTCharacter::OnFirearmFingerActionChanged_Implementation(const FGameplayTag& PreviousFirearmFingerActionTag) {}

// FirearmFingerActionHand

void AALSXTCharacter::SetDesiredFirearmFingerActionHand(const FGameplayTag& NewFirearmFingerActionHandTag)
{
	if (DesiredFirearmFingerActionHand != NewFirearmFingerActionHandTag)
	{
		DesiredFirearmFingerActionHand = NewFirearmFingerActionHandTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFirearmFingerActionHand, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFirearmFingerActionHand(NewFirearmFingerActionHandTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredFirearmFingerActionHand_Implementation(const FGameplayTag& NewFirearmFingerActionHandTag)
{
	SetDesiredFirearmFingerActionHand(NewFirearmFingerActionHandTag);
}

void AALSXTCharacter::SetFirearmFingerActionHand(const FGameplayTag& NewFirearmFingerActionHandTag)
{

	if (FirearmFingerActionHand != NewFirearmFingerActionHandTag)
	{
		const auto PreviousFirearmFingerActionHand{ FirearmFingerActionHand };

		FirearmFingerActionHand = NewFirearmFingerActionHandTag;

		OnFirearmFingerActionHandChanged(PreviousFirearmFingerActionHand);
	}
}

void AALSXTCharacter::OnFirearmFingerActionHandChanged_Implementation(const FGameplayTag& PreviousFirearmFingerActionHandTag) {}

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