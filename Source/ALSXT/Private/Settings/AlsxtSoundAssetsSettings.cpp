#include "Settings/AlsxtSoundAssetsSettings.h"

UAlsxtSoundAssetsArraySettings::UAlsxtSoundAssetsArraySettings()
{
	
}

#if WITH_EDITOR
void UAlsxtSoundAssetsArraySettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif