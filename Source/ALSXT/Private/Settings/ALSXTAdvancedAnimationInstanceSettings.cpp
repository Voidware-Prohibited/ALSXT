#include "Settings/ALSXTAdvancedAnimationInstanceSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAdvancedAnimationInstanceSettings)

UALSXTAdvancedAnimationInstanceSettings::UALSXTAdvancedAnimationInstanceSettings()
{
	// InAir.GroundPredictionResponseChannels =
	// {
	// 	ECC_WorldStatic,
	// 	ECC_WorldDynamic,
	// 	ECC_Destructible
	// };
	// 
	// InAir.GroundPredictionSweepResponses.WorldStatic = ECR_Block;
	// InAir.GroundPredictionSweepResponses.WorldDynamic = ECR_Block;
	// InAir.GroundPredictionSweepResponses.Destructible = ECR_Block;
}

#if WITH_EDITOR
void UALSXTAdvancedAnimationInstanceSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// InAir.PostEditChangeProperty(PropertyChangedEvent);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
