#include "Settings/AlsxtOverlayAnimationAssetsSettings.h"

UAlsxtOverlayAnimationAssetsSettings::UAlsxtOverlayAnimationAssetsSettings()
{
	
}

#if WITH_EDITOR
void UAlsxtOverlayAnimationAssetsSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif