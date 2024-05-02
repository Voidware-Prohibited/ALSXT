#pragma once

#include "ALSXTHeldItemInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTHeldItemInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTHeldItemInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HeldItem Interface")
  void GetCurrentWeaponSight(FTransform& CurrentTransform, float& CurrentFOV, bool& Attachment);
};