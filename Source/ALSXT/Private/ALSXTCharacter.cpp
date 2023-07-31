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
#include "Settings/ALSXTCombatSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/ALSXTCollisionInterface.h"
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
#include "Kismet/KismetMathLibrary.h"

AALSXTCharacter::AALSXTCharacter()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	AimAnimationHelperBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Aim Animation Helper Box"));
	AimAnimationHelperBox->AttachToComponent(this->RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
	AimAnimationHelperBox->SetBoxExtent(FVector(0.1f, 32.0f, 360.0f), true);
	AimAnimationHelperBox->SetCollisionObjectType(ECC_Visibility);
	AimAnimationHelperBox->SetGenerateOverlapEvents(true);
	AimAnimationHelperBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	AimAnimationHelperBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	AimAnimationHelperBox->bEditableWhenInherited = false;

	ALSXTCharacterMovement = Cast<UALSXTCharacterMovementComponent>(GetCharacterMovement());

	// Add Physical Animation Component
	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation"));
	AddOwnedComponent(PhysicalAnimation);
}

void AALSXTCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	RefreshVaulting();

	FVector Difference = GetActorUpVector() - GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(Difference.X, Difference.Y)) -90;
	if (Angle > 45.00)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), Angle));
	}
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DefensiveModeState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFreelooking, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredSex, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLocomotionVariant, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredInjury, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredCombatStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponFirearmStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponReadyPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredDefensiveMode, Parameters)
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PreviousLookInput, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FreelookState, Parameters)

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MovementInput, Parameters)
}

void AALSXTCharacter::BeginPlay()
{
	AlsCharacter = Cast<AAlsCharacter>(GetParentActor());
	Super::BeginPlay();

	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");
	GetMesh()->SetEnablePhysicsBlending(true);

	FreelookTimerDelegate.BindUFunction(this, "AttackCollisionTrace");
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
		EnhancedInput->BindAction(SwitchForegripPositionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchForegripPosition);
		EnhancedInput->BindAction(SelectEmoteAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectEmote);
		EnhancedInput->BindAction(SelectGestureAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectGesture);
		
		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

void AALSXTCharacter::DisableInputMovement(const bool Disable)
{

}

void AALSXTCharacter::DisableLookAt(const bool Disable)
{

}

void AALSXTCharacter::InputLookMouse(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	PreviousLookInput.Y = Value.Y;
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
	PreviousLookInput.X = Value.X;
}

void AALSXTCharacter::InputLook(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.X * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void AALSXTCharacter::InputMove(const FInputActionValue& ActionValue)
{
	if (GetDesiredStatus() == ALSXTStatusTags::Normal)
	{
		const auto Value{ UAlsMath::ClampMagnitude012D(ActionValue.Get<FVector2D>()) };
		FRotator CapsuleRotation = GetActorRotation();
		const auto ForwardDirection{ UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw)) };
		const auto RightDirection{ UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection) };
		const auto CharForwardDirection{ UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(CapsuleRotation.Yaw)) };
		const auto CharRightDirection{ UAlsMath::PerpendicularCounterClockwiseXY(CharForwardDirection) };

		if (GetDesiredFreelooking() == ALSXTFreelookingTags::True)
		{
			AddMovementInput(CharForwardDirection * Value.Y + CharRightDirection * Value.X);
			MovementInput = CharForwardDirection * Value.Y + CharRightDirection * Value.X;
			GetLocomotionState().PreviousVelocity;
			// AddMovementInput(GetLocomotionState().PreviousVelocity);
			// MovementInput = GetLocomotionState().PreviousVelocity;
		}
		else
		{
			AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
			MovementInput = ForwardDirection * Value.Y + RightDirection * Value.X;
		}
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
			TryStartSliding(1.3f);
		}
		else {
			SetDesiredStance(AlsStanceTags::Crouching);
		}
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
		if (GetDesiredStatus() != ALSXTStatusTags::Normal)
		{
			SetDesiredStatus(ALSXTStatusTags::Normal);
		}
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
			if (GetDesiredStatus() != ALSXTStatusTags::Normal)
			{
				SetDesiredStatus(ALSXTStatusTags::Normal);
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
					SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
				}
			}
			else 
			{
				SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
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
		                       ? AlsRotationModeTags::ViewDirection
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

void AALSXTCharacter::InputSwitchForegripPosition()
{
	if (CanSwitchForegripPosition())
	{
		// SetDesiredHoldingBreath(ActionValue.Get<bool>() ? ALSXTHoldingBreathTags::True : ALSXTHoldingBreathTags::False);
	}
}

void AALSXTCharacter::InputSelectEmote(const FInputActionValue& ActionValue)
{
	if (CanSelectEmote())
	{
		// SetDesiredHoldingBreath(ActionValue.Get<bool>() ? ALSXTHoldingBreathTags::True : ALSXTHoldingBreathTags::False);
	}
}

void AALSXTCharacter::InputSelectGesture(const FInputActionValue& ActionValue)
{
	if (CanSelectGesture())
	{
		// SetDesiredHoldingBreath(ActionValue.Get<bool>() ? ALSXTHoldingBreathTags::True : ALSXTHoldingBreathTags::False);
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

// Vaulting State
void AALSXTCharacter::SetVaultingState(const FALSXTVaultingState& NewVaultingState)
{
	const auto PreviousVaultingState{ VaultingState };

	VaultingState = NewVaultingState;

	OnVaultingStateChanged(PreviousVaultingState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetVaultingState(NewVaultingState);
	}
}

void AALSXTCharacter::ServerSetVaultingState_Implementation(const FALSXTVaultingState& NewVaultingState)
{
	SetVaultingState(NewVaultingState);
}


void AALSXTCharacter::ServerProcessNewVaultingState_Implementation(const FALSXTVaultingState& NewVaultingState)
{
	ProcessNewVaultingState(NewVaultingState);
}

void AALSXTCharacter::OnReplicate_VaultingState(const FALSXTVaultingState& PreviousVaultingState)
{
	OnVaultingStateChanged(PreviousVaultingState);
}

void AALSXTCharacter::OnVaultingStateChanged_Implementation(const FALSXTVaultingState& PreviousVaultingState) {}

// Footprints State
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
	if (CanEnterBlockingDefensiveMode())
	{
		if (ActionValue.Get<bool>() == true)
		{
			FALSXTDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();
			FALSXTDefensiveModeState NewDefensiveModeState = PreviousDefensiveModeState;
			FAnticipationPose NewDefensiveMontage;
			NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::None ? ALSXTDefensiveModeTags::Blocking : PreviousDefensiveModeState.Mode;
			NewDefensiveModeState.Side = PreviousDefensiveModeState.Side == FGameplayTag::EmptyTag ? ALSXTImpactSideTags::Front : PreviousDefensiveModeState.Side;
			NewDefensiveModeState.Form = PreviousDefensiveModeState.Form == FGameplayTag::EmptyTag ? ALSXTImpactFormTags::Blunt : PreviousDefensiveModeState.Form;
			NewDefensiveModeState.Velocity = PreviousDefensiveModeState.Velocity == FGameplayTag::EmptyTag ? ALSXTImpactVelocityTags::Slow : PreviousDefensiveModeState.Velocity;
			NewDefensiveMontage = SelectAttackAnticipationMontage(NewDefensiveModeState.Velocity, GetStance(), NewDefensiveModeState.Side, NewDefensiveModeState.Form);
			NewDefensiveModeState.Montage = NewDefensiveMontage.Pose;
			SetDefensiveModeState(NewDefensiveModeState);
			SetDesiredDefensiveMode(ALSXTDefensiveModeTags::Blocking);
		}
		else 
		{
			ResetDefensiveModeState();
			SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
		}
	}
	else if ((DesiredDefensiveMode == ALSXTDefensiveModeTags::Blocking) && (ActionValue.Get<bool>()  == false))
	{
		ResetDefensiveModeState();
		SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
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

bool AALSXTCharacter::CanPerformPrimaryAction_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanPerformSecondaryAction_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanPerformPrimaryInteraction_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanPerformSecondaryInteraction_Implementation() const
{
	return true;
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
	// LockRotation(GetActorRotation().Yaw);
	SetDesiredFreelooking(ALSXTFreelookingTags::True);
}

void AALSXTCharacter::DeactivateFreelooking()
{
	// UnLockRotation();
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

		if (NewFreelookingTag == ALSXTFreelookingTags::True)
		{
			// FreelookTimerHandle.IsValid()
			BeginFreelookTimer();
		}
		if (NewFreelookingTag == ALSXTFreelookingTags::False)
		{
			EndFreelookTimer();
		}

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
					SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
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

// DefensiveMode

bool AALSXTCharacter::IsInDefensiveMode() const
{
	if (GetDefensiveMode() != ALSXTDefensiveModeTags::None)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool AALSXTCharacter::IsInAnticipationMode() const
{
	if (GetDefensiveMode() == ALSXTDefensiveModeTags::Anticipation)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AALSXTCharacter::SetDesiredDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
{
	if (DesiredDefensiveMode != NewDefensiveModeTag)
	{
		DesiredDefensiveMode = NewDefensiveModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredDefensiveMode, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredDefensiveMode(NewDefensiveModeTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredDefensiveMode_Implementation(const FGameplayTag& NewDefensiveModeTag)
{
	SetDesiredDefensiveMode(NewDefensiveModeTag);
}

void AALSXTCharacter::SetDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
{
	if (DefensiveMode != NewDefensiveModeTag)
	{
		const auto PreviousDefensiveMode{ DefensiveMode };

		DefensiveMode = NewDefensiveModeTag;

		if (DefensiveMode == ALSXTDefensiveModeTags::Anticipation)
		{

		}
		if (DefensiveMode == ALSXTDefensiveModeTags::Blocking)
		{

		}
		OnDefensiveModeChanged(PreviousDefensiveMode);
	}
}

void AALSXTCharacter::OnDefensiveModeChanged_Implementation(const FGameplayTag& PreviousDefensiveModeTag) {}

void AALSXTCharacter::SetAimState(const FALSXTAimState& NewAimState)
{
	const auto PreviousAimState{ AimState };

	AimState = NewAimState;

	OnAimStateChanged(PreviousAimState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetAimState(NewAimState);
	}
}

void AALSXTCharacter::ServerSetAimState_Implementation(const FALSXTAimState& NewAimState)
{
	SetAimState(NewAimState);
}


void AALSXTCharacter::ServerProcessNewAimState_Implementation(const FALSXTAimState& NewAimState)
{
	ProcessNewAimState(NewAimState);
}

void AALSXTCharacter::OnReplicate_AimState(const FALSXTAimState& PreviousAimState)
{
	OnAimStateChanged(PreviousAimState);
}

void AALSXTCharacter::OnAimStateChanged_Implementation(const FALSXTAimState& PreviousAimState) {}

void AALSXTCharacter::SetFreelookState(const FALSXTFreelookState& NewFreelookState)
{
	const auto PreviousFreelookState{ FreelookState };

	FreelookState = NewFreelookState;

	OnFreelookStateChanged(PreviousFreelookState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetFreelookState(NewFreelookState);
	}
}

void AALSXTCharacter::ServerSetFreelookState_Implementation(const FALSXTFreelookState& NewFreelookState)
{
	SetFreelookState(NewFreelookState);
}


void AALSXTCharacter::ServerProcessNewFreelookState_Implementation(const FALSXTFreelookState& NewFreelookState)
{
	ProcessNewFreelookState(NewFreelookState);
}

void AALSXTCharacter::OnReplicate_FreelookState(const FALSXTFreelookState& PreviousFreelookState)
{
	OnFreelookStateChanged(PreviousFreelookState);
}

void AALSXTCharacter::OnFreelookStateChanged_Implementation(const FALSXTFreelookState& PreviousFreelookState) {}

void AALSXTCharacter::SetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	const auto PreviousDefensiveModeState{ DefensiveModeState };

	DefensiveModeState = NewDefensiveModeState;
	// ServerSetDefensiveModeState(NewDefensiveModeState);

	OnDefensiveModeStateChanged(PreviousDefensiveModeState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetDefensiveModeState(NewDefensiveModeState);
	}
}

void AALSXTCharacter::ResetDefensiveModeState()
{
	FALSXTDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();
	FALSXTDefensiveModeState NewDefensiveModeState = PreviousDefensiveModeState;
	NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::None;
	NewDefensiveModeState.Side = FGameplayTag::EmptyTag;
	NewDefensiveModeState.Form = FGameplayTag::EmptyTag;
	NewDefensiveModeState.Velocity = FGameplayTag::EmptyTag;
	NewDefensiveModeState.Location = FVector::ZeroVector;
	// NewDefensiveModeState.Montage = nullptr;

	DefensiveModeState = NewDefensiveModeState;

	OnDefensiveModeStateChanged(PreviousDefensiveModeState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetDefensiveModeState(NewDefensiveModeState);
	}
}

void AALSXTCharacter::ServerSetDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	SetDefensiveModeState(NewDefensiveModeState);
}


void AALSXTCharacter::ServerProcessNewDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	ProcessNewDefensiveModeState(NewDefensiveModeState);
}

void AALSXTCharacter::OnReplicate_DefensiveModeState(const FALSXTDefensiveModeState& PreviousDefensiveModeState)
{
	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
}

void AALSXTCharacter::OnDefensiveModeStateChanged_Implementation(const FALSXTDefensiveModeState& PreviousDefensiveModeState) {}

// Blocking

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
			else if (GetLocalRole() == ROLE_SimulatedProxy && GetRemoteRole() == ROLE_Authority)
			{
				// MulticastSetDesiredStatus(NewStatusTag);
				// ServerSetDesiredStatus(NewStatusTag);
				SetStatus(NewStatusTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	// SetDesiredStatus(NewStatusTag);
	MulticastSetDesiredStatus(NewStatusTag);
}

void AALSXTCharacter::MulticastSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	SetStatus(NewStatusTag);
}

void AALSXTCharacter::SetStatus(const FGameplayTag& NewStatusTag)
{

	if (Status != NewStatusTag)
	{
		const auto PreviousStatus{ Status };

		Status = NewStatusTag;

		if (NewStatusTag != ALSXTStatusTags::Normal)
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Neutral);
			SetDesiredStance(AlsStanceTags::Crouching);
			Crouch();
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
		else
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			SetDesiredStance(AlsStanceTags::Standing);
			UnCrouch();
			IALSXTCharacterInterface::Execute_TryGetUp(this);
		}

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
	Side = ALSXTImpactSideTags::Left;
}

void AALSXTCharacter::GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength)
{
	float HitMass = Hit.HitResult.Mass;
	FVector HitVelocity = Hit.HitResult.Velocity;
	FVector HitMomentum = HitMass * HitVelocity;

	float SelfMass = Hit.OriginHitResult.Mass;
	FVector SelfVelocity = Hit.OriginHitResult.Velocity;
	FVector SelfMomentum = SelfMass * SelfVelocity;

	FVector MomemtumSum = HitMomentum + SelfMomentum;

	Strength = ALSXTActionStrengthTags::Light;
}

void AALSXTCharacter::BeginFreelookTimer()
{
	if (ALSXTSettings->Freelook.bEnableFreelook)
	{
		GetWorld()->GetTimerManager().SetTimer(FreelookTimerHandle, FreelookTimerDelegate, 0.1f, true);
	}
}

void AALSXTCharacter::FreelookTimer()
{
	FGameplayTag CurrentViewMode = GetViewMode();
	FALSXTFreelookState NewState;

	if (CurrentViewMode == AlsViewModeTags::FirstPerson && ALSXTSettings->Freelook.bEnableFirstPersonFreelook)
	{
		FVector2D FirstPersonYawRange = ALSXTSettings->Freelook.FirstPersonYawRange;
		FVector2D FirstPersonPitchRange = ALSXTSettings->Freelook.FirstPersonPitchRange;
		FMinimalViewInfo ViewInfo;
		Camera->GetViewInfo(ViewInfo);
		FVector CurrentViewTarget = ViewInfo.Location + (Camera->GetForwardVector() * 1000.0);
		FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetMesh()->GetSocketTransform("head", ERelativeTransformSpace::RTS_Component), CurrentViewTarget);
		LookAtRotation.Yaw = FMath::Clamp(LookAtRotation.Yaw, FirstPersonYawRange.X, FirstPersonYawRange.Y);
		LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch, FirstPersonPitchRange.X, FirstPersonPitchRange.Y);
		NewState.LockedPitchAngle = LookAtRotation.Pitch;
		NewState.LockedYawAngle = LookAtRotation.Yaw;
		NewState.LockedControlRotation = LookAtRotation;
		SetFreelookState(NewState);
	}
	if (CurrentViewMode == AlsViewModeTags::ThirdPerson && ALSXTSettings->Freelook.bEnableThirdPersonFreelook)
	{
		FVector2D ThirdPersonYawRange = ALSXTSettings->Freelook.ThirdPersonYawRange;
		FVector2D ThirdPersonPitchRange = ALSXTSettings->Freelook.ThirdPersonPitchRange;
		FMinimalViewInfo ViewInfo;
		Camera->GetViewInfo(ViewInfo);
		FVector CurrentViewTarget = ViewInfo.Location + (Camera->GetForwardVector() * 1000.0);
		FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetMesh()->GetSocketTransform("head", ERelativeTransformSpace::RTS_Component), CurrentViewTarget);
		LookAtRotation.Yaw = FMath::Clamp(LookAtRotation.Yaw, ThirdPersonYawRange.X, ThirdPersonYawRange.Y);
		LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch, ThirdPersonPitchRange.X, ThirdPersonPitchRange.Y);
		NewState.LockedPitchAngle = LookAtRotation.Pitch;
		NewState.LockedYawAngle = LookAtRotation.Yaw;
		NewState.LockedControlRotation = LookAtRotation;
		SetFreelookState(NewState);
	}
}

void AALSXTCharacter::EndFreelookTimer()
{
	// Clear Attack Trace Timer
	FALSXTFreelookState EmptyState;
	SetFreelookState(EmptyState);
	GetWorld()->GetTimerManager().ClearTimer(FreelookTimerHandle);
}

void AALSXTCharacter::BeginAttackCollisionTrace(FALSXTCombatAttackTraceSettings TraceSettings)
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

		FALSXTGeneralCombatSettings GeneralCombatSettings = IALSXTCombatInterface::Execute_GetGeneralCombatSettings(this);
		TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;
		AttackTraceObjectTypes = GeneralCombatSettings.AttackTraceObjectTypes;

		// Initial Trace
		bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), AttackTraceSettings.Start, AttackTraceSettings.End, AttackTraceSettings.Radius, AttackTraceObjectTypes, false, InitialIgnoredActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Green, FLinearColor::Red, 0.0f);

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
					FGameplayTag ImpactStrength;
					FGameplayTag ImpactSide;
					FGameplayTag ImpactForm;
					AActor* HitActor = HitResult.GetActor();
					FString HitActorname;
					FVector HitActorVelocity { FVector::ZeroVector };
					float HitActorMass { 0.0f };
					float HitActorAttackVelocity { 0.0f };
					float HitActorAttackMass { 0.0f };
					FVector TotalImpactEnergy { FVector::ZeroVector };

					// Populate Hit
					// 
					
					// Call OnActorAttackCollision on CollisionInterface
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCollisionInterface::StaticClass()))
					{
						IALSXTCollisionInterface::Execute_GetActorVelocity(HitActor, HitActorVelocity);
						IALSXTCollisionInterface::Execute_GetActorMass(HitActor, HitActorMass);
					}

					// Get Attack Physics
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCharacterInterface::StaticClass()))
					{
						IALSXTCharacterInterface::Execute_GetCombatAttackPhysics(HitActor, HitActorAttackMass, HitActorAttackVelocity);
					}

					// TotalImpactEnergy = 50.0f + (HitActorVelocity * HitActorMass) + (HitActorAttackVelocity * HitActorAttackMass);
					TotalImpactEnergy = (HitActorVelocity * HitActorMass) + (HitActorAttackVelocity * HitActorAttackMass);
					// FMath::Square(TossSpeed)

					FVector HitDirection = HitResult.ImpactPoint - GetActorLocation();
					HitDirection.Normalize();
					CurrentHitResult.DoubleHitResult.HitResult.Direction = HitDirection;
					CurrentHitResult.DoubleHitResult.HitResult.Impulse = HitResult.Normal * TotalImpactEnergy;
					CurrentHitResult.DoubleHitResult.HitResult.HitResult = HitResult;
					GetLocationFromBoneName(CurrentHitResult.DoubleHitResult.HitResult.HitResult.BoneName, ImpactLoc);
					CurrentHitResult.DoubleHitResult.HitResult.ImpactLocation = ImpactLoc;
					CurrentHitResult.Type = AttackTraceSettings.AttackType;				
					CurrentHitResult.DoubleHitResult.HitResult.ImpactSide = ImpactSide;
					CurrentHitResult.Strength = AttackTraceSettings.AttackStrength;
					CurrentHitResult.DoubleHitResult.HitResult.ImpactStrength = AttackTraceSettings.AttackStrength;					
					HitActor = CurrentHitResult.DoubleHitResult.HitResult.HitResult.GetActor();
					HitActorname = HitActor->GetName();


					// Setup Origin Trace
					FHitResult OriginHitResult;
					OriginTraceIgnoredActors.Add(HitResult.GetActor());	// Add Hit Actor to Origin Trace Ignored Actors

					// Perform Origin Trace
					bool isOriginHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), HitResult.Location, AttackTraceSettings.Start, AttackTraceSettings.Radius, AttackTraceObjectTypes, false, OriginTraceIgnoredActors, EDrawDebugTrace::None, OriginHitResult, true, FLinearColor::Green, FLinearColor::Red, 4.0f);

					// Perform Origin Hit Trace to get PhysMat etc for ImpactLocation
					if (isOriginHit)
					{
						// Populate Origin Hit
						CurrentHitResult.DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
					
						// Populate Values based if Holding Item
						if (IsHoldingItem())
						{
							GetHeldItemAttackDamageInfo(CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.HitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
						}
						else
						{
							GetUnarmedAttackDamageInfo(CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.HitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
						}
						FString OriginHitActorname = OriginHitResult.GetActor()->GetName();
						CurrentHitResult.DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;

						GetFormFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.DoubleHitResult.ImpactForm);
						GetSideFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.DoubleHitResult.ImpactSide);
						GetStrengthFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.Strength);
						CurrentHitResult.DoubleHitResult.HitResult.ImpactStrength = CurrentHitResult.Strength;
					}
					// Call OnActorAttackCollision on CollisionInterface
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCharacterInterface::StaticClass()))
					{
						IALSXTCharacterInterface::Execute_AttackReaction(HitActor, CurrentHitResult);
					}
					// Call OnActorAttackCollision on CollisionInterface
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCollisionInterface::StaticClass()))
					{
						// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, HitActorname);
						IALSXTCollisionInterface::Execute_OnActorAttackCollision(HitActor, CurrentHitResult);
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
			// GetMesh()->UpdateCollisionProfile();
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Default", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(30);
			GetMesh()->SetAllBodiesSimulatePhysics(false);
			GetMesh()->ResetAllBodiesSimulatePhysics();
			GetMesh()->SetAllBodiesPhysicsBlendWeight(0.0f, false);
			GetMesh()->SetPhysicsBlendWeight(0);
			// GetMesh()->SetAllBodiesSimulatePhysics(false);
		}
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Bump)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			// GetMesh()->UpdateCollisionProfile();
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Bump", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(14);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);
		}
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Hit)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Hit", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(8);
			GetMesh()->WakeAllRigidBodies();			
			// GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);			
			
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

// ForegripPosition

void AALSXTCharacter::SetDesiredForegripPosition(const FGameplayTag& NewForegripPositionTag)
{
	if (DesiredForegripPosition != NewForegripPositionTag)
	{
		DesiredForegripPosition = NewForegripPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredForegripPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredForegripPosition(NewForegripPositionTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredForegripPosition_Implementation(const FGameplayTag& NewForegripPositionTag)
{
	SetDesiredForegripPosition(NewForegripPositionTag);
}

void AALSXTCharacter::SetForegripPosition(const FGameplayTag& NewForegripPositionTag)
{

	if (ForegripPosition != NewForegripPositionTag)
	{
		const auto PreviousForegripPosition{ ForegripPosition };

		ForegripPosition = NewForegripPositionTag;

		OnForegripPositionChanged(PreviousForegripPosition);
	}
}

void AALSXTCharacter::OnForegripPositionChanged_Implementation(const FGameplayTag& PreviousForegripPositionTag) {}

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