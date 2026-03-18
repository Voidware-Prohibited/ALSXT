// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#include "AlsxtCharacterNpc.h"
#include "AlsxtCharacter.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtMovementAttributeSet.h"
#include "Components/Character/AlsxtCharacterCustomizationComponent.h"

/**
* @file AlsxtCharacterNpc.cpp
* @brief Base AI ALSXT Character class. AI do not have Player States so we implement everything directly on the Avatar.
*/

/**
* @brief Constructor for AlsxtCharacterNPC adding the Ability System Component
*/
AAlsxtCharacterNpc::AAlsxtCharacterNpc(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	 // If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	 SetNetUpdateFrequency(100.0f);
 
	 // Create the Ability System Component sub-object.
	 AbilitySystemComponent = CreateDefaultSubobject<UAlsxtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	 AbilitySystemComponent->SetIsReplicated(true);
 
	 // Set Replication Mode to Mixed for NPCs.
	 AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	 // Create the Character Customization Component sub-object.
	 CharacterCustomizationComponent = CreateDefaultSubobject<UAlsxtCharacterCustomizationComponent>(TEXT("Character Customization Component"));
	 AddOwnedComponent(CharacterCustomizationComponent);
}

void AAlsxtCharacterNpc::BeginPlay()
{
	Super::BeginPlay();
	 
	// Provide this character as owner and avatar
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAbilitySystem();
}

void AAlsxtCharacterNpc::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		// Shouldn't happen, but if it is, return an error.
		return;
	}
	
	// Call the function on "Custom Ability System Component" to set up references and Init data. (Client)
	AbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, this, this);

	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAlsxtMovementAttributeSet::GetMovementSpeedMultiplierAttribute()).AddUObject(this, &ThisClass::MovementSpeedMultiplierChanged);
	
	PostInitializeAbilitySystem();
}

void AAlsxtCharacterNpc::PostInitializeAbilitySystem_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
}