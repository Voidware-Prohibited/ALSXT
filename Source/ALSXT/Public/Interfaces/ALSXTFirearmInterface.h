#pragma once

#include "ALSXTFirearmInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTFirearmInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTFirearmInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  void GetCurrentWeaponSight(FTransform& CurrentTransform, float& CurrentFOV, bool& Attachment);
};