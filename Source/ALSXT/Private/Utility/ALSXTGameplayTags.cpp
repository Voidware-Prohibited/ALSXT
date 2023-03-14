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

namespace ALSXTActionStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Prone, TEXT("Als.Action Stance.Prone"))
	UE_DEFINE_GAMEPLAY_TAG(Crouched, TEXT("Als.Action Stance.Crouched"))
	UE_DEFINE_GAMEPLAY_TAG(Standing, TEXT("Als.Action Stance.Standing"))
	UE_DEFINE_GAMEPLAY_TAG(InAir, TEXT("Als.Action Stance.In Air"))
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
	UE_DEFINE_GAMEPLAY_TAG(Bump, TEXT("Als.Impact Type.Bump"))
	UE_DEFINE_GAMEPLAY_TAG(Hit, TEXT("Als.Impact Type.Hit"))
	UE_DEFINE_GAMEPLAY_TAG(Fall, TEXT("Als.Impact Type.Fall"))
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
	UE_DEFINE_GAMEPLAY_TAG(Above, TEXT("Als.Impact Side.Above"))
	UE_DEFINE_GAMEPLAY_TAG(Below, TEXT("Als.Impact Side.Below"))
}

namespace ALSXTImpactSurfaceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.Impact Surface.Cloth"))
}

namespace ALSXTBloodSpatterDistanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(LessThanTwo, TEXT("Als.Blood Spatter Distance.Less than Two"))
	UE_DEFINE_GAMEPLAY_TAG(TwoToFive, TEXT("Als.Blood Spatter Distance.Two to Five"))
	UE_DEFINE_GAMEPLAY_TAG(FiveToMax, TEXT("Als.Blood Spatter Distance.FiveToMax"))
	UE_DEFINE_GAMEPLAY_TAG(Max, TEXT("Als.Blood Spatter Distance.Max"))
}

namespace ALSXTDamageTypeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Impact, TEXT("Game.Damge Type.Impact"))
	UE_DEFINE_GAMEPLAY_TAG(Edge, TEXT("Game.Damge Type.Edge"))
	UE_DEFINE_GAMEPLAY_TAG(Burn, TEXT("Game.Damge Type.Burn"))
	UE_DEFINE_GAMEPLAY_TAG(Explosion, TEXT("Game.Damge Type.Explosion"))
	UE_DEFINE_GAMEPLAY_TAG(Gas, TEXT("Game.Damge Type.Gas"))
	UE_DEFINE_GAMEPLAY_TAG(Poison, TEXT("Game.Damge Type.Poison"))
	UE_DEFINE_GAMEPLAY_TAG(Illness, TEXT("Game.Damge Type.Illness"))
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
	UE_DEFINE_GAMEPLAY_TAG(Sul, TEXT("Als.Weapon Ready Position.Sul"))
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

namespace ALSXTDefensiveModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Defensive Mode.None"))
	UE_DEFINE_GAMEPLAY_TAG(Avoiding, TEXT("Als.Defensive Mode.Avoiding"))
	UE_DEFINE_GAMEPLAY_TAG(Blocking, TEXT("Als.Defensive Mode.Blocking"))
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

namespace ALSXTWeaponObstructionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Environment, TEXT("Als.Weapon Obstruction.Environment"))
	UE_DEFINE_GAMEPLAY_TAG(Flagging, TEXT("Als.Weapon Obstruction.Flagging"))
}

namespace ALSXTStaminaTags
{
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
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.Attack Method.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(Riposte, TEXT("Als.Attack Method.Riposte"))
	UE_DEFINE_GAMEPLAY_TAG(Special, TEXT("Als.Attack Method.Special"))
	UE_DEFINE_GAMEPLAY_TAG(Grapple, TEXT("Als.Attack Method.Grapple"))
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

namespace ALSXTClothingStateTags
{
	UE_DEFINE_GAMEPLAY_TAG(HoodUp, TEXT("Als.Clothing State.Hood Up"))
	UE_DEFINE_GAMEPLAY_TAG(Open, TEXT("Als.Clothing State.Open"))
	UE_DEFINE_GAMEPLAY_TAG(Closed, TEXT("Als.Clothing State.Closed"))
	UE_DEFINE_GAMEPLAY_TAG(Tucked, TEXT("Als.Clothing State.Tucked"))
	UE_DEFINE_GAMEPLAY_TAG(LeftSleeveUp, TEXT("Als.Clothing State.Left Sleeve Up"))
	UE_DEFINE_GAMEPLAY_TAG(RightSleeveUp, TEXT("Als.Clothing State.Right Sleeve Up"))
}