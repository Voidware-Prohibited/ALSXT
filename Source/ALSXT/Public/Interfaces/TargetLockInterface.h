#pragma once

#include "TargetLockInterface.generated.h"

UINTERFACE(Blueprintable)
class UTargetLockInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API ITargetLockInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Target Lock")
  bool IsTrue();

};