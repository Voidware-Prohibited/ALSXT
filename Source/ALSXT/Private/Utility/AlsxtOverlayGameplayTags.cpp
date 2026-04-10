#include "Utility/AlsxtOverlayGameplayTags.h"

namespace AlsxtTags
{
	namespace Animation
	{
		namespace Blending
		{
			namespace Layer
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Both, TEXT("Alsxt.Animation.Blending.Layer.Both"), "Layer uses Both Arms")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Right, TEXT("Alsxt.Animation.Blending.Layer.Right"), "Layer uses Left Arm Only")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Left, TEXT("Alsxt.Animation.Blending.Layer.Left"), "Layer uses Right Arm Only")
			}
			namespace LayerPosture
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(FullBody, TEXT("Alsxt.Animation.Blending.LayerPosture.FullBody"), "Full Body Posture")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Active, TEXT("Alsxt.Animation.Blending.LayerPosture.Active"), "Active or Forward Posture")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Passive, TEXT("Alsxt.Animation.Blending.LayerPosture.Passive"), "Passive or Backward Posture")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Neutral, TEXT("Alsxt.Animation.Blending.LayerPosture.Neutral"), "Neutral or Centered Posture")
			}
			namespace LayerSettings
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(UniqueCombatStanceAssets, TEXT("Alsxt.Animation.Blending.LayerSettings.UniqueCombatStanceAssets"), "Als.Animation.UniqueCombatStanceAssets")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Aimable, TEXT("Alsxt.Animation.Blending.LayerSettings.Aimable"), "Als.Animation.Mirror")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(UseGunBoneForOverlayObjects, TEXT("Alsxt.Animation.Blending.LayerSettings.UseGunBoneForOverlayObjects"), "Als.Animation.Mirror")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(UseAssetsForLeaning, TEXT("Alsxt.Animation.Blending.LayerSettings.UseAssetsForLeaning"), "Als.Animation.Mirror")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondaryHandIK, TEXT("Alsxt.Animation.Blending.LayerSettings.SecondaryHandIK"), "Secondary Hand holding an Object will use Inverse Kinematics for its Grip or Socket")
			}
		}
	}
}



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