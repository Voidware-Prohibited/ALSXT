#pragma once

#include "Chooser.h"
#include "AlsxtLocomotionActionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionActionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> ChooserTable; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantling")
	bool bHoldJumpToMantle{ false };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vaulting")
	bool bHoldJumpToVault{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vaulting")
	bool bHoldJumpToWallRun{ false };
};
