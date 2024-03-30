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

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Acrobatic Action Component Interface")
  bool CanPerformAcrobaticAction();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Acrobatic Action Component Interface")
  bool CanFlip();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Acrobatic Action Component Interface")
  bool CanWallJump();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Acrobatic Action Component Interface")
  bool CanWallRun();
};