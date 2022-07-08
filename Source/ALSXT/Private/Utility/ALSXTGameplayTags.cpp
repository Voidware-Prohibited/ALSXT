#include "Utility/ALSXTGameplayTags.h"

namespace ALSXTOverlayModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(AxeOneHanded, TEXT("Als.OverlayMode.AxeOneHanded"))
	UE_DEFINE_GAMEPLAY_TAG(AxeTwoHanded, TEXT("Als.OverlayMode.AxeTwoHanded"))
	UE_DEFINE_GAMEPLAY_TAG(Pickaxe, TEXT("Als.OverlayMode.Pickaxe"))
	UE_DEFINE_GAMEPLAY_TAG(Knife, TEXT("Als.OverlayMode.Knife"))
	UE_DEFINE_GAMEPLAY_TAG(Hammer, TEXT("Als.OverlayMode.Hammer"))
	UE_DEFINE_GAMEPLAY_TAG(Grenade, TEXT("Als.OverlayMode.Grenade"))
	UE_DEFINE_GAMEPLAY_TAG(BuildingPlan, TEXT("Als.OverlayMode.BuildingPlan"))
	UE_DEFINE_GAMEPLAY_TAG(Flashlight, TEXT("Als.OverlayMode.Flashlight"))
	UE_DEFINE_GAMEPLAY_TAG(Katana, TEXT("Als.OverlayMode.Katana"))
	UE_DEFINE_GAMEPLAY_TAG(Sword, TEXT("Als.OverlayMode.Sword"))
}


namespace ALSXTLocomotionActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Vaulting, TEXT("Als.LocomotionAction.Vaulting"))
	UE_DEFINE_GAMEPLAY_TAG(Sliding, TEXT("Als.LocomotionAction.Sliding"))
	UE_DEFINE_GAMEPLAY_TAG(WallJumping, TEXT("Als.LocomotionAction.WallJumping"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunning, TEXT("Als.LocomotionAction.WallRunning"))
	UE_DEFINE_GAMEPLAY_TAG(Unarmed, TEXT("Als.LocomotionAction.Attacking.Unarmed"))
	UE_DEFINE_GAMEPLAY_TAG(MeleeWeapon, TEXT("Als.LocomotionAction.Attacking.MeleeWeapon"))
}

namespace ALSXTSexTags
{
	UE_DEFINE_GAMEPLAY_TAG(Male, TEXT("Als.Sex.Male"))
	UE_DEFINE_GAMEPLAY_TAG(Female, TEXT("Als.Sex.Female"))
}

namespace ALSXTLocomotionVariantTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, TEXT("Als.LocomotionVariant.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Injured, TEXT("Als.LocomotionVariant.Injured"))
	UE_DEFINE_GAMEPLAY_TAG(Style01, TEXT("Als.LocomotionVariant.Style01"))
	UE_DEFINE_GAMEPLAY_TAG(Style02, TEXT("Als.LocomotionVariant.Style02"))
	UE_DEFINE_GAMEPLAY_TAG(Style03, TEXT("Als.LocomotionVariant.Style03"))
}

namespace ALSXTInjuryTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Injury.None"))
	UE_DEFINE_GAMEPLAY_TAG(Head, TEXT("Als.Injury.Head"))
	UE_DEFINE_GAMEPLAY_TAG(Abdomen, TEXT("Als.Injury.Abdomen"))
	UE_DEFINE_GAMEPLAY_TAG(LeftArm, TEXT("Als.Injury.LeftArm"))
	UE_DEFINE_GAMEPLAY_TAG(RightArm, TEXT("Als.Injury.RightArm"))
	UE_DEFINE_GAMEPLAY_TAG(LeftLeg, TEXT("Als.Injury.LeftLeg"))
	UE_DEFINE_GAMEPLAY_TAG(RightLeg, TEXT("Als.Injury.RightLeg"))
}

namespace ALSXTCombatStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Neutral, TEXT("Als.CombatStance.Neutral"))
	UE_DEFINE_GAMEPLAY_TAG(Ready, TEXT("Als.CombatStance.Ready"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.CombatStance.Aiming"))
	UE_DEFINE_GAMEPLAY_TAG(Focused, TEXT("Als.CombatStance.Focused"))
}

namespace ALSXTWeaponFirearmStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Regular, TEXT("Als.WeaponFirearmStance.Regular"))
	UE_DEFINE_GAMEPLAY_TAG(PatrolReady, TEXT("Als.WeaponFirearmStance.CantedLeft"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.WeaponFirearmStance.CantedRight"))
	UE_DEFINE_GAMEPLAY_TAG(HighReady, TEXT("Als.WeaponFirearmStance.Underbarrel"))
}

namespace ALSXTWeaponReadyPositionTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.WeaponReadyPosition.None"))
	UE_DEFINE_GAMEPLAY_TAG(Carry, TEXT("Als.WeaponReadyPosition.Carry"))
	UE_DEFINE_GAMEPLAY_TAG(PatrolReady, TEXT("Als.WeaponReadyPosition.PatrolReady"))
	UE_DEFINE_GAMEPLAY_TAG(LowReady, TEXT("Als.WeaponReadyPosition.LowReady"))
	UE_DEFINE_GAMEPLAY_TAG(HighReady, TEXT("Als.WeaponReadyPosition.HighReady"))
	UE_DEFINE_GAMEPLAY_TAG(AroundCover, TEXT("Als.WeaponReadyPosition.AroundCover"))
	UE_DEFINE_GAMEPLAY_TAG(Retention, TEXT("Als.WeaponReadyPosition.Retention"))
	UE_DEFINE_GAMEPLAY_TAG(Hip, TEXT("Als.WeaponReadyPosition.Hip"))
	UE_DEFINE_GAMEPLAY_TAG(Ready, TEXT("Als.WeaponReadyPosition.Ready"))
	UE_DEFINE_GAMEPLAY_TAG(Aiming, TEXT("Als.WeaponReadyPosition.Aiming"))
	UE_DEFINE_GAMEPLAY_TAG(Hidden, TEXT("Als.WeaponReadyPosition.Hidden"))
}