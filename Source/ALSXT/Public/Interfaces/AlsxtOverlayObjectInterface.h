#pragma once

#include "Settings/AlsxtFirearmSettings.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "AlsxtOverlayObjectInterface.generated.h"

UINTERFACE(Blueprintable)
class UAlsxtOverlayObjectInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IAlsxtOverlayObjectInterface {
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Overlay Object Interface|Settings")
  FAlsxtOverlayInfo GetObjectOverlayInfo() const;
	
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Overlay Object Interface|Settings")
  FAlsxtFirearmDischargeEffectsSettings GetOverlayObjectEffectsSettings() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Overlay Object Interface|Settings")
  FAlsxtFirearmRecoilSettings GetOverlayObjectRecoilSettings() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State")
  FAlsxtFirearmSightsState GetOverlayObjectState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State")
  FAlsxtRecoilState GetRecoilState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void InitializeOverlayObject();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischarge();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischargeAnimation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischargeControlRotation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischargeCameraShake();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischargeMuzzleFlash();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlay Object Interface|State", Meta = (ForceAsFunction))
  void OnOverlayObjectDischargeSound();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State", Meta = (ForceAsFunction))
  void OnFirearmDischargeEnd();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Overlay Object Interface")
  TArray <FAlsxtFirearmActionSound> GetOverlayObjectActionSounds(const FGameplayTag& ActionType) const;
};