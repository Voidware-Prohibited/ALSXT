#pragma once

#include "Utility/ALSXTStructs.h"
#include "Utility/ALSXTGameplayTags.h"
#include "NativeGameplayTags.h"
#include "ALSXTCharacterBreathEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterBreathEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	bool EnableCharacterBreathEffects{ true };

	/* Min and Max for the Breathing animations Blending value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	FVector2D BreathAnimationBlendRange{ 0.25f, 1.0f };

	/* Min and Max for the Breathing animations Play Rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	FVector2D BreathAnimationPlayRateRange{ 2.0f, 0.5f };

	/* Stamina Levels that will trigger a sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina", TitleProperty = "{Stamina}"))
	FGameplayTagContainer AudibleBreathStaminaLevels{ALSXTStaminaTags::Empty};

	/* Breath types that will spawn a Niagara particle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Breath Type", TitleProperty = "{BreathType}"))
	FGameplayTagContainer VisibleBreathTypes{ALSXTBreathTypeTags::Cold};

	/* Breath Particles for All Vocalizations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< FALSXTBreathParticle> BreathParticles;

	/*Holding Breath */
	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseHoldingBreathRange{ 6.0f, 10.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseReleasedBreathRange{ 2.0f, 3.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseGaspingBreathRange{ 3.0f, 6.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseExhaustedBreathRange{ 6.0f, 10.0f };

	/* Default Holding Breath Duration before any Modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	float BaseHoldingBreathTimerDuration{ 3.0f };

	/* Default Stamina Cost for Exausting the Holding Breath Timer before any Modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	float BaseBreathExhaustedStaminaCost{ 10.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseGaspingBreathStaminaCost{ 6.0f, 10.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D BaseExhaustedBreathStaminaCost{ 6.0f, 10.0f };

	/* Range of Breath Rate to calculate with Stamina after releasing Hold Breath in relation to the Holding Breath Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30), Category = "Holding Breath")
	FVector2D ReleaseStartRateRange{ 0.33f, 2.0f };
};
