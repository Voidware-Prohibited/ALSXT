#include "Settings/AlsxtAnimationAssetsSettings.h"

UAlsxtAnimationAssetsSettings::UAlsxtAnimationAssetsSettings()
{
	
}

#if WITH_EDITOR
void UAlsxtAnimationAssetsSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif