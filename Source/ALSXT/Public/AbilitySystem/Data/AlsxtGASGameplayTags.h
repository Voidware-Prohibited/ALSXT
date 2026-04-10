// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace AlsxtAbilityGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Prone);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Running);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Breathing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HoldBreath);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StaminaRegen);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slide);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mantle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vault);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HoldItem);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PickupItem);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ThirdPerson);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Focus);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aim);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Targeting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Light);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Medium);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Heavy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Grapple);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Throw);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_MeleeAttack_Takedown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryFire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WallJump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WallRun);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HandGesture);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Emote);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_SwitchCombatStance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_BreathRegen);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_CameraZoom);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Readiness);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_WeaponReadyPosition);
}

namespace AlsxtGASGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_NoLevel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Type_Cooldown);

	namespace Character
	{
		namespace Controller
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Controller_PC);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Controller_NPC);
		}
		namespace Role
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Rank_ServerOwner);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Rank_Admin);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Rank_Moderator);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Rank_Player);
		}
	}

	namespace Event
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Trace_Melee);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Trace_Melee_Begin);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Trace_Melee_End);
	}
	
	namespace CharacterTags
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_PC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_NPC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_BlockHealthRegen);
	}

	namespace Cost
	{
		namespace Stamina
		{
			namespace Infinite
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Infinite_Combat);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Infinite_Running);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Infinite_Sprint);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Infinite_WallRun);
			}
			namespace Instant
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_HoldingBreath);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Crouching);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Jump);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Flip);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_WallJump);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Roll);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Slide);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Mantle);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Vault);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Attack_Light);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Attack_Medium);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Attack_Heavy);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Fall_Light);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Fall_Medium);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Fall_Heavy);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Damage_Light);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Damage_Medium);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cost_Stamina_Instant_Damage_Heavy);
			}
		}

		namespace Breath
		{
			namespace Infinite
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_BreathCost_Infinite_HoldingBreath);
			}
		}
	}

	namespace Cooldown
	{
		namespace Stamina
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Sprint);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Jump);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Roll);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Slide);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Mantle);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Vault);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Flip);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_WallJump);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_WallRun);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_HoldingBreath);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Fall_Light);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Fall_Medium);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Fall_Heavy);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Damage_Light);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Damage_Medium);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Damage_Heavy);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Attack_Light);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Attack_Medium);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Stamina_Attack_Heavy);
		}
	}
	
	namespace StaminaCost
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Infinite_Combat);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Infinite_Running);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Infinite_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Infinite_WallRun);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_HoldingBreath);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Crouching);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Jump);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Flip);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_WallJump);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Roll);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Slide);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Mantle);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Vault);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Attack_Light);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Attack_Medium);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Attack_Heavy);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Fall_Light);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Fall_Medium);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Fall_Heavy);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Damage_Light);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Damage_Medium);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StaminaCost_Instant_Damage_Heavy);
	}

	namespace BreathCost
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_BreathCost_Infinite_HoldingBreath);
	}

	namespace BasicAttack
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_DirectDamage);
	}
	
	namespace State
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_MovementInput);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Consuming_Stamina);
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
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Movement_Combat);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Movement_Running);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Movement_Sprinting);
	}
}