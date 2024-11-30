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
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FALSXTFirearmSightsState GetFirearmState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FGameplayTag GetFirearmFireMode() const;
	
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FALSXTFirearmSightsState GetFirearmSightsState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FALSXTFirearmAimState GetFirearmAimState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FALSXTRecoilState GetRecoilState() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State", Meta = (ForceAsFunction))
  void InitializeFirearm();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State", Meta = (ForceAsFunction))
  void OnFirearmDischarge();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|State", Meta = (ForceAsFunction))
  void OnFirearmDischargeEnd();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface|State")
  FALSXTFirearmDischargeEffectsState GetFirearmDischargeEffectsState() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Firearm Interface")
  TArray <FALSXTFirearmActionSound> GetFirearmActionSounds(const FGameplayTag& ActionType) const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|Actions")
  void ChargeFirearm();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|Actions")
  void Reload();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|Actions")
  void ReloadWithRetention();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firearm Interface|Actions")
  void ClearWeaponMalfunction();
};