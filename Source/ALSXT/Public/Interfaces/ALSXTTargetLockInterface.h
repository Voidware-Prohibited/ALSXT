#pragma once

#include "ALSXTTargetLockInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTTargetLockInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTTargetLockInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Target Lock")
  bool IsTrue();

};