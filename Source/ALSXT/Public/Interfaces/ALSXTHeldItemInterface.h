#pragma once

#include "Settings/ALSXTHeldItemSettings.h"
#include "ALSXTHeldItemInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTHeldItemInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTHeldItemInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FALSXTHeldItemSettings GetHeldItemSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FHeldItemGrip GetHeldItemGrip();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FHeldItemGrip GetHeldItemForegrip();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemEquipMontage> GetHeldItemEquipMontages(const FGameplayTag& Type);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemActionMontage> GetHeldItemActionMontages(const FGameplayTag& Type);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemAttackMontage> GetHeldItemAttackMontages(const FGameplayTag& Type, const FGameplayTag& Strength);

  // UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Held Item Interface")
  // void GetHeldItemAttackDamageInfo(const FGameplayTag& HeldItemAttackType, const FGameplayTag& HeldItemAttackStrength, float& BaseDamage, FGameplayTag& ImpactForm, FGameplayTag& DamageType) const;
};