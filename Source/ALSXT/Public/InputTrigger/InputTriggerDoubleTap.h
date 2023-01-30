// MIT

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "InputTriggerDoubleTap.generated.h"

// UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Double Tap", NotInputConfigurable = "true"))
// class ALSXT_API InputTriggerDoubleTap : public UInputTrigger
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Double Tap", NotInputConfigurable = "true"))
class UInputTriggerDoubleTap : public UInputTrigger
{
 GENERATED_BODY()
 
protected:
    virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
public:
 
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false"))
    float Delay = .5f;
 
private:
    float LastTappedTime = 0.f;
};
