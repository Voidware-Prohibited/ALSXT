#pragma once

#include "Settings/ALSXTFirearmSettings.h"
#include "ALSXTFirearmInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTFirearmInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTFirearmInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  FALSXTFirearmAimState GetFirearmAimState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  UALSXTFirearmRecoilAsset* GetFirearmRecoilInfo();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  TArray<FALSXTMuzzleFlash> GetFirearmMuzzleFlashes();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  TArray <FALSXTFirearmDischargeSound> GetFirearmDischargeSounds();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  TArray <FALSXTFirearmActionSound> GetFirearmActionSounds();
};