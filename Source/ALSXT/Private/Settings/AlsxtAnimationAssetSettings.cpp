#include "Settings/AlsxtLocomotionAnimationAssetsSettings.h"

UAlsxtLocomotionAnimationAssetsSettings::UAlsxtLocomotionAnimationAssetsSettings()
{
	
}

#if WITH_EDITOR
void UAlsxtLocomotionAnimationAssetsSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif