#pragma once

#include "NativeGameplayTags.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Settings/ALSXTCameraEffectsSettings.h"
#include "ALSXTCharacterInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetStamina();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetStaminaTag();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTStaminaThresholdSettings GetStaminaThresholdSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetHealth();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  UPARAM(meta = (Categories = "Als.Status")) FGameplayTag GetStatus();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTStatusState GetStatusState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetStaminaThresholds(float& Optimal, float& Low);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetBreathType();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  UALSXTCameraShakeSettings* SelectCameraShakeSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FVector GetCameraLocationOld();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FRotator GetCameraRotationOld();
	
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetCombatAttackPhysics(float& Mass, float& Velocity);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetVoiceInfo(const FGameplayTag& Sex, const FGameplayTag& VoiceVariant, float& Speed, float& Pitch);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTClothingState GetClothingState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetWeightTag();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
  void GetClothingSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
  void GetAccentSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement", Meta = (AutoCreateRefTerm = "StaminaOverride"))
  void CharacterBreathEffects(UPARAM(meta = (Categories = "Als.Stamina"))const FGameplayTag& StaminaOverride);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement", Meta = (AutoCreateRefTerm = "Type, Weight"))
  void PlayCharacterMovementSound(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTBumpPoseState GetCrowdNavigationPoseState();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTBumpPoseState GetBumpPoseState();

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