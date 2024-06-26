#pragma once

#include "NativeGameplayTags.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Settings/ALSXTCameraEffectsSettings.h"
#include "Settings/ALSXTHeldItemSettings.h"
#include "Settings/ALSXTFirearmSettings.h"
#include "State/ALSXTPoseState.h"
#include "State/ALSXTHeadLookAtState.h"
#include "State/ALSXTFreelookState.h"
#include "State/ALSXTAimState.h"
#include "State/ALSXTDefensiveModeState.h"
#include "State/ALSXTFootstepState.h"
#include "State/ALSXTBreathState.h"
#include "AlsCharacterMovementComponent.h"
#include "ALSXTCharacterInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterInterface {
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  AALSXTCharacter* GetCharacter();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  USkeletalMeshComponent* GetCharacterMesh() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UALSXTAnimationInstance* GetCharacterAnimInstance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterLocomotionMode() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterGait() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterLean() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterEmote() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterEmote(const FGameplayTag& NewEmote);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void GetCharacterGesture(FGameplayTag& NewGesture, FGameplayTag& NewGestureHand) const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterGesture(const FGameplayTag& NewGesture, const FGameplayTag& NewGestureHand);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTDefensiveModeState GetCharacterDefensiveModeState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTFootprintsState GetCharacterFootprintsState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTFootwearDetails GetCharacterFootwearDetails() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UCapsuleComponent* GetCharacterCapsuleComponent() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UAlsCharacterMovementComponent* GetCharacterMovementComponent() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  bool IsBlocking() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterLocomotionAction() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterLocomotionAction(const FGameplayTag& NewLocomotionAction);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void ResetCharacterDefensiveModeState();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterDefensiveMode() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterDefensiveMode(const FGameplayTag& NewDefensiveMode);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterStatus(const FGameplayTag& NewStatus);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterDefensiveModeState(FALSXTDefensiveModeState NewDefensiveModeState);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterMovementModeLocked(bool NewLocked);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterFreelooking() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterWeaponFirearmStance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterWeaponReadyPosition() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterStationaryMode() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterHoldingBreath() const;

  // Firearm

  // Get the current Recoil Info for the Held Item
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface|Firearm")
  UALSXTFirearmRecoilAsset* GetFirearmRecoilInfo() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterReloadingType() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterForegripPosition() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterFirearmFingerAction() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterFirearmFingerActionHand() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterWeaponCarryPosition() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterFirearmSightLocation() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FTransform GetCharacterCurrentForegripTransform() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterVaultType() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTAimState GetCharacterAimState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTFreelookState GetCharacterFreelookState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTHeadLookAtState GetCharacterHeadLookAtState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterLocomotionVariant() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterLocomotionVariant(const FGameplayTag& NewLocomotionVariant);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetViewportMask(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask);

  UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterWeaponObstruction() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UALSXTCharacterSettings* GetCharacterSettings() const;
  
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterSex() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterStance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterStance(const FGameplayTag& NewStance);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterOverlayMode() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterCombatStance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterInjury() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  void SetCharacterInjury(const FGameplayTag& NewInjury);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UAlsCameraComponent* GetCharacterCamera() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UALSXTCameraAnimationInstance* GetCharacterCameraAnimationInstance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FRotator GetCharacterControlRotation() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetStamina() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetStaminaTag();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTStaminaThresholdSettings GetStaminaThresholdSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetHealth() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  UPARAM(meta = (Categories = "Als.Status")) FGameplayTag GetCharacterStatus() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTStatusState GetStatusState() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetStaminaThresholds(float& Optimal, float& Low) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetBreathType() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  UALSXTCameraShakeSettings* SelectCameraShakeSettings() const;
	
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void GetCombatAttackPhysics(float& Mass, float& Velocity) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTClothingState GetClothingState() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetWeightTag() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
  void GetClothingSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
  void GetAccentSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement", Meta = (AutoCreateRefTerm = "StaminaOverride"))
  void CharacterBreathEffects(UPARAM(meta = (Categories = "Als.Stamina"))const FGameplayTag& StaminaOverride);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement", Meta = (AutoCreateRefTerm = "Type, Weight"))
  void PlayCharacterMovementSound(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTBreathState GetCharacterBreathState() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTPoseState GetCharacterPoseState() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
  UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag GetGait() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
  UPARAM(meta = (Categories = "Als.Combat Stance")) FGameplayTag GetCombatStance() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  void TryGetUp();
};