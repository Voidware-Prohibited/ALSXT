// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "AlsCameraComponent.h"
#include "AlsxtCharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Mesh/AlsxtPaintableSkeletalMeshComponent.h"
#include "Components/Mesh/AlsxtPaintableStaticMeshComponent.h"
#include "Components/Character/AlsxtImpactReactionComponent.h"
#include "Components/Character/AlsxtCharacterSoundComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "CineCameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Settings/AlsxtVaultingSettings.h"
#include "Settings/AlsxtImpactReactionSettings.h"
#include "Settings/AlsxtOverlaySettings.h"
#include "State/AlsLocomotionState.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtAimableOverlayObjectGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "Utility/AlsxtStructs.h"
#include "State/AlsxtPoseState.h"
#include "State/AlsxtFootstepState.h"
#include "State/AlsxtAimState.h"
#include "State/AlsxtDefensiveModeState.h"
#include "Settings/AlsxtStationaryModeSettings.h"
#include "State/AlsxtFreelookState.h"
#include "State/AlsxtHeadLookAtState.h"
#include "State/AlsxtSlidingState.h"
#include "State/AlsxtVaultingState.h"
#include "State/AlsxtBreathState.h"

#include "Interfaces/AlsxtCharacterGameplayCameraComponentInterface.h"

#include "Components/BoxComponent.h"
#include "Settings/AlsxtCombatSettings.h"
#include "Interfaces/AlsxtTargetLockInterface.h"
#include "Interfaces/AlsxtCombatInterface.h"


#include "Interfaces/AlsxtStationaryModeComponentInterface.h"
#include "Interfaces/AlsxtCollisionInterface.h"
#include "Interfaces/AlsxtHeadLookAtInterface.h"
#include "Interfaces/AlsxtMeshPaintingInterface.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtHeldItemInterface.h"
#include "Interfaces/AlsxtCharacterSoundComponentInterface.h"
#include "Interfaces/AlsxtCharacterCustomizationComponentInterface.h"
#include "Interfaces/AlsxtIdleAnimationComponentInterface.h"
#include "Notifies/AlsAnimNotify_FootstepEffects.h"
#include "State/AlsxtFootstepState.h"
#include "InputActionValue.h"
#include "AbilitySystem/Interfaces/AlsxtAbilitySystemInterface.h"
// #include "ALSXTAnimationInstance.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "AbilitySystem/Data/AlsxtAbilitySystemData.h"
#include "ContextualAnimSceneActorComponent.h"
#include "PhysicsControlComponent.h"
#include "State/AlsxtAnimationParametersState.h"
#include "Utility/AlsxtOverlayStructs.h"

#include "AlsxtCharacter.generated.h"

class UAlsxtOverlayLookupTableDataAsset;
class UAlsxtOverlayObjectLookupTableDataAsset;
class UAlsxtAimableOverlayObjectLookupTableDataAsset;
class UAlsxtMovementSettings;
class UAlsxtCameraAnimationInstance;
class UAlsxtMantlingSettings;
class UAlsxtAnimationInstance;
class UAlsxtCharacterMovementComponent;
class UAlsxtCharacterSettings;
class USceneComponent;
class UAlsCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAbilitySystemComponent;
class UAlsxtAbilitySystemComponent;

/**
* @file ALSXTCharacter.h
* @brief Template class that contains all shared Logic and Data for Player and NPC Child Classes.
* This Class is set up for Modular Gameplay Features
* This Class is Abstract and should not be used directly! (Not-Blueprintable)
* @todo change to UCLASS(Abstract, NotBlueprintable) after restructuring classes
*/

/**
 * @defgroup player_input_delegates Player Input Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetupPlayerInputComponentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementInputDelegate);

/**
 * @defgroup locomotion_delegates Locomotion Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartProneDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndProneDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSlideDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSlideDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSlopeSlideDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSlopeSlideDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSidleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSidleDelegate);

/**
 * @defgroup ragdoll_delegates Ragdoll Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRagdollingStartedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRagdollingEndedDelegate);

/**
 * @defgroup character_idle_delegates Character Idle Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterIdleTimerBeginDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterIdleTimerCancelledDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterIdleTimerEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterIdleBeginDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterIdleEndDelegate);


UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class ALSXT_API AAlsxtCharacter : public AAlsCharacter, public IAbilitySystemInterface, public IAlsxtCharacterGameplayCameraComponentInterface, public IAlsxtCharacterCustomizationComponentInterface, public IAlsxtStationaryModeComponentInterface, public IAlsxtCollisionInterface, public IAlsxtHeadLookAtInterface, public IAlsxtTargetLockInterface, public IAlsxtCharacterSoundComponentInterface, public IAlsxtMeshPaintingInterface, public IAlsxtCharacterInterface, public IAlsxtHeldItemInterface, public IAlsxtIdleAnimationComponentInterface
{
	GENERATED_BODY()

// Main	
public:
	AAlsxtCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{Cast<AAlsCharacter>(GetParentActor())};

	/** Set by character movement to specify that this Character is currently prone. */
	UPROPERTY(BlueprintReadOnly, replicatedUsing=OnRep_IsProne, Category=Character)
	uint8 bIsProne:1;

	/** Handle Prone replicated from server */
	UFUNCTION()
	virtual void OnRep_IsProne();

	/** Set by character movement to specify that this Character is currently crouched. This function returns bIsCrouched.*/
	bool IsProne() const;
	
	/** Specifies whether this Character is currently crouched or not. This function sets bIsCrouched and marks property as dirty.*/
	void SetIsProne(const bool bInIsProne);

	UFUNCTION(BlueprintCallable, Category = "State|Als Character")
	bool GetOverlaySlot(const FGameplayTag& Slot, FGameplayTag& Overlay, FGameplayTag& Posture);

	/** Tries to set an Overlay slot with a specified Posture by checking First, if Overlay supports the Slot, and Second, checks if the existing Overlay slot (if any) supports the Posture, otherwise sets a supported Posture. bForce will Set the Overlay slot regardless of restrictions */
	UFUNCTION(BlueprintCallable, Category = "State|Als Character")
	bool TrySetOverlaySlot(const FGameplayTag& TargetSlot, const FGameplayTag& NewOverlayMode, const FGameplayTag& NewCombatStance, const bool bForce);

	UFUNCTION()
	void SetMovingTag(bool bIsMoving);

	UFUNCTION()
	void SetMovingTagLocal(bool bIsMoving);
	
	// Server RPC to update the moving tag
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovingTag(bool bIsMoving);

// Settings
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")
	TObjectPtr<UAlsxtMovementSettings> AlsxtMovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")
	TSoftObjectPtr<UAlsxtLocomotionSettingsDataAsset> AlsxtLocomotionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")
	TSoftObjectPtr<UAlsxtOverlaySettingsDataAsset> AlsxtOverlaySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", ReplicatedUsing = "OnReplicated_OverlayModes")
	FGameplayTagContainer OverlayModes{AlsOverlayModeTags::Default};

private:
	UFUNCTION()
	void OnReplicated_OverlayModes(const FGameplayTagContainer& PreviousOverlayModes);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnOverlayModesChanged(const FGameplayTagContainer& PreviousOverlayModes);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtCharacterSettings> ALSXTSettings;

// Ability System
protected:
	// Data used to initialize the Ability System Component. (Can be found in "AbilitySystemData.h")
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System", Meta = (ShowOnlyInnerProperties))
	TSoftObjectPtr<UAlsxtAbilitySystemInitializationDataAsset> AbilitySystemInitializationData;

public:
	// Implement the IAbilitySystemInterface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// virtual void SetDesiredStance(const FGameplayTag& NewDesiredStance) override;

	virtual void ServerSetDesiredStance_Implementation(FGameplayTag NewDesiredStance) override;

	

	// State
	virtual void OnRep_PlayerState() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Parameters", ReplicatedUsing=OnRep_AnimationParametersState, BlueprintGetter=GetAnimationParametersState, Meta =(AllowPrivateAccess))
	FAlsxtAnimationParametersState AnimationParametersState;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ALS|State|Parameters", Meta=(AllowPrivateAccess))
	FAlsxtAnimationParametersState GetAnimationParametersState() const;

	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "ALS|State|Parameters", Meta=(AllowPrivateAccess))
	void SetAnimationParametersState(FAlsxtAnimationParametersState NewAnimationParametersState);

protected:
	UFUNCTION()
	void OnRep_AnimationParametersState();

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|State|Parameters", Meta = (AllowPrivateAccess), Transient)
	FAlsxtStatusState StatusState;

public:
	/**
	* @ingroup player_input_delegates
	*/
	UPROPERTY(BlueprintAssignable)
	FSetupPlayerInputComponentDelegate OnSetupPlayerInputComponentUpdated;
	
	FOnMovementInputDelegate OnMovementInputDelegate;

	/**
	* @ingroup locomotion_delegates
	*/
	FOnStartProneDelegate OnStartProneDelegate;
	FOnEndProneDelegate OnEndProneDelegate;
	FOnStartSlideDelegate OnStartSlideDelegate;
	FOnEndSlideDelegate OnEndSlideDelegate;

	/**
	* @ingroup ragdoll_delegates
	*/
	FScriptDelegate OnRagdollingStartedDelegate;
	FScriptDelegate OnRagdollingEndedDelegate;

	/**
	* @ingroup character_idle_delegates
	*/
	FOnCharacterIdleTimerBeginDelegate OnCharacterIdleTimerBeginDelegate;
	FOnCharacterIdleTimerCancelledDelegate OnCharacterIdleTimerCancelledDelegate;
	FOnCharacterIdleTimerEndDelegate OnCharacterIdleTimerEndDelegate;
	FOnCharacterIdleBeginDelegate OnCharacterIdleBeginDelegate;
	FOnCharacterIdleEndDelegate OnCharacterIdleEndDelegate;	

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnEnterSlideSlopeAngle();

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnExitSlideSlopeAngle();

	/**
	* @section Component members
	*/

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> ALSXTMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UAlsxtCharacterMovementComponent> ALSXTCharacterMovement;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UContextualAnimSceneActorComponent> ContextualAnimationComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics Control")
	TObjectPtr<UPhysicsControlComponent> PhysicsControlComponent;
	
	// Cameras
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	// Overlay
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> OverlaySkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableStaticMeshComponent> OverlayStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> OverlaySkeletalMeshSecondary;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableStaticMeshComponent> OverlayStaticMeshSecondary;
	
	// Body
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> BodyParts;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Head;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HeadDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Teeth;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Tongue;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Hair;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HairDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> FacialHair;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> FacialHairDummyShadow;

	// Equipment
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> ClothingSlots;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Headwear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HeadwearDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Facewear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> FacewearDummyShadow;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Eyewear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> EyewearDummyShadow;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Earwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> BottomUnderwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> TopUnderwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Bottom;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Top;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> TopJacket;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> TopVest;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Gloves;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtPaintableSkeletalMeshComponent> Footwear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneCaptureComponent2D> MeshPaintingSceneCapture;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> PhysicsConstraints;

	// Code Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UAlsxtImpactReactionComponent* ImpactReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UAlsxtCharacterSoundComponent* CharacterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UAlsxtProceduralRecoilAnimComponent* ProceduralRecoil;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UPhysicalAnimationComponent* PhysicalAnimation;
	
	virtual void OnStanceChanged_Implementation(FGameplayTag PreviousStance) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> OverlayLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayObjectLookupTableDataAsset> OverlayObjectLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtAimableOverlayObjectLookupTableDataAsset> AimableOverlayObjectLookupTable {nullptr};
	
protected:
	// Breath State
	void UpdateBreathState();
	bool ShouldUpdateBreathState() const;
	bool ShouldTransitionBreathState();
	FAlsxtTargetBreathState CalculateTargetBreathState();
	void SetTargetBreathState(const FAlsxtTargetBreathState& NewTargetBreathState);
	void TransitionBreathState();

	void UpdateAnimationBlendState() const;
	bool ShouldUpdateAnimationBlendState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Als Character")
	FAlsxtTargetBreathState CalculateAnimationBlendState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Als Character")
	FAlsxtAnimationBlendState GetAnimationBlendState() const;
	
	void SetAnimationBlendState(FAlsxtAnimationBlendState NewAnimationBlendState);

public:
	virtual void OnOverlayModeChanged_Implementation(FGameplayTag PreviousOverlayMode) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALSXT|State")
	void OnAnimationBlendStateChanged(FAlsxtAnimationBlendState PreviousBlendState);
	

protected:
	virtual void OnJumped_Implementation() override;
	virtual void OnMantlingStarted_Implementation(const FAlsMantlingParameters& Parameters) override;
	virtual void OnMantlingEnded_Implementation() override;
	virtual void OnRagdollingStarted_Implementation() override;
	virtual void OnRagdollingEnded_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	int32 VaultingRootMotionSourceId;

	FAlsxtCameraShakeSetting SelectMovementCameraShakeAsset();

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableInputMovement(const bool Disable);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableLookAt(const bool Disable);

protected:
	virtual void NotifyLocomotionModeChanged(FGameplayTag PreviousLocomotionMode) override;

public:
	virtual bool IsMantlingAllowedToStart_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual bool CanProne() const;

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual void OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual void OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	
	// Character Interface

	// Controller
	virtual FRotator GetCharacterControlRotation_Implementation() const override;
	
	// Ragdoll
	virtual void SetCharacterRagdoll_Implementation(const bool NewRagdoll) override;

	// Sex
	virtual FGameplayTag GetCharacterSex_Implementation() const override;

	// Stance
	virtual FGameplayTag GetCharacterStance_Implementation() const override;

	// Camera
	virtual TSoftObjectPtr<UGameplayCameraComponent> GetCharacterGameplayCameraComponent_Implementation() const override;
	virtual bool GetCharacterIsCameraRightShoulder_Implementation() const override;
	virtual FVector GetCharacterFirstPersonCameraLocation_Implementation() const override;

	// Focus
	virtual bool CanCharacterFocus_Implementation() const override;
	virtual bool GetCharacterFocus_Implementation() const override;
	virtual void SetCharacterFocus_Implementation(const bool NewFocus) override;

	// Overlay Mode
	virtual FGameplayTag GetCharacterOverlayMode_Implementation() const override;
	virtual FGameplayTag GetCharacterDominantOverlayLayer_Implementation(FGameplayTag& OverlaySlot) const override;
	virtual TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> GetCharacterOverlayLookupTable_Implementation() const override;
	virtual FAlsxtOverlayLayers GetCharacterOverlayLayers_Implementation() const override;
	virtual FAlsxtOverlayInfo GetCharacterOverlayInfo_Implementation(const FGameplayTag& OverlayModeTag) const override;
	virtual FAlsxtOverlayInfo GetCharacterOverlayObjectInfo_Implementation(const FGameplayTag& OverlayModeTag) const override;
	virtual FAlsxtOverlayInfo GetCharacterAimableOverlayObjectInfo_Implementation(const FGameplayTag& OverlayModeTag) const override;
	virtual FGameplayTag GetCharacterInjury_Implementation() const override;
	virtual FGameplayTag GetCharacterCombatStance_Implementation() const override;
	virtual void SetCharacterCombatStance_Implementation(UPARAM(meta = (Categories = "Als.Combat Stance"))const FGameplayTag& NewCombatStance) override;

	// Readiness
	virtual FGameplayTag GetCharacterReadiness_Implementation() const override;
	virtual void SetCharacterReadiness_Implementation(const FGameplayTag& NewReadiness) override;

	// Weapon Ready Positions
	virtual FGameplayTag GetCharacterWeaponReadyPosition_Implementation() const override;
	virtual FGameplayTagContainer GetCharacterAvailableWeaponReadyPositions_Implementation() const override;
	virtual void SetCharacterWeaponReadyPosition_Implementation(FGameplayTag NewWeaponReadyPosition) override;

	// Weapon Firearm Stance
	virtual FGameplayTag GetCharacterWeaponFirearmStance_Implementation() const override;

	// Mesh Painting Interface
	virtual FAlsxtGlobalGeneralMeshPaintingSettings GetGlobalGeneralMeshPaintingSettings_Implementation() const override;
	virtual USceneCaptureComponent2D* GetSceneCaptureComponent_Implementation() const override;
	virtual void GetElementalCondition_Implementation(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Elemental Condition"))FGameplayTag& ElementalCondition) const override;
	virtual void PaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Radius) override;
	virtual void VolumePaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent) override;
	virtual void ResetPaintTypeOnComponent_Implementation(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) override;
	virtual void ResetPaintOnComponent_Implementation(UPrimitiveComponent* Component) override;
	virtual void ResetPaintTypeOnAllComponents_Implementation(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) override;
	virtual void ResetPaintOnAllComponents_Implementation() override;

	// Camera Old
	virtual UAlsxtCameraAnimationInstance* GetCharacterCameraAnimationInstance_Implementation() const override;
	virtual UAlsCameraComponent* GetCharacterCamera_Implementation() const override;

protected:

	// Camera Zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float CameraZoom {0.0f};
	
	//Overlay Object

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TMap<FGameplayTag, TSubclassOf<UAnimInstance>> OverlayAnimationInstanceClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bUseGunBoneForOverlayObjects {false};

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay", Meta = (ForceAsFunction))
	void RefreshOverlayLinkedAnimationLayer();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay", Meta = (ForceAsFunction))
	void RefreshOverlayObject();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay", Meta = (ForceAsFunction))
	void AttachOverlayObject(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, TSubclassOf<UAnimInstance> NewAnimationClass, FName SocketName, FName GripSocketName, bool bUseLeftGunBone);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay", Meta = (ForceAsFunction))
	void ClearOverlayObject();

	FOnMontageBlendingOutStarted OnSlidingStartedBlendOutDelegate;
	FOnMontageEnded OnSlidingStartedEndedDelegate;

	void OnSlidingStartedBlendOut(UAnimMontage* Montage, bool bInterrupted);

	FOnMontageBlendingOutStarted OnEnterStationaryModeBlendOutDelegate;
	// FOnMontageBlendingOutStarted OnExitStationaryModeBlendOutDelegate;

	void OnEnterStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted);
	// void OnExitStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted);

// PhysicalAnimationState
private:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", ReplicatedUsing = "OnReplicate_PhysicalAnimationState", Meta = (AllowPrivateAccess), Transient)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FAlsxtPhysicalAnimationState PhysicalAnimationState;

protected:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void MulticastSetPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void ServerSetPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState);

	//
	// UFUNCTION()
	// void OnReplicate_PhysicalAnimationState(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState);

	//
	UFUNCTION(Server, Unreliable)
	void ServerProcessNewPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewALSXTPhysicalAnimationState);

public:
	//
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtPhysicalAnimationState GetPhysicalAnimationState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void StartBlendOutPhysicalAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void BlendOutPhysicalAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void EndBlendOutPhysicalAnimation();

	//
	UFUNCTION()
	void OnPhysicalAnimationStateChanged(const FAlsxtPhysicalAnimationState& PreviousPhysicalAnimationState);

	//
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationState"))
	void SetPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState);

	//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationState"))
	FAlsxtPhysicalAnimationState ProcessNewPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState);

// HeldItemState
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FAlsxtHeldItemState HeldItemState;

protected:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void MulticastSetHeldItemState(const FAlsxtHeldItemState& NewHeldItemState);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void ServerSetHeldItemState(const FAlsxtHeldItemState& NewHeldItemState);

	UFUNCTION()
	void OnReplicate_HeldItemState(const FAlsxtHeldItemState& PreviousHeldItemState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewHeldItemState(const FAlsxtHeldItemState& NewALSXTHeldItemState);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtHeldItemState& GetHeldItemState() const;

	UFUNCTION()
	void OnHeldItemStateChanged(const FAlsxtHeldItemState& PreviousHeldItemState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeldItemState"))
	void SetHeldItemState(const FAlsxtHeldItemState& NewHeldItemState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeldItemState"))
	FAlsxtHeldItemState ProcessNewHeldItemState(const FAlsxtHeldItemState& NewHeldItemState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character", Meta = (ForceAsFunction))
	void OnFirstPersonOverrideChanged(float FirstPersonOverride);

	// Breath State

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FAlsxtBreathState BreathState;

	// Breath State
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtBreathState& GetBreathState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBreathState"))
	void SetBreathState(const FAlsxtBreathState& NewBreathState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBreathState"))
	FAlsxtBreathState ProcessNewBreathState(const FAlsxtBreathState& NewALSXTBreathState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewBreathState(const FAlsxtBreathState& NewALSXTBreathState);

	// Pose State
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Pose", Meta = (AllowPrivateAccess), Transient)
	FAlsxtPoseState ALSXTPoseState;

	// Pose State
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtPoseState& GetALSXTPoseState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPoseState"))
	void SetALSXTPoseState(const FAlsxtPoseState& NewALSXTPoseState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPoseState"))
	FAlsxtPoseState ProcessNewALSXTPoseState(const FAlsxtPoseState& NewALSXTPoseState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewALSXTPoseState(const FAlsxtPoseState& NewALSXTPoseState);

	// Vaulting State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Vaulting", Meta = (AllowPrivateAccess), Transient)
	FAlsxtVaultingState VaultingState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtVaultingState& GetVaultingState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	void SetVaultingState(const FAlsxtVaultingState& NewVaultingState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	FAlsxtVaultingState ProcessNewVaultingState(const FAlsxtVaultingState& NewVaultingState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewVaultingState(const FAlsxtVaultingState& NewVaultingState);

	// Stationary Mode State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Vaulting", Meta = (AllowPrivateAccess), Transient)
	FAlsxtStationaryModeState StationaryModeState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtStationaryModeState& GetStationaryModeState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStationaryModeState"))
	void SetStationaryModeState(const FAlsxtStationaryModeState& NewStationaryModeState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStationaryModeState"))
	FAlsxtStationaryModeState ProcessNewStationaryModeState(const FAlsxtStationaryModeState& NewStationaryModeState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewStationaryModeState(const FAlsxtStationaryModeState& NewStationaryModeState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetStationaryModeState(const FAlsxtStationaryModeState& NewStationaryModeState);

	UFUNCTION()
	void OnReplicate_StationaryModeState(const FAlsxtStationaryModeState& PreviousStationaryModeState);
	
	UFUNCTION(Server, Unreliable)
	void ServerSetALSXTPoseState(const FAlsxtPoseState& NewALSXTPoseState);

	UFUNCTION()
	void OnReplicate_ALSXTPoseState(const FAlsxtPoseState& PreviousALSXTPoseState);

// Breath
	UFUNCTION(Client, Reliable)
	void ClientSetBreathState(const FAlsxtBreathState& NewBreathState);

	UFUNCTION(Server, Reliable)
	void ServerSetBreathState(const FAlsxtBreathState& NewBreathState);

	UFUNCTION()
	void OnReplicate_BreathState(const FAlsxtBreathState& PreviousBreathState);


	UFUNCTION(Server, Unreliable)
	void ServerSetVaultingState(const FAlsxtVaultingState& NewVaultingState);

	UFUNCTION()
	void OnReplicate_VaultingState(const FAlsxtVaultingState& PreviousVaultingState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnALSXTPoseStateChanged(const FAlsxtPoseState& PreviousALSXTPoseState);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnStationaryModeStateChanged(const FAlsxtStationaryModeState& PreviousStationaryModeState);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnBreathStateChanged(const FAlsxtBreathState& PreviousBreathState);
	
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnVaultingStateChanged(const FAlsxtVaultingState& PreviousVaultingState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	bool bMovementEnabled{ true };

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UAlsxtAnimationInstance> XTAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	FVector2D PreviousLookInput;

	// Footstep State

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_FootprintsState", Meta = (AllowPrivateAccess))
	FAlsxtFootprintsState FootprintsState;

protected:

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ClientSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void MulticastSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void UpdateFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& Target, const float& Alpha);

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void SetFootprintNewSurface(const FAlsxtFootprintsState& UpdatedTargetState, const FAlsxtFootprintsState& NewFootprintsState, const EAlsFootBone& Foot);

protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	void StartFootSaturation(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& TargetState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ResetFootSaturationTimeline(const EAlsFootBone& Foot);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	UTimelineComponent* GetFootTimeline(const EAlsFootBone& Foot);

	// Aim State

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_AimState", Meta = (AllowPrivateAccess))
	FAlsxtAimState AimState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtAimState& GetAimState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewAimState"))
	void SetAimState(const FAlsxtAimState& NewAimState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewAimState"))
	FAlsxtAimState ProcessNewAimState(const FAlsxtAimState& NewAimState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewAimState(const FAlsxtAimState& NewAimState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetAimState(const FAlsxtAimState& NewAimState);

	UFUNCTION()
	void OnReplicate_AimState(const FAlsxtAimState& PreviousAimState);

public:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnAimStateChanged(const FAlsxtAimState& PreviousAimState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetHeldItemInfo();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableCombatStances();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableWeaponCarryPositions();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableWeaponReadyPositions();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableFirearmStances();

	// Grip
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableGripPositions();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FTransform GetCurrentGripTransform() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "ForegripPosition"))
	const FName GetSocketNameForGripPosition(UPARAM(meta = (Categories = "Als.Foregrip Position"))const FGameplayTag& ForegripPosition) const;

	// Foregrip
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool DoesOverlayObjectUseLeftHandIK() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "ForegripPosition"))
	const FName GetSocketNameForForegripPosition(UPARAM(meta = (Categories = "Als.Foregrip Position"))const FGameplayTag& ForegripPosition) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableForegripPositions();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FTransform GetCurrentForegripTransform() const;

	// Emote

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanEmote() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSelectEmote() const;

	// Freelooking
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", ReplicatedUsing = "OnReplicate_FreelookState", Meta = (AllowPrivateAccess))
	FAlsxtFreelookState FreelookState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFreelooking{ALSXTFreelookingTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Freelooking{ALSXTFreelookingTags::False};

public:
	virtual void CharacterActivateFreelooking_Implementation() override;
	virtual void CharacterDeactivateFreelooking_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtFreelookState& GetFreelookState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookState"))
	void SetFreelookState(const FAlsxtFreelookState& NewFreelookState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookState"))
	FAlsxtFreelookState ProcessNewFreelookState(const FAlsxtFreelookState& NewFreelookState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewFreelookState(const FAlsxtFreelookState& NewFreelookState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetFreelookState(const FAlsxtFreelookState& NewFreelookState);

	UFUNCTION()
	void OnReplicate_FreelookState(const FAlsxtFreelookState& PreviousFreelookState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFreelookStateChanged(const FAlsxtFreelookState& PreviousFreelookState);

	// HeadLookAt
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Look At|State", ReplicatedUsing = "OnReplicate_HeadLookAtState", Meta = (AllowPrivateAccess))
	FAlsxtHeadLookAtState HeadLookAtState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Look At", Meta = (AllowPrivateAccess))
	TArray<FAlsxtHeadLookAtEntry> HeadLookAtEntries;

public:
	UFUNCTION(BlueprintCallable, Category = "Head Look At|State")
	const FAlsxtHeadLookAtState& GetHeadLookAtState() const;

	virtual FAlsxtHeadLookAtEntry GetBestHeadLookAtEntry_Implementation() const override;

	virtual bool CanHeadLookAt_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Head Look At|State", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	void SetHeadLookAtState(const FAlsxtHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Head Look At|State", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	FAlsxtHeadLookAtState ProcessNewHeadLookAtState(const FAlsxtHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewHeadLookAtState(const FAlsxtHeadLookAtState& NewHeadLookAtState);

	virtual void AddHeadLookAtEntry_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry) override;
	virtual void RemoveHeadLookAtEntry_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry) override;
	virtual void BeginHeadLookAt_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry) override;
	virtual void EndHeadLookAt_Implementation() override;

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetHeadLookAtState(const FAlsxtHeadLookAtState& NewHeadLookAtState);

	UFUNCTION()
	void OnReplicate_HeadLookAtState(const FAlsxtHeadLookAtState& PreviousHeadLookAtState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Head Look At|State")
	void OnHeadLookAtStateChanged(const FAlsxtHeadLookAtState& PreviousHeadLookAtState);

	// Lean

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredLean{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Lean{ FGameplayTag::EmptyTag };

	// Sex

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredSex{ALSXTSexTags::Male};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Sex{ALSXTSexTags::Male};

	// LocomotionVariant

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredLocomotionVariant{ALSXTLocomotionVariantTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionVariant{ALSXTLocomotionVariantTags::Default};

	// Injury

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredInjury{AlsxtStatusEffectLocomotionVariantTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Injury{AlsxtStatusEffectLocomotionVariantTags::None};

	// CombatStance

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredReadyStance{AlsxtReadinessTags::Relaxed};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ReadyStance{AlsxtReadinessTags::Relaxed};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredCombatStance{ALSXTCombatStanceTags::Orthodox};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag CombatStance{ALSXTCombatStanceTags::Orthodox};

	// WeaponFirearmStance

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredWeaponFirearmStance{ALSXTWeaponFirearmStanceTags::Regular};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponFirearmStance{ALSXTWeaponFirearmStanceTags::Regular};

	// WeaponReadyPosition

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredWeaponReadyPosition{ALSXTWeaponReadyPositionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponReadyPosition{ALSXTWeaponReadyPositionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsxtSlidingState SlidingState;

	// Defensive Mode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredDefensiveMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag DefensiveMode {FGameplayTag::EmptyTag};

	// AvailableHands

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTagContainer DesiredAvailableHands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTagContainer AvailableHands;
	

	// Defensive Mode State
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Defensive Mode", ReplicatedUsing = "OnReplicate_DefensiveModeState", Meta = (AllowPrivateAccess))
	FAlsxtDefensiveModeState DefensiveModeState;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_DefensiveModeAnimations", Meta = (AllowPrivateAccess))
	// DefensiveModeAnimations DefensiveModeAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", Meta = (AllowPrivateAccess))
	FAlsxtDefensiveModeAnimations DefensiveModeAnimations;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", Transient)
	// FALSXTDefensiveModeState DefensiveModeState;

public:	
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtSlidingState& GetSlidingState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewSlidingState"))
	void SetSlidingState(const FAlsxtSlidingState& NewSlidingState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewSlidingState"))
	FAlsxtSlidingState ProcessNewSlidingState(const FAlsxtSlidingState& NewSlidingState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewSlidingState(const FAlsxtSlidingState& NewSlidingState);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetSlidingState(const FAlsxtSlidingState& NewSlidingState);

	UFUNCTION()
	void OnReplicate_SlidingState(const FAlsxtSlidingState& PreviousSlidingState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Sliding State")
	void ClientSetSlidingState(const FAlsxtSlidingState& NewSlidingState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Sliding State")
	void MulticastSetSlidingState(const FAlsxtSlidingState& NewSlidingState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnSlidingStateChanged(const FAlsxtSlidingState& PreviousSlidingState);

	/////

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtDefensiveModeState& GetDefensiveModeState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtDefensiveModeAnimations& GetDefensiveModeAnimations() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	void SetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeAnimations"))
	void SetDefensiveModeAnimations(const FAlsxtDefensiveModeAnimations& NewDefensiveModeAnimations);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	void SetDefensiveModeStateImplementation(const FAlsxtDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void ResetDefensiveModeState();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	FAlsxtDefensiveModeState ProcessNewDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

	UFUNCTION()
	void OnReplicate_DefensiveModeState(const FAlsxtDefensiveModeState& PreviousDefensiveModeState);

	// UFUNCTION()
	// void OnReplicate_DefensiveModeAnimations(const FALSXTDefensiveModeAnimations& PreviousDefensiveModeAnimations);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Defensive Mode State")
	void ClientSetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Defensive Mode State")
	void MulticastSetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnDefensiveModeStateChanged(const FAlsxtDefensiveModeState& PreviousDefensiveModeState);

	// StationaryMode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredStationaryMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag StationaryMode{FGameplayTag::EmptyTag};

	// Status

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredStatus{ALSXTStatusTags::Normal};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Status{ALSXTStatusTags::Normal};

	// Focus
	// Allows for a zooming in of the camera in First Person similar to Arma

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFocus{ALSXTFocusedTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Focus{ALSXTFocusedTags::False};

	// Timers

	FTimerHandle FreelookTimerHandle;	// Timer Handle for Freelook Trace
	FTimerDelegate FreelookTimerDelegate; // Delegate to bind function with parameters

	FTimerHandle AnticipationTimerHandle;	// Timer Handle for Freelook Trace
	FTimerDelegate AnticipationTimerDelegate; // Delegate to bind function with parameters

	FTimerHandle BlendOutPhysicalAnimationTimerHandle;
	FTimerDelegate BlendOutPhysicalAnimationTimerDelegate;

	FTimerHandle SlidingTimerHandle;	// Timer Handle for Freelook Trace
	FTimerDelegate SlidingTimerDelegate; // Delegate to bind function with parameters

	FTimerHandle FallingTimerHandle;	// Timer Handle for Falling
	FTimerDelegate FallingTimerDelegate; // Delegate to bind function with parameters

public:
	virtual void Tick(float DeltaTime) override;

private:

// PhysicalAnimationMode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredPhysicalAnimationMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag PhysicalAnimationMode{FGameplayTag::EmptyTag};

// GripPosition

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGripPosition {ALSXTGripPositionTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GripPosition {ALSXTGripPositionTags::Default};

// ForegripPosition

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredForegripPosition {ALSXTForegripPositionTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ForegripPosition {ALSXTForegripPositionTags::Default};

// WeaponCarryPosition

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredWeaponCarryPosition{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponCarryPosition{FGameplayTag::EmptyTag};

// VaultType

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredVaultType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag VaultType{FGameplayTag::EmptyTag};

// WeaponObstruction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredWeaponObstruction{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponObstruction{FGameplayTag::EmptyTag};

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookMouseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MantleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AimToggleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> FocusAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RagdollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> RotationModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ViewModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchShoulderAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> FreelookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ToggleFreelookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> PrimaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SecondaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ToggleGaitAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ToggleCombatReadyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LeanLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ToggleLeanLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LeanRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> ToggleLeanRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SlideAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchWeaponReadyPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitcWeaponFirearmStanceAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchGripPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchForegripPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example",
		Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float LookUpMouseSensitivity{3.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example",
		Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float LookRightMouseSensitivity{3.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example",
		Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "deg/s"))
	float LookUpRate{90.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example",
		Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "deg/s"))
	float LookRightRate{240.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	float PreviousYaw{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	float PreviousPitch{ 0.0f };

public:
	AAlsxtCharacter();

	virtual void NotifyControllerChanged() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Crouch(bool bClientSimulation = false) override;

	virtual void InputCrouch();

	virtual void ApplyDesiredStance() override;

	void ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport);

	// Camera

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;
	void CalcADSCamera(FMinimalViewInfo& ViewInfo);

	// Input

public:
	UFUNCTION()
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

public:
	void InputBlock(const FInputActionValue& ActionValue);

	FInputActionValue InputMantleValue;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputPrimaryInteraction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputSecondaryInteraction();

private:
	void InputLookMouse(const FInputActionValue& ActionValue);

	void InputLook(const FInputActionValue& ActionValue);

	void InputMove(const FInputActionValue& ActionValue);

	void InputSprint(const FInputActionValue& ActionValue);

	void InputWalk();

	void InputJump(const FInputActionValue& ActionValue);

	void InputMantle(const FInputActionValue& ActionValue);

	void InputAim(const FInputActionValue& ActionValue);

	void InputToggleAim();

	void InputFocus(const FInputActionValue& ActionValue);

	void InputRagdoll();

	void InputRoll();

	void InputRotationMode();

	void InputViewMode();

	void InputSwitchShoulder();

	void InputFreelook(const FInputActionValue& ActionValue);

	void InputToggleGait();

	void InputToggleCombatReady();

	void InputSwitchWeaponFirearmStance();

	void InputSwitchWeaponReadyPosition();

	void InputSwitchGripPosition();

	void InputSwitchForegripPosition();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnViewModeChanged(const FGameplayTag& PreviousViewModeTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryInteraction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "APrimaryInteractionls|Input Actions")
	bool CanPerformSecondaryInteraction() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetMovementModeLocked(bool bNewMovementModeLocked);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSetToViewMode(const FGameplayTag& ViewModeTag) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const float GetCameraZoom() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetCameraZoom(const float NewCamerZoom);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetUnarmedAttackDamageInfo(const FGameplayTag& UnarmedAttackType, const FGameplayTag& UnarmedAttackStrength, float& BaseDamage, FGameplayTag& ImpactForm, FGameplayTag& DamageType) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetBlockingInfo(const FAttackDoubleHitResult& AttackDoubleHitResult, float& BaseDamage) const;

	virtual bool IsHoldingItem_Implementation() const override;
	virtual bool IsHoldingAimableItem_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	void GetHeldItemViewTarget(FTransform& Transform, float& FOV, float& VignetteIntensity, bool& Attachment) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Aim Down Sights")
	bool IsAimingDownSights() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|First Person Eye Focus")
	bool IsFirstPersonEyeFocusActive() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Stamina")
	float GetActionBaseStaminaCost(UPARAM(meta = (Categories = "Als.LocomotionAction"))const FGameplayTag& Action, UPARAM(meta = (Categories = "Als.Action Strength")) const FGameplayTag& ActionStrength) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Stamina")
	float GetDamageBaseStaminaCost(float Damage, UPARAM(meta = (Categories = "Als.Impact Form")) const FGameplayTag& ImpactForm) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanSprint() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FAnticipationPose SelectAttackAnticipationMontage(const FGameplayTag& CharacterCombatStance, const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form);

	// Vaulting

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	bool IsVaultingAllowedToStart(FVaultAnimation VaultAnimation) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	bool TryStartVaultingGrounded();

private:
	bool TryStartVaultingInAir();

	bool TryStartVaulting(const FAlsxtVaultingTraceSettings& TraceSettings);

	UFUNCTION(Server, Reliable)
	void ServerStartVaulting(const FAlsxtVaultingParameters& Parameters);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartVaulting(const FAlsxtVaultingParameters& Parameters);

	void StartVaultingImplementation(const FAlsxtVaultingParameters& Parameters);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAlsxtMantlingSettings* SelectMantlingSettingsXT();

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAlsxtSlidingSettings* SelectSlidingSettings();

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAlsxtVaultingSettings* SelectVaultingSettings(const FGameplayTag& VaultingType);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	FVaultAnimation SelectVaultingMontage(const FGameplayTag& CurrentGait, const FGameplayTag& VaultingType);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnVaultingStarted(const FAlsxtVaultingParameters& Parameters);

private:
	void RefreshVaulting();

	void StopVaulting();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnVaultingEnded();

	// Sliding

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void TryStartSliding(float PlayRate = 1.0f);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAnimMontage* SelectSlideMontage();

private:
	bool IsSlidingAllowedToStart(const UAnimMontage* Montage) const;

	void StartSliding(float PlayRate, float TargetYawAngle);

	UFUNCTION(Server, Reliable)
	void ServerStartSliding(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSliding(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void StartSlidingImplementation(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void RefreshSliding(float DeltaTime);

	void RefreshSlidingPhysics(float DeltaTime);

	void StopSliding();

public:
	// Footstep Values

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
		FAlsxtFootwearDetails GetFootwearDetails() const;

	// Footstep State

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtFootprintsState& GetFootprintsState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	void SetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	FAlsxtFootprintsState ProcessNewFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

private:

	UFUNCTION()
	void OnReplicate_FootprintsState(const FAlsxtFootprintsState& PreviousFootprintsState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFootprintsStateChanged(const FAlsxtFootprintsState& PreviousFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnSlidingStarted();

public:

	// Debug

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalLocation) override;

	/** BP implementable function for AI to check if they can Mantle or Vault obstacle */
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void OnAIJumpObstacle_Implementation();

	/** BP implementable function for AI to trace for Mantle or Vault obstacles */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void AIObstacleTrace();
	virtual void AIObstacleTrace_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanFocus() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanToggleGait() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool CanLean() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool CanAimDownSights() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool CanAim() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool CanRoll() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanVault() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanSlide() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanWallRun() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanWallJump() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool TryVault() const;

protected:		
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartVault();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartWallrun();

	// Desired Leaning

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredLean() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetLean() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewLeanTag"))
	void SetDesiredLean(const FGameplayTag& NewLeanTag);

private:
	void SetLean(const FGameplayTag& NewLeanTag);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetDesiredLean(const FGameplayTag& NewLeanTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLeanChanged(const FGameplayTag& PreviousLeanTag);

	// Desired Freelooking

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FGameplayTag& GetDesiredFreelooking() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookingTag"))
	void SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Movement System")
	bool CanFreelook() const;

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredFreelooking(const FGameplayTag& NewFreelookingTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void IsFreelooking(bool& bIsFreelooking, bool& bIsFreelookingInFirstPerson) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ActivateFreelooking();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void DeactivateFreelooking();

	// Freelooking

public:
	const FGameplayTag& GetFreelooking() const;

private:
	void SetFreelooking(const FGameplayTag& NewFreelookingTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFreelookingChanged(const FGameplayTag& PreviousFreelookingTag);

	// Desired Sex

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredSex() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewSexTag"))
	void SetDesiredSex(const FGameplayTag& NewSexTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredSex(const FGameplayTag& NewSexTag);

	// Sex

public:
	const FGameplayTag& GetSex() const;

private:
	void SetSex(const FGameplayTag& NewSexTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnSexChanged(const FGameplayTag& PreviousSexTag);

	// Desired LocomotionVariant

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredLocomotionVariant() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewLocomotionVariantTag"))
	void SetDesiredLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag);

	// LocomotionVariant

public:
	const FGameplayTag& GetLocomotionVariant() const;

private:
	void SetLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLocomotionVariantChanged(const FGameplayTag& PreviousLocomotionVariantTag);

	// Desired Injury

public:
	virtual void SetCharacterInjury_Implementation(const FGameplayTag& NewInjury) override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredInjury() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewInjuryTag"))
	void SetDesiredInjury(const FGameplayTag& NewInjuryTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredInjury(const FGameplayTag& NewInjuryTag);

	// Injury

public:
	const FGameplayTag& GetInjury() const;

private:
	void SetInjury(const FGameplayTag& NewInjuryTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnInjuryChanged(const FGameplayTag& PreviousInjuryTag);

	// Desired CombatStance

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredReadyStance() const;
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredCombatStance() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewCombatReadyTag"))
	void SetDesiredReadyStance(const FGameplayTag& NewCombatReadyTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewCombatStanceTag"))
	void SetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetDesiredReadyStance(const FGameplayTag& NewReadyStanceTag);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag);

	// CombatStance

public:
	const FGameplayTag& GetReadyStance() const;
	const FGameplayTag& GetCombatStance() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanToggleCombatReady() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSwitchReadyStance() const;

	virtual bool CanCharacterSwitchCombatStance_Implementation() const override;

	virtual void SetCharacterCameraRightShoulder_Implementation(const bool NewCameraRightShoulder) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSwitchCombatStance() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanBecomeCombatReady() const;

private:
	void SetCombatStance(const FGameplayTag& NewCombatStanceTag);
	void SetReadyStance(const FGameplayTag& NewCombatReadyTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnReadyStanceChanged(const FGameplayTag& PreviousCombatReadyTag);
	
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnCombatStanceChanged(const FGameplayTag& PreviousCombatStanceTag);

	// Desired WeaponFirearmStance

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredWeaponFirearmStance() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewWeaponFirearmStanceTag"))
	void SetDesiredWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag);

	// WeaponFirearmStance

public:
	const FGameplayTag& GetWeaponFirearmStance() const;

private:
	void SetWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnWeaponFirearmStanceChanged(const FGameplayTag& PreviousWeaponFirearmStanceTag);

	// Desired WeaponReadyPosition

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredWeaponReadyPosition() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewWeaponReadyPositionTag"))
	void SetDesiredWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag);

	// WeaponReadyPosition

public:
	const FGameplayTag& GetWeaponReadyPosition() const;

private:
	void SetWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnWeaponReadyPositionChanged(const FGameplayTag& PreviousWeaponReadyPositionTag);

	// Desired DefensiveMode

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanEnterDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanEnterBlockingDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool ShouldEnterBlockingDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool ShouldEnterAnticipationDefensiveMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool IsBlocking() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	bool IsInDefensiveMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	bool IsInAnticipationMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeTag"))
	void SetDesiredDefensiveMode(UPARAM(meta = (Categories = "Als.Defensive Mode"))const FGameplayTag& NewDefensiveModeTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredDefensiveMode(const FGameplayTag& NewDefensiveModeTag);

	// Blocking

public:
	const FGameplayTag& GetDefensiveMode() const;

private:
	void SetDefensiveMode(const FGameplayTag& NewDefensiveModeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnDefensiveModeChanged(const FGameplayTag& PreviousDefensiveModeTag);

	// Desired StationaryMode

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredStationaryMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanEnterStationaryMode() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanExitStationaryMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStationaryModeTag"))
	void SetDesiredStationaryMode(const FGameplayTag& NewStationaryModeTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredStationaryMode(const FGameplayTag& NewStationaryModeTag);

	// StationaryMode

public:
	const FGameplayTag& GetStationaryMode() const;

private:
	void SetStationaryMode(const FGameplayTag& NewStationaryModeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnStationaryModeChanged(const FGameplayTag& PreviousStationaryModeTag);

	// Desired Status

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredStatus() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStatusTag"))
		void SetDesiredStatus(const FGameplayTag& NewStatusTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredStatus(const FGameplayTag& NewStatusTag);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetDesiredStatus(const FGameplayTag& NewStatusTag);

	// Status

public:
	const FGameplayTag& GetStatus() const;

private:
	void SetStatus(const FGameplayTag& NewStatusTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnStatusChanged(const FGameplayTag& PreviousStatusTag);

	// Desired Focus

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredFocus() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFocusTag"))
		void SetDesiredFocus(const FGameplayTag& NewFocusTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredFocus(const FGameplayTag& NewFocusTag);

	// Focus

public:
	const FGameplayTag& GetFocus() const;

private:
	void SetFocus(const FGameplayTag& NewFocusTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnFocusChanged(const FGameplayTag& PreviousFocusTag);

	
public:
	// Freelook
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void BeginAnticipationTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void AnticipationTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void EndAnticipationTimer();


	// Freelook
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void BeginFreelookTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void FreelookTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void EndFreelookTimer();

	// Impact Reacion

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void GetCameraShakeInfoFromHit(FAttackDoubleHitResult Hit, TSubclassOf<UCameraShakeBase>& CameraShakeClass, float& Scale);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnImpactCollision(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnImpactHit(FDoubleHitResult Hit);

protected:
	// Desired PhysicalAnimationMode

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredPhysicalAnimationMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationModeTag"))
	void SetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames);

	// PhysicalAnimationMode

public:
	const FGameplayTag& GetPhysicalAnimationMode() const;

private:
	void SetPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames);

	void ResetPhysicalAnimationMode();

protected:
	void PhysicalAnimationBlendOut();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnPhysicalAnimationModeChanged(const FGameplayTag& PreviousPhysicalAnimationModeTag);

// Desired GripPosition

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredGripPosition() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewGripPositionTag"))
	void SetDesiredGripPosition(const FGameplayTag& NewGripPositionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGripPosition(const FGameplayTag& NewGripPositionTag);

// GripPosition

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSwitchGripPosition() const;

	const FGameplayTag& GetGripPosition() const;

private:
	void SetGripPosition(const FGameplayTag& NewGripPositionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGripPositionChanged(const FGameplayTag& PreviousGripPositionTag);

// Desired ForegripPosition

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredForegripPosition() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewForegripPositionTag"))
	void SetDesiredForegripPosition(const FGameplayTag& NewForegripPositionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredForegripPosition(const FGameplayTag& NewForegripPositionTag);

// ForegripPosition

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSwitchForegripPosition() const;

	const FGameplayTag& GetForegripPosition() const;

private:
	void SetForegripPosition(const FGameplayTag& NewForegripPositionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnForegripPositionChanged(const FGameplayTag& PreviousForegripPositionTag);

// Desired WeaponCarryPosition

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredWeaponCarryPosition() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewWeaponCarryPositionTag"))
	void SetDesiredWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag);

// WeaponCarryPosition

public:
	const FGameplayTag& GetWeaponCarryPosition() const;

private:
	void SetWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnWeaponCarryPositionChanged(const FGameplayTag& PreviousWeaponCarryPositionTag);

// Desired VaultType

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredVaultType() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultTypeTag"))
	void SetDesiredVaultType(const FGameplayTag& NewVaultTypeTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredVaultType(const FGameplayTag& NewVaultTypeTag);

// VaultType

public:
	const FGameplayTag& GetVaultType() const;

private:
	void SetVaultType(const FGameplayTag& NewVaultTypeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnVaultTypeChanged(const FGameplayTag& PreviousVaultTypeTag);

// Desired WeaponObstruction

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredWeaponObstruction() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewWeaponObstructionTag"))
	void SetDesiredWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag);

// WeaponObstruction

public:
	const FGameplayTag& GetWeaponObstruction() const;

private:
	void SetWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnWeaponObstructionChanged(const FGameplayTag& PreviousWeaponObstructionTag);

	// INTERFACE FUNCTIONS

	// Core Interface Functions
	virtual AAlsxtCharacter* GetCharacter_Implementation() override;
	virtual UAlsxtAnimationInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual UAlsxtCharacterSettings* GetCharacterSettings_Implementation() const override;
	virtual UInputComponent* GetCharacterInputComponent_Implementation() const override;
	virtual bool IsCharacterPlayerControlled_Implementation() const override;

public:
	virtual bool GetCharacterCanRoll_Implementation() const override;
	virtual void StartCharacterRoll_Implementation(float PlayRate) override;

	virtual bool CanCharacterSprint_Implementation() const override;

	virtual bool GetCanCharacterAim_Implementation() const override;
	virtual bool GetCharacterAim_Implementation() const override;
	virtual void SetCharacterAim_Implementation(const bool NewStance) override;

protected:
	virtual bool GetCharacterFirstPersonFocus_Implementation() const override;
	virtual bool GetCharacterAimingDownSights_Implementation() const override;
	virtual bool CanEmote_Implementation() const override;
	virtual bool CanGesture_Implementation() const override;
	virtual FAlsxtAnimationParametersState GetCharacterAnimationParametersState_Implementation() const override;

	virtual void SetCharacterLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariant) override;

	virtual FAlsxtStationaryModeState GetCharacterStationaryModeState_Implementation() const override;

	// Viewport Mask Functions
	virtual void SetViewportMask_Implementation(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask) override;

	// Component Interface Functions
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() const override;
	virtual UCapsuleComponent* GetCharacterCapsuleComponent_Implementation() const override;
	virtual UAlsCharacterMovementComponent* GetCharacterMovementComponent_Implementation() const override;

	// Footprint Interface Functions
	virtual FAlsxtFootprintsState GetCharacterFootprintsState_Implementation() const override;
	virtual FAlsxtFootwearDetails GetCharacterFootwearDetails_Implementation() const override;

	// Collision Interface Functions
	virtual void AddCollisionImpulse_Implementation(FVector NewImpulse) override;
	virtual void OnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit) override;
	virtual void OnActorAttackCollision_Implementation(FAttackDoubleHitResult Hit) override;
	virtual FAlsxtSlidingState GetCharacterSlidingState_Implementation() const override;
	virtual FAlsxtDefensiveModeState GetCharacterDefensiveModeState_Implementation() const override;
	virtual FAlsxtDefensiveModeAnimations GetCharacterDefensiveModeAnimations_Implementation() const override;
	virtual FAlsxtBumpPoseState GetCrowdNavigationPoseState_Implementation() const override;

	virtual FALSXTJigglePhysicsSettings GetCharacterJigglePhysicsSettings_Implementation() const override;
	virtual FAlsxtPhysicalAnimationState GetCharacterPhysicalAnimationState_Implementation() const override;
	virtual void SetCharacterPhysicalAnimationState_Implementation(FAlsxtPhysicalAnimationState NewPhysicalAnimationState) override;

	virtual FGameplayTag GetCharacterPhysicalAnimationMode_Implementation() const override;
	virtual void SetCharacterPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationMode, const TArray<FName>& BelowBoneNames) override;
	virtual void ResetCharacterPhysicalAnimationMode_Implementation() override;

	// State Interface Functions
	virtual void SetCharacterStatus_Implementation(const FGameplayTag& NewStatus) override;
	virtual void SetCharacterMovementModeLocked_Implementation(bool NewLocked) override;
	virtual void SetCharacterStance_Implementation(const FGameplayTag& NewStance) override;
	virtual FGameplayTag GetCharacterGait_Implementation() const override;

public:
	virtual void SetCharacterGait_Implementation(const FGameplayTag& NewGait) override;

protected:
	virtual FGameplayTag GetCharacterRotationMode_Implementation() const override;

public:
	virtual void SetCharacterRotationMode_Implementation(const FGameplayTag& NewRotationMode) override;

protected:
	virtual FGameplayTag GetCharacterViewMode_Implementation() const override;

	virtual FGameplayTag GetCharacterLean_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionMode_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionAction_Implementation() const override;		
	virtual void SetCharacterLocomotionAction_Implementation(const FGameplayTag& NewLocomotionAction) override;
	virtual FAlsxtPoseState GetCharacterPoseState_Implementation() const override;
	virtual FAlsxtBreathState GetCharacterBreathState_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionVariant_Implementation() const override;
	virtual FGameplayTag GetCharacterVaultType_Implementation() const override;

	// Character Sound Component Interface Functions
	virtual bool CanPlayCharacterMovementSound_Implementation() const override;
	virtual bool CanPlayWeaponMovementSound_Implementation() const override;
	virtual bool ShouldPlayWeaponMovementSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Strength) const override;
	virtual bool ShouldPlayMovementAccentSound_Implementation(UPARAM(meta = (Categories = "Als.LocomotionAction"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight) const override;
	virtual void PlayBreathEffects_Implementation(const FGameplayTag& StaminaOverride) override;
	virtual void PlayCharacterMovementSound_Implementation(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight) override;
	virtual void PlayActionSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const float Stamina) override;
	virtual void PlayAttackSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina) override;
	virtual void PlayDamageSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage) override;

	virtual float GetCurrentStamina_Implementation() const override;
	virtual FGameplayTag GetCurrentStaminaTag_Implementation() const override;
	virtual FGameplayTag GetCurrentBreathType_Implementation() const override;
	virtual FString GetCurrentBreathSounds_Implementation() const override;
	virtual FString GetCurrentBreathSound_Implementation() const override;
	// virtual FString GetCurrentVocalizations_Implementation() const;
	virtual FString GetCurrentVocalization_Implementation() const override;

	// Freelooking Interface Functions
	virtual FGameplayTag GetCharacterFreelooking_Implementation() const override;
	virtual FAlsxtFreelookState GetCharacterFreelookState_Implementation() const override;
	virtual FAlsxtHeadLookAtState GetCharacterHeadLookAtState_Implementation() const override;
	
	// Defensive Mode Interface Functions
	virtual FGameplayTag GetCharacterDefensiveMode_Implementation() const override;
	virtual bool IsBlocking_Implementation() const override;	
	virtual void SetCharacterDefensiveMode_Implementation(const FGameplayTag& NewDefensiveMode) override;
	virtual void SetCharacterDefensiveModeState_Implementation(FAlsxtDefensiveModeState NewDefensiveModeState) override;
	virtual void SetCharacterDefensiveModeAnimations_Implementation(FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations) override;
	virtual void ResetCharacterDefensiveModeState_Implementation() override;

	// Held Item Interface Functions
	virtual FGameplayTag GetCharacterForegripPosition_Implementation() const override;	
	virtual FGameplayTag GetCharacterWeaponCarryPosition_Implementation() const override;
	virtual FTransform GetCharacterCurrentForegripTransform_Implementation() const override;
	virtual FAlsxtAimState GetCharacterAimState_Implementation() const override;
	virtual FAlsxtHeldItemState GetCharacterHeldItemState_Implementation() const override;

	// Idle Animation Component
	virtual bool ShouldIdle_Implementation() const override;
};

inline const FGameplayTag& AAlsxtCharacter::GetDesiredLean() const
{
	return DesiredLean;
}

inline const FGameplayTag& AAlsxtCharacter::GetLean() const
{
	return Lean;
}

inline const FAlsxtBreathState& AAlsxtCharacter::GetBreathState() const
{
	return BreathState;
}

inline const FAlsxtPoseState& AAlsxtCharacter::GetALSXTPoseState() const
{
	return ALSXTPoseState;
}

inline const FAlsxtStationaryModeState& AAlsxtCharacter::GetStationaryModeState() const
{
	return StationaryModeState;
}

inline const FAlsxtVaultingState& AAlsxtCharacter::GetVaultingState() const
{
	return VaultingState;
}

inline const FAlsxtFootprintsState& AAlsxtCharacter::GetFootprintsState() const
{
	return FootprintsState;
}

inline const FAlsxtFreelookState& AAlsxtCharacter::GetFreelookState() const
{
	return FreelookState;
}

inline const FAlsxtHeadLookAtState& AAlsxtCharacter::GetHeadLookAtState() const
{
	return HeadLookAtState;
}

inline const FAlsxtAimState& AAlsxtCharacter::GetAimState() const
{
	return AimState;
}

inline const FAlsxtSlidingState& AAlsxtCharacter::GetSlidingState() const
{
	return SlidingState;
}

inline const FAlsxtDefensiveModeState& AAlsxtCharacter::GetDefensiveModeState() const
{
	return DefensiveModeState;
}

inline const FAlsxtDefensiveModeAnimations& AAlsxtCharacter::GetDefensiveModeAnimations() const
{
	return DefensiveModeAnimations;
}

inline const FAlsxtHeldItemState& AAlsxtCharacter::GetHeldItemState() const
{
	return HeldItemState;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredFreelooking() const
{
	return DesiredFreelooking;
}

inline const FGameplayTag& AAlsxtCharacter::GetFreelooking() const
{
	return Freelooking;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredSex() const
{
	return DesiredSex;
}

inline const FGameplayTag& AAlsxtCharacter::GetSex() const
{
	return Sex;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredLocomotionVariant() const
{
	return DesiredLocomotionVariant;
}

inline const FGameplayTag& AAlsxtCharacter::GetLocomotionVariant() const
{
	return LocomotionVariant;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredInjury() const
{
	return DesiredInjury;
}

inline const FGameplayTag& AAlsxtCharacter::GetInjury() const
{
	return Injury;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredReadyStance() const
{
	return DesiredReadyStance;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredCombatStance() const
{
	return DesiredCombatStance;
}

inline const FGameplayTag& AAlsxtCharacter::GetCombatStance() const
{
	return CombatStance;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredWeaponFirearmStance() const
{
	return DesiredWeaponFirearmStance;
}

inline const FGameplayTag& AAlsxtCharacter::GetWeaponFirearmStance() const
{
	return WeaponFirearmStance;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredWeaponReadyPosition() const
{
	return DesiredWeaponReadyPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetWeaponReadyPosition() const
{
	return WeaponReadyPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredDefensiveMode() const
{
	return DesiredDefensiveMode;
}

inline const FGameplayTag& AAlsxtCharacter::GetDefensiveMode() const
{
	return DefensiveMode;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredStationaryMode() const
{
	return DesiredStationaryMode;
}

inline const FGameplayTag& AAlsxtCharacter::GetStationaryMode() const
{
	return StationaryMode;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredStatus() const
{
	return DesiredStatus;
}

inline const FGameplayTag& AAlsxtCharacter::GetStatus() const
{
	return Status;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredFocus() const
{
	return DesiredFocus;
}

inline const FGameplayTag& AAlsxtCharacter::GetFocus() const
{
	return Focus;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredPhysicalAnimationMode() const
{
	return DesiredPhysicalAnimationMode;
}

inline const FAlsxtPhysicalAnimationState AAlsxtCharacter::GetPhysicalAnimationState() const
{
	return PhysicalAnimationState;
}

inline const FGameplayTag& AAlsxtCharacter::GetPhysicalAnimationMode() const
{
	return PhysicalAnimationMode;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredGripPosition() const
{
	return DesiredGripPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetGripPosition() const
{
	return GripPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredForegripPosition() const
{
	return DesiredForegripPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetForegripPosition() const
{
	return ForegripPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredWeaponCarryPosition() const
{
	return DesiredWeaponCarryPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetWeaponCarryPosition() const
{
	return WeaponCarryPosition;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredVaultType() const
{
	return DesiredVaultType;
}

inline const FGameplayTag& AAlsxtCharacter::GetVaultType() const
{
	return VaultType;
}

inline const FGameplayTag& AAlsxtCharacter::GetDesiredWeaponObstruction() const
{
	return DesiredWeaponObstruction;
}

inline const FGameplayTag& AAlsxtCharacter::GetWeaponObstruction() const
{
	return WeaponObstruction;
}