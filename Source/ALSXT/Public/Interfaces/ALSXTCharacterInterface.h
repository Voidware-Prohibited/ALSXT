#pragma once

#include "NativeGameplayTags.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Settings/ALSXTCameraEffectsSettings.h"
#include "State/ALSXTPoseState.h"
#include "State/ALSXTHeadLookAtState.h"
#include "State/ALSXTFreelookState.h"
#include "State/ALSXTAimState.h"
#include "State/ALSXTDefensiveModeState.h"
#include "State/ALSXTFootstepState.h"
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

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterPhysicalAnimationMode() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterGesture() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterGestureHand() const;

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
  bool DoesCharacterOverlayObjectUseLeftHandIK() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FGameplayTag GetCharacterLocomotionVariant() const;

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
  UAlsCameraComponent* GetCharacterCamera() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  UALSXTCameraAnimationInstance* GetCharacterCameraAnimationInstance() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FRotator GetCharacterControlRotation() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetStamina();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FGameplayTag GetStaminaTag();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  FALSXTStaminaThresholdSettings GetStaminaThresholdSettings();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  float GetHealth();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
  UPARAM(meta = (Categories = "Als.Status")) FGameplayTag GetCharacterStatus();

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

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
  FALSXTPoseState GetCharacterPoseState() const;

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