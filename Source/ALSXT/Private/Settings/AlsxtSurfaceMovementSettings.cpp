#include "Settings/AlsxtSurfaceMovementSettings.h"

#if WITH_EDITOR
void UAlsxtSurfaceMovementSettingsAsset::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, VelocityAngleToSpeedInterpolationRange))
	{
		VelocityAngleToSpeedInterpolationRange.Y = FMath::Max(VelocityAngleToSpeedInterpolationRange.X,
		                                                      VelocityAngleToSpeedInterpolationRange.Y);
	}

	if (ChangedEvent.MemberProperty)
	{
		
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif
