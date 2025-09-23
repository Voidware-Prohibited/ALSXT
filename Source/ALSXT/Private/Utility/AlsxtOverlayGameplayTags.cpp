#include "Utility/AlsxtOverlayGameplayTags.h"

namespace AlsxtOverlayLeftHandMirrorPolicyTags
{
	UE_DEFINE_GAMEPLAY_TAG(Mirror, TEXT("Als.OverlayLeftHandMirrorPolicy.Mirror"))
	UE_DEFINE_GAMEPLAY_TAG(Unique, TEXT("Als.OverlayLeftHandMirrorPolicy.Unique"))
}

namespace AlsxtOverlaySettingsTags
{
	UE_DEFINE_GAMEPLAY_TAG(Aimable, TEXT("Als.OverlaySettings.Aimable"))
	UE_DEFINE_GAMEPLAY_TAG(UseGunBoneForOverlayObjects, TEXT("Als.OverlaySettings.UseGunBoneForOverlayObjects"))
	UE_DEFINE_GAMEPLAY_TAG(LeftHandIK, TEXT("Als.OverlaySettings.LeftHandIK"))
}

namespace AlsxtOverlaySlotTags
{
	UE_DEFINE_GAMEPLAY_TAG(Left, TEXT("Als.OverlaySlotHand.Left"))
	UE_DEFINE_GAMEPLAY_TAG(Right, TEXT("Als.OverlaySlotHand.Right"))
	UE_DEFINE_GAMEPLAY_TAG(TwoHanded, TEXT("Als.OverlaySlotHand.TwoHanded"))
}

namespace AlsxtOverlayStanceTags
{
	UE_DEFINE_GAMEPLAY_TAG(TwoHanded, TEXT("Als.OverlaySlot.TwoHanded"))
	UE_DEFINE_GAMEPLAY_TAG(Active, TEXT("Als.OverlaySlot.Active"))
	UE_DEFINE_GAMEPLAY_TAG(Passive, TEXT("Als.OverlaySlot.Passive"))
	UE_DEFINE_GAMEPLAY_TAG(Neutral, TEXT("Als.OverlaySlot.Neutral"))
}