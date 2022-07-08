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