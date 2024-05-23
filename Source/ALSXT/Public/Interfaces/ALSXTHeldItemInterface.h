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

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Held Item Interface")
  bool IsHoldingItem() const;
  
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Held Item Interface")
  bool IsHoldingAimableItem() const;
	
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FALSXTHeldItemSettings GetHeldItemSettings() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FHeldItemGrip GetHeldItemGrip() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  FHeldItemGrip GetHeldItemForegrip() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemEquipMontage> GetHeldItemEquipMontages(const FGameplayTag& Type) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemActionMontage> GetHeldItemActionMontages(const FGameplayTag& Type) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Held Item Interface")
  TArray<FHeldItemAttackMontage> GetHeldItemAttackMontages(const FGameplayTag& Type, const FGameplayTag& Strength) const;
};