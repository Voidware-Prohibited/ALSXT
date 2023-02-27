#include "ALSXTCharacter.h"

#include "AlsCharacter.h"
#include "AlsCharacterMovementComponent.h"
#include "AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Utility/ALSXTStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTUnarmedCombatSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/CollisionInterface.h"
#include "Interfaces/ALSXTCharacterInterface.h"
//
#include "Curves/CurveVector.h"
#include "RootMotionSources/ALSXTRootMotionSource_Vaulting.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsUtility.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AALSXTCharacter::AALSXTCharacter()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	// Add Character Sound Component
	// CharacterSound = CreateDefaultSubobject<UALSXTCharacterSoundComponent>(TEXT("Character Sound"));
	// 
	// // Add Sliding Action Component
	// SlidingAction = CreateDefaultSubobject<USlidingActionComponent>(TEXT("Sliding Action"));
	// 
	// // Add Combat Component
	// Combat = CreateDefaultSubobject<UUnarmedCombatComponent>(TEXT("Combat"));
	// 
	// // Add Impact Reaction Component
	// ImpactReaction = CreateDefaultSubobject<UImpactReactionComponent>(TEXT("Impact Reaction"));
	// 
	// Add Physical Animation Component
	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation"));
	AddOwnedComponent(PhysicalAnimation);
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredHoldingBreath, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStationaryMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredPhysicalAnimationMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGesture, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGestureHand, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredReloadingType, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFirearmFingerAction, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFirearmFingerActionHand, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponCarryPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFirearmSightLocation, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredVaultType, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponObstruction, Parameters)

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MovementInput, Parameters)
}

void AALSXTCharacter::BeginPlay()
{
	AlsCharacter = Cast<AAlsCharacter>(GetParentActor());
	Super::BeginPlay();

	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");
	AttackTraceTimerDelegate.BindUFunction(this, "AttackCollisionTrace", AttackTraceSettings);
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
			if (TryStartVaultingGrounded())
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
	else
	{
		if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling && ALSXTSettings->Rolling.bCrouchOnStart) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding && ALSXTSettings->Sliding.bCrouchOnStart))
		{
			Crouch();
		}
	}
}

void AALSXTCharacter::ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport)
{
	RefreshRotationInstant(TargetYawAngle, Teleport);
}

void AALSXTCharacter::SetMovementModeLocked(bool bNewMovementModeLocked)
{
	// AlsCharacterMovement->SetMovementModeLocked(bNewMovementModeLocked);
	Cast<UAlsCharacterMovementComponent>(GetCharacterMovement())->SetMovementModeLocked(bNewMovementModeLocked);
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

void AALSXTCharacter::InputLeanLeft()
{
	// 
}

void AALSXTCharacter::InputLeanRight()
{
	// 
}

void AALSXTCharacter::InputAcrobatic()
{
	// 
}

void AALSXTCharacter::InputSwitchTargetLeft()
{
	// 
}

void AALSXTCharacter::InputSwitchTargetRight()
{
	// 
}

void AALSXTCharacter::InputToggleWeaponFirearmStance()
{
	// 
}

void AALSXTCharacter::InputToggleWeaponReadyPosition()
{
	// 
}

void AALSXTCharacter::InputReload()
{
	// 
}

void AALSXTCharacter::InputReloadWithRetention()
{
	// 
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

bool AALSXTCharacter::IsVaultingAllowedToStart_Implementation() const
{
	return !LocomotionAction.IsValid();
}

bool AALSXTCharacter::TryStartVaulting(const FALSXTVaultingTraceSettings& TraceSettings)
{
	if (!ALSXTSettings->Vaulting.bAllowVaulting || GetLocalRole() <= ROLE_SimulatedProxy || !IsVaultingAllowedToStart())
	{
		return false;
	}

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

	// Vaulting Room Trace

	// Vaulting Room Trace Hit Result
	FHitResult LandingTraceHit;
	static const FName LandingTraceTag{ __FUNCTION__ TEXT(" (Landing Trace)") };

	// Set Local Variables
	const FVector LandingStartLocation{	TargetCapsuleLocation + (DepthTraceHit.Normal * 60) + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1)) };
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
	const FVector LandingLocation = LandingEndLocation * (DownwardTraceHit.Normal - 100);

	// Vaulting Room Trace Hit Result
	FHitResult LandingLocationTraceHit;
	static const FName LandingLocationTraceTag{ __FUNCTION__ TEXT(" (Landing Location Trace)") };

	GetWorld()->SweepSingleByObjectType(LandingLocationTraceHit, LandingEndLocation, LandingLocation, FQuat::Identity, ObjectQueryParameters,
		FCollisionShape::MakeCapsule(TraceCapsuleRadius, ForwardTraceCapsuleHalfHeight),
		{ LandingLocationTraceTag, false, this });

	UAlsUtility::DrawDebugSweepSingleCapsuleAlternative(GetWorld(), ForwardTraceStart, ForwardTraceEnd, TraceCapsuleRadius,
		ForwardTraceCapsuleHalfHeight, true, ForwardTraceHit,
		FLinearColor::Yellow, FLinearColor::Red, 5.0f);

	const auto TargetRotation{(-ForwardTraceHit.ImpactNormal.GetSafeNormal2D()).ToOrientationQuat()};

	FALSXTVaultingParameters Parameters;

	Parameters.TargetPrimitive = TargetPrimitive;
	// Parameters.VaultingHeight = UE_REAL_TO_FLOAT((TargetLocation.Z - CapsuleBottomLocation.Z) / CapsuleScale);
	Parameters.VaultingHeight = UE_REAL_TO_FLOAT(LandingEndLocation.Z);

	// Determine the Vaulting type by checking the movement mode and Vaulting height.

	Parameters.VaultingType = LocomotionMode != AlsLocomotionModeTags::Grounded
		                          ? EAlsMantlingType::InAir
		                          : Parameters.VaultingHeight > ALSXTSettings->Vaulting.VaultingHighHeightThreshold
		                          ? EAlsMantlingType::High
		                          : EAlsMantlingType::Low;

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

		const auto TargetRelativeTransform{
			TargetPrimitive->GetComponentTransform().GetRelativeTransform({TargetRotation, LandingEndLocation + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1))})
		};

		Parameters.TargetRelativeLocation = TargetRelativeTransform.GetLocation();
		Parameters.TargetRelativeRotation = TargetRelativeTransform.Rotator();
	}
	else
	{
		// Parameters.TargetRelativeLocation = TargetLocation;
		// Parameters.TargetRelativeRotation = TargetRotation.Rotator();
		
		Parameters.TargetRelativeLocation = LandingEndLocation + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1));
		Parameters.TargetRelativeRotation = TargetRotation.Rotator();

	}

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
	if (IsVaultingAllowedToStart())
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
	if (!IsVaultingAllowedToStart())
	{
		return;
	}

	auto* VaultingSettings{SelectVaultingSettings(Parameters.VaultingType)};

	if (!ALS_ENSURE(IsValid(VaultingSettings)) ||
	    !ALS_ENSURE(IsValid(VaultingSettings->BlendInCurve)) ||
	    !ALS_ENSURE(IsValid(VaultingSettings->InterpolationAndCorrectionAmountsCurve)))
	{
		return;
	}

	const auto StartTime{VaultingSettings->GetStartTimeForHeight(Parameters.VaultingHeight)};
	const auto PlayRate{VaultingSettings->GetPlayRateForHeight(Parameters.VaultingHeight)};

	// Calculate Vaulting duration.

	auto MinTime{0.0f};
	auto MaxTime{0.0f};
	VaultingSettings->InterpolationAndCorrectionAmountsCurve->GetTimeRange(MinTime, MaxTime);

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

	// GetCharacterMovement()->SetMovementMode(MOVE_Custom);
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

	if (ALS_ENSURE(IsValid(VaultingSettings->Montage)))
	{
		// TODO Magic. I can't explain why, but this code fixes animation and root motion source desynchronization.

		const auto MontageStartTime{
			Parameters.VaultingType == EAlsMantlingType::InAir && IsLocallyControlled()
				? StartTime - FMath::GetMappedRangeValueClamped(
					  FVector2f{VaultingSettings->ReferenceHeight}, {GetWorld()->GetDeltaSeconds(), 0.0f}, Parameters.VaultingHeight)
				: StartTime
		};

		if (GetMesh()->GetAnimInstance()->Montage_Play(VaultingSettings->Montage, PlayRate,
		                                               EMontagePlayReturnType::MontageLength,
		                                               MontageStartTime, false))
		{
			SetLocomotionAction(AlsLocomotionActionTags::Vaulting);
		}
	}

	OnVaultingStarted(Parameters);
}

UALSXTVaultingSettings* AALSXTCharacter::SelectVaultingSettings_Implementation(EAlsMantlingType VaultingType)
{
	return nullptr;
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

// Attack Collision Trace}

void AALSXTCharacter::OnAttackHit_Implementation(FAttackDoubleHitResult Hit)
{
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	float Scale;
	GetCameraShakeInfoFromHit(Hit, CameraShakeClass, Scale);
	if (CameraShakeClass != nullptr)
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(CameraShakeClass, Scale);
	}
}

void AALSXTCharacter::GetLocationFromBoneName_Implementation(FName Hit, FGameplayTag& Location) {}

void AALSXTCharacter::GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side)
{
	float DotProduct { this->GetDotProductTo(Hit.OriginHitResult.HitResult.GetActor()) };

	// 1 is face to face, 0 is side,, -1 is behind

	FVector CrossProduct{ FVector::CrossProduct(Hit.HitResult.Impulse, Hit.HitResult.Impulse) };
	Side = ALSXTImpactSideTags::Front;
}

void AALSXTCharacter::GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength)
{
	float HitMass = Hit.HitResult.Mass;
	float HitVelocity = Hit.HitResult.Velocity;
	float HitMomentum = HitMass * HitVelocity;

	float SelfMass = Hit.OriginHitResult.Mass;
	float SelfVelocity = Hit.OriginHitResult.Velocity;
	float SelfMomentum = SelfMass * SelfVelocity;

	float MomemtumSum = HitMomentum + SelfMomentum;
}

void AALSXTCharacter::BeginAttackCollisionTrace(FALSXTAttackTraceSettings TraceSettings)
{
	AttackTraceSettings = TraceSettings;
	GetWorld()->GetTimerManager().SetTimer(AttackTraceTimerHandle, AttackTraceTimerDelegate, 0.1f, true);
}

void AALSXTCharacter::AttackCollisionTrace()
{
		// Update AttackTraceSettings
		GetUnarmedTraceLocations(AttackTraceSettings.AttackType, AttackTraceSettings.Start, AttackTraceSettings.End, AttackTraceSettings.Radius);

		// Declare Local Vars
		TArray<AActor*> InitialIgnoredActors;
		TArray<AActor*> OriginTraceIgnoredActors;
		TArray<FHitResult> HitResults;
		InitialIgnoredActors.Add(this);	// Add Self to Initial Trace Ignored Actors

		TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;
		AttackTraceObjectTypes = ALSXTSettings->UnarmedCombat.AttackTraceObjectTypes;

		// Initial Trace
		bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), AttackTraceSettings.Start, AttackTraceSettings.End, AttackTraceSettings.Radius, ALSXTSettings->UnarmedCombat.AttackTraceObjectTypes, false, InitialIgnoredActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Green, FLinearColor::Red, 0.0f);

		if (isHit)
		{
			// Loop through HitResults Array
			for (auto& HitResult : HitResults)
			{
				// Check if not in AttackedActors Array
				if (!AttackTraceLastHitActors.Contains(HitResult.GetActor()))
				{
					// Add to AttackedActors Array
					AttackTraceLastHitActors.AddUnique(HitResult.GetActor());

					// Declare Local Vars
					FAttackDoubleHitResult CurrentHitResult;
					FGameplayTag ImpactLoc;
					FGameplayTag ImpactSide;
					FGameplayTag ImpactForm;
					AActor* HitActor{ nullptr };
					FString HitActorname;
					float HitActorVelocity { 0.0f };
					float HitActorMass { 0.0f };
					float HitActorAttackVelocity { 0.0f };
					float HitActorAttackMass { 0.0f };
					float TotalImpactEnergy { 0.0f };

					// Populate Hit
					// 
					
					// Call OnActorAttackCollision on CollisionInterface
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UCollisionInterface::StaticClass()))
					{
						ICollisionInterface::Execute_GetActorVelocity(HitActor, HitActorVelocity);
						ICollisionInterface::Execute_GetActorMass(HitActor, HitActorMass);
					}

					// Get Attack Physics
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCharacterInterface::StaticClass()))
					{
						IALSXTCharacterInterface::Execute_GetCombatAttackPhysics(HitActor, HitActorAttackMass, HitActorAttackVelocity);
					}

					TotalImpactEnergy = (HitActorVelocity * HitActorMass) + (HitActorAttackVelocity * HitActorAttackMass);
					// FMath::Square(TossSpeed)
					CurrentHitResult.DoubleHitResult.HitResult.Impulse = HitResult.Normal * TotalImpactEnergy;
					CurrentHitResult.DoubleHitResult.HitResult.HitResult = HitResult;
					GetLocationFromBoneName(CurrentHitResult.DoubleHitResult.HitResult.HitResult.BoneName, ImpactLoc);
					CurrentHitResult.DoubleHitResult.ImpactLocation = ImpactLoc;
					CurrentHitResult.Type = AttackTraceSettings.AttackType;
					GetSideFromHit(CurrentHitResult.DoubleHitResult, ImpactSide);
					CurrentHitResult.DoubleHitResult.ImpactSide = ImpactSide;
					CurrentHitResult.Strength = AttackTraceSettings.AttackStrength;
					GetFormFromHit(CurrentHitResult.DoubleHitResult, ImpactForm);
					HitActor = CurrentHitResult.DoubleHitResult.HitResult.HitResult.GetActor();
					HitActorname = HitActor->GetName();

					// Setup Origin Trace
					FHitResult OriginHitResult;
					OriginTraceIgnoredActors.Add(HitResult.GetActor());	// Add Hit Actor to Origin Trace Ignored Actors

					// Perform Origin Trace
					bool isOriginHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), HitResult.Location, AttackTraceSettings.Start, AttackTraceSettings.Radius, ALSXTSettings->UnarmedCombat.AttackTraceObjectTypes, false, OriginTraceIgnoredActors, EDrawDebugTrace::None, OriginHitResult, true, FLinearColor::Green, FLinearColor::Red, 4.0f);

					// Perform Origin Hit Trace to get PhysMat eyc for ImpactLocation
					if (isOriginHit)
					{
						// Populate Origin Hit
						CurrentHitResult.DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
					
						// Populate Values based if Holding Item
						if (IsHoldingItem())
						{
							GetHeldItemAttackDamageInfo(CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
						}
						else
						{
							GetUnarmedAttackDamageInfo(CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
						}
						FString OriginHitActorname = OriginHitResult.GetActor()->GetName();
						
					}
					// Call OnActorAttackCollision on CollisionInterface
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UCollisionInterface::StaticClass()))
					{
						// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, HitActorname);
						ICollisionInterface::Execute_OnActorAttackCollision(HitActor, CurrentHitResult);
					}
					OnAttackHit(CurrentHitResult);
				}
			}
		}
}

void AALSXTCharacter::EndAttackCollisionTrace()
{
	// Clear Attack Trace Timer
	GetWorld()->GetTimerManager().ClearTimer(AttackTraceTimerHandle);

	// Reset Attack Trace Settings
	AttackTraceSettings.Start = { 0.0f, 0.0f, 0.0f };
	AttackTraceSettings.End = { 0.0f, 0.0f, 0.0f };
	AttackTraceSettings.Radius = { 0.0f };

	// Empty AttackTraceLastHitActors Array
	AttackTraceLastHitActors.Empty();
}

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

// PhysicalAnimationMode

void AALSXTCharacter::SetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName)
{
	FString Tag = NewPhysicalAnimationModeTag.ToString();
	FString ClientRole;
	ClientRole = UEnum::GetValueAsString(GetLocalRole());
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Tag);
	if (DesiredPhysicalAnimationMode != NewPhysicalAnimationModeTag)
	{
		DesiredPhysicalAnimationMode = NewPhysicalAnimationModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredPhysicalAnimationMode, this)

			if (GetLocalRole() == ROLE_Authority)
			{
				ServerSetDesiredPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneName);
			}
			else if (GetLocalRole() >= ROLE_SimulatedProxy)
			{
				SetPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneName);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName)
{
	SetDesiredPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneName);
}

void AALSXTCharacter::SetPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName)
{

	if (PhysicalAnimationMode != NewPhysicalAnimationModeTag)
	{
		const auto PreviousPhysicalAnimationMode{ PhysicalAnimationMode };

		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::None)
		{
			
			GetMesh()->SetCollisionProfileName("CharacterMesh");
			GetMesh()->UpdateCollisionProfile();
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Default", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(30);
			GetMesh()->SetAllBodiesPhysicsBlendWeight(0, false);
			GetMesh()->SetPhysicsBlendWeight(0);
		}
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Bump)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			GetMesh()->UpdateCollisionProfile();
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Bump", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(14);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 1, false, true);
		}
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Hit)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			GetMesh()->UpdateCollisionProfile();
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Hit", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(8);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 1, false, true);
		}

		PhysicalAnimationMode = NewPhysicalAnimationModeTag;

		OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
	}
}

void AALSXTCharacter::OnPhysicalAnimationModeChanged_Implementation(const FGameplayTag& PreviousPhysicalAnimationModeTag) {}

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

// WeaponCarryPosition

void AALSXTCharacter::SetDesiredWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
{
	if (DesiredWeaponCarryPosition != NewWeaponCarryPositionTag)
	{
		DesiredWeaponCarryPosition = NewWeaponCarryPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponCarryPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponCarryPosition(NewWeaponCarryPositionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredWeaponCarryPosition_Implementation(const FGameplayTag& NewWeaponCarryPositionTag)
{
	SetDesiredWeaponCarryPosition(NewWeaponCarryPositionTag);
}

void AALSXTCharacter::SetWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
{

	if (WeaponCarryPosition != NewWeaponCarryPositionTag)
	{
		const auto PreviousWeaponCarryPosition{ WeaponCarryPosition };

		WeaponCarryPosition = NewWeaponCarryPositionTag;

		OnWeaponCarryPositionChanged(PreviousWeaponCarryPosition);
	}
}

void AALSXTCharacter::OnWeaponCarryPositionChanged_Implementation(const FGameplayTag& PreviousWeaponCarryPositionTag) {}

// FirearmSightLocation

void AALSXTCharacter::SetDesiredFirearmSightLocation(const FGameplayTag& NewFirearmSightLocationTag)
{
	if (DesiredFirearmSightLocation != NewFirearmSightLocationTag)
	{
		DesiredFirearmSightLocation = NewFirearmSightLocationTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFirearmSightLocation, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFirearmSightLocation(NewFirearmSightLocationTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredFirearmSightLocation_Implementation(const FGameplayTag& NewFirearmSightLocationTag)
{
	SetDesiredFirearmSightLocation(NewFirearmSightLocationTag);
}

void AALSXTCharacter::SetFirearmSightLocation(const FGameplayTag& NewFirearmSightLocationTag)
{

	if (FirearmSightLocation != NewFirearmSightLocationTag)
	{
		const auto PreviousFirearmSightLocation{ FirearmSightLocation };

		FirearmSightLocation = NewFirearmSightLocationTag;

		OnFirearmSightLocationChanged(PreviousFirearmSightLocation);
	}
}

void AALSXTCharacter::OnFirearmSightLocationChanged_Implementation(const FGameplayTag& PreviousFirearmSightLocationTag) {}

// VaultType

void AALSXTCharacter::SetDesiredVaultType(const FGameplayTag& NewVaultTypeTag)
{
	if (DesiredVaultType != NewVaultTypeTag)
	{
		DesiredVaultType = NewVaultTypeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredVaultType, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredVaultType(NewVaultTypeTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredVaultType_Implementation(const FGameplayTag& NewVaultTypeTag)
{
	SetDesiredVaultType(NewVaultTypeTag);
}

void AALSXTCharacter::SetVaultType(const FGameplayTag& NewVaultTypeTag)
{

	if (VaultType != NewVaultTypeTag)
	{
		const auto PreviousVaultType{ VaultType };

		VaultType = NewVaultTypeTag;

		OnVaultTypeChanged(PreviousVaultType);
	}
}

void AALSXTCharacter::OnVaultTypeChanged_Implementation(const FGameplayTag& PreviousVaultTypeTag) {}

// WeaponObstruction

void AALSXTCharacter::SetDesiredWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
{
	if (DesiredWeaponObstruction != NewWeaponObstructionTag)
	{
		DesiredWeaponObstruction = NewWeaponObstructionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponObstruction, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponObstruction(NewWeaponObstructionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredWeaponObstruction_Implementation(const FGameplayTag& NewWeaponObstructionTag)
{
	SetDesiredWeaponObstruction(NewWeaponObstructionTag);
}

void AALSXTCharacter::SetWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
{

	if (WeaponObstruction != NewWeaponObstructionTag)
	{
		const auto PreviousWeaponObstruction{ WeaponObstruction };

		WeaponObstruction = NewWeaponObstructionTag;

		OnWeaponObstructionChanged(PreviousWeaponObstruction);
	}
}

void AALSXTCharacter::OnWeaponObstructionChanged_Implementation(const FGameplayTag& PreviousWeaponObstructionTag) {}

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