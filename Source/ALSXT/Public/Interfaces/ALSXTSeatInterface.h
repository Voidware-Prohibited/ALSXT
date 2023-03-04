#pragma once

#include "ALSXTSeatInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTSeatInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTSeatInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
  void GetSeatLocationAndRotation(FVector& Location, FRotator& Rotation);

};