// MIT

#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "NativeGameplayTags.h"

namespace AlsxtAbilityGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Crouching, TEXT("Gameplay.Ability.Crouching"))
	UE_DEFINE_GAMEPLAY_TAG(Prone, TEXT("Gameplay.Ability.Prone"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement, TEXT("Gameplay.Ability.Movement"))
	UE_DEFINE_GAMEPLAY_TAG(Running, TEXT("Gameplay.Ability.Running"))
	UE_DEFINE_GAMEPLAY_TAG(Sprint, TEXT("Gameplay.Ability.Sprint"))
	UE_DEFINE_GAMEPLAY_TAG(Breathing, TEXT("Gameplay.Ability.Breathing"))
	UE_DEFINE_GAMEPLAY_TAG(HoldBreath, TEXT("Gameplay.Ability.HoldBreath"))
	UE_DEFINE_GAMEPLAY_TAG(StaminaRegen, TEXT("Gameplay.Ability.StaminaRegen"))
	UE_DEFINE_GAMEPLAY_TAG(Jump, TEXT("Gameplay.Ability.Jump"))
	UE_DEFINE_GAMEPLAY_TAG(Roll, TEXT("Gameplay.Ability.Roll"))
	UE_DEFINE_GAMEPLAY_TAG(Slide, TEXT("Gameplay.Ability.Slide"))
	UE_DEFINE_GAMEPLAY_TAG(Mantle, TEXT("Gameplay.Ability.Mantle"))
	UE_DEFINE_GAMEPLAY_TAG(Vault, TEXT("Gameplay.Ability.Vault"))
	UE_DEFINE_GAMEPLAY_TAG(HoldItem, TEXT("Gameplay.Ability.HoldItem"))
	UE_DEFINE_GAMEPLAY_TAG(PickupItem, TEXT("Gameplay.Ability.PickupItem"))
	UE_DEFINE_GAMEPLAY_TAG(ThirdPerson, TEXT("Gameplay.Ability.ThirdPerson"))
	UE_DEFINE_GAMEPLAY_TAG(Focus, TEXT("Gameplay.Ability.Focus"))
	UE_DEFINE_GAMEPLAY_TAG(Aim, TEXT("Gameplay.Ability.Aim"))
	UE_DEFINE_GAMEPLAY_TAG(Targeting, TEXT("Gameplay.Ability.Targeting"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack, TEXT("Gameplay.Ability.MeleeAttack"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Light, TEXT("Gameplay.Ability.MeleeAttack.Light"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Medium, TEXT("Gameplay.Ability.MeleeAttack.Medium"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Heavy, TEXT("Gameplay.Ability.MeleeAttack.Heavy"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Grapple, TEXT("Gameplay.Ability.MeleeAttack.Grapple"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Throw, TEXT("Gameplay.Ability.MeleeAttack.Throw"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Finisher, TEXT("Gameplay.Ability.MeleeAttack.Finisher"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Takedown, TEXT("Gameplay.Ability.MeleeAttack.Takedown"))
	UE_DEFINE_GAMEPLAY_TAG(PrimaryFire, TEXT("Gameplay.Ability.PrimaryFire"))
	UE_DEFINE_GAMEPLAY_TAG(Block, TEXT("Gameplay.Ability.Block"))
	UE_DEFINE_GAMEPLAY_TAG(Flip, TEXT("Gameplay.Ability.Flip"))
	UE_DEFINE_GAMEPLAY_TAG(WallJump, TEXT("Gameplay.Ability.Wall Jump"))
	UE_DEFINE_GAMEPLAY_TAG(WallRun, TEXT("Gameplay.Ability.Wall Run"))
	UE_DEFINE_GAMEPLAY_TAG(HandGesture, TEXT("Gameplay.Ability.HandGesture"))
	UE_DEFINE_GAMEPLAY_TAG(Emote, TEXT("Gameplay.Ability.Emote"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_SwitchCombatStance, TEXT("Gameplay.Ability.SwitchCombatStance"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_BreathRegen, TEXT("Gameplay.Ability.Breath Regen"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_CameraZoom, TEXT("Gameplay.Ability.Camera Zoom"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Readiness, TEXT("Gameplay.Ability.Readiness"))
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_WeaponReadyPosition, TEXT("Gameplay.Ability.WeaponReadyPosition"))
}

namespace AlsxtGASGameplayTags
{
	namespace Effect
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effect_NoLevel, "Effect.NoLevel", "This GE won't be leveled up or down. Base level will always be the same (upon granting it).")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effect_Type_Cooldown, "Effect.Type.Cooldown", "This is a cooldown GE")
	}

	namespace Character
	{
		namespace Controller
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Controller_PC, "Character.Controller.PlayerCharacter", "A Gameplay Tag applied to Characters that are controlled by a local player.")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Controller_NPC, "Character.Controller.NonPlayerCharacter", "A Gameplay Tag applied to Characters that are AI controlled.")
		}
		namespace Role
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Rank_ServerOwner, "Character.Rank.ServerOwner", "A Gameplay Tag applied to Characters that are controlled by a local player.")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Rank_Admin, "Character.Rank.Admin", "A Gameplay Tag applied to Characters that are AI controlled.")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Rank_Moderator, "Character.Rank.Moderator", "A Gameplay Tag applied to Characters that are AI controlled.")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Rank_Player, "Character.Rank.Player", "A Gameplay Tag applied to Characters that are AI controlled.")
		}
	}

	namespace Event
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Event_Trace_Melee, "Event.Trace.Melee", "General Melee Trace Event Tag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Event_Trace_Melee_Begin, "Event.Trace.Melee.Begin", "Begin Melee Trace Event Tag")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Event_Trace_Melee_End, "Event.Trace.Melee.End", "End Melee Trace Event Tag")
	}
	
	namespace CharacterTags
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Type_PC, "Character.Type.PlayerCharacter", "A Gameplay Tag applied to Characters that are controlled by a local player.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Type_NPC, "Character.Type.NonPlayerCharacter", "A Gameplay Tag applied to Characters that are AI controlled.")
	}

	namespace Cost
	{
		namespace Stamina
		{
			namespace Infinite
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Infinite_Combat, "StaminaCost.Infinite.Combat", "A Gameplay Tag applied to Combat Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Infinite_Running, "StaminaCost.Infinite.Running", "A Gameplay Tag applied to Running Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Infinite_Sprint, "StaminaCost.Infinite.Sprint", "A Gameplay Tag applied to Sprinting Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Infinite_WallRun, "StaminaCost.Infinite.WallRun", "A Gameplay Tag applied to Wall Running Stamina Cost")
			}
			namespace Instant
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_HoldingBreath, "StaminaCost.Instant.HoldingBreath", "A Gameplay Tag applied to Holding Breath Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Crouching, "StaminaCost.Instant.Crouching", "A Gameplay Tag applied to Crouching Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Jump, "StaminaCost.Instant.Jump", "A Gameplay Tag applied to Jumping Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Flip, "StaminaCost.Instant.Flip", "A Gameplay Tag applied to Flipping Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_WallJump, "StaminaCost.Instant.WallJump", "A Gameplay Tag applied to Wall Jumping Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Roll, "StaminaCost.Instant.Roll", "A Gameplay Tag applied to Rolling Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Slide, "StaminaCost.Instant.Slide", "A Gameplay Tag applied to Sliding Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Mantle, "StaminaCost.Instant.Mantle", "A Gameplay Tag applied to Mantle Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Vault, "StaminaCost.Instant.Vault", "A Gameplay Tag applied to Vaulting Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Attack_Light, "StaminaCost.Instant.Attack.Light", "A Gameplay Tag applied to ight Attack Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Attack_Medium, "StaminaCost.Instant.Attack.Medium", "A Gameplay Tag applied to Medium Attack Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Attack_Heavy, "StaminaCost.Instant.Attack.Heavy", "A Gameplay Tag applied to Heavy Attack Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Fall_Light, "StaminaCost.Instant.Fall.Light", "A Gameplay Tag applied to Light Fall Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Fall_Medium, "StaminaCost.Instant.Fall.Medium", "A Gameplay Tag applied to Medium Fall Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Fall_Heavy, "StaminaCost.Instant.Fall.Heavy", "A Gameplay Tag applied to Heavy Fall Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Damage_Light, "StaminaCost.Instant.Damage.Light", "A Gameplay Tag applied to Light Damage Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Damage_Medium, "StaminaCost.Instant.Damage.Medium", "A Gameplay Tag applied to Medium Damage Stamina Cost")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cost_Stamina_Instant_Damage_Heavy, "StaminaCost.Instant.Damage.Heavy", "A Gameplay Tag applied to Heavy Damage Stamina Cost")
			}
		}

		namespace Breath
		{
			namespace Infinite
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_BreathCost_Infinite_HoldingBreath, "Cost.Breath.Infinite.Breath.HoldingBreath", "A Gameplay Tag applied to Holding Breath Cost")
			}
		}
	}

	namespace Cooldown
	{
		namespace Stamina
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Sprint, "Cooldown.Stamina.Sprint", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Jump, "Cooldown.Stamina.Jump", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Roll, "Cooldown.Stamina.Roll", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Slide, "Cooldown.Stamina.Slide", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Mantle, "Cooldown.Stamina.Mantle", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Vault, "Cooldown.Stamina.Vault", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Flip, "Cooldown.Stamina.Flip", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_WallJump, "Cooldown.Stamina.WallJump", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_WallRun, "Cooldown.Stamina.WallRun", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_HoldingBreath, "Cooldown.Stamina.HoldingBreath", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Fall_Light, "Cooldown.Stamina.Fall.Light", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Fall_Medium, "Cooldown.Stamina.Fall.Medium", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Fall_Heavy, "Cooldown.Stamina.Fall.Heavy", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Damage_Light, "Cooldown.Damage.Light", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Damage_Medium, "Cooldown.Damage.Medium", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Damage_Heavy, "Cooldown.Damage.Heavy", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Attack_Light, "Cooldown.Attack.Light", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Attack_Medium, "Cooldown.Attack.Medium", "A Gameplay Tag applied to Combat Stamina Cost")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Stamina_Attack_Heavy, "Cooldown.Attack.Heavy", "A Gameplay Tag applied to Combat Stamina Cost")
		}
	}

	namespace StaminaCost
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Infinite_Combat, "StaminaCost.Infinite.Combat", "A Gameplay Tag applied to Combat Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Infinite_Running, "StaminaCost.Infinite.Running", "A Gameplay Tag applied to Running Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Infinite_Sprint, "StaminaCost.Infinite.Sprint", "A Gameplay Tag applied to Sprinting Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Infinite_WallRun, "StaminaCost.Infinite.WallRun", "A Gameplay Tag applied to Wall Running Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_HoldingBreath, "StaminaCost.Instant.HoldingBreath", "A Gameplay Tag applied to Holding Breath Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Crouching, "StaminaCost.Instant.Crouching", "A Gameplay Tag applied to Crouching Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Jump, "StaminaCost.Instant.Jump", "A Gameplay Tag applied to Jumping Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Flip, "StaminaCost.Instant.Flip", "A Gameplay Tag applied to Flipping Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_WallJump, "StaminaCost.Instant.WallJump", "A Gameplay Tag applied to Wall Jumping Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Roll, "StaminaCost.Instant.Roll", "A Gameplay Tag applied to Rolling Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Slide, "StaminaCost.Instant.Slide", "A Gameplay Tag applied to Sliding Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Mantle, "StaminaCost.Instant.Mantle", "A Gameplay Tag applied to Mantle Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Vault, "StaminaCost.Instant.Vault", "A Gameplay Tag applied to Vaulting Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Attack_Light, "StaminaCost.Instant.Attack.Light", "A Gameplay Tag applied to ight Attack Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Attack_Medium, "StaminaCost.Instant.Attack.Medium", "A Gameplay Tag applied to Medium Attack Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Attack_Heavy, "StaminaCost.Instant.Attack.Heavy", "A Gameplay Tag applied to Heavy Attack Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Fall_Light, "StaminaCost.Instant.Fall.Light", "A Gameplay Tag applied to Light Fall Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Fall_Medium, "StaminaCost.Instant.Fall.Medium", "A Gameplay Tag applied to Medium Fall Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Fall_Heavy, "StaminaCost.Instant.Fall.Heavy", "A Gameplay Tag applied to Heavy Fall Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Damage_Light, "StaminaCost.Instant.Damage.Light", "A Gameplay Tag applied to Light Damage Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Damage_Medium, "StaminaCost.Instant.Damage.Medium", "A Gameplay Tag applied to Medium Damage Stamina Cost")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_StaminaCost_Instant_Damage_Heavy, "StaminaCost.Instant.Damage.Heavy", "A Gameplay Tag applied to Heavy Damage Stamina Cost")
		
	}

	namespace BreathCost
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_BreathCost_Infinite_HoldingBreath, "BreathCost.Infinite.HoldingBreath", "A Gameplay Tag applied to Holding Breath Cost")
		
	}
	
	namespace BasicAttack
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_DirectDamage, "DamageType.DirectDamage", "Basic Damage Type. Used to calculated Resistance to Basic Damage.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Modifier_DirectDamage, "Resistance.Modifier.DirectDamage", "Granting Resistance to basic Damage")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Ignore_DirectDamage, "Resistance.Ignore.DirectDamage", "Ignore Resistance to basic Damage. Will deal true damage.")
	}

	namespace State
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_MovementInput, "State.MovementInput", "MovementInput")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Consuming_Stamina, "State.Consuming.Stamina", "Consuming Stamina")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Regen_Stamina, "State.Regen.Stamina", "Regenerating Stamina")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_KnockedDown, "State.KnockedDown", "KnockedDown")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Unconscious, "State.Unconscious", "Unconscious")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Dead, "Gameplay.State.Dead", "Dead")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_HoldingBreath, "State.HoldingBreath", "HoldingBreath Active")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_OutOfBreath, "State.OutOfBreath", "OutOfBreath")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Exhausted, "State.Exhausted", "Exhausted")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Cooldown_StaminaRegen, "State.Cooldown.StaminaRegen", "Stamina Regeneration Cooldown")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Min_Stamina, "State.Min.Stamina", "Min Stamina reached")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Health, "State.Max.Health", "Max Health reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Stamina, "State.Max.Stamina", "Max Stamina reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Strength, "State.Max.Strength", "Max Strength reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Stability, "State.Max.Stamina", "Max Stamina reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_HoldingBreath, "Gameplay.State.Max.HoldingBreath", "Max HoldingBreath reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Breath, "Gameplay.State.Max.Breath", "Max Breath reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_BreathRegen, "Gameplay.State.Max.BreathRegen", "Max Breath reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_HoldingBreathRegen, "Gameplay.State.Max.HoldingBreathRegen", "Max Breath reached")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Blood, "State.Max.Blood", "Max Blood reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_BodyTemp, "State.Max.BodyTemp", "Max BodyTemp reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Energy, "State.Max.Energy", "Max Energy reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Hydration, "State.Max.Hydration", "Max Hydration reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Nutrition, "State.Max.Nutrition", "Max Nutrition reached")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Health, "State.Blocked.Regen.Health", "Block Health regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Stamina, "State.Blocked.Regen.Stamina", "Block Stamina regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Strength, "State.Blocked.Regen.Strength", "Block Strength regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Stability, "State.Blocked.Regen.Stability", "Block Stability regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_HoldBreath, "Gameplay.State.Blocked.Regen.HoldingBreath", "Block HoldingBreath regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Breath, "Gameplay.State.Blocked.Regen.Breath", "Block Breath regeneration.")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Blood, "State.Blocked.Regen.Blood", "Block Blood regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_BodyTemp, "State.Blocked.Regen.BodyTemp", "Block BodyTemp regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Energy, "State.Blocked.Regen.Energy", "Block Energy regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Hydration, "State.Blocked.Regen.Hydration", "Block Hydration regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Nutrition, "State.Blocked.Regen.Nutrition", "Block Nutrition regeneration.")
	}
	
	namespace Statuses
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Dead, "Status.Dead", "Is Dead. Can be used to disable effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_KnockedDown, "State.KnockedDown", "KnockedDown")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Unconscious, "State.Unconscious", "Unconscious")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Exhausted, "Status.Exhausted", "Is Exhausted. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Freezing, "Status.Freezing", "Is Freezing. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Overheated, "Status.Overheated", "Is Overheated. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Poisoned, "Status.Poisoned", "Is Overheated. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Infected, "Status.Infected", "Is Overheated. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Dizzy, "Status.Dizzy", "Is Dizzy. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_Drunk, "Status.Drunk", "Is Drunk. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Status_High, "Status.High", "Is High. Can be used to trigger more effects.")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Bleeding, "Debuff.Bleeding", "Is Bleeding. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Bleeding, "DamageType.Bleeding", "Bleeding Damage Type. Used to calculated Resistance to Bleeding Damage.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Immunity_Bleeding, "Immunity.Bleeding", "Immune to Bleeding")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Modifier_Bleeding, "Resistance.Modifier.Bleeding", "Granting Resistance to Bleeding")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Ignore_Bleeding, "Resistance.Ignore.Bleeding", "Ignore Resistance to Bleeding. Will increase bleeding with the source value.")
	}
	
	namespace Statuses
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Movement_Combat, "State.Movement.Combat", "Owned when user is in Combat gait")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Movement_Running, "State.Movement.Running", "Owned when user Runs")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Movement_Sprinting, "State.Movement.Sprinting", "Owned when user Sprints")
	}
}