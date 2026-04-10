#include "Settings/AlsxtMovementSettings.h"

#if WITH_EDITOR
void UAlsxtMovementSettings::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, VelocityAngleToSpeedInterpolationRange))
	{
		VelocityAngleToSpeedInterpolationRange.Y = FMath::Max(VelocityAngleToSpeedInterpolationRange.X,
		                                                      VelocityAngleToSpeedInterpolationRange.Y);
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif
