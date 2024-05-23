#include "Utility/ALSXTGameplayTags.h"
#include "Utility/AlsGameplayTags.h"

namespace AlsOverlayModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(AxeOneHanded, TEXT("Als.OverlayMode.Axe One Handed"))
	UE_DEFINE_GAMEPLAY_TAG(AxeTwoHanded, TEXT("Als.OverlayMode.Axe Two Handed"))
	UE_DEFINE_GAMEPLAY_TAG(Pickaxe, TEXT("Als.OverlayMode.Pickaxe"))
	UE_DEFINE_GAMEPLAY_TAG(Knife, TEXT("Als.OverlayMode.Knife"))
	UE_DEFINE_GAMEPLAY_TAG(Hammer, TEXT("Als.OverlayMode.Hammer"))
	UE_DEFINE_GAMEPLAY_TAG(Grenade, TEXT("Als.OverlayMode.Grenade"))
	UE_DEFINE_GAMEPLAY_TAG(BuildingPlan, TEXT("Als.OverlayMode.Building Plan"))
	UE_DEFINE_GAMEPLAY_TAG(Flashlight, TEXT("Als.OverlayMode.Flashlight"))
	UE_DEFINE_GAMEPLAY_TAG(Katana, TEXT("Als.OverlayMode.Katana"))
	UE_DEFINE_GAMEPLAY_TAG(Sword, TEXT("Als.OverlayMode.Sword"))
}


namespace AlsLocomotionActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(CrowdNavigationReaction, TEXT("Als.LocomotionAction.Crowd Navigation Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(DefensiveReaction, TEXT("Als.LocomotionAction.Defensive Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(ImpactReaction, TEXT("Als.LocomotionAction.Impact Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(ClutchImpactPoint, TEXT("Als.LocomotionAction.Clutch Impact Point"))
	UE_DEFINE_GAMEPLAY_TAG(SyncedAttackReaction, TEXT("Als.LocomotionAction.Synced Attack Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(ImpactFall, TEXT("Als.LocomotionAction.Impact Fall"))
	UE_DEFINE_GAMEPLAY_TAG(ImpactFallLand, TEXT("Als.LocomotionAction.Impact Fall Land"))
	UE_DEFINE_GAMEPLAY_TAG(ImpactGetUp, TEXT("Als.LocomotionAction.Impact Get Up"))
	UE_DEFINE_GAMEPLAY_TAG(ImpactResponse, TEXT("Als.LocomotionAction.Impact Response"))
	UE_DEFINE_GAMEPLAY_TAG(ThrowDefault, TEXT("Als.LocomotionAction.Throw.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Vaulting, TEXT("Als.LocomotionAction.Vaulting"))
	UE_DEFINE_GAMEPLAY_TAG(Sliding, TEXT("Als.LocomotionAction.Sliding"))
	UE_DEFINE_GAMEPLAY_TAG(Acrobatic, TEXT("Als.LocomotionAction.Acrobatic"))
	UE_DEFINE_GAMEPLAY_TAG(WallJumping, TEXT("Als.LocomotionAction.Wall Jumping"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunning, TEXT("Als.LocomotionAction.Wall Running"))
	UE_DEFINE_GAMEPLAY_TAG(Emote, TEXT("Als.LocomotionAction.Emote"))
	UE_DEFINE_GAMEPLAY_TAG(EnteringStationary, TEXT("Als.LocomotionAction.Entering Stationary"))
	UE_DEFINE_GAMEPLAY_TAG(ExitingStationary, TEXT("Als.LocomotionAction.Exiting Stationary"))
	UE_DEFINE_GAMEPLAY_TAG(PrimaryAction, TEXT("Als.LocomotionAction.Primary Action"))
	UE_DEFINE_GAMEPLAY_TAG(SecondaryAction, TEXT("Als.LocomotionAction.Secondary Action"))
	UE_DEFINE_GAMEPLAY_TAG(PrimaryInteraction, TEXT("Als.LocomotionAction.Primary Interaction"))
	UE_DEFINE_GAMEPLAY_TAG(SecondaryInteraction, TEXT("Als.LocomotionAction.Secondary Interaction"))
	UE_DEFINE_GAMEPLAY_TAG(EquipingItem, TEXT("Als.LocomotionAction.Equiping Item"))
	UE_DEFINE_GAMEPLAY_TAG(UnequipingItem, TEXT("Als.LocomotionAction.Unequiping Item"))
	UE_DEFINE_GAMEPLAY_TAG(InspectHeldItem, TEXT("Als.LocomotionAction.Inspect Held Item"))
	UE_DEFINE_GAMEPLAY_TAG(InspectMagazine, TEXT("Als.LocomotionAction.Firearm.Inspect Magazine"))
	UE_DEFINE_GAMEPLAY_TAG(Reloading, TEXT("Als.LocomotionAction.Firearm.Reloading"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingFireMode, TEXT("Als.LocomotionAction.Firearm.Toggling Fire Mode"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingFirearmStance, TEXT("Als.LocomotionAction.Firearm.Toggling Firearm Stance"))
	UE_DEFINE_GAMEPLAY_TAG(Charging, TEXT("Als.LocomotionAction.Firearm.Charging"))
	UE_DEFINE_GAMEPLAY_TAG(FixingMalfunction1, TEXT("Als.LocomotionAction.Firearm.Fixing Malfunction 1"))
	UE_DEFINE_GAMEPLAY_TAG(FixingMalfunction2, TEXT("Als.LocomotionAction.Firearm.Fixing Malfunction 2"))
}

namespace ALSXTLeanDirectionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Lean Direction.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Lean Direction.Right"))
}

namespace ALSXTActionTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Primary, TEXT("Als.Action Type.Primary"))
	UE_DEFINE_GAMEPLAY_TAG(Secondary, TEXT("Als.Action Type.Secondary"))
}

namespace ALSXTActionStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Prone, TEXT("Als.Action Stance.Prone"))
	UE_DEFINE_GAMEPLAY_TAG(Crouched, TEXT("Als.Action Stance.Crouched"))
	UE_DEFINE_GAMEPLAY_TAG(Standing, TEXT("Als.Action Stance.Standing"))
	UE_DEFINE_GAMEPLAY_TAG(InAir, TEXT("Als.Action Stance.In Air"))
}

namespace ALSXTMantleTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(High, TEXT("Als.Mantle Type.High"))
	UE_DEFINE_GAMEPLAY_TAG(Low, TEXT("Als.Mantle Type.Low"))
	UE_DEFINE_GAMEPLAY_TAG(InAir, TEXT("Als.Mantle Type.In Air"))
}

namespace ALSXTVaultTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Low, TEXT("Als.Vault Type.Low"))
	UE_DEFINE_GAMEPLAY_TAG(Medium, TEXT("Als.Vault Type.Medium"))
	UE_DEFINE_GAMEPLAY_TAG(High, TEXT("Als.Vault Type.High"))
	UE_DEFINE_GAMEPLAY_TAG(Fence, TEXT("Als.Vault Type.Fence"))
	UE_DEFINE_GAMEPLAY_TAG(InAir, TEXT("Als.Vault Type.In Air"))
}

namespace ALSXTAcrobaticActionTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Flip, TEXT("Als.Acrobatic Action.Flip"))
	UE_DEFINE_GAMEPLAY_TAG(WallJump, TEXT("Als.Acrobatic Action.Wall Jump"))
	UE_DEFINE_GAMEPLAY_TAG(WallRun, TEXT("Als.Acrobatic Action.Wall Run"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunLeft, TEXT("Als.Acrobatic Action.Wall Run Left"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunRight, TEXT("Als.Acrobatic Action.Wall Run Right"))

}

namespace ALSXTPhysicalAnimationModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Game.Physical Animation Mode.None"))
	UE_DEFINE_GAMEPLAY_TAG(Bump, TEXT("Game.Physical Animation Mode.Bump"))
	UE_DEFINE_GAMEPLAY_TAG(Hit, TEXT("Game.Physical Animation Mode.Hit"))
}

namespace ALSXTReactionAnimationTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Game.Reaction Animation Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(WithRecvovery, TEXT("Game.Reaction Animation Type.WithRecvovery"))
	UE_DEFINE_GAMEPLAY_TAG(Synced, TEXT("Game.Reaction Animation Type.Synced"))
}

namespace ALSXTFallTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Bump, TEXT("Game.Fall Type.Bump"))
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Game.Fall Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(Attack, TEXT("Game.Fall Type.Attack"))
	UE_DEFINE_GAMEPLAY_TAG(SyncedAttack, TEXT("Game.Fall Type.Synced Attack"))
}

namespace ALSXTPlayerServerStateTags
{
	UE_DEFINE_GAMEPLAY_TAG(Loading, TEXT("Game.Player Server State.Loading"))
	UE_DEFINE_GAMEPLAY_TAG(InLobby, TEXT("Game.Player Server State.In Lobby"))
	UE_DEFINE_GAMEPLAY_TAG(InPreGame, TEXT("Game.Player Server State.In PreGame"))
	UE_DEFINE_GAMEPLAY_TAG(Spectating, TEXT("Game.Player Server State.Spectating"))
	UE_DEFINE_GAMEPLAY_TAG(InGame, TEXT("Game.Player Server State.InGame"))
	UE_DEFINE_GAMEPLAY_TAG(Disconnected, TEXT("Game.Player Server State.Disconnected"))
}

namespace ALSXTPlayerGameRoleTags
{
	UE_DEFINE_GAMEPLAY_TAG(OrganizationLeader, TEXT("Game.Player Game Role.Organization Leader"))
	UE_DEFINE_GAMEPLAY_TAG(SectionLeader, TEXT("Game.Player Game Role.Section Leader"))
	UE_DEFINE_GAMEPLAY_TAG(FireteamLeader, TEXT("Game.Player Game Role.Fireteam Leader"))
	UE_DEFINE_GAMEPLAY_TAG(Unit, TEXT("Game.Player Game Role.Unit"))
}

namespace ALSXTPlayerServerRoleTags
{
	UE_DEFINE_GAMEPLAY_TAG(GameMaster, TEXT("Game.Player Server Role.Game Master"))
	UE_DEFINE_GAMEPLAY_TAG(Moderator, TEXT("Game.Player Server Role.Moderator"))
	UE_DEFINE_GAMEPLAY_TAG(Player, TEXT("Game.Player Server Role.Player"))
}

namespace ALSXTSexTags
{
	UE_DEFINE_GAMEPLAY_TAG(Male, TEXT("Als.Sex.Male"))
	UE_DEFINE_GAMEPLAY_TAG(Female, TEXT("Als.Sex.Female"))
}

namespace ALSXTVoiceVariantTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, TEXT("Als.Voice Variant.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Variant01, TEXT("Als.Voice Variant.Variant 01"))
}

namespace ALSXTStatusTags
{
	UE_DEFINE_GAMEPLAY_TAG(Normal, TEXT("Als.Status.Normal"))
	UE_DEFINE_GAMEPLAY_TAG(KnockedDown, TEXT("Als.Status.Knocked Down"))
	UE_DEFINE_GAMEPLAY_TAG(Unconscious, TEXT("Als.Status.Unconscious"))
	UE_DEFINE_GAMEPLAY_TAG(Dead, TEXT("Als.Status.Dead"))
}

namespace ALSXTFacialExpressionTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Facial Expression.None"))
	UE_DEFINE_GAMEPLAY_TAG(Happy, TEXT("Als.Facial Expression.Happy"))
	UE_DEFINE_GAMEPLAY_TAG(Pain, TEXT("Als.Facial Expression.Pain"))
}

namespace ALSXTFreelookingTags
{
	UE_DEFINE_GAMEPLAY_TAG(False, TEXT("Als.Freelooking.False"))
	UE_DEFINE_GAMEPLAY_TAG(True, TEXT("Als.Freelooking.True"))
}

namespace ALSXTFocusedTags
{
	UE_DEFINE_GAMEPLAY_TAG(False, TEXT("Als.Focused.False"))
	UE_DEFINE_GAMEPLAY_TAG(True, TEXT("Als.Focused.True"))
}

namespace ALSXTHoldingBreathTags
{
	UE_DEFINE_GAMEPLAY_TAG(False, TEXT("Als.Holding Breath.False"))
	UE_DEFINE_GAMEPLAY_TAG(True, TEXT("Als.Holding Breath.True"))
	UE_DEFINE_GAMEPLAY_TAG(Released, TEXT("Als.Holding Breath.Released"))
	UE_DEFINE_GAMEPLAY_TAG(Exhausted, TEXT("Als.Holding Breath.Exhausted"))
	UE_DEFINE_GAMEPLAY_TAG(Gasping, TEXT("Als.Holding Breath.Gasping"))
	UE_DEFINE_GAMEPLAY_TAG(Coughing, TEXT("Als.Holding Breath.Coughing"))
}

namespace ALSXTCursorLocationTags
{
	UE_DEFINE_GAMEPLAY_TAG(ScreenCenter, TEXT("Als.Cursor Location.Screen Center"))
	UE_DEFINE_GAMEPLAY_TAG(MouseLocation, TEXT("Als.Cursor Location.Mouse Location"))
	UE_DEFINE_GAMEPLAY_TAG(Location, TEXT("Als.Cursor Location.Location"))
}

namespace ALSXTObjectCollisionTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Game.Object Collision Type.None"))
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Game.Object Collision Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(Overlap, TEXT("Game.Object Collision Type.Overlap"))
	UE_DEFINE_GAMEPLAY_TAG(Drag, TEXT("Game.Object Collision Type.Drag"))
	UE_DEFINE_GAMEPLAY_TAG(Roll, TEXT("Game.Object Collision Type.Roll"))
}


namespace ALSXTImpactTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(CrowdNavigation, TEXT("Als.Impact Type.Crowd Navigation"))
	UE_DEFINE_GAMEPLAY_TAG(Bump, TEXT("Als.Impact Type.Bump"))
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Als.Impact Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(Attack, TEXT("Als.Impact Type.Attack"))
	UE_DEFINE_GAMEPLAY_TAG(SyncedAttack, TEXT("Als.Impact Type.Synced Attack"))
	UE_DEFINE_GAMEPLAY_TAG(Hit, TEXT("Als.Impact Type.Hit"))
	UE_DEFINE_GAMEPLAY_TAG(Fall, TEXT("Als.Impact Type.Fall"))
}

namespace ALSXTImpactVelocityTags
{
	UE_DEFINE_GAMEPLAY_TAG(Slow, TEXT("Als.Impact Velocity.Slow"))
	UE_DEFINE_GAMEPLAY_TAG(Moderate, TEXT("Als.Impact Velocity.Moderate"))
	UE_DEFINE_GAMEPLAY_TAG(Fast, TEXT("Als.Impact Velocity.Fast"))
	UE_DEFINE_GAMEPLAY_TAG(Faster, TEXT("Als.Impact Velocity.Faster"))
	UE_DEFINE_GAMEPLAY_TAG(TerminalVelocity, TEXT("Als.Impact Velocity.Terminal Velocity"))
}

namespace ALSXTImpactFormTags
{
	UE_DEFINE_GAMEPLAY_TAG(Push, TEXT("Als.Impact Form.Push"))
	UE_DEFINE_GAMEPLAY_TAG(Blunt, TEXT("Als.Impact Form.Blunt"))
	UE_DEFINE_GAMEPLAY_TAG(Blade, TEXT("Als.Impact Form.Blade"))
	UE_DEFINE_GAMEPLAY_TAG(Bullet, TEXT("Als.Impact Form.Bullet"))
	UE_DEFINE_GAMEPLAY_TAG(Explosion, TEXT("Als.Impact Form.Explosion"))
	UE_DEFINE_GAMEPLAY_TAG(Electric, TEXT("Als.Impact Form.Electric"))	
}

namespace ALSXTImpactLocationTags
{
	UE_DEFINE_GAMEPLAY_TAG(Head, TEXT("Als.Impact Location.Head"))
	UE_DEFINE_GAMEPLAY_TAG(Neck, TEXT("Als.Impact Location.Neck"))
	UE_DEFINE_GAMEPLAY_TAG(UpperTorso, TEXT("Als.Impact Location.Upper Torso"))
	UE_DEFINE_GAMEPLAY_TAG(LowerTorso, TEXT("Als.Impact Location.Lower Torso"))
	UE_DEFINE_GAMEPLAY_TAG(Hip, TEXT("Als.Impact Location.Hip"))
	UE_DEFINE_GAMEPLAY_TAG(LeftUpperArm, TEXT("Als.Impact Location.Left Upper Arm"))
	UE_DEFINE_GAMEPLAY_TAG(LeftForearm, TEXT("Als.Impact Location.Left Forearm"))
	UE_DEFINE_GAMEPLAY_TAG(LeftHand, TEXT("Als.Impact Location.Left Hand"))
	UE_DEFINE_GAMEPLAY_TAG(RightUpperArm, TEXT("Als.Impact Location.Right Upper Arm"))
	UE_DEFINE_GAMEPLAY_TAG(RightForearm, TEXT("Als.Impact Location.Right Forearm"))
	UE_DEFINE_GAMEPLAY_TAG(RightHand, TEXT("Als.Impact Location.Right Hand"))
	UE_DEFINE_GAMEPLAY_TAG(LeftThigh, TEXT("Als.Impact Location.Left Thigh"))
	UE_DEFINE_GAMEPLAY_TAG(LeftLowerLeg, TEXT("Als.Impact Location.Left Lower Leg"))
	UE_DEFINE_GAMEPLAY_TAG(LeftFoot, TEXT("Als.Impact Location.Left Foot"))
	UE_DEFINE_GAMEPLAY_TAG(RightThigh, TEXT("Als.Impact Location.Right Thigh"))
	UE_DEFINE_GAMEPLAY_TAG(RightLowerLeg, TEXT("Als.Impact Location.Right Lower Leg"))
	UE_DEFINE_GAMEPLAY_TAG(RightFoot, TEXT("Als.Impact Location.Right Foot"))
}

namespace ALSXTImpactSideTags
{
	UE_DEFINE_GAMEPLAY_TAG(Front, TEXT("Als.Impact Side.Front"))
	UE_DEFINE_GAMEPLAY_TAG(Back, TEXT("Als.Impact Side.Back"))
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Impact Side.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Impact Side.Right"))
	UE_DEFINE_GAMEPLAY_TAG(High, TEXT("Als.Impact Side.High"))
	UE_DEFINE_GAMEPLAY_TAG(Middle, TEXT("Als.Impact Side.Middle"))
	UE_DEFINE_GAMEPLAY_TAG(Low, TEXT("Als.Impact Side.Low"))
}

namespace ALSXTReboundTags
{
	UE_DEFINE_GAMEPLAY_TAG(False, TEXT("Als.Rebound.False"))
	UE_DEFINE_GAMEPLAY_TAG(True, TEXT("Als.Rebound.True"))
}

namespace ALSXTElementalConditionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Dry, TEXT("Als.Elemental Condition.Dry"))
	UE_DEFINE_GAMEPLAY_TAG(Wet, TEXT("Als.Elemental Condition.Wet"))
	UE_DEFINE_GAMEPLAY_TAG(Doused, TEXT("Als.Elemental Condition.Doused"))
}

namespace ALSXTImpactSurfaceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.Impact Surface.Cloth"))
}

namespace ALSXTBloodSpatterVelocityTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Blood Spatter Velocity.None"))
	UE_DEFINE_GAMEPLAY_TAG(Slow, TEXT("Als.Blood Spatter Velocity.Slow"))
	UE_DEFINE_GAMEPLAY_TAG(Moderate, TEXT("Als.Blood Spatter Velocity.Moderate"))
	UE_DEFINE_GAMEPLAY_TAG(Fast, TEXT("Als.Blood Spatter Velocity.Fast"))
	UE_DEFINE_GAMEPLAY_TAG(TerminalVelocity, TEXT("Als.Blood Spatter Velocity.Terminal Velocity"))
}

namespace ALSXTBloodSpatterVolumeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Small, TEXT("Als.Blood Spatter Volume.Small"))
	UE_DEFINE_GAMEPLAY_TAG(Medium, TEXT("Als.Blood Spatter Volume.Medium"))
	UE_DEFINE_GAMEPLAY_TAG(Large, TEXT("Als.Blood Spatter Volume.Large"))
	UE_DEFINE_GAMEPLAY_TAG(ExtraLarge, TEXT("Als.Blood Spatter Volume.Extra Large"))
	UE_DEFINE_GAMEPLAY_TAG(Max, TEXT("Als.Blood Spatter Volume.Max"))
}

namespace ALSXTBloodSpatterDistanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(LessThanTwo, TEXT("Als.Blood Spatter Distance.Less than Two"))
	UE_DEFINE_GAMEPLAY_TAG(TwoToFive, TEXT("Als.Blood Spatter Distance.Two to Five"))
	UE_DEFINE_GAMEPLAY_TAG(FiveToMax, TEXT("Als.Blood Spatter Distance.FiveToMax"))
	UE_DEFINE_GAMEPLAY_TAG(Max, TEXT("Als.Blood Spatter Distance.Max"))
}

namespace ALSXTHealthTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Game.Health.None"))
	UE_DEFINE_GAMEPLAY_TAG(Little, TEXT("Game.Health.Little"))
	UE_DEFINE_GAMEPLAY_TAG(Moderate, TEXT("Game.Health.Moderate"))
	UE_DEFINE_GAMEPLAY_TAG(Most, TEXT("Game.Health.Most"))
	UE_DEFINE_GAMEPLAY_TAG(All, TEXT("Game.Health.All"))
}

namespace ALSXTDamageAmountTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Game.Damage Amount.None"))
	UE_DEFINE_GAMEPLAY_TAG(Little, TEXT("Game.Damage Amount.Little"))
	UE_DEFINE_GAMEPLAY_TAG(Moderate, TEXT("Game.Damage Amount.Moderate"))
	UE_DEFINE_GAMEPLAY_TAG(Most, TEXT("Game.Damage Amount.Most"))
	UE_DEFINE_GAMEPLAY_TAG(All, TEXT("Game.Damage Amount.All"))
}

namespace ALSXTDamageTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Game.Damage Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(Edge, TEXT("Game.Damage Type.Edge"))
	UE_DEFINE_GAMEPLAY_TAG(Burn, TEXT("Game.Damage Type.Burn"))
	UE_DEFINE_GAMEPLAY_TAG(Explosion, TEXT("Game.Damage Type.Explosion"))
	UE_DEFINE_GAMEPLAY_TAG(Gas, TEXT("Game.Damage Type.Gas"))
	UE_DEFINE_GAMEPLAY_TAG(Poison, TEXT("Game.Damage Type.Poison"))
	UE_DEFINE_GAMEPLAY_TAG(Illness, TEXT("Game.Damage Type.Illness"))
}

namespace ALSXTLocomotionVariantTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, TEXT("Als.Locomotion Variant.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Exaggerated, TEXT("Als.Locomotion Variant.Exaggerated"))
	UE_DEFINE_GAMEPLAY_TAG(Style01, TEXT("Als.Locomotion Variant.Style01"))
	UE_DEFINE_GAMEPLAY_TAG(Style02, TEXT("Als.Locomotion Variant.Style02"))
	UE_DEFINE_GAMEPLAY_TAG(Style03, TEXT("Als.Locomotion Variant.Style03"))
}

namespace ALSXTInjuryTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Injury.None"))
	UE_DEFINE_GAMEPLAY_TAG(Head, TEXT("Als.Injury.Head"))
	UE_DEFINE_GAMEPLAY_TAG(Abdomen, TEXT("Als.Injury.Abdomen"))
	UE_DEFINE_GAMEPLAY_TAG(LeftArm, TEXT("Als.Injury.Left Arm"))
	UE_DEFINE_GAMEPLAY_TAG(RightArm, TEXT("Als.Injury.Right Arm"))
	UE_DEFINE_GAMEPLAY_TAG(LeftLeg, TEXT("Als.Injury.Left Leg"))
	UE_DEFINE_GAMEPLAY_TAG(RightLeg, TEXT("Als.Injury.Right Leg"))
}

namespace ALSXTCombatStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Neutral, TEXT("Als.Combat Stance.Neutral"))
	UE_DEFINE_GAMEPLAY_TAG(Ready, TEXT("Als.Combat Stance.Ready"))
	UE_DEFINE_GAMEPLAY_TAG(Aiming, TEXT("Als.Combat Stance.Aiming"))
}

namespace ALSXTIdleAnimationActivityTags
{
	UE_DEFINE_GAMEPLAY_TAG(Neutral, TEXT("Als.Idle Animation Activity.Neutral"))
	UE_DEFINE_GAMEPLAY_TAG(Smoking, TEXT("Als.Idle Animation Activity.Smoking"))
}

namespace ALSXTUnarmedAttackTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(LeftFist, TEXT("Als.Unarmed Attack Type.Left Fist"))
	UE_DEFINE_GAMEPLAY_TAG(RightFist, TEXT("Als.Unarmed Attack Type.Right Fist"))
	UE_DEFINE_GAMEPLAY_TAG(LeftFoot, TEXT("Als.Unarmed Attack Type.Left Foot"))
	UE_DEFINE_GAMEPLAY_TAG(RightFoot, TEXT("Als.Unarmed Attack Type.Right Foot"))
	UE_DEFINE_GAMEPLAY_TAG(LeftElbow, TEXT("Als.Unarmed Attack Type.Left Elbow"))
	UE_DEFINE_GAMEPLAY_TAG(RightElbow, TEXT("Als.Unarmed Attack Type.Right Elbow"))
	UE_DEFINE_GAMEPLAY_TAG(LeftKnee, TEXT("Als.Unarmed Attack Type.Left Knee"))
	UE_DEFINE_GAMEPLAY_TAG(RightKnee, TEXT("Als.Unarmed Attack Type.Right Knee"))
	UE_DEFINE_GAMEPLAY_TAG(Head, TEXT("Als.Unarmed Attack Type.Head"))
}

namespace ALSXTAttackTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(LeftFist, TEXT("Als.Attack Type.Left Fist"))
	UE_DEFINE_GAMEPLAY_TAG(RightFist, TEXT("Als.Attack Type.Right Fist"))
	UE_DEFINE_GAMEPLAY_TAG(LeftFoot, TEXT("Als.Attack Type.Left Foot"))
	UE_DEFINE_GAMEPLAY_TAG(RightFoot, TEXT("Als.Attack Type.Right Foot"))
	UE_DEFINE_GAMEPLAY_TAG(LeftElbow, TEXT("Als.Attack Type.Left Elbow"))
	UE_DEFINE_GAMEPLAY_TAG(RightElbow, TEXT("Als.Attack Type.Right Elbow"))
	UE_DEFINE_GAMEPLAY_TAG(LeftKnee, TEXT("Als.Attack Type.Left Knee"))
	UE_DEFINE_GAMEPLAY_TAG(RightKnee, TEXT("Als.Attack Type.Right Knee"))
	UE_DEFINE_GAMEPLAY_TAG(Head, TEXT("Als.Attack Type.Head"))
}

namespace ALSXTWeaponFirearmStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.Weapon Firearm Stance.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(PatrolReady, TEXT("Als.Weapon Firearm Stance.Canted Left"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.Weapon Firearm Stance.Canted Right"))
}

namespace ALSXTWeaponCarryPositionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Concealed, TEXT("Als.Weapon Carry Position.Concealed"))
	UE_DEFINE_GAMEPLAY_TAG(SlingChest, TEXT("Als.Weapon Carry Position.Sling.Chest"))
	UE_DEFINE_GAMEPLAY_TAG(SlingOffSideDrop, TEXT("Als.Weapon Carry Position.Sling.OffSideDrop"))
	UE_DEFINE_GAMEPLAY_TAG(SlingRearSweep, TEXT("Als.Weapon Carry Position.Sling.RearSweep"))
	UE_DEFINE_GAMEPLAY_TAG(SlingClimber, TEXT("Als.Weapon Carry Position.Sling.Climber"))
	UE_DEFINE_GAMEPLAY_TAG(HolsterChest, TEXT("Als.Weapon Carry Position.Holster.Chest"))
	UE_DEFINE_GAMEPLAY_TAG(HolsterHip, TEXT("Als.Weapon Carry Position.Holster.Hip"))
	UE_DEFINE_GAMEPLAY_TAG(HolsterLeg, TEXT("Als.Weapon Carry Position.Holster.Leg"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeChestSheath, TEXT("Als.Weapon Carry Position.Melee.ChestSheath"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeBelt, TEXT("Als.Weapon Carry Position.Melee.Belt"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeArm, TEXT("Als.Weapon Carry Position.Melee.Arm"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeLeg, TEXT("Als.Weapon Carry Position.Melee.Leg"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeBack, TEXT("Als.Weapon Carry Position.Melee.Back"))
}

namespace ALSXTWeaponReadyPositionTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Weapon Ready Position.None"))
	UE_DEFINE_GAMEPLAY_TAG(Carry, TEXT("Als.Weapon Ready Position.Carry"))
	UE_DEFINE_GAMEPLAY_TAG(PatrolReady, TEXT("Als.Weapon Ready Position.Patrol Ready"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.Weapon Ready Position.Low Ready"))
	UE_DEFINE_GAMEPLAY_TAG(HighReady, TEXT("Als.Weapon Ready Position.High Ready"))
	UE_DEFINE_GAMEPLAY_TAG(Sul, TEXT("Als.Weapon Ready Position.Sul"))
	UE_DEFINE_GAMEPLAY_TAG(AroundCover, TEXT("Als.Weapon Ready Position.Around Cover"))
	UE_DEFINE_GAMEPLAY_TAG(Retention, TEXT("Als.Weapon Ready Position.Retention"))
	UE_DEFINE_GAMEPLAY_TAG(Hip, TEXT("Als.Weapon Ready Position.Hip"))
	UE_DEFINE_GAMEPLAY_TAG(Ready, TEXT("Als.Weapon Ready Position.Ready"))
	UE_DEFINE_GAMEPLAY_TAG(Aiming, TEXT("Als.Weapon Ready Position.Aiming"))
	UE_DEFINE_GAMEPLAY_TAG(Hidden, TEXT("Als.Weapon Ready Position.Hidden"))
}

namespace ALSXTDefensiveModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Defensive Mode.None"))
	UE_DEFINE_GAMEPLAY_TAG(Avoiding, TEXT("Als.Defensive Mode.Avoiding"))
	UE_DEFINE_GAMEPLAY_TAG(Anticipation, TEXT("Als.Defensive Mode.Anticipation"))
	UE_DEFINE_GAMEPLAY_TAG(Blocking, TEXT("Als.Defensive Mode.Blocking"))
	UE_DEFINE_GAMEPLAY_TAG(ClutchImpactPoint, TEXT("Als.Defensive Mode.Clutch Impact Point"))
	UE_DEFINE_GAMEPLAY_TAG(BraceForImpact, TEXT("Als.Defensive Mode.Brace For Impact"))
}

namespace ALSXTBlockingTags
{
	UE_DEFINE_GAMEPLAY_TAG(NotBlocking, TEXT("Als.Blocking.Not Blocking"))
	UE_DEFINE_GAMEPLAY_TAG(Blocking, TEXT("Als.Blocking.Blocking"))
}

namespace ALSXTStationaryModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Static, TEXT("Als.Stationary Mode.Static"))
	UE_DEFINE_GAMEPLAY_TAG(Interaction, TEXT("Als.Stationary Mode.Interaction"))
	UE_DEFINE_GAMEPLAY_TAG(VehicleDriver, TEXT("Als.Stationary Mode.Vehicle Driver"))
	UE_DEFINE_GAMEPLAY_TAG(VehiclePassenger, TEXT("Als.Stationary Mode.Vehicle Passenger"))
}

namespace ALSXTStaticStationaryModeTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Seat, TEXT("Als.Static Stationary Mode Type.Seat"))
	UE_DEFINE_GAMEPLAY_TAG(Surface, TEXT("Als.Static Stationary Mode Type.Surface"))
	UE_DEFINE_GAMEPLAY_TAG(Ground, TEXT("Als.Static Stationary Mode Type.Ground"))
}

namespace ALSXTVehicleStationaryModeTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Driver, TEXT("Als.Vehicle Stationary Mode Type.Driver"))
	UE_DEFINE_GAMEPLAY_TAG(Passenger, TEXT("Als.Vehicle Stationary Mode Type.Passenger"))
}

namespace ALSXTVehicleSeatSideTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Vehicle Seat Side.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Vehicle Seat Side.Right"))
}

namespace ALSXTEmoteTags
{
	UE_DEFINE_GAMEPLAY_TAG(RunningMan, TEXT("Als.Emote.Running Man"))
}

namespace ALSXTGestureTags
{
	UE_DEFINE_GAMEPLAY_TAG(Point, TEXT("Als.Gesture.Point"))
}

namespace ALSXTHandTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Hand.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Hand.Right"))
}

namespace ALSXTWeaponObstructionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Environment, TEXT("Als.Weapon Obstruction.Environment"))
	UE_DEFINE_GAMEPLAY_TAG(Flagging, TEXT("Als.Weapon Obstruction.Flagging"))
}

namespace ALSXTStaminaTags
{
	UE_DEFINE_GAMEPLAY_TAG(Full, TEXT("Als.Stamina.Full"))
	UE_DEFINE_GAMEPLAY_TAG(Optimal, TEXT("Als.Stamina.Optimal"))
	UE_DEFINE_GAMEPLAY_TAG(Half, TEXT("Als.Stamina.Half"))
	UE_DEFINE_GAMEPLAY_TAG(Low, TEXT("Als.Stamina.Low"))
	UE_DEFINE_GAMEPLAY_TAG(Empty, TEXT("Als.Stamina.Empty"))
}

namespace ALSXTActionStrengthTags
{
	UE_DEFINE_GAMEPLAY_TAG(Light, TEXT("Als.Action Strength.Light"))
	UE_DEFINE_GAMEPLAY_TAG(Medium, TEXT("Als.Action Strength.Medium"))
	UE_DEFINE_GAMEPLAY_TAG(Heavy, TEXT("Als.Action Strength.Heavy"))
}

namespace ALSXTAttackMethodTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cancelled, TEXT("Als.Attack Method.Cancelled"))
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.Attack Method.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(Riposte, TEXT("Als.Attack Method.Riposte"))
	UE_DEFINE_GAMEPLAY_TAG(Unique, TEXT("Als.Attack Method.Unique"))
	UE_DEFINE_GAMEPLAY_TAG(Grapple, TEXT("Als.Attack Method.Grapple"))
	UE_DEFINE_GAMEPLAY_TAG(Throw, TEXT("Als.Attack Method.Throw"))
	UE_DEFINE_GAMEPLAY_TAG(KnockOut, TEXT("Als.Attack Method.Knock Out"))
	UE_DEFINE_GAMEPLAY_TAG(TakeDown, TEXT("Als.Attack Method.Take Down"))
	UE_DEFINE_GAMEPLAY_TAG(Cybernetic, TEXT("Als.Attack Method.Cybernetic"))
	UE_DEFINE_GAMEPLAY_TAG(Device, TEXT("Als.Attack Method.Device"))
}

namespace ALSXTDefenseTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.Defense Type.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(Parry, TEXT("Als.Defense Type.Parry"))
	UE_DEFINE_GAMEPLAY_TAG(Dodge, TEXT("Als.Defense Type.Dodge"))
	UE_DEFINE_GAMEPLAY_TAG(Redirection, TEXT("Als.Defense Type.Redirection"))
	UE_DEFINE_GAMEPLAY_TAG(Cybernetic, TEXT("Als.Defense Type.Cybernetic"))
	UE_DEFINE_GAMEPLAY_TAG(Device, TEXT("Als.Defense Type.Device"))
}

namespace ALSXTItemConditionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Prestine, TEXT("Als.Item Condition.Prestine"))
	UE_DEFINE_GAMEPLAY_TAG(Good, TEXT("Als.Item Condition.Good"))
	UE_DEFINE_GAMEPLAY_TAG(Worn, TEXT("Als.Item Condition.Worn"))
	UE_DEFINE_GAMEPLAY_TAG(Ruined, TEXT("Als.Item Condition.Ruined"))
}

namespace ALSXTClothingStateTags
{
	UE_DEFINE_GAMEPLAY_TAG(HoodUp, TEXT("Als.Clothing State.Hood Up"))
	UE_DEFINE_GAMEPLAY_TAG(Open, TEXT("Als.Clothing State.Open"))
	UE_DEFINE_GAMEPLAY_TAG(Closed, TEXT("Als.Clothing State.Closed"))
	UE_DEFINE_GAMEPLAY_TAG(Tucked, TEXT("Als.Clothing State.Tucked"))
	UE_DEFINE_GAMEPLAY_TAG(LeftSleeveUp, TEXT("Als.Clothing State.Left Sleeve Up"))
	UE_DEFINE_GAMEPLAY_TAG(RightSleeveUp, TEXT("Als.Clothing State.Right Sleeve Up"))
}

namespace ALSXTGripPositionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, FName{ TEXTVIEW("Als.Grip Position.Default") })
	UE_DEFINE_GAMEPLAY_TAG(UnderBarrel, FName{ TEXTVIEW("Als.Grip Position.Under Barrel") })
}

namespace ALSXTForegripPositionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, FName{ TEXTVIEW("Als.Foregrip Position.Default") })
	UE_DEFINE_GAMEPLAY_TAG(MagazineWell, FName{ TEXTVIEW("Als.Foregrip Position.Magazine Well") })
	UE_DEFINE_GAMEPLAY_TAG(VerticalGrip, FName{ TEXTVIEW("Als.Foregrip Position.Vertical Grip") })
	UE_DEFINE_GAMEPLAY_TAG(AngledForeGrip, FName{ TEXTVIEW("Als.Foregrip Position.Angled Fore Grip") })
	UE_DEFINE_GAMEPLAY_TAG(TopHorizontalGrip, FName{ TEXTVIEW("Als.Foregrip Position.Top Horizontal Grip") })
}

namespace ALSXTCharacterMovementSoundTags
{
	UE_DEFINE_GAMEPLAY_TAG(Walking, FName{ TEXTVIEW("Als.Character Movement Sound.Walking") })
	UE_DEFINE_GAMEPLAY_TAG(Running, FName{ TEXTVIEW("Als.Character Movement Sound.Running") })
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, FName{ TEXTVIEW("Als.Character Movement Sound.Sprinting") })
	UE_DEFINE_GAMEPLAY_TAG(Jumping, FName{ TEXTVIEW("Als.Character Movement Sound.Jumping") })
	UE_DEFINE_GAMEPLAY_TAG(Crouch, FName{ TEXTVIEW("Als.Character Movement Sound.Crouch") })
	UE_DEFINE_GAMEPLAY_TAG(Uncrouch, FName{ TEXTVIEW("Als.Character Movement Sound.Uncrouch") })
	UE_DEFINE_GAMEPLAY_TAG(GettingUp, FName{ TEXTVIEW("Als.Character Movement Sound.Getting Up") })
	UE_DEFINE_GAMEPLAY_TAG(Sliding, FName{ TEXTVIEW("Als.Character Movement Sound.Sliding") })
	UE_DEFINE_GAMEPLAY_TAG(Equip, FName{ TEXTVIEW("Als.Character Movement Sound.Equip") })
	UE_DEFINE_GAMEPLAY_TAG(Unequip, FName{ TEXTVIEW("Als.Character Movement Sound.Unequip") })
	UE_DEFINE_GAMEPLAY_TAG(ChangeCombatStance, FName{ TEXTVIEW("Als.Character Movement Sound.Change Combat Stance") })
	UE_DEFINE_GAMEPLAY_TAG(Impact, FName{ TEXTVIEW("Als.Character Movement Sound.Impact") })
	UE_DEFINE_GAMEPLAY_TAG(Blocking, FName{ TEXTVIEW("Als.Character Movement Sound.Blocking") })
	UE_DEFINE_GAMEPLAY_TAG(ChangeWeaponReadyPosition, FName{ TEXTVIEW("Als.Character Movement Sound.Change Weapon Ready Position") })
	UE_DEFINE_GAMEPLAY_TAG(ChangeGripPosition, FName{ TEXTVIEW("Als.Character Movement Sound.Change Grip Position") })
	UE_DEFINE_GAMEPLAY_TAG(ChangeForegripPosition, FName{ TEXTVIEW("Als.Character Movement Sound.Change Foregrip Position") })
	UE_DEFINE_GAMEPLAY_TAG(ChangeSightPosition, FName{ TEXTVIEW("Als.Character Movement Sound.Change Sight Position") })
	UE_DEFINE_GAMEPLAY_TAG(Gesture, FName{ TEXTVIEW("Als.Character Movement Sound.Gesture") })
	UE_DEFINE_GAMEPLAY_TAG(MantlingLow, FName{ TEXTVIEW("Als.Character Movement Sound.Mantling Low") })
	UE_DEFINE_GAMEPLAY_TAG(MantlingHigh, FName{ TEXTVIEW("Als.Character Movement Sound.Mantling High") })
	UE_DEFINE_GAMEPLAY_TAG(Vaulting, FName{ TEXTVIEW("Als.Character Movement Sound.Vaulting") })
	UE_DEFINE_GAMEPLAY_TAG(Leaning, FName{ TEXTVIEW("Als.Character Movement Sound.Leaning") })
	UE_DEFINE_GAMEPLAY_TAG(Flip, FName{ TEXTVIEW("Als.Character Movement Sound.Flip") })
}

namespace ALSXTObjectWeightTags
{
	UE_DEFINE_GAMEPLAY_TAG(Stealth, FName{ TEXTVIEW("Als.Object Weight.Stealth") })
	UE_DEFINE_GAMEPLAY_TAG(VeryLight, FName{ TEXTVIEW("Als.Object Weight.VeryLight") })
	UE_DEFINE_GAMEPLAY_TAG(Light, FName{ TEXTVIEW("Als.Object Weight.Light") })
	UE_DEFINE_GAMEPLAY_TAG(Default, FName{ TEXTVIEW("Als.Object Weight.Default") })
	UE_DEFINE_GAMEPLAY_TAG(Heavy, FName{ TEXTVIEW("Als.Object Weight.Heavy") })
	UE_DEFINE_GAMEPLAY_TAG(VeryHeavy, FName{ TEXTVIEW("Als.Object Weight.VeryHeavy") })
	UE_DEFINE_GAMEPLAY_TAG(ExtremelyHeavy, FName{ TEXTVIEW("Als.Object Weight.ExtremelyHeavy") })
	UE_DEFINE_GAMEPLAY_TAG(MassivelyHeavy, FName{ TEXTVIEW("Als.Object Weight.MassivelyHeavy") })
}

namespace ALSXTWeaponTags
{
	UE_DEFINE_GAMEPLAY_TAG(M4, FName{ TEXTVIEW("Als.Weapon.M4") })
	UE_DEFINE_GAMEPLAY_TAG(Pistol, FName{ TEXTVIEW("Als.Weapon.Pistol") })
	UE_DEFINE_GAMEPLAY_TAG(PistolTwoHanded, FName{ TEXTVIEW("Als.Weapon.Pistol Two Handed") })
	UE_DEFINE_GAMEPLAY_TAG(Knife, FName{ TEXTVIEW("Als.Weapon.Knife") })
}

namespace ALSXTWeaponActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Reloading, FName{ TEXTVIEW("Als.Weapon Action.Reloading") })
	UE_DEFINE_GAMEPLAY_TAG(Cycling, FName{ TEXTVIEW("Als.Weapon Action.Cycling") })
	UE_DEFINE_GAMEPLAY_TAG(InpectingMagazine, FName{ TEXTVIEW("Als.Weapon Action.Inpecting Magazine") })
	UE_DEFINE_GAMEPLAY_TAG(InspectingChamber, FName{ TEXTVIEW("Als.Weapon Action.Inspecting Chamber") })
	UE_DEFINE_GAMEPLAY_TAG(ClearingJam, FName{ TEXTVIEW("Als.Weapon Action.Clearing Jam") })
}

namespace ALSXTMotionSoundAreaTags
{
	UE_DEFINE_GAMEPLAY_TAG(Upper, FName{ TEXTVIEW("Als.Motion Sound Area.Upper") })
	UE_DEFINE_GAMEPLAY_TAG(Lower, FName{ TEXTVIEW("Als.Motion Sound Area.Lower") })
}

namespace ALSXTBreathTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Regular, FName{ TEXTVIEW("Als.Breath Type.Regular") })
	UE_DEFINE_GAMEPLAY_TAG(Coughing, FName{ TEXTVIEW("Als.Breath Type.Coughing") })
	UE_DEFINE_GAMEPLAY_TAG(Cold, FName{ TEXTVIEW("Als.Breath Type.Cold") })
	UE_DEFINE_GAMEPLAY_TAG(Injured, FName{ TEXTVIEW("Als.Breath Type.Injured") })
}

namespace ALSXTMeshPaintTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(BloodDamage, FName{ TEXTVIEW("Als.Mesh Paint Type.Blood Damage") })
	UE_DEFINE_GAMEPLAY_TAG(SurfaceDamage, FName{ TEXTVIEW("Als.Mesh Paint Type.Surface Damage") })
	UE_DEFINE_GAMEPLAY_TAG(BackSpatter, FName{ TEXTVIEW("Als.Mesh Paint Type.Back Spatter") })
	UE_DEFINE_GAMEPLAY_TAG(Saturation, FName{ TEXTVIEW("Als.Mesh Paint Type.Saturation") })
	UE_DEFINE_GAMEPLAY_TAG(Burn, FName{ TEXTVIEW("Als.Mesh Paint Type.Burn") })
}