// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#include "AlsxtPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "AbilitySystem/Interfaces/AlsxtAbilitySystemInterface.h"
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsVector.h"
#include "Net/UnrealNetwork.h"

// #include UE_INLINE_GENERATED_CPP_BY_NAME(AlsxtPlayerController)

/**
* @file AlsxtPlayerController.cpp
* @brief template class that contains all shared Logic and Data for Player Classes.
* AAlsxtCharacterPlayer depends on AAlsxtPlayerState and AAlsxtPlayerController to function.
* Create a Blueprint class based on this class, do not use the C++ class directly in the Editor
*/

// Server only
void AAlsxtPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAlsxtPlayerState* PS = GetPlayerState<AAlsxtPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AAlsxtPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	// CreateHUD();
}

void AAlsxtPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Retrieve and Set a Reference to AAlsxtPlayerState
	if (const TSoftObjectPtr<AAlsxtPlayerState> CurrentAlsxtPlayerState = Cast<AAlsxtPlayerState>(PlayerState.Get()))
	{
		AlsxtPlayerState = CurrentAlsxtPlayerState;
	}

	// Retrieve and Set a Reference to AAlsxtCharacterPlayer
	if (const TSoftObjectPtr<AAlsxtCharacterPlayer> CurrentAlsxtPlayerCharacter = Cast<AAlsxtCharacterPlayer>(GetPawn()))
	{
		AlsxtCharacterPlayer = CurrentAlsxtPlayerCharacter;
	}

	// Retrieve and Set a Reference to AlsxtAbilitySystemComponent
	if (const IAlsxtAbilitySystemInterface* AlsxtAbilitySystemInterface = Cast<IAlsxtAbilitySystemInterface>(PlayerState.Get()))
	{
		AlsxtAbilitySystemComponent = AlsxtAbilitySystemInterface->GetAlsxtAbilitySystemComponent();
	}

	// Get the Enhanced Input Local Player Subsystem and add the mapping context
	// if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	// {
	// 	if (InputMappingContext)
	// 	{
	// 		Subsystem->AddMappingContext(InputMappingContext, 0); // Priority 0
	// 	}
	// }
}

void AAlsxtPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Cast the InputComponent to UEnhancedInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (AnyInputDetectionAction)
		{
			EnhancedInputComponent->BindAction(AnyInputDetectionAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnHandleAnyInputKey);
		}

		EnhancedInputComponent->BindAction(MainMenuAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMainMenuAction);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnInventoryAction);
		EnhancedInputComponent->BindAction(HandSignalAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnHandSignalAction);
		
		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

/**
* @brief Connect Interface Function to provide a pointer to the AbilitySystemComponent
*/
UAbilitySystemComponent* AAlsxtPlayerController::GetAbilitySystemComponent() const
{
	AAlsxtPlayerState* PS = GetPlayerState<AAlsxtPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		return PS->GetAbilitySystemComponent();
	}
	return nullptr;
}

void AAlsxtPlayerController::Input_OnHandleAnyInputKey()
{
	OnAnyInputDetected.Broadcast();
}

void AAlsxtPlayerController::Input_OnMainMenuAction(const FInputActionValue& ActionValue)
{
}

void AAlsxtPlayerController::Input_OnInventoryAction(const FInputActionValue& ActionValue)
{
}

void AAlsxtPlayerController::Input_OnHandSignalAction(const FInputActionValue& ActionValue)
{
}
