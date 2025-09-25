// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtLinkedAnimationInstance.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "AlsxtOverlayLinkedAnimationInstance.generated.h"

class UAlsxtOverlayAnimationAssetsSettings;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtOverlayLinkedAnimationInstance : public UAlsxtLinkedAnimationInstance
{
	GENERATED_BODY()

public:
	UAlsxtOverlayLinkedAnimationInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void UpdateAnimationAssets_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animation|Chooser")
	UAlsxtOverlayAnimationAssetsSettings* EvaluateOverlayChooserTable();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", Meta = (AllowPrivateAccess))
	FAlsxtOverlayAnimationInstanceAssets OverlayAnimationInstanceAssets {FAlsxtOverlayAnimationInstanceAssets()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", Meta = (Categories="Als.OverlaySlot", AllowPrivateAccess))
	FGameplayTag OverlaySlot;

};
