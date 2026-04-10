// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).


#include "AlsxtCharacterPlayer.h"

#include "AlsxtCharacterNpc.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Utility/AlsVector.h"


/**
* @file AlsxtCharacterPlayer.cpp
* @brief template class that contains all shared Logic and Data for Player Classes.
* AAlsxtCharacterPlayer depends on AAlsxtPlayerState and AAlsxtPlayerController to function.
* Create a Blueprint class based on this class, do not use the C++ class directly in the Editor
*/

void AAlsxtCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	
	// Set a Soft Object Pointer to the Player State
	if (const APlayerController* CurrentPlayerController = Cast<APlayerController>(GetController()))
	{
		if (CurrentPlayerController->PlayerState)
		{
			PlayerState = CurrentPlayerController->PlayerState;
		}
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CurrentPlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // Add the context
			}
		}
	}	
}

void AAlsxtCharacterPlayer::SetupPlayerInputComponent(UInputComponent* Input)
{
	// Super::SetupPlayerInputComponent(Input);

	EnhancedInput = Cast<UEnhancedInputComponent>(Input);
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnMoveReleased);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnCrouch);
		EnhancedInput->BindAction(ProneAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnProne);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(MantleAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMantle);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnAim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnAimReleased);
		EnhancedInput->BindAction(AimToggleAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnToggleAim);
		EnhancedInput->BindAction(FocusAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnFocus);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnRollReleased);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchShoulder);
		EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLeanLeft);
		EnhancedInput->BindAction(ToggleLeanLeftAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLeanLeft);
		EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLeanRight);
		EnhancedInput->BindAction(ToggleLeanRightAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLeanRight);
		EnhancedInput->BindAction(FreelookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnFreelook);
		EnhancedInput->BindAction(ToggleFreelookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnFreelook);
		EnhancedInput->BindAction(ToggleGaitAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnToggleGait);
		EnhancedInput->BindAction(SwitchCombatStanceAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchCombatStance);
		EnhancedInput->BindAction(ToggleCombatReadyAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnToggleReadiness);
		EnhancedInput->BindAction(PrimaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnPrimaryInteraction);
		EnhancedInput->BindAction(SecondaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSecondaryInteraction);
		EnhancedInput->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnBlock);
		EnhancedInput->BindAction(SwitchWeaponReadyPositionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchWeaponReadyPosition);
		EnhancedInput->BindAction(SwitchGripPositionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchGripPosition);
		EnhancedInput->BindAction(SwitchForegripPositionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchForegripPosition);

		InputMantleValue = EnhancedInput->GetBoundActionValue(MantleAction);
		
		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

// Server only
void AAlsxtCharacterPlayer::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	// AAlsxtPlayerState* PS = GetPlayerState<AAlsxtPlayerState>();
	// if (PS)
	// {
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		// AbilitySystemComponent = Cast<UAlsxtAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		// PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		// AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		// InitializeAttributes();

		
		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		// AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		// SetHealth(GetMaxHealth());
		// SetMana(GetMaxMana());
		// SetStamina(GetMaxStamina());

		// End respawn specific things


		// AddStartupEffects();

		// AddCharacterAbilities();

		// AGDPlayerController* PC = Cast<AGDPlayerController>(GetController());
		// if (PC)
		// {
		// 	PC->CreateHUD();
		// }
// 
		// InitializeFloatingStatusBar();
	// }
}

UAbilitySystemComponent* AAlsxtCharacterPlayer::GetAbilitySystemComponent() const
{
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetPlayerState()))
	{
		return AbilitySystemInterface->GetAbilitySystemComponent();
	}
	else
	{
		return nullptr;
	}
}

bool AAlsxtCharacterPlayer::GetCharacterIsCameraRightShoulder_Implementation() const
{
	return IsCameraRightShoulder();
}

void AAlsxtCharacterPlayer::SetCharacterCameraRightShoulder_Implementation(const bool NewCameraRightShoulder)
{
	SetCameraRightShoulder(NewCameraRightShoulder);
}

void AAlsxtCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AAlsxtPlayerState* PS = GetPlayerState<AAlsxtPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void AAlsxtCharacterPlayer::Input_OnLookMouse(const FInputActionValue& ActionValue)
{
	const FVector2f Value{ActionValue.Get<FVector2D>()};

	if (GetViewMode() == AlsViewModeTags::ThirdPerson && ALSXTSettings->ViewSettings.IsValid())
	{
		if (ALSXTSettings->ViewSettings.Get()->bClampControlRotation)
		{
			AddControllerPitchInput(FMath::Clamp(Value.Y, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MinValue, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MaxValue) * LookUpMouseSensitivity);
			AddControllerYawInput(FMath::Clamp(Value.X, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MinValue, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MaxValue) * LookRightMouseSensitivity);
		}
	}
	else
	{
		AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
		AddControllerYawInput(Value.X * LookRightMouseSensitivity);
	}
}

void AAlsxtCharacterPlayer::Input_OnLook(const FInputActionValue& ActionValue)
{
	const FVector2f Value{ActionValue.Get<FVector2D>()};

	// AddControllerPitchInput(Value.Y * LookUpRate);
	// AddControllerYawInput(Value.X * LookRightRate);

	if (GetViewMode() == AlsViewModeTags::ThirdPerson && ALSXTSettings->ViewSettings.IsValid())
	{
		if (ALSXTSettings->ViewSettings.Get()->bClampControlRotation)
		{
			AddControllerPitchInput(FMath::Clamp(Value.Y, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MinValue, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MaxValue) * LookUpMouseSensitivity);
			AddControllerYawInput(FMath::Clamp(Value.X, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MinValue, ALSXTSettings->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MaxValue) * LookRightMouseSensitivity);
		}
	}
	else
	{
		AddControllerPitchInput(Value.Y * LookUpRate);
		AddControllerYawInput(Value.X * LookRightRate);
	}
}

void AAlsxtCharacterPlayer::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>())};
	auto ViewRotation{GetViewState().Rotation};

	if (IsValid(GetController()))
	{
		// Use exact camera rotation instead of target rotation whenever possible.
		FVector ViewLocation;
		GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	// const auto ForwardDirection{UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(ViewRotation.Yaw))};
	// const auto RightDirection{UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection)};
	
	FGameplayTagContainer AbilityTags;

	/**
	 * @todo Create Data/DB/Settings for All Tags Associated to Gameplay Abilities
	 **/
	
	FGameplayTag MovementTag {AlsxtGASGameplayTags::State::TAG_State_MovementInput};
	AbilityTags.AddTag(MovementTag);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.Movement")));
	
	const FRotator MovementRotation = GetFreelookState().bFreelooking ? 
			GetFreelookState().LockedControlRotation : 
			FRotator(0, GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y);

	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTags);
	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}

void AAlsxtCharacterPlayer::Input_OnMoveReleased()
{
	FGameplayTagContainer AbilityTags;
	FGameplayTag MovementTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Movement"));
	AbilityTags.AddTagFast(MovementTag);
	GetAbilitySystemComponent()->CancelAbilities(&AbilityTags, nullptr, nullptr);
}

void AAlsxtCharacterPlayer::Input_OnSprint(const FInputActionValue& ActionValue)
{
	if (GetLocomotionState().Velocity.Length() > KINDA_SMALL_NUMBER && GetLocomotionState().bMoving && GetLocomotionState().bHasInput)
	{
		if (GetAbilitySystemComponent())
		{
			FGameplayTagContainer SprintGameplayTags;
			SprintGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.Sprint")));
			if (ActionValue.Get<bool>() && GetAbilitySystemComponent()->TryActivateAbilitiesByTag(SprintGameplayTags, true))
			{
				// OnSprint Event Dispatcher
			}
			else
			{
				GetAbilitySystemComponent()->CancelAbilities(&SprintGameplayTags);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("On Sprint: GetAbilitySystemComponent failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Sprint: Input Vector: %s"), *GetLocomotionState().Velocity.ToString());
	}
}

void AAlsxtCharacterPlayer::Input_OnWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayTag RunningTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Running"));
        
			if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(RunningTag)))
			{
				SetDesiredGait(AlsGaitTags::Running);			
			}
			else
			{
				if (GEngine)
				{
					FString ClassName = this->GetClass()->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Ability with Tag Gameplay.Ability.Running could NOT be Activated in %s"), *ClassName));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				FString ClassName = this->GetClass()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AbilitySystemComponent NOT found in %s"), *ClassName));
			}
		}
	}
	else
	{
		if ((GetDesiredGait() == AlsGaitTags::Running) || (GetDesiredGait() == AlsGaitTags::Combat))
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			FGameplayTag RunningTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Running"));
			FGameplayTagContainer CancelTags;
			CancelTags.AddTag(RunningTag);
			if (ASC)
			{
				ASC->CancelAbilities(&CancelTags);
				SetDesiredGait(AlsGaitTags::Walking);
			}
			if (GEngine)
			{
				FString ClassName = this->GetClass()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AbilitySystemComponent NOT found in %s"), *ClassName));
			}
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnCrouch()
{
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayTag CrouchingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Crouching"));
        
			if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(CrouchingTag)))
			{
				UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    
				// Get MinForwardVelocity for Sliding
				// float MinSlideVelocity = GetSettings()->Sliding.MinForwardVelocity; // Example
				float MinSlideVelocity = 600.0f; // Replace with your variable from AlsSettings

				// Calculate forward velocity
				float ForwardVelocity = FVector::DotProduct(GetVelocity(), GetActorForwardVector());

				if (CanSlide() && (ForwardVelocity >= MinSlideVelocity))
				{
					FGameplayTag SlidingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Slide"));
					ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(SlidingTag));

					// TryStartSliding(1.3f);
				}
				else {
					SetDesiredStance(AlsStanceTags::Crouching);
				}
			}
			else
			{
				if (GEngine)
				{
					FString ClassName = this->GetClass()->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Ability with Tag Gameplay.Ability.Crouching could NOT be Activated in %s"), *ClassName));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				FString ClassName = this->GetClass()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AbilitySystemComponent NOT found in %s"), *ClassName));
			}
		}
	}
	else
	{
		if (GetDesiredStance() == AlsStanceTags::Crouching)
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			FGameplayTag CrouchingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Crouching"));
			FGameplayTagContainer CancelTags;
			CancelTags.AddTag(CrouchingTag);
			if (ASC)
			{
				ASC->CancelAbilities(&CancelTags);
				SetDesiredStance(AlsStanceTags::Standing);	
			}
			if (GEngine)
			{
				FString ClassName = this->GetClass()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AbilitySystemComponent NOT found in %s"), *ClassName));
			}
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnProne()
{
	if (GetDesiredStance() == AlsStanceTags::Prone)
	{
		SetDesiredStance(AlsStanceTags::Crouching);
	}
	else if (CanProne())
	{
		SetDesiredStance(AlsStanceTags::Prone);
	}
}

void AAlsxtCharacterPlayer::Input_OnJump(const FInputActionValue& ActionValue)
{
	// New Code
	if (ActionValue.Get<bool>())
	{
		if (StopRagdolling())
		{
			return;
		}

		if (StartMantlingGrounded())
		{
			return;
		}

		// Try StartMantlingGrounded
		if (GetAbilitySystemComponent())
		{
			FGameplayTagContainer MantlingGameplayTags;
			MantlingGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.Mantle")));
			if (ActionValue.Get<bool>() && GetAbilitySystemComponent()->TryActivateAbilitiesByTag(MantlingGameplayTags, true) && StartMantlingGrounded())
			{
				// OnMantle Event Dispatcher
			}
			else
			{
				GetAbilitySystemComponent()->CancelAbilities(&MantlingGameplayTags);
				// UE_LOG(LogTemp, Warning, TEXT("OnJump: TryActivateAbilitiesByTag Gameplay.Ability.Mantle failed!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OnJump: GetAbilitySystemComponent failed!"));
		}

		if (GetStance() == AlsStanceTags::Crouching)
		{
			SetDesiredStance(AlsStanceTags::Standing);
			return;
		}

		// Try to Jump
		if (GetAbilitySystemComponent())
		{
			FGameplayTagContainer JumpGameplayTags;
			JumpGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.Jump")));
			if (ActionValue.Get<bool>() && GetAbilitySystemComponent()->TryActivateAbilitiesByTag(JumpGameplayTags, true))
			{
				// OnJumpEvent Dispatcher
				UE_LOG(LogTemp, Warning, TEXT("OnJump: activated!"));
			}
			else
			{
				GetAbilitySystemComponent()->CancelAbilities(&JumpGameplayTags);
				// UE_LOG(LogTemp, Warning, TEXT("OnJump: TryActivateAbilitiesByTag Gameplay.Ability.Jump failed!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OnJump: GetAbilitySystemComponent failed!"));
		}
	}
	else
	{
		StopJumping();
	}

	// Legacy Code
	// if (ActionValue.Get<bool>())
	// {
	// 	if (StopRagdolling())
	// 	{
	// 		return;
	// 	}
// 
	// 	if (StartMantlingGrounded())
	// 	{
	// 		return;
	// 	}
// 
	// 	if (GetStance() == AlsStanceTags::Crouching)
	// 	{
	// 		SetDesiredStance(AlsStanceTags::Standing);
	// 		return;
	// 	}
// 
	// 	Jump();
	// }
	// else
	// {
	// 	StopJumping();
	// }
}

void AAlsxtCharacterPlayer::Input_OnAim(const FInputActionValue& ActionValue)
{
	// if (GetAbilitySystemComponent())
	// {
	// 	FGameplayTag AimTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Aim"));
	// 	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(AimTag));
	// }

	if (CanAim())
	{
		FGameplayTagContainer AimTagContainer;
		AimTagContainer.AddTagFast(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Aim")));
		// ActionValue.Get<bool>() ? GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FocusTagContainer) : GetAbilitySystemComponent()->CancelAbilities(&FocusTagContainer);
		if (ActionValue.Get<bool>())
		{
			// SetDesiredFocus(ALSXTFocusedTags::True);
			GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AimTagContainer);
		}
		else
		{
			// SetDesiredFocus(ALSXTFocusedTags::False);
			GetAbilitySystemComponent()->CancelAbilities(&AimTagContainer);
		}
	}

	// SetDesiredAiming(ActionValue.Get<bool>());
}

void AAlsxtCharacterPlayer::Input_OnAimReleased()
{
	FGameplayTag AimTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Aim"));
	FGameplayTagContainer AimGameplayTags;
	AimGameplayTags.AddTagFast(AimTag);
	GetAbilitySystemComponent()->CancelAbilities(&AimGameplayTags);
}

void AAlsxtCharacterPlayer::Input_OnRagdoll()
{
	if (!StopRagdolling())
	{
		StartRagdolling();
	}
}

void AAlsxtCharacterPlayer::Input_OnRoll()
{
	static constexpr auto PlayRate{1.3f};

	if (GetAbilitySystemComponent())
	{
		FGameplayTag RollingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Roll"));
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(RollingTag));
        
		// if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(RollingTag)))
		// {
		// 	StartRolling(PlayRate);
		// }
	}
}

void AAlsxtCharacterPlayer::Input_OnRollReleased()
{
	FGameplayTag RollingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Roll"));
	FGameplayTagContainer RollingGameplayTags;
	RollingGameplayTags.AddTagFast(RollingTag);
	GetAbilitySystemComponent()->CancelAbilities(&RollingGameplayTags);
}

void AAlsxtCharacterPlayer::Input_OnRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
							   ? AlsRotationModeTags::ViewDirection
							   : AlsRotationModeTags::VelocityDirection);
}

void AAlsxtCharacterPlayer::Input_OnViewMode()
{

	if (GetViewMode() == AlsViewModeTags::FirstPerson)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayTag ThirdPersonTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.ThirdPerson"));
            
			if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(ThirdPersonTag)))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Input_OnViewMode: Running Ability Activated")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Input_OnViewMode: Running Ability Could NOT be Activated!")));
			}
		}
		else
		{
			if (GEngine)
			{
				FString ClassName = this->GetClass()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Input_OnViewMode: AbilitySystemComponent NOT found in %s"), *ClassName));
			}
		}
	}
	else
	{
		if (GetViewMode() == AlsViewModeTags::ThirdPerson)
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			if (ASC)
			{
				FGameplayTag ThirdPersonTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.ThirdPerson"));
				FGameplayTagContainer CancelTags;
				CancelTags.AddTag(ThirdPersonTag);
				ASC->CancelAbilities(&CancelTags);
				if (GEngine)
				{
					FString ClassName = this->GetClass()->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Input_OnWalk: Cancel Third Person Ability in %s"), *ClassName));
				}
			}
			else
			{
				SetViewMode(AlsViewModeTags::FirstPerson);

				if (GEngine)
				{
					FString ClassName = this->GetClass()->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Input_OnWalk: AbilitySystemComponent NOT found in %s"), *ClassName));
				}
			}
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnSwitchShoulder()
{
	// Camera->SetRightShoulder(!Camera->IsRightShoulder());
	SetCameraRightShoulder(!IsCameraRightShoulder());
}

void AAlsxtCharacterPlayer::Input_OnMantle(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		// RefreshMantle();
	}
	else
	{

	}
}

void AAlsxtCharacterPlayer::Input_OnToggleAim()
{
	if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this)) 
	{
		if (GetDesiredRotationMode() == AlsRotationModeTags::Aiming)
		{
			SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
			SetDesiredReadyStance(AlsxtReadinessTags::Ready);
		}
		if (CanAim())
		{
			SetViewMode(AlsViewModeTags::FirstPerson);
			SetDesiredReadyStance(AlsxtReadinessTags::Aiming);
			SetDesiredRotationMode(AlsRotationModeTags::Aiming);
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnFocus(const FInputActionValue& ActionValue)
{
	if (CanFocus())
	{
		FGameplayTagContainer FocusTagContainer;
		FocusTagContainer.AddTagFast(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Focus")));
		// ActionValue.Get<bool>() ? GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FocusTagContainer) : GetAbilitySystemComponent()->CancelAbilities(&FocusTagContainer);
		if (ActionValue.Get<bool>())
		{
			// SetDesiredFocus(ALSXTFocusedTags::True);
			GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FocusTagContainer);
		}
		else
		{
			// SetDesiredFocus(ALSXTFocusedTags::False);
			GetAbilitySystemComponent()->CancelAbilities(&FocusTagContainer);
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnFreelook(const FInputActionValue& ActionValue)
{
	if (CanFreelook())
	{
		FGameplayTagContainer FreelookTagContainer;
		FreelookTagContainer.AddTagFast(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Freelook")));
		FAlsxtFreelookState NewFreelookState = GetFreelookState();
		if (ActionValue.Get<bool>())
		{
			GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FreelookTagContainer);
			// SetDesiredFreelooking(ALSXTFreelookingTags::True);
		}
		else
		{
			GetAbilitySystemComponent()->CancelAbilities(&FreelookTagContainer);
			// SetDesiredFreelooking(ALSXTFreelookingTags::False);
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnToggleGait()
{
	if (CanToggleGait())
	{
		if ((GetDesiredGait() == AlsGaitTags::Walking))
		{
			SetDesiredGait(AlsGaitTags::Running);
		}
		else
		{
			SetDesiredGait(AlsGaitTags::Walking);
		}
	}
}

// Input Action to Switch between Relaxed and Ready Stances
void AAlsxtCharacterPlayer::Input_OnToggleReadiness()
{
	if (CanToggleCombatReady())
	{
		if ((GetDesiredReadyStance() == FGameplayTag::EmptyTag) || (GetDesiredReadyStance() == AlsxtReadinessTags::Relaxed))
		{
			if (GetAbilitySystemComponent() && CanBecomeCombatReady())
			{
				if (GetAbilitySystemComponent())
				{
					FGameplayTagContainer ReadinessTags;
					ReadinessTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Readiness")));
					GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ReadinessTags);
				}
				SetDesiredReadyStance(AlsxtReadinessTags::Ready);
				if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this))
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
			if (GetDesiredReadyStance() != AlsxtReadinessTags::Relaxed)
			{
				UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
				FGameplayTag ReadinessTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Readiness"));
				FGameplayTagContainer CancelTags;
				CancelTags.AddTag(ReadinessTag);
				if (ASC)
				{
					ASC->CancelAbilities(&CancelTags);
					SetDesiredReadyStance(AlsxtReadinessTags::Relaxed);
				}
				if (GEngine)
				{
					FString ClassName = this->GetClass()->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AbilitySystemComponent NOT found in %s"), *ClassName));
				}
			}

			// SetDesiredReadyStance(AlsxtReadinessTags::Relaxed);
			// SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::PatrolReady);
		}
	}
}

// Input Action to Switch between Orthodox and Southpaw stances
void AAlsxtCharacterPlayer::Input_OnSwitchCombatStance()
{
	if (GetAbilitySystemComponent())
	{
		FGameplayTagContainer CombatStanceTags;
		CombatStanceTags.AddTag(AlsxtAbilityGameplayTags::TAG_Ability_SwitchCombatStance);
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(CombatStanceTags);
	}
	if (CanSwitchCombatStance())
	{
		if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Orthodox)
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Southpaw);
			SetCameraRightShoulder(!IsCameraRightShoulder());
		}
		else
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Orthodox);
			SetCameraRightShoulder(!IsCameraRightShoulder());
		}
	}
}

void AAlsxtCharacterPlayer::Input_OnBlock(const FInputActionValue& ActionValue)
{
	FAlsxtDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();

	if (CanEnterBlockingDefensiveMode())
	{
		if (ActionValue.Get<bool>() == true)
		{
			FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
			NewDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation;
			NewDefensiveModeState.AnticipationSide = PreviousDefensiveModeState.AnticipationSide == FGameplayTag::EmptyTag ? ALSXTImpactSideTags::Front : PreviousDefensiveModeState.AnticipationSide;
			NewDefensiveModeState.AnticipationHeight = PreviousDefensiveModeState.AnticipationHeight == FGameplayTag::EmptyTag ? ALSXTImpactHeightTags::Middle : PreviousDefensiveModeState.AnticipationHeight;
			NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
			SetDefensiveModeState(NewDefensiveModeState);
			// SetDesiredDefensiveMode(ALSXTDefensiveModeTags::Blocking);

		}
		else 
		{
			// ResetDefensiveModeState();
			FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation ? FGameplayTag::EmptyTag : PreviousDefensiveModeState.Mode;
			NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
			SetDefensiveModeState(NewDefensiveModeState);
			// SetDesiredDefensiveMode(FGameplayTag::EmptyTag);
		}
	}
	else if ((DesiredDefensiveMode == ALSXTDefensiveModeTags::Blocking) && (ActionValue.Get<bool>()  == false))
	{
		FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
		NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation ? FGameplayTag::EmptyTag : PreviousDefensiveModeState.Mode;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
		SetDefensiveModeState(NewDefensiveModeState);
		SetDesiredDefensiveMode(FGameplayTag::EmptyTag);
	}
}

void AAlsxtCharacterPlayer::Input_OnLeanLeft(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (CanLean())
		{
			SetDesiredLean(ALSXTLeanDirectionTags::Left);
			FAlsxtPoseState NewPoseState = GetALSXTPoseState();
			NewPoseState.LeanDirection = ALSXTLeanDirectionTags::Left;
			SetALSXTPoseState(NewPoseState);
		}
		if (IsCameraRightShoulder())
		{
			SetCameraRightShoulder(false);
		}
	}
	else
	{
		SetDesiredLean(FGameplayTag::EmptyTag);
		FAlsxtPoseState NewPoseState = GetALSXTPoseState();
		NewPoseState.LeanDirection = FGameplayTag::EmptyTag;
		SetALSXTPoseState(NewPoseState);
	}
}

void AAlsxtCharacterPlayer::Input_OnLeanRight(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (CanLean())
		{
			SetDesiredLean(ALSXTLeanDirectionTags::Right);
			FAlsxtPoseState NewPoseState = GetALSXTPoseState();
			NewPoseState.LeanDirection = ALSXTLeanDirectionTags::Right;
			SetALSXTPoseState(NewPoseState);
		}
		if (!IsCameraRightShoulder())
		{
			SetCameraRightShoulder(true);
		}
	}
	else
	{
		SetDesiredLean(FGameplayTag::EmptyTag);
		FAlsxtPoseState NewPoseState = GetALSXTPoseState();
		NewPoseState.LeanDirection = FGameplayTag::EmptyTag;
		SetALSXTPoseState(NewPoseState);
	}
}

void AAlsxtCharacterPlayer::Input_OnSwitchWeaponFirearmStance()
{
	if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
	{
		return;
	}
	else
	{
		FGameplayTagContainer AvailableStances = GetAvailableFirearmStances();
		TArray<FGameplayTag> AvailableStancesArray;
		AvailableStances.GetGameplayTagArray(AvailableStancesArray);
		if (AvailableStances.Num() <= 0 || AvailableStances.Num() == 1 && AvailableStancesArray[0] == GetDesiredWeaponFirearmStance())
		{
			return;
		}
		int CurrentIndex = AvailableStancesArray.IndexOfByKey(GetDesiredWeaponFirearmStance());
		int NextIndex;
		if ((CurrentIndex + 1) > (AvailableStancesArray.Num() - 1))
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}
		SetDesiredWeaponFirearmStance(AvailableStancesArray[NextIndex]);
	}
}

void AAlsxtCharacterPlayer::Input_OnSwitchWeaponReadyPosition()
{
	TArray<FGameplayTag> AvailablePositions;
	// TArray<FGameplayTag> AvailablePositions = (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral) ? {ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden } : { ALSXTWeaponReadyPositionTags::Ready, ALSXTWeaponReadyPositionTags::Retention, ALSXTWeaponReadyPositionTags::Hip, ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
	{
		AvailablePositions = {ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	}
	else
	{
		AvailablePositions = { ALSXTWeaponReadyPositionTags::Ready, ALSXTWeaponReadyPositionTags::Retention, ALSXTWeaponReadyPositionTags::Hip, ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	}
	int CurrentIndex = AvailablePositions.IndexOfByKey(GetDesiredWeaponReadyPosition());
	int NextIndex = (CurrentIndex + 1) > (AvailablePositions.Num() - 1) ? 0 : CurrentIndex + 1;

	// if ((CurrentIndex + 1) > (AvailablePositions.Num() - 1))
	// {
	// 	NextIndex = 0;
	// }
	// else
	// {
	// 	NextIndex = CurrentIndex + 1;
	// }
	SetDesiredWeaponReadyPosition(AvailablePositions[NextIndex]);
}

void AAlsxtCharacterPlayer::Input_OnSwitchGripPosition()
{
	if (CanSwitchGripPosition())
	{
		FGameplayTagContainer AvailableGripPositions = GetAvailableGripPositions();
		TArray<FGameplayTag> AvailableGripPositionsArray;
		AvailableGripPositions.GetGameplayTagArray(AvailableGripPositionsArray);
		if (AvailableGripPositions.IsEmpty() || AvailableGripPositions.Num() == 1 && AvailableGripPositionsArray[0] == GetDesiredGripPosition())
		{
			return;
		}
		int LastIndex = AvailableGripPositionsArray.Num() - 1;
		int CurrentIndex = AvailableGripPositionsArray.IndexOfByKey(GetDesiredGripPosition());
		int NextIndex;
		if ((CurrentIndex + 1) > LastIndex)
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}

		SetDesiredGripPosition(AvailableGripPositionsArray[NextIndex]);
	}
}

void AAlsxtCharacterPlayer::Input_OnSwitchForegripPosition()
{
	if (CanSwitchForegripPosition())
	{
		FGameplayTagContainer AvailableForegripPositions = GetAvailableForegripPositions();
		TArray<FGameplayTag> AvailableForegripPositionsArray;
		AvailableForegripPositions.GetGameplayTagArray(AvailableForegripPositionsArray);
		if (AvailableForegripPositions.IsEmpty() || AvailableForegripPositions.Num() == 1 && AvailableForegripPositionsArray[0] == GetDesiredGripPosition())
		{
			return;
		}
		int LastIndex = AvailableForegripPositionsArray.Num() - 1;
		int CurrentIndex = AvailableForegripPositionsArray.IndexOfByKey(GetDesiredForegripPosition());
		int NextIndex;
		if ((CurrentIndex + 1) > LastIndex)
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}
		
		SetDesiredForegripPosition(AvailableForegripPositionsArray[NextIndex]);
		FAlsxtHeldItemState NewHeldItemState = GetHeldItemState();
		NewHeldItemState.GripState.Foregrip.Grip = IAlsxtHeldItemInterface::Execute_GetHeldItemForegrip(this);
		SetHeldItemState(NewHeldItemState);
	}
}

