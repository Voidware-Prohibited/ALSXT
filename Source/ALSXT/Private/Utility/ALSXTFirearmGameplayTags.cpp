#include "Utility/ALSXTFirearmGameplayTags.h"

namespace ALSXTFirearmTags
{
	UE_DEFINE_GAMEPLAY_TAG(Grenade, TEXT("Als.OverlayMode.Grenade"))
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

namespace ALSXTFirearmFingerPoseTags
{
	UE_DEFINE_GAMEPLAY_TAG(OutsideGuard, TEXT("Als.Firearm Finger Pose.Outside Guard"))
	UE_DEFINE_GAMEPLAY_TAG(OnTrigger, TEXT("Als.Firearm Finger Pose.On Trigger"))
}

namespace ALSXTFirearmFingerActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("Als.Firearm Finger Action.None"))
	UE_DEFINE_GAMEPLAY_TAG(MagazineRelease, TEXT("Als.Firearm Finger Action.Magazine Release"))
	UE_DEFINE_GAMEPLAY_TAG(SafetyOn, TEXT("Als.Firearm Finger Action.Safety On"))
	UE_DEFINE_GAMEPLAY_TAG(SafetyOff, TEXT("Als.Firearm Finger Action.Safety Off"))
	UE_DEFINE_GAMEPLAY_TAG(SelectUp, TEXT("Als.Firearm Finger Action.Select Up"))
	UE_DEFINE_GAMEPLAY_TAG(SelectDown, TEXT("Als.Firearm Finger Action.Select Down"))
	UE_DEFINE_GAMEPLAY_TAG(PullingTrigger, TEXT("Als.Firearm Finger Action.Pulling Trigger"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingAccessory1, TEXT("Als.Firearm Finger Action.Toggling Accessory1"))
	UE_DEFINE_GAMEPLAY_TAG(TogglingAccessory2, TEXT("Als.Firearm Finger Action.Toggling Accessory2"))
}

namespace ALSXTFirearmSightLocationTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ironsights, TEXT("Als.Firearm Sight Location.Ironsights"))
	UE_DEFINE_GAMEPLAY_TAG(ReceiverTop, TEXT("Als.Firearm Sight Location.Receiver Top"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelTop, TEXT("Als.Firearm Sight Location.Barrel Top"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelLeft, TEXT("Als.Firearm Sight Location.Barrel Left"))
	UE_DEFINE_GAMEPLAY_TAG(BarrelRight, TEXT("Als.Firearm Sight Location.Barrel Right"))
}