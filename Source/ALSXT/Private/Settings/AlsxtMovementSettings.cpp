#include "Settings/AlsxtMovementSettings.h"

#if WITH_EDITOR
void UAlsxtMovementSettings::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, VelocityAngleToSpeedInterpolationRange))
	{
		VelocityAngleToSpeedInterpolationRange.Y = FMath::Max(VelocityAngleToSpeedInterpolationRange.X,
		                                                      VelocityAngleToSpeedInterpolationRange.Y);
	}

	if (ChangedEvent.MemberProperty)
	{
		if (ChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, RotationModes))
		{
			for (auto& Stances : RotationModes)
			{
				// for (auto& Gait : Stances)
				// {
				// 	FAlsxtMovementGaitSettings& GaitSettings = Gait.Value;
				// 	
				// 	GaitSettings.WalkForwardSpeed = FMath::Clamp(GaitSettings.WalkForwardSpeed, 0.0f, GaitSettings.CombatForwardSpeed);
				// }
				
			}
		}
		// If a specific property within a struct in the map was changed directly (e.g., CurrentValue or MaxValue)
		else if (ChangedEvent.MemberProperty->GetOwnerStruct() == FAlsxtMovementStanceSettings::StaticStruct())
		{
			for (auto& Stances : RotationModes)
			{
				// for (auto& Gait : Stances)
				// {
				// 	FAlsxtMovementGaitSettings& GaitSettings = Gait.Value;
				// 	
				// 	GaitSettings.WalkForwardSpeed = FMath::Clamp(GaitSettings.WalkForwardSpeed, 0.0f, GaitSettings.CombatForwardSpeed);
				// }
				
			}
		}
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif
