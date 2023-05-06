#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTCharacterInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetCombatAttackPhysics(float& Mass, float& Velocity);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetVoiceInfo(const FGameplayTag& Sex, const FGameplayTag& VoiceVariant, float& Speed, float& Pitch);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void BumpReaction(FDoubleHitResult Hit, FGameplayTag Gait, FGameplayTag Side, FGameplayTag Form);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void CrowdNavigationReaction(FGameplayTag Gait, FDoubleHitResult Hit, FGameplayTag Side, FGameplayTag Form);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
  UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag GetGait();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
  UPARAM(meta = (Categories = "Als.Combat Stance")) FGameplayTag GetCombatStance();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
  void AnticipationReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void AttackReaction(FAttackDoubleHitResult Hit);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetImpactFallLocation(FVector& Location, FDoubleHitResult Hit);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void TryGetUp();
};