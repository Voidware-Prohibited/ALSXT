#include "Utility/AlsxtOverlayGameplayTags.h"

namespace AlsxtOverlayLeftHandMirrorPolicyTags
{
	UE_DEFINE_GAMEPLAY_TAG(Mirror, TEXT("Als.OverlayLeftHandMirrorPolicy.Mirror"))
	UE_DEFINE_GAMEPLAY_TAG(Unique, TEXT("Als.OverlayLeftHandMirrorPolicy.Unique"))
}

namespace AlsxtOverlaySettingsTags
{
	UE_DEFINE_GAMEPLAY_TAG(Mirror, TEXT("Als.OverlaySettings.Mirror"))
	UE_DEFINE_GAMEPLAY_TAG(Aimable, TEXT("Als.OverlaySettings.Aimable"))
	UE_DEFINE_GAMEPLAY_TAG(UseGunBoneForOverlayObjects, TEXT("Als.OverlaySettings.UseGunBoneForOverlayObjects"))
	UE_DEFINE_GAMEPLAY_TAG(LeftHandIK, TEXT("Als.OverlaySettings.LeftHandIK"))
}

namespace AlsxtOverlaySlotTags
{
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.OverlaySlot.Right"))
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.OverlaySlot.Left"))
	UE_DEFINE_GAMEPLAY_TAG(TwoHanded, TEXT("Als.OverlaySlot.TwoHanded"))
}

namespace AlsxtOverlaySlotStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(TwoHanded, TEXT("Als.OverlaySlotStance.TwoHanded"))
	UE_DEFINE_GAMEPLAY_TAG(Active, TEXT("Als.OverlaySlotStance.Active"))
	UE_DEFINE_GAMEPLAY_TAG(Passive, TEXT("Als.OverlaySlotStance.Passive"))
	UE_DEFINE_GAMEPLAY_TAG(Neutral, TEXT("Als.OverlaySlotStance.Neutral"))
}