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
	UE_DEFINE_GAMEPLAY_TAG(BumpReaction, TEXT("Als.LocomotionAction.Bump Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(HitReaction, TEXT("Als.LocomotionAction.Hit Reaction"))
	UE_DEFINE_GAMEPLAY_TAG(ThrowDefault, TEXT("Als.LocomotionAction.Throw.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Vaulting, TEXT("Als.LocomotionAction.Vaulting"))
	UE_DEFINE_GAMEPLAY_TAG(Sliding, TEXT("Als.LocomotionAction.Sliding"))
	UE_DEFINE_GAMEPLAY_TAG(StandingBackflip, TEXT("Als.LocomotionAction.Acrobatic"))
	UE_DEFINE_GAMEPLAY_TAG(WallJumping, TEXT("Als.LocomotionAction.Wall Jumping"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunning, TEXT("Als.LocomotionAction.Wall Running"))
	UE_DEFINE_GAMEPLAY_TAG(Emote, TEXT("Als.LocomotionAction.Emote"))
	UE_DEFINE_GAMEPLAY_TAG(EnteringStationary, TEXT("Als.LocomotionAction.Entering Stationary"))
	UE_DEFINE_GAMEPLAY_TAG(ExitingStationary, TEXT("Als.LocomotionAction.Exiting Stationary"))
	UE_DEFINE_GAMEPLAY_TAG(PrimaryInteraction, TEXT("Als.LocomotionAction.Primary Interaction"))
	UE_DEFINE_GAMEPLAY_TAG(SecondaryInteraction, TEXT("Als.LocomotionAction.Secondary Interaction"))
	UE_DEFINE_GAMEPLAY_TAG(PrimaryAction, TEXT("Als.LocomotionAction.Primary Action"))
	UE_DEFINE_GAMEPLAY_TAG(SecondaryAction, TEXT("Als.LocomotionAction.Secondary Action"))
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

namespace ALSXTVaultTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(LowWalking, TEXT("Als.Vault Type.Low Walking"))
	UE_DEFINE_GAMEPLAY_TAG(LowRunning, TEXT("Als.Vault Type.Low Running"))
	UE_DEFINE_GAMEPLAY_TAG(LowSprinting, TEXT("Als.Vault Type.Low Sprinting"))
	UE_DEFINE_GAMEPLAY_TAG(LowCrouched, TEXT("Als.Vault Type.Low Crouched"))
	UE_DEFINE_GAMEPLAY_TAG(MediumWalking, TEXT("Als.Vault Type.Medium Walking"))
	UE_DEFINE_GAMEPLAY_TAG(MediumRunning, TEXT("Als.Vault Type.Medium Running"))
	UE_DEFINE_GAMEPLAY_TAG(MediumSprinting, TEXT("Als.Vault Type.Medium Sprinting"))
	UE_DEFINE_GAMEPLAY_TAG(MediumCrouched, TEXT("Als.Vault Type.Medium Crouched"))
	UE_DEFINE_GAMEPLAY_TAG(HighWalking, TEXT("Als.Vault Type.High Walking"))
	UE_DEFINE_GAMEPLAY_TAG(HighRunning, TEXT("Als.Vault Type.High Running"))
	UE_DEFINE_GAMEPLAY_TAG(HighSprinting, TEXT("Als.Vault Type.High Sprinting"))
	UE_DEFINE_GAMEPLAY_TAG(FenceWalking, TEXT("Als.Vault Type.Fence Walking"))
	UE_DEFINE_GAMEPLAY_TAG(FenceRunning, TEXT("Als.Vault Type.Fence Running"))
	UE_DEFINE_GAMEPLAY_TAG(FenceSprinting, TEXT("Als.Vault Type.Fence Sprinting"))
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
	UE_DEFINE_GAMEPLAY_TAG(FireteamLeader, TEXT("Game.Player Game Role.FireteamL eader"))
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

namespace ALSXTStatusTags
{
	UE_DEFINE_GAMEPLAY_TAG(Normal, TEXT("Als.Status.Normal"))
	UE_DEFINE_GAMEPLAY_TAG(KnockedDown, TEXT("Als.Status.Knocked Down"))
	UE_DEFINE_GAMEPLAY_TAG(Unconscious, TEXT("Als.Status.Unconscious"))
	UE_DEFINE_GAMEPLAY_TAG(Dead, TEXT("Als.Status.Dead"))
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
}

namespace ALSXTImpactTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Bump, TEXT("Als.Impact Type.Bump"))
	UE_DEFINE_GAMEPLAY_TAG(Hit, TEXT("Als.Impact Type.Hit"))
}

namespace ALSXTBumpReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(HeadBackLight, TEXT("Als.BumpReaction.Head.Back.Light"))
}

namespace ALSXTBumpSurfaceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.Bump Surface.Cloth"))

}

namespace ALSXTImpactReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(BluntHeadBackLight, TEXT("Als.Hit Reaction.Blunt.Head.Back.Light"))

}

namespace ALSXTImpactReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.Hit Surface.Cloth"))
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

namespace ALSXTWeaponFirearmStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.Weapon Firearm Stance.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(PatrolReady, TEXT("Als.Weapon Firearm Stance.Canted Left"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.Weapon Firearm Stance.Canted Right"))
	UE_DEFINE_GAMEPLAY_TAG(HighReady, TEXT("Als.Weapon Firearm Stance.Underbarrel"))
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
	UE_DEFINE_GAMEPLAY_TAG(AroundCover, TEXT("Als.Weapon Ready Position.Around Cover"))
	UE_DEFINE_GAMEPLAY_TAG(Retention, TEXT("Als.Weapon Ready Position.Retention"))
	UE_DEFINE_GAMEPLAY_TAG(Hip, TEXT("Als.Weapon Ready Position.Hip"))
	UE_DEFINE_GAMEPLAY_TAG(Ready, TEXT("Als.Weapon Ready Position.Ready"))
	UE_DEFINE_GAMEPLAY_TAG(Aiming, TEXT("Als.Weapon Ready Position.Aiming"))
	UE_DEFINE_GAMEPLAY_TAG(Hidden, TEXT("Als.Weapon Ready Position.Hidden"))
}

namespace ALSXTFirearmSightLocationTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ironsights, TEXT("Als.Firearm Sight Location.Ironsights"))
	UE_DEFINE_GAMEPLAY_TAG(ReceiverTop, TEXT("Als.Firearm Sight Location.Receiver Top"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelTop, TEXT("Als.Firearm Sight Location.Barrel Top"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelLeft, TEXT("Als.Firearm Sight Location.Barrel Left"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelRight, TEXT("Als.Firearm Sight Location.Barrel Right"))
}

namespace ALSXTBlockingTags
{
	UE_DEFINE_GAMEPLAY_TAG(NotBlocking, TEXT("Als.Blocking.Not Blocking"))
	UE_DEFINE_GAMEPLAY_TAG(Blocking, TEXT("Als.Blocking.Blocking"))
}

namespace ALSXTStationaryModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Sitting01, TEXT("Als.Stationary Mode.Sitting 01"))
	UE_DEFINE_GAMEPLAY_TAG(SittingFloor01, TEXT("Als.Stationary Mode.Sitting Floor 01"))
	UE_DEFINE_GAMEPLAY_TAG(Petting, TEXT("Als.Stationary Mode.Petting"))
}

namespace ALSXTEmoteTags
{
	UE_DEFINE_GAMEPLAY_TAG(RunningMan, TEXT("Als.Emote.Running Man"))
}

namespace ALSXTGestureTags
{
	UE_DEFINE_GAMEPLAY_TAG(Point, TEXT("Als.Gesture.Point"))
}

namespace ALSXTGestureHandTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Gesture Hand.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Gesture Hand.Right"))
}

namespace ALSXTReloadingTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Drop, TEXT("Als.Reloading Type.Drop"))
	UE_DEFINE_GAMEPLAY_TAG(Retention, TEXT("Als.Reloading Type.Retention"))
	UE_DEFINE_GAMEPLAY_TAG(Tactical, TEXT("Als.Reloading Type.Tactical"))
	UE_DEFINE_GAMEPLAY_TAG(DropAimed, TEXT("Als.Reloading Type.Drop Aimed"))
	UE_DEFINE_GAMEPLAY_TAG(RetentionAimed, TEXT("Als.Reloading Type.Retention Aimed"))
	UE_DEFINE_GAMEPLAY_TAG(TacticalAimed, TEXT("Als.Reloading Type.Tactical Aimed"))
}

namespace ALSXTFirearmFingerActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Firearm Finger Action.None"))
	UE_DEFINE_GAMEPLAY_TAG(OutsideGuard, TEXT("Als.Firearm Finger Action.Outside Guard"))
	UE_DEFINE_GAMEPLAY_TAG(MagazineRelease, TEXT("Als.Firearm Finger Action.Magazine Release"))
	UE_DEFINE_GAMEPLAY_TAG(SafetyOn, TEXT("Als.Firearm Finger Action.Safety On"))
	UE_DEFINE_GAMEPLAY_TAG(SafetyOff, TEXT("Als.Firearm Finger Action.Safety Off"))
	UE_DEFINE_GAMEPLAY_TAG(SelectUp, TEXT("Als.Firearm Finger Action.Select Up"))
	UE_DEFINE_GAMEPLAY_TAG(SelectDown, TEXT("Als.Firearm Finger Action.Select Down"))
	UE_DEFINE_GAMEPLAY_TAG(OnTrigger, TEXT("Als.Firearm Finger Action.On Trigger"))
	UE_DEFINE_GAMEPLAY_TAG(PullingTrigger, TEXT("Als.Firearm Finger Action.Pulling Trigger"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingAccessory1, TEXT("Als.Firearm Finger Action.Toggling Accessory1"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingAccessory2, TEXT("Als.Firearm Finger Action.Toggling Accessory2"))
}

namespace ALSXTFirearmFingerActionHandTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.Firearm Finger Action Hand.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.Firearm Finger Action Hand.Right"))
}