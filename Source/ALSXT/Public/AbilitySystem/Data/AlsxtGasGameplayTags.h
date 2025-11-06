// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace ALSXTAbilityGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Breathing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StaminaRegen);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mantle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vault);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HoldItem);
}

namespace ALSXTGASGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_NoLevel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Type_Cooldown);
	
	namespace CharacterTags
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_PC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_NPC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_BlockHealthRegen);
	}
	
	namespace StaminaCost
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Infinite_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Jump);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Roll);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Mantle);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Vault);
	}

	namespace BasicAttack
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_DirectDamage);
	}
	
	namespace State
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_ConsumingStamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Consuming_Stamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_RegeneratingStamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Regen_Stamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_KnockedDown);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Unconscious);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Dead);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_HoldingBreath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_OutOfBreath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Exhausted);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Health);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Cooldown_StaminaRegen);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Min_Stamina);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Blood);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_BodyTemp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Hydration);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Nutrition);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Encumberance);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Energy);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Stamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Strength);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Stability);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_HoldingBreath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Breath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_BreathRegen);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_HoldingBreathRegen);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Blood);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_BodyTemp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Hydration);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Nutrition);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Encumberance);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Energy);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Stamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Strength);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Stability);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_HoldingBreath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Breath);
	}
	
	namespace Statuses
	{
		
		// Status Example
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Dead);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_KnockedDown);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Unconscious);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Exhausted);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Freezing);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Overheated);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Poisoned);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Infected);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Dizzy);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Drunk);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_High);
		// -- Bleeding --
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Debuff_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_Bleeding);
	}

	namespace Movement
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Movement_Sprinting);
	}
}