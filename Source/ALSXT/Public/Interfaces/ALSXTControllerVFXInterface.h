#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTControllerVFXInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTControllerVFXInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTControllerVFXInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void AddSuppressionEffect(float Amount);
};