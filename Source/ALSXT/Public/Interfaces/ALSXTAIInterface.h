#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTAIInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTAIInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTAIInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTAI Interface")
  void SetLockMovement(bool NewLocked);
};