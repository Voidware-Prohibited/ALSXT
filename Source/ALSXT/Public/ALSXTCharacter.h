#pragma once

#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "AlsCameraComponent.h"
#include "ALSXTCharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Mesh/ALSXTPaintableSkeletalMeshComponent.h"
#include "Components/Mesh/ALSXTPaintableStaticMeshComponent.h"
#include "Components/Character/ALSXTCharacterCustomizationComponent.h"
#include "Components/Character/ALSXTImpactReactionComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTIdleAnimationComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CineCameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTImpactReactionSettings.h"
#include "Settings/ALSXTOverlaySettings.h"
#include "State/AlsLocomotionState.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Utility/ALSXTFirearmGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "State/ALSXTPoseState.h"
#include "State/ALSXTFootstepState.h"
#include "State/ALSXTAimState.h"
#include "State/ALSXTDefensiveModeState.h"
#include "Settings/ALSXTStationaryModeSettings.h"
#include "State/ALSXTFreelookState.h"
#include "State/ALSXTHeadLookAtState.h"
#include "State/ALSXTSlidingState.h"
#include "State/ALSXTVaultingState.h"
#include "State/ALSXTBreathState.h"
#include "Interfaces/ALSXTCharacterCustomizationComponentInterface.h"

#include "Components/BoxComponent.h"
#include "Settings/ALSXTCombatSettings.h"
#include "Interfaces/ALSXTTargetLockInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"


#include "Interfaces/ALSXTStationaryModeComponentInterface.h"
#include "Interfaces/ALSXTCollisionInterface.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Interfaces/ALSXTMeshPaintingInterface.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTHeldItemInterface.h"
#include "Interfaces/ALSXTCharacterSoundComponentInterface.h"
#include "Interfaces/ALSXTCharacterCustomizationComponentInterface.h"
#include "Interfaces/ALSXTIdleAnimationComponentInterface.h"
#include "Notifies/AlsAnimNotify_FootstepEffects.h"
#include "State/ALSXTFootstepState.h"
#include "InputActionValue.h"
// #include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.generated.h"

class UALSXTAnimationInstance;
class UALSXTCharacterMovementComponent;
class UALSXTCharacterSettings;
class USceneComponent;
class UAlsCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetupPlayerInputComponentDelegate);

UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class ALSXT_API AALSXTCharacter : public AAlsCharacter, public IALSXTCharacterCustomizationComponentInterface, public IALSXTStationaryModeComponentInterface, public IALSXTCollisionInterface, public IALSXTHeadLookAtInterface, public IALSXTTargetLockInterface, public IALSXTCharacterSoundComponentInterface, public IALSXTMeshPaintingInterface, public IALSXTCharacterInterface, public IALSXTHeldItemInterface, public IALSXTIdleAnimationComponentInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTCharacterSettings> ALSXTSettings;

	UPROPERTY(BlueprintAssignable)
	FSetupPlayerInputComponentDelegate OnSetupPlayerInputComponentUpdated;

	UPROPERTY(BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{Cast<AAlsCharacter>(GetParentActor())};

	FScriptDelegate OnRagdollingStartedDelegate;
	FScriptDelegate OnRagdollingEndedDelegate;

	// Components

	// Cameras
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> KillerCameraSpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UCineCameraComponent> KillerCamera;

	// Meshes

	// Overlay
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> OverlaySkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableStaticMeshComponent> OverlayStaticMesh;
	
	// Body
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> BodyParts;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Head;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HeadDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Hair;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HairDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> FacialHair;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> FacialHairDummyShadow;

	// Equipment
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> ClothingSlots;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Headwear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> HeadwearDummyShadow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Facewear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> FacewearDummyShadow;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Eyewear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> EyewearDummyShadow;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Earwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> BottomUnderwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> TopUnderwear;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Bottom;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Top;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> TopJacket;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> TopVest;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Gloves;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> Footwear;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneCaptureComponent2D> MeshPaintingSceneCapture;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> PhysicsConstraints;

	// Code Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UALSXTImpactReactionComponent* ImpactReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UALSXTCharacterSoundComponent* CharacterSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCustomizationComponent* CharacterCustomization;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UPhysicalAnimationComponent* PhysicalAnimation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTIdleAnimationComponent* IdleAnimation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTEmoteComponent* Emotes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTGestureComponent* Gestures;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> ALSXTMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UALSXTCharacterMovementComponent> ALSXTCharacterMovement;

	virtual void OnStanceChanged_Implementation(const FGameplayTag& PreviousStance) override;

	// Breath State
	void UpdateBreathState();
	bool ShouldUpdateBreathState() const;
	bool ShouldTransitionBreathState();
	FALSXTTargetBreathState CalculateTargetBreathState();
	void SetTargetBreathState(FALSXTTargetBreathState NewTargetBreathState);
	void TransitionBreathState();

	virtual void OnOverlayModeChanged_Implementation(const FGameplayTag& PreviousOverlayMode) override;
	virtual void OnJumped_Implementation() override;
	virtual void OnMantlingStarted_Implementation(const FAlsMantlingParameters& Parameters) override;
	virtual void OnMantlingEnded_Implementation() override;
	virtual void OnRagdollingStarted_Implementation() override;
	virtual void OnRagdollingEnded_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	int32 VaultingRootMotionSourceId;

	FALSXTCameraShakeSetting SelectMovementCameraShakeAsset();

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableInputMovement(const bool Disable);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableLookAt(const bool Disable);

	virtual bool IsMantlingAllowedToStart_Implementation() const override;
	
	//Character Interface
	virtual FRotator GetCharacterControlRotation_Implementation() const override;
	virtual UALSXTCameraAnimationInstance* GetCharacterCameraAnimationInstance_Implementation() const override;
	virtual UAlsCameraComponent* GetCharacterCamera_Implementation() const override;
	virtual FGameplayTag GetCharacterSex_Implementation() const override;
	virtual FGameplayTag GetCharacterStance_Implementation() const override;
	virtual FGameplayTag GetCharacterOverlayMode_Implementation() const override;
	virtual FGameplayTag GetCharacterInjury_Implementation() const override;
	virtual FGameplayTag GetCharacterCombatStance_Implementation() const override;
	virtual void SetCharacterCombatStance_Implementation(UPARAM(meta = (Categories = "Als.Combat Stance"))const FGameplayTag& NewCombatStance) override;
	virtual FGameplayTag GetCharacterWeaponReadyPosition_Implementation() const override;
	virtual FGameplayTag GetCharacterWeaponFirearmStance_Implementation() const override;
	virtual FGameplayTag GetCharacterEmote_Implementation() const override;
	virtual void SetCharacterRagdoll_Implementation(const bool NewRagdoll) override;
	virtual void SetCharacterEmote_Implementation(const FGameplayTag& NewEmote) override;

	// Mesh Painting Interface
	virtual FALSXTGlobalGeneralMeshPaintingSettings GetGlobalGeneralMeshPaintingSettings_Implementation() const override;
	virtual USceneCaptureComponent2D* GetSceneCaptureComponent_Implementation() const override;
	virtual void GetElementalCondition_Implementation(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Elemental Condition"))FGameplayTag& ElementalCondition) const override;
	virtual void PaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Radius) override;
	virtual void VolumePaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent) override;
	virtual void ResetPaintTypeOnComponent_Implementation(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) override;
	virtual void ResetPaintOnComponent_Implementation(UPrimitiveComponent* Component) override;
	virtual void ResetPaintTypeOnAllComponents_Implementation(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) override;
	virtual void ResetPaintOnAllComponents_Implementation() override;

protected:

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
	FALSXTPhysicalAnimationState PhysicalAnimationState;

protected:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void MulticastSetPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void ServerSetPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState);

	//
	// UFUNCTION()
	// void OnReplicate_PhysicalAnimationState(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState);

	//
	UFUNCTION(Server, Unreliable)
	void ServerProcessNewPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewALSXTPhysicalAnimationState);

public:
	//
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTPhysicalAnimationState GetPhysicalAnimationState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void StartBlendOutPhysicalAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void BlendOutPhysicalAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void EndBlendOutPhysicalAnimation();

	//
	UFUNCTION()
	void OnPhysicalAnimationStateChanged(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState);

	//
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationState"))
	void SetPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState);

	//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationState"))
	FALSXTPhysicalAnimationState ProcessNewPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState);

// HeldItemState
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FALSXTHeldItemState HeldItemState;

protected:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void MulticastSetHeldItemState(const FALSXTHeldItemState& NewHeldItemState);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State")
	void ServerSetHeldItemState(const FALSXTHeldItemState& NewHeldItemState);

	UFUNCTION()
	void OnReplicate_HeldItemState(const FALSXTHeldItemState& PreviousHeldItemState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewHeldItemState(const FALSXTHeldItemState& NewALSXTHeldItemState);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTHeldItemState& GetHeldItemState() const;

	UFUNCTION()
	void OnHeldItemStateChanged(const FALSXTHeldItemState& PreviousHeldItemState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeldItemState"))
	void SetHeldItemState(const FALSXTHeldItemState& NewHeldItemState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeldItemState"))
	FALSXTHeldItemState ProcessNewHeldItemState(const FALSXTHeldItemState& NewHeldItemState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character", Meta = (ForceAsFunction))
	void OnFirstPersonOverrideChanged(float FirstPersonOverride);

	// Breath State

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FALSXTBreathState BreathState;

	// Breath State
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTBreathState& GetBreathState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBreathState"))
	void SetBreathState(const FALSXTBreathState& NewBreathState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBreathState"))
	FALSXTBreathState ProcessNewBreathState(const FALSXTBreathState& NewALSXTBreathState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewBreathState(const FALSXTBreathState& NewALSXTBreathState);

	// Pose State
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FALSXTPoseState ALSXTPoseState;

	// Pose State
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTPoseState& GetALSXTPoseState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPoseState"))
	void SetALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPoseState"))
	FALSXTPoseState ProcessNewALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState);

	// Vaulting State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Vaulting", Meta = (AllowPrivateAccess), Transient)
	FALSXTVaultingState VaultingState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTVaultingState& GetVaultingState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	void SetVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	FALSXTVaultingState ProcessNewVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewVaultingState(const FALSXTVaultingState& NewVaultingState);

	// Stationary Mode State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|State|Vaulting", Meta = (AllowPrivateAccess), Transient)
	FALSXTStationaryModeState StationaryModeState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTStationaryModeState& GetStationaryModeState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStationaryModeState"))
	void SetStationaryModeState(const FALSXTStationaryModeState& NewStationaryModeState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStationaryModeState"))
	FALSXTStationaryModeState ProcessNewStationaryModeState(const FALSXTStationaryModeState& NewStationaryModeState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewStationaryModeState(const FALSXTStationaryModeState& NewStationaryModeState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetStationaryModeState(const FALSXTStationaryModeState& NewStationaryModeState);

	UFUNCTION()
	void OnReplicate_StationaryModeState(const FALSXTStationaryModeState& PreviousStationaryModeState);
	
	UFUNCTION(Server, Unreliable)
	void ServerSetALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState);

	UFUNCTION()
	void OnReplicate_ALSXTPoseState(const FALSXTPoseState& PreviousALSXTPoseState);

// Breath
	UFUNCTION(Client, Reliable)
	void ClientSetBreathState(const FALSXTBreathState& NewBreathState);

	UFUNCTION(Server, Reliable)
	void ServerSetBreathState(const FALSXTBreathState& NewBreathState);

	UFUNCTION()
	void OnReplicate_BreathState(const FALSXTBreathState& PreviousBreathState);


	UFUNCTION(Server, Unreliable)
	void ServerSetVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION()
	void OnReplicate_VaultingState(const FALSXTVaultingState& PreviousVaultingState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnALSXTPoseStateChanged(const FALSXTPoseState& PreviousALSXTPoseState);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnStationaryModeStateChanged(const FALSXTStationaryModeState& PreviousStationaryModeState);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnBreathStateChanged(const FALSXTBreathState& PreviousBreathState);
	
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnVaultingStateChanged(const FALSXTVaultingState& PreviousVaultingState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	bool bMovementEnabled{ true };

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UALSXTAnimationInstance> XTAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	FVector2D PreviousLookInput;

	// Footstep State

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_FootprintsState", Meta = (AllowPrivateAccess))
	FALSXTFootprintsState FootprintsState;

protected:

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ClientSetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void MulticastSetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void UpdateFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintStatePhase& Target, const float& Alpha);

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void SetFootprintNewSurface(const FALSXTFootprintsState& UpdatedTargetState, const FALSXTFootprintsState& NewFootprintsState, const EAlsFootBone& Foot);

protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	void StartFootSaturation(const EAlsFootBone& Foot, const FALSXTFootprintStatePhase& TargetState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ResetFootSaturationTimeline(const EAlsFootBone& Foot);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	UTimelineComponent* GetFootTimeline(const EAlsFootBone& Foot);

	// Aim State

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_AimState", Meta = (AllowPrivateAccess))
	FALSXTAimState AimState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTAimState& GetAimState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewAimState"))
	void SetAimState(const FALSXTAimState& NewAimState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewAimState"))
	FALSXTAimState ProcessNewAimState(const FALSXTAimState& NewAimState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewAimState(const FALSXTAimState& NewAimState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetAimState(const FALSXTAimState& NewAimState);

	UFUNCTION()
	void OnReplicate_AimState(const FALSXTAimState& PreviousAimState);

public:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnAimStateChanged(const FALSXTAimState& PreviousAimState);

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
	FALSXTFreelookState FreelookState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFreelooking{ALSXTFreelookingTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Freelooking{ALSXTFreelookingTags::False};

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTFreelookState& GetFreelookState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookState"))
	void SetFreelookState(const FALSXTFreelookState& NewFreelookState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookState"))
	FALSXTFreelookState ProcessNewFreelookState(const FALSXTFreelookState& NewFreelookState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewFreelookState(const FALSXTFreelookState& NewFreelookState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetFreelookState(const FALSXTFreelookState& NewFreelookState);

	UFUNCTION()
	void OnReplicate_FreelookState(const FALSXTFreelookState& PreviousFreelookState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFreelookStateChanged(const FALSXTFreelookState& PreviousFreelookState);

	// HeadLookAt
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Look At|State", ReplicatedUsing = "OnReplicate_HeadLookAtState", Meta = (AllowPrivateAccess))
	FALSXTHeadLookAtState HeadLookAtState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Look At", Meta = (AllowPrivateAccess))
	TArray<FALSXTHeadLookAtEntry> HeadLookAtEntries;

public:
	UFUNCTION(BlueprintCallable, Category = "Head Look At|State")
	const FALSXTHeadLookAtState& GetHeadLookAtState() const;

	virtual FALSXTHeadLookAtEntry GetBestHeadLookAtEntry_Implementation() const override;

	virtual bool CanHeadLookAt_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Head Look At|State", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	void SetHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Head Look At|State", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	FALSXTHeadLookAtState ProcessNewHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	virtual void AddHeadLookAtEntry_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry) override;
	virtual void RemoveHeadLookAtEntry_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry) override;
	virtual void BeginHeadLookAt_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry) override;
	virtual void EndHeadLookAt_Implementation() override;

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION()
	void OnReplicate_HeadLookAtState(const FALSXTHeadLookAtState& PreviousHeadLookAtState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Head Look At|State")
	void OnHeadLookAtStateChanged(const FALSXTHeadLookAtState& PreviousHeadLookAtState);

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
	FGameplayTag DesiredInjury{ALSXTInjuryTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Injury{ALSXTInjuryTags::None};

	// CombatStance

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredCombatStance{ALSXTCombatStanceTags::Neutral};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag CombatStance{ALSXTCombatStanceTags::Neutral};

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
	FALSXTSlidingState SlidingState;

	// Defensive Mode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredDefensiveMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag DefensiveMode {FGameplayTag::EmptyTag};

	// Emote

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredEmote{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Emote{ FGameplayTag::EmptyTag };

	// Gesture

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGesture{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Gesture{ FGameplayTag::EmptyTag };

	// GestureHand

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGestureHand{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GestureHand{ FGameplayTag::EmptyTag };

	// Desired Emote

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredEmote() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewEmoteTag"))
	void SetDesiredEmote(const FGameplayTag& NewEmoteTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredEmote(const FGameplayTag& NewEmoteTag);

	// Emote

public:
	const FGameplayTag& GetEmote() const;

private:
	void SetEmote(const FGameplayTag& NewGestureTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnEmoteChanged(const FGameplayTag& PreviousEmoteTag);

	// Desired Gesture

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredGesture() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewGestureTag"))
	void SetDesiredGesture(const FGameplayTag& NewGestureTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGesture(const FGameplayTag& NewGestureTag);

	// Gesture

public:
	const FGameplayTag& GetGesture() const;

private:
	void SetGesture(const FGameplayTag& NewGestureTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGestureChanged(const FGameplayTag& PreviousGestureTag);

	// Desired GestureHand

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredGestureHand() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewGestureHandTag"))
	void SetDesiredGestureHand(const FGameplayTag& NewGestureHandTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGestureHand(const FGameplayTag& NewGestureHandTag);

	// GestureHand

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanGesture() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSelectGesture() const;

	const FGameplayTagContainer GetAvailableGestureHands() const;

	const FGameplayTag& GetGestureHand() const;

private:
	void SetGestureHand(const FGameplayTag& NewGestureHandTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGestureHandChanged(const FGameplayTag& PreviousGestureHandTag);

	// Defensive Mode State
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Defensive Mode", ReplicatedUsing = "OnReplicate_DefensiveModeState", Meta = (AllowPrivateAccess))
	FALSXTDefensiveModeState DefensiveModeState;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_DefensiveModeAnimations", Meta = (AllowPrivateAccess))
	// DefensiveModeAnimations DefensiveModeAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", Meta = (AllowPrivateAccess))
	FALSXTDefensiveModeAnimations DefensiveModeAnimations;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", Transient)
	// FALSXTDefensiveModeState DefensiveModeState;

public:	
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTSlidingState& GetSlidingState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewSlidingState"))
	void SetSlidingState(const FALSXTSlidingState& NewSlidingState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewSlidingState"))
	FALSXTSlidingState ProcessNewSlidingState(const FALSXTSlidingState& NewSlidingState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewSlidingState(const FALSXTSlidingState& NewSlidingState);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetSlidingState(const FALSXTSlidingState& NewSlidingState);

	UFUNCTION()
	void OnReplicate_SlidingState(const FALSXTSlidingState& PreviousSlidingState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Sliding State")
	void ClientSetSlidingState(const FALSXTSlidingState& NewSlidingState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Sliding State")
	void MulticastSetSlidingState(const FALSXTSlidingState& NewSlidingState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnSlidingStateChanged(const FALSXTSlidingState& PreviousSlidingState);

	/////

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTDefensiveModeState& GetDefensiveModeState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTDefensiveModeAnimations& GetDefensiveModeAnimations() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	void SetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeAnimations"))
	void SetDefensiveModeAnimations(const FALSXTDefensiveModeAnimations& NewDefensiveModeAnimations);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	void SetDefensiveModeStateImplementation(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void ResetDefensiveModeState();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	FALSXTDefensiveModeState ProcessNewDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION()
	void OnReplicate_DefensiveModeState(const FALSXTDefensiveModeState& PreviousDefensiveModeState);

	// UFUNCTION()
	// void OnReplicate_DefensiveModeAnimations(const FALSXTDefensiveModeAnimations& PreviousDefensiveModeAnimations);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Defensive Mode State")
	void ClientSetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Defensive Mode State")
	void MulticastSetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnDefensiveModeStateChanged(const FALSXTDefensiveModeState& PreviousDefensiveModeState);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectGestureAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectEmoteAction;

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
	AALSXTCharacter();

	virtual void NotifyControllerChanged() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Crouch(bool bClientSimulation = false) override;

	virtual void InputCrouch();

	void ApplyDesiredStance() override;

	void ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport);

	// Camera

protected:
	virtual void BeginPlay() override;
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

	void InputLeanLeft(const FInputActionValue& ActionValue);

	void InputToggleLeanLeft(const FInputActionValue& ActionValue);

	void InputLeanRight(const FInputActionValue& ActionValue);

	void InputToggleLeanRight(const FInputActionValue& ActionValue);

	void InputSwitchWeaponFirearmStance();

	void InputSwitchWeaponReadyPosition();

	void InputSwitchGripPosition();

	void InputSwitchForegripPosition();

	void InputSelectEmote(const FInputActionValue& ActionValue);

	void InputSelectGesture(const FInputActionValue& ActionValue);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnViewModeChanged(const FGameplayTag& PreviousViewModeTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryInteraction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformSecondaryInteraction() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetMovementModeLocked(bool bNewMovementModeLocked);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSetToViewMode(const FGameplayTag& ViewModeTag) const;

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

	bool TryStartVaulting(const FALSXTVaultingTraceSettings& TraceSettings);

	UFUNCTION(Server, Reliable)
	void ServerStartVaulting(const FALSXTVaultingParameters& Parameters);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartVaulting(const FALSXTVaultingParameters& Parameters);

	void StartVaultingImplementation(const FALSXTVaultingParameters& Parameters);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UALSXTMantlingSettings* SelectMantlingSettingsXT();

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UALSXTSlidingSettings* SelectSlidingSettings();

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UALSXTVaultingSettings* SelectVaultingSettings(const FGameplayTag& VaultingType);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	FVaultAnimation SelectVaultingMontage(const FGameplayTag& CurrentGait, const FGameplayTag& VaultingType);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnVaultingStarted(const FALSXTVaultingParameters& Parameters);

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
		FALSXTFootwearDetails GetFootwearDetails() const;

	// Footstep State

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTFootprintsState& GetFootprintsState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	void SetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	FALSXTFootprintsState ProcessNewFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

private:

	UFUNCTION()
	void OnReplicate_FootprintsState(const FALSXTFootprintsState& PreviousFootprintsState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFootprintsStateChanged(const FALSXTFootprintsState& PreviousFootprintsState);

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
	const FGameplayTag& GetDesiredCombatStance() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewCombatStanceTag"))
	void SetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag);

	// CombatStance

public:
	const FGameplayTag& GetCombatStance() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanToggleCombatReady() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanBecomeCombatReady() const;

private:
	void SetCombatStance(const FGameplayTag& NewCombatStanceTag);

protected:
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
	virtual AALSXTCharacter* GetCharacter_Implementation() override;
	virtual UALSXTAnimationInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual UALSXTCharacterSettings* GetCharacterSettings_Implementation() const override;
	virtual UInputComponent* GetCharacterInputComponent_Implementation() const override;
	virtual bool IsCharacterPlayerControlled_Implementation() const override;
	virtual bool CanEmote_Implementation() const override;
	virtual bool CanGesture_Implementation() const override;

	virtual void SetCharacterLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariant) override;

	virtual FALSXTStationaryModeState GetCharacterStationaryModeState_Implementation() const override;
	virtual void GetCharacterGesture_Implementation(FGameplayTag& NewGesture, FGameplayTag& NewGestureHand) const override;
	virtual void SetCharacterGesture_Implementation(const FGameplayTag& NewGesture, const FGameplayTag& NewGestureHand) override;

	// Viewport Mask Functions
	virtual void SetViewportMask_Implementation(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask) override;

	// Component Interface Functions
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() const override;
	virtual UCapsuleComponent* GetCharacterCapsuleComponent_Implementation() const override;
	virtual UAlsCharacterMovementComponent* GetCharacterMovementComponent_Implementation() const override;

	// Footprint Interface Functions
	virtual FALSXTFootprintsState GetCharacterFootprintsState_Implementation() const override;
	virtual FALSXTFootwearDetails GetCharacterFootwearDetails_Implementation() const override;

	// Collision Interface Functions
	virtual void AddCollisionImpulse_Implementation(FVector NewImpulse) override;
	virtual void OnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit) override;
	virtual void OnActorAttackCollision_Implementation(FAttackDoubleHitResult Hit) override;
	virtual FALSXTSlidingState GetCharacterSlidingState_Implementation() const override;
	virtual FALSXTDefensiveModeState GetCharacterDefensiveModeState_Implementation() const override;
	virtual FALSXTDefensiveModeAnimations GetCharacterDefensiveModeAnimations_Implementation() const override;
	virtual FALSXTBumpPoseState GetCrowdNavigationPoseState_Implementation() const override;

	virtual FALSXTPhysicalAnimationState GetCharacterPhysicalAnimationState_Implementation() const override;
	virtual void SetCharacterPhysicalAnimationState_Implementation(FALSXTPhysicalAnimationState NewPhysicalAnimationState) override;

	virtual FGameplayTag GetCharacterPhysicalAnimationMode_Implementation() const override;
	virtual void SetCharacterPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationMode, const TArray<FName>& BelowBoneNames) override;
	virtual void ResetCharacterPhysicalAnimationMode_Implementation() override;

	// State Interface Functions
	virtual void SetCharacterStatus_Implementation(const FGameplayTag& NewStatus) override;
	virtual void SetCharacterMovementModeLocked_Implementation(bool NewLocked) override;
	virtual void SetCharacterStance_Implementation(const FGameplayTag& NewStance) override;
	virtual FGameplayTag GetCharacterGait_Implementation() const override;
	virtual FGameplayTag GetCharacterLean_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionMode_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionAction_Implementation() const override;		
	virtual void SetCharacterLocomotionAction_Implementation(const FGameplayTag& NewLocomotionAction) override;
	virtual FALSXTPoseState GetCharacterPoseState_Implementation() const override;
	virtual FALSXTBreathState GetCharacterBreathState_Implementation() const override;
	virtual FGameplayTag GetCharacterLocomotionVariant_Implementation() const override;
	virtual FGameplayTag GetCharacterVaultType_Implementation() const override;

	// Character Sound Component Interface Functions
	virtual bool CanPlayCharacterMovementSound_Implementation() const;
	virtual bool CanPlayWeaponMovementSound_Implementation() const;
	virtual bool ShouldPlayWeaponMovementSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Strength) const;
	virtual bool ShouldPlayMovementAccentSound_Implementation(UPARAM(meta = (Categories = "Als.LocomotionAction"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight) const;
	virtual void PlayBreathEffects_Implementation(const FGameplayTag& StaminaOverride);
	virtual void PlayCharacterMovementSound_Implementation(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight);
	virtual void PlayActionSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const float Stamina) override;
	virtual void PlayAttackSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina) override;
	virtual void PlayDamageSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage) override;

	virtual float GetCurrentStamina_Implementation() const;
	virtual FGameplayTag GetCurrentStaminaTag_Implementation() const;
	virtual FGameplayTag GetCurrentBreathType_Implementation() const;
	virtual FString GetCurrentBreathSounds_Implementation() const;
	virtual FString GetCurrentBreathSound_Implementation() const;
	// virtual FString GetCurrentVocalizations_Implementation() const;
	virtual FString GetCurrentVocalization_Implementation() const;

	// Freelooking Interface Functions
	virtual FGameplayTag GetCharacterFreelooking_Implementation() const override;
	virtual FALSXTFreelookState GetCharacterFreelookState_Implementation() const override;
	virtual FALSXTHeadLookAtState GetCharacterHeadLookAtState_Implementation() const override;
	
	// Defensive Mode Interface Functions
	virtual FGameplayTag GetCharacterDefensiveMode_Implementation() const override;
	virtual bool IsBlocking_Implementation() const override;	
	virtual void SetCharacterDefensiveMode_Implementation(const FGameplayTag& NewDefensiveMode) override;
	virtual void SetCharacterDefensiveModeState_Implementation(FALSXTDefensiveModeState NewDefensiveModeState) override;
	virtual void SetCharacterDefensiveModeAnimations_Implementation(FALSXTDefensiveModeAnimations NewDefensiveModeAnimations) override;
	virtual void ResetCharacterDefensiveModeState_Implementation() override;

	// Held Item Interface Functions
	virtual FGameplayTag GetCharacterForegripPosition_Implementation() const override;	
	virtual FGameplayTag GetCharacterWeaponCarryPosition_Implementation() const override;
	virtual FTransform GetCharacterCurrentForegripTransform_Implementation() const override;
	virtual FALSXTAimState GetCharacterAimState_Implementation() const override;
	virtual FALSXTHeldItemState GetCharacterHeldItemState_Implementation() const override;

	// Idle Animation Component
	virtual bool ShouldIdle_Implementation() const override;
	virtual FALSXTIdleState GetIdleState_Implementation() const override;
};

inline const FGameplayTag& AALSXTCharacter::GetDesiredEmote() const
{
	return DesiredEmote;
}

inline const FGameplayTag& AALSXTCharacter::GetEmote() const
{
	return Emote;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredGesture() const
{
	return DesiredGesture;
}

inline const FGameplayTag& AALSXTCharacter::GetGesture() const
{
	return Gesture;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredGestureHand() const
{
	return DesiredGestureHand;
}

inline const FGameplayTag& AALSXTCharacter::GetGestureHand() const
{
	return GestureHand;
}

inline const FGameplayTagContainer AALSXTCharacter::GetAvailableGestureHands() const
{
	FGameplayTagContainer AvailableHands;
	return AvailableHands;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredLean() const
{
	return DesiredLean;
}

inline const FGameplayTag& AALSXTCharacter::GetLean() const
{
	return Lean;
}

inline const FALSXTBreathState& AALSXTCharacter::GetBreathState() const
{
	return BreathState;
}

inline const FALSXTPoseState& AALSXTCharacter::GetALSXTPoseState() const
{
	return ALSXTPoseState;
}

inline const FALSXTStationaryModeState& AALSXTCharacter::GetStationaryModeState() const
{
	return StationaryModeState;
}

inline const FALSXTVaultingState& AALSXTCharacter::GetVaultingState() const
{
	return VaultingState;
}

inline const FALSXTFootprintsState& AALSXTCharacter::GetFootprintsState() const
{
	return FootprintsState;
}

inline const FALSXTFreelookState& AALSXTCharacter::GetFreelookState() const
{
	return FreelookState;
}

inline const FALSXTHeadLookAtState& AALSXTCharacter::GetHeadLookAtState() const
{
	return HeadLookAtState;
}

inline const FALSXTAimState& AALSXTCharacter::GetAimState() const
{
	return AimState;
}

inline const FALSXTSlidingState& AALSXTCharacter::GetSlidingState() const
{
	return SlidingState;
}

inline const FALSXTDefensiveModeState& AALSXTCharacter::GetDefensiveModeState() const
{
	return DefensiveModeState;
}

inline const FALSXTDefensiveModeAnimations& AALSXTCharacter::GetDefensiveModeAnimations() const
{
	return DefensiveModeAnimations;
}

inline const FALSXTHeldItemState& AALSXTCharacter::GetHeldItemState() const
{
	return HeldItemState;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredFreelooking() const
{
	return DesiredFreelooking;
}

inline const FGameplayTag& AALSXTCharacter::GetFreelooking() const
{
	return Freelooking;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredSex() const
{
	return DesiredSex;
}

inline const FGameplayTag& AALSXTCharacter::GetSex() const
{
	return Sex;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredLocomotionVariant() const
{
	return DesiredLocomotionVariant;
}

inline const FGameplayTag& AALSXTCharacter::GetLocomotionVariant() const
{
	return LocomotionVariant;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredInjury() const
{
	return DesiredInjury;
}

inline const FGameplayTag& AALSXTCharacter::GetInjury() const
{
	return Injury;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredCombatStance() const
{
	return DesiredCombatStance;
}

inline const FGameplayTag& AALSXTCharacter::GetCombatStance() const
{
	return CombatStance;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredWeaponFirearmStance() const
{
	return DesiredWeaponFirearmStance;
}

inline const FGameplayTag& AALSXTCharacter::GetWeaponFirearmStance() const
{
	return WeaponFirearmStance;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredWeaponReadyPosition() const
{
	return DesiredWeaponReadyPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetWeaponReadyPosition() const
{
	return WeaponReadyPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredDefensiveMode() const
{
	return DesiredDefensiveMode;
}

inline const FGameplayTag& AALSXTCharacter::GetDefensiveMode() const
{
	return DefensiveMode;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredStationaryMode() const
{
	return DesiredStationaryMode;
}

inline const FGameplayTag& AALSXTCharacter::GetStationaryMode() const
{
	return StationaryMode;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredStatus() const
{
	return DesiredStatus;
}

inline const FGameplayTag& AALSXTCharacter::GetStatus() const
{
	return Status;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredFocus() const
{
	return DesiredFocus;
}

inline const FGameplayTag& AALSXTCharacter::GetFocus() const
{
	return Focus;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredPhysicalAnimationMode() const
{
	return DesiredPhysicalAnimationMode;
}

inline const FALSXTPhysicalAnimationState AALSXTCharacter::GetPhysicalAnimationState() const
{
	return PhysicalAnimationState;
}

inline const FGameplayTag& AALSXTCharacter::GetPhysicalAnimationMode() const
{
	return PhysicalAnimationMode;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredGripPosition() const
{
	return DesiredGripPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetGripPosition() const
{
	return GripPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredForegripPosition() const
{
	return DesiredForegripPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetForegripPosition() const
{
	return ForegripPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredWeaponCarryPosition() const
{
	return DesiredWeaponCarryPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetWeaponCarryPosition() const
{
	return WeaponCarryPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredVaultType() const
{
	return DesiredVaultType;
}

inline const FGameplayTag& AALSXTCharacter::GetVaultType() const
{
	return VaultType;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredWeaponObstruction() const
{
	return DesiredWeaponObstruction;
}

inline const FGameplayTag& AALSXTCharacter::GetWeaponObstruction() const
{
	return WeaponObstruction;
}