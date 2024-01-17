#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTLookAtInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTLookAtInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTLookAtInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Look At Interface")
  float GetRating();
};