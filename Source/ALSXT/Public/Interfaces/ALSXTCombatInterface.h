#pragma once

#include "State/ALSXTCombatState.h"
#include "Settings/ALSXTCombatSettings.h"
#include "GameplayTagContainer.h"
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
  UALSXTCombatSettings* SelectCombatSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  bool Attacking();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  AActor* GetCurrentTarget();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  FALSXTCombatState GetCombatState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void GetCombatUnarmedTraceLocations(FGameplayTag UnarmedAttackType, FVector& Start, FVector& End, float& Radius);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void GetCombatHeldItemTraceLocations(bool& Found, FVector& Start, FVector& End, float& Radius);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
  void BeginCombatAttackCollisionTrace(FALSXTCombatAttackTraceSettings TraceSettings);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
  void EndCombatAttackCollisionTrace();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void AnticipateSyncedAttack(int32 Index);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void SyncedAttackReaction(int32 Index);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
  void StopAttack();
};