// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtLinkedAnimationInstance.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "AlsxtLocomotionLinkedAnimationInstance.generated.h"

class UAlsxtLocomotionAnimationAssetsSettings;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtLocomotionLinkedAnimationInstance : public UAlsxtLinkedAnimationInstance
{
	GENERATED_BODY()

public:
	UAlsxtLocomotionLinkedAnimationInstance();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void UpdateAnimationAssets_Implementation() override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animation|Chooser")
	UAlsxtLocomotionAnimationAssetsSettings* EvaluateLocomotionChooserTable();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Assets", Meta = (AllowPrivateAccess))
	FAlsxtLocomotionAnimationInstanceAssets LocomotionAnimationInstanceAssets;
	
};
