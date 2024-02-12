#pragma once

#include "Settings/ALSXTAcrobaticActionSettings.h"
#include "ALSXTAcrobaticActionComponentInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTAcrobaticActionComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTAcrobaticActionComponentInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Acrobatic Action Component Interface")
  UALSXTAcrobaticActionSettings* GetAcrobaticActionSettings();
};