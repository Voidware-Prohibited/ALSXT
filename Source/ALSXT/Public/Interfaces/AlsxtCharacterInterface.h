#pragma once

#include "NativeGameplayTags.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Settings/AlsxtHeldItemSettings.h"
#include "Settings/AlsxtStationaryModeSettings.h"
#include "Settings/AlsxtFirearmSettings.h"
#include "State/AlsxtPoseState.h"
#include "State/AlsxtHeadLookAtState.h"
#include "State/AlsxtFreelookState.h"
#include "State/AlsxtAimState.h"
#include "State/AlsxtDefensiveModeState.h"
#include "State/AlsxtFootstepState.h"
#include "State/AlsxtBreathState.h"
#include "State/AlsxtSlidingState.h"
#include "AlsCharacterMovementComponent.h"
#include "Settings/AlsxtStatusSettings.h"
#include "State/AlsxtAnimationParametersState.h"
#include "State/AlsxtClothingState.h"
#include "State/AlsxtStatusState.h"
#include "AlsxtCharacterInterface.generated.h"

class UAlsxtAimableOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayLookupTableDataAsset;
class UAlsxtCameraAnimationInstance;
class UAlsCameraComponent;
class UAlsxtCharacterSettings;
class UAlsxtAnimationInstance;
class AAlsxtCharacter;

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTJigglePhysicsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableBreastsJigglePhysics{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BreastsJigglePhysicsScale{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableButtJigglePhysics{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ButtJigglePhysicsScale{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableBellyJigglePhysics{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BellyJigglePhysicsScale{ 1.0 };
};

/**
* @file AlsxtCharacterInterface.cpp
* @brief Interface for base Character functions.
* @note Required for ALSXT Characters
*/

UINTERFACE(Blueprintable)
class UAlsxtCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IAlsxtCharacterInterface {
	GENERATED_BODY()

public:

	/**
	* @section Base
	* @brief Base Character States
	*/
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	AAlsxtCharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	USkeletalMeshComponent* GetCharacterMesh() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UCapsuleComponent* GetCharacterCapsuleComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UAlsCharacterMovementComponent* GetCharacterMovementComponent() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	void GetCombatAttackPhysics(float& Mass, float& Velocity) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FGameplayTag GetWeightTag() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
	void GetClothingSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
	void GetAccentSurfaceForMovement(TEnumAsByte<EPhysicalSurface>& Surface, UPARAM(meta = (Categories = "Als.Character Movement Sound"))FGameplayTag Movement) const;

	/**
	* @section Animation
	* @brief Animation related functions
	*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UAlsxtAnimationInstance* GetCharacterAnimInstance() const;

	/**
	* @section State
	* @brief Extended Character States
	*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtAnimationParametersState GetCharacterAnimationParametersState() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FAlsxtClothingState GetClothingState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtPoseState GetCharacterPoseState() const;	

	// Status

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	UPARAM(meta = (Categories = "Als.Status")) FGameplayTag GetCharacterStatus() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FAlsxtStatusState GetCharacterStatusState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterStatus(const FGameplayTag& NewStatus);
	
	// Overlay Mode

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterOverlayMode() const;

	// Lookup Tables
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> GetCharacterOverlayLookupTable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	TSoftObjectPtr<UAlsxtOverlayObjectLookupTableDataAsset> GetCharacterOverlayObjectLookupTable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	TSoftObjectPtr<UAlsxtAimableOverlayObjectLookupTableDataAsset> GetCharacterAimableOverlayObjectLookupTable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtOverlayLayers GetCharacterOverlayLayers() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtAnimationBlendState GetAnimationBlendState() const;

	// Loops through all Active Overlay Layers slots and returns the one with the highest Weight. In the case of multiple identical Weights, a random one will be selected.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterDominantOverlayLayer(FGameplayTag& OverlaySlot) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtOverlayInfo GetCharacterOverlayInfo(const FGameplayTag& OverlayMode) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtOverlayInfo GetCharacterOverlayObjectInfo(const FGameplayTag& ForOverlayMode) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtOverlayInfo GetCharacterAimableOverlayObjectInfo(const FGameplayTag& OverlayMode) const;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterWeaponObstruction() const;

	// Rotation Mode
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterRotationMode() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterRotationMode(const FGameplayTag& NewRotationMode);

	// Locomotion Mode

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterLocomotionMode() const;

	// Locomotion Variant

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterLocomotionVariant() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterLocomotionVariant(const FGameplayTag& NewLocomotionVariant);

	// Gait

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterGait() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface", meta=(Categories="Als.Gait"))
	void SetCharacterGait(const FGameplayTag& NewGait);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTagContainer GetCharacterAvailableGaits() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool CanCharacterSprint() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
	UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag GetGait() const;

	// Combat Stance

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterCombatStance() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool CanCharacterSwitchCombatStance() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterCombatStance(UPARAM(meta = (Categories = "Als.Combat Stance"))const FGameplayTag& NewCombatStance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
	UPARAM(meta = (Categories = "Als.Combat Stance")) FGameplayTag GetCombatStance() const;

	// Readiness
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterReadiness() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface", meta=(Categories="Alsxt.Readiness"))
	void SetCharacterReadiness(const FGameplayTag& NewReadiness);

	// Injury
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterInjury() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterInjury(const FGameplayTag& NewInjury);

	// Stance

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterStance() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterStance(const FGameplayTag& NewStance);

	// Stances

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTagContainer GetCharacterAvailableStances() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTagContainer GetCharacterAvailableCombatStances() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTagContainer GetCharacterAvailableReadyStances() const;

	// Weapon Ready Position

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterWeaponReadyPosition() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FGameplayTagContainer GetCharacterAvailableWeaponReadyPositions() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterWeaponReadyPosition(FGameplayTag NewWeaponReadyPosition);

	// Lean

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterLean() const;

	// Weapon Firearm Stance

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterWeaponFirearmStance() const;

	/**
	* @section Settings
	*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FALSXTJigglePhysicsSettings GetCharacterJigglePhysicsSettings() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UAlsxtCharacterSettings* GetCharacterSettings() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterSex() const;

	/**
	* @section View
	*/
	
	// View Mode

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterViewMode() const;

	// Camera

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UAlsCameraComponent* GetCharacterCamera() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FVector GetCharacterFirstPersonCameraLocation() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	UAlsxtCameraShakeSettings* SelectCameraShakeSettings() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetViewportMask(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask);

	// Focus
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool CanCharacterFocus() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCharacterFocus() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterFocus(const bool NewFocus);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCharacterFirstPersonFocus() const;

	// Aim

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtAimState GetCharacterAimState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCanCharacterAim() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCharacterAim() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterAim(const bool NewStance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCharacterAimingDownSights() const;

	// Free Look

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterFreelooking() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetIsCharacterFreelooking() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void CharacterActivateFreelooking();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void CharacterDeactivateFreelooking();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtFreelookState GetCharacterFreelookState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterFreelookState(FAlsxtFreelookState NewFreelookState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtHeadLookAtState GetCharacterHeadLookAtState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterHeadLookAtState(FAlsxtHeadLookAtState NewHeadLookAtState);

	/**
	* @section Actions
	*/

	// Input

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UInputComponent* GetCharacterInputComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FRotator GetCharacterControlRotation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool IsCharacterPlayerControlled() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterMovementModeLocked(bool NewLocked);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	void TryGetUp();

	// Breath

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FGameplayTag GetBreathType() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtBreathState GetCharacterBreathState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterHoldingBreath() const;

	// Locomotion Action

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterLocomotionAction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterLocomotionAction(const FGameplayTag& NewLocomotionAction);
	
	// Roll
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool GetCharacterCanRoll() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void StartCharacterRoll(float PlayRate);

	// Vaulting

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterVaultType() const;

	// Emote

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterEmote() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterEmote(const FGameplayTag& NewEmote);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool CanEmote() const;

	// Ragdoll
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterRagdoll(const bool NewRagdoll);

	// Gesture
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void GetCharacterGesture(FGameplayTag& NewGesture, FGameplayTag& NewGestureHand) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterGesture(const FGameplayTag& NewGesture, const FGameplayTag& NewGestureHand);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool CanGesture() const;
	
	// Sliding

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtSlidingState GetCharacterSlidingState() const;

	/**
	 * @section Features
	 */

	// Defensive Mode

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	bool IsBlocking() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXTCharacter Interface")
	FVector GetCharacterThreatPoint() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtDefensiveModeState GetCharacterDefensiveModeState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtDefensiveModeAnimations GetCharacterDefensiveModeAnimations() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void ResetCharacterDefensiveModeState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterDefensiveMode(const FGameplayTag& NewDefensiveMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterDefensiveModeState(FAlsxtDefensiveModeState NewDefensiveModeState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	void SetCharacterDefensiveModeAnimations(FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations);

	// Stationary Mode

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetCharacterStationaryMode() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtStationaryModeState GetCharacterStationaryModeState() const;

	// Footstep

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtFootprintsState GetCharacterFootprintsState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FAlsxtFootwearDetails GetCharacterFootwearDetails() const;

	/**
	 * @section Deprecated code
	 * @brief Code to review for deletion or moving to other Interfaces
	 * @deprecated 
	 */

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	UAlsxtCameraAnimationInstance* GetCharacterCameraAnimationInstance() const;

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

	// Stamina

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXTCharacter Interface")
	void SubtractStamina(const float& Stamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FGameplayTag GetStaminaTag();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	FAlsxtStaminaThresholdSettings GetStaminaThresholdSettings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	void GetStaminaThresholds(float& Optimal, float& Low) const;

	// Health

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALSXTCharacter Interface")
	float GetHealth() const;
};