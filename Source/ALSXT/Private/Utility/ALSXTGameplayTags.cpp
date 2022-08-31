#include "Utility/ALSXTGameplayTags.h"
#include "Utility/AlsGameplayTags.h"

namespace AlsOverlayModeTags
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


namespace AlsLocomotionActionTags
{
	UE_DEFINE_GAMEPLAY_TAG(BumpReaction, TEXT("Als.LocomotionAction.BumpReaction"))
	UE_DEFINE_GAMEPLAY_TAG(HitReaction, TEXT("Als.LocomotionAction.HitReaction"))
	UE_DEFINE_GAMEPLAY_TAG(ThrowDefault, TEXT("Als.LocomotionAction.Throw.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Vaulting, TEXT("Als.LocomotionAction.Vaulting"))
	UE_DEFINE_GAMEPLAY_TAG(Sliding, TEXT("Als.LocomotionAction.Sliding"))
	UE_DEFINE_GAMEPLAY_TAG(StandingBackflip, TEXT("Als.LocomotionAction.StandingBackflip"))
	UE_DEFINE_GAMEPLAY_TAG(RunningFlip, TEXT("Als.LocomotionAction.RunningFlip"))
	UE_DEFINE_GAMEPLAY_TAG(WallJumping, TEXT("Als.LocomotionAction.WallJumping"))
	UE_DEFINE_GAMEPLAY_TAG(WallRunning, TEXT("Als.LocomotionAction.WallRunning"))
	UE_DEFINE_GAMEPLAY_TAG(RunningMan, TEXT("Als.LocomotionAction.Emote.Dance.RunningMan"))
	UE_DEFINE_GAMEPLAY_TAG(Sitting01Entering, TEXT("Als.LocomotionAction.StationaryMode.Sitting01.Entering"))
	UE_DEFINE_GAMEPLAY_TAG(Sitting01Exiting, TEXT("Als.LocomotionAction.StationaryMode.Sitting01.Exiting"))
	UE_DEFINE_GAMEPLAY_TAG(PushButton, TEXT("Als.LocomotionAction.Interaction.PushButton"))
	UE_DEFINE_GAMEPLAY_TAG(JabLeft, TEXT("Als.LocomotionAction.Attacking.Unarmed.JabLeft"))
	UE_DEFINE_GAMEPLAY_TAG(HookRight, TEXT("Als.LocomotionAction.Attacking.Unarmed.HookRight"))
	UE_DEFINE_GAMEPLAY_TAG(KickFront, TEXT("Als.LocomotionAction.Attacking.Unarmed.KickFront"))
	UE_DEFINE_GAMEPLAY_TAG(KickSoccer, TEXT("Als.LocomotionAction.Attacking.Unarmed.KickSoccer"))
	UE_DEFINE_GAMEPLAY_TAG(Stab, TEXT("Als.LocomotionAction.Attacking.Knife.Stab"))
	UE_DEFINE_GAMEPLAY_TAG(OverHead, TEXT("Als.LocomotionAction.Attacking.Axe.OverHead"))
	UE_DEFINE_GAMEPLAY_TAG(OutSide, TEXT("Als.LocomotionAction.Attacking.Axe.OutSide"))
	UE_DEFINE_GAMEPLAY_TAG(Reloading, TEXT("Als.LocomotionAction.Weapon.Reloading"))
	
}

namespace ALSXTSexTags
{
	UE_DEFINE_GAMEPLAY_TAG(Male, TEXT("Als.Sex.Male"))
	UE_DEFINE_GAMEPLAY_TAG(Female, TEXT("Als.Sex.Female"))
}

namespace ALSXTStatusTags
{
	UE_DEFINE_GAMEPLAY_TAG(Normal, TEXT("Als.Status.Normal"))
		UE_DEFINE_GAMEPLAY_TAG(KnockedDown, TEXT("Als.Status.KnockedDown"))
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

namespace ALSXTBumpReactionTags
{
	ALSXT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HeadBackLight)
}

namespace ALSXTBumpSurfaceTags
{
	ALSXT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cloth)
}

namespace ALSXTHitReactionTags
{
	ALSXT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BluntHeadBackLight)
}

namespace ALSXTHitSurfaceTags
{
	ALSXT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cloth)
}

namespace ALSXTBumpReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(HeadBackLight, TEXT("Als.BumpReaction.Head.Back.Light"))
}

namespace ALSXTBumpSurfaceTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.BumpSurface.Cloth"))

}

namespace ALSXTImpactReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(BluntHeadBackLight, TEXT("Als.HitReaction.Blunt.Head.Back.Light"))

}

namespace ALSXTImpactReactionTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cloth, TEXT("Als.HitSurface.Cloth"))
}

namespace ALSXTLocomotionVariantTags
{
	UE_DEFINE_GAMEPLAY_TAG(Default, TEXT("Als.LocomotionVariant.Default"))
	UE_DEFINE_GAMEPLAY_TAG(Exaggerated, TEXT("Als.LocomotionVariant.Exaggerated"))
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

namespace ALSXTBlockingTags
{
	UE_DEFINE_GAMEPLAY_TAG(NotBlocking, TEXT("Als.Blocking.NotBlocking"))
	UE_DEFINE_GAMEPLAY_TAG(Blocking, TEXT("Als.Blocking.Blocking"))
}

namespace ALSXTStationaryModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Sitting01, TEXT("Als.StationaryMode.Sitting01"))
	UE_DEFINE_GAMEPLAY_TAG(Petting, TEXT("Als.StationaryMode.Petting"))
}