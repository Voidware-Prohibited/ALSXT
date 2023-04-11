#pragma once

#include "State/ALSXTCombatState.h"
#include "ALSXTCombatInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCombatInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCombatInterface {
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  FALSXTGeneralCombatSettings GetGeneralCombatSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  UALSXTCombatSettings* GetCombatSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool Blocking();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool Attacking();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanReceiveSpecialAttacks();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanReceiveUniqueAttacks();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanBeGrappled();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanBeThrown();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanBeRagdolled();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanBeTakenDown();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool CanBeKnockedDown();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool WillDamageKnockDownPlayer(float& Damage);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool WillDamageKillPlayer(float& Damage);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  FALSXTCombatState GetCombatState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void AnticipateSyncedAttack(int32 Index);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void SyncedAttackReaction(int32 Index);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void StopAttack();
};