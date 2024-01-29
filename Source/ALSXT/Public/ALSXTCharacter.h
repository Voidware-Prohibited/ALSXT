#pragma once

#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "ALSXTCharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Mesh/ALSXTPaintableSkeletalMeshComponent.h"
#include "Components/Mesh/ALSXTPaintableStaticMeshComponent.h"
#include "Components/Character/ALSXTCharacterCustomizationComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CineCameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTCombatSettings.h"
#include "Settings/ALSXTImpactReactionSettings.h"
#include "State/AlsLocomotionState.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "State/ALSXTPoseState.h"
#include "State/ALSXTFootstepState.h"
#include "State/ALSXTAimState.h"
#include "State/ALSXTDefensiveModeState.h"
#include "State/ALSXTFreelookState.h"
#include "State/ALSXTHeadLookAtState.h"
#include "State/ALSXTSlidingState.h"
#include "State/ALSXTVaultingState.h"
#include "Interfaces/ALSXTCharacterCustomizationComponentInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTStationaryModeComponentInterface.h"
#include "Interfaces/ALSXTCollisionInterface.h"
#include "Interfaces/ALSXTMeshPaintingInterface.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTIdleAnimationComponentInterface.h"
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
class ALSXT_API AALSXTCharacter : public AAlsCharacter, public IALSXTCharacterCustomizationComponentInterface, public IALSXTStationaryModeComponentInterface, public IALSXTCollisionInterface, public IALSXTMeshPaintingInterface, public IALSXTCharacterInterface, public IALSXTIdleAnimationComponentInterface
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UCineCameraComponent> BodyCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> KillerCameraSpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UCineCameraComponent> KillerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> OverlaySkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTPaintableStaticMeshComponent> OverlayStaticMesh;
	
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> PhysicsConstraints;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneCaptureComponent2D> MeshPaintingSceneCaptureComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> AimAnimationHelperBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class UALSXTCharacterSoundComponent* CharacterSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCustomizationComponent* CharacterCustomization;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UPhysicalAnimationComponent* PhysicalAnimation;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UALSXTPaintableSkeletalMeshComponent> ALSXTMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UALSXTCharacterMovementComponent> ALSXTCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FALSXTPoseState ALSXTPoseState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	int32 VaultingRootMotionSourceId;

public:
	virtual FALSXTGlobalGeneralMeshPaintingSettings GetGlobalGeneralMeshPaintingSettings_Implementation() const override;

	virtual USceneCaptureComponent2D* GetSceneCaptureComponent_Implementation() const override;

	virtual void GetElementalCondition_Implementation(USceneComponent* Component, UPARAM(meta = (Categories = "Als.Elemental Condition"))FGameplayTag& ElementalCondition) const override;

	virtual void PaintMesh_Implementation(USceneComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Radius) const override;

	virtual void VolumePaintMesh_Implementation(USceneComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent) const override;

	virtual void ResetPaintTypeOnComponent_Implementation(USceneComponent* Component, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) const override;

	virtual void ResetPaintOnComponent_Implementation(USceneComponent* Component) const override;

	virtual void ResetPaintTypeOnAllComponents_Implementation(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) const override;

	virtual void ResetPaintOnAllComponents_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableInputMovement(const bool Disable);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void DisableLookAt(const bool Disable);

public:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", Meta = (AllowPrivateAccess))
	FALSXTVaultingState VaultingState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTVaultingState& GetVaultingState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	void SetVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewVaultingState"))
	FALSXTVaultingState ProcessNewVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewVaultingState(const FALSXTVaultingState& NewVaultingState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState);

	UFUNCTION()
	void OnReplicate_ALSXTPoseState(const FALSXTPoseState& PreviousALSXTPoseState);


	UFUNCTION(Server, Unreliable)
	void ServerSetVaultingState(const FALSXTVaultingState& NewVaultingState);

	UFUNCTION()
	void OnReplicate_VaultingState(const FALSXTVaultingState& PreviousVaultingState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnALSXTPoseStateChanged(const FALSXTPoseState& PreviousALSXTPoseState);
	
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnVaultingStateChanged(const FALSXTVaultingState& PreviousVaultingState);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	bool bMovementEnabled {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UALSXTAnimationInstance> XTAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Replicated, Meta = (AllowPrivateAccess))
	FVector2D PreviousLookInput;

	// Footstep State

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_FootprintsState", Meta = (AllowPrivateAccess))
	FALSXTFootprintsState FootprintsState;

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

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnAimStateChanged(const FALSXTAimState& PreviousAimState);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	bool DoesOverlayObjectUseLeftHandIK() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "ForegripPosition"))
	FName GetSocketNameForForegripPosition(UPARAM(meta = (Categories = "Als.Foregrip Position"))const FGameplayTag& ForegripPosition);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FGameplayTagContainer GetAvailableForegripPositionsForOvelayObject();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	FTransform GetCurrentForegripTransform();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", ReplicatedUsing = "OnReplicate_HeadLookAtState", Meta = (AllowPrivateAccess))
	FALSXTHeadLookAtState HeadLookAtState;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTHeadLookAtState& GetHeadLookAtState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	void SetHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHeadLookAtState"))
	FALSXTHeadLookAtState ProcessNewHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState);

	UFUNCTION()
	void OnReplicate_HeadLookAtState(const FALSXTHeadLookAtState& PreviousHeadLookAtState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnHeadLookAtStateChanged(const FALSXTHeadLookAtState& PreviousHeadLookAtState);

private:
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
	FGameplayTag DesiredDefensiveMode{ALSXTDefensiveModeTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag DefensiveMode {ALSXTDefensiveModeTags::None};

	// Defensive Mode State
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_DefensiveModeState", Meta = (AllowPrivateAccess))
	FALSXTDefensiveModeState DefensiveModeState;

public:	
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTDefensiveModeState& GetDefensiveModeState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDefensiveModeState"))
	void SetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFocus{ALSXTFocusedTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Focus{ALSXTFocusedTags::False};

	FTimerHandle FreelookTimerHandle;	// Timer Handle for Attack Trace
	FTimerDelegate FreelookTimerDelegate; // Delegate to bind function with parameters

	// Attack Trace Settings

	FTimerHandle AttackTraceTimerHandle;	// Timer Handle for Attack Trace
	FTimerDelegate AttackTraceTimerDelegate; // Delegate to bind function with parameters

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	FALSXTCombatAttackTraceSettings AttackTraceSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	TArray<AActor*> AttackTraceLastHitActors;

private:

	// HoldingBreath

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredHoldingBreath{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HoldingBreath{FGameplayTag::EmptyTag};

// PhysicalAnimationMode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredPhysicalAnimationMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag PhysicalAnimationMode{FGameplayTag::EmptyTag};

// Gesture

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGesture{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Gesture{FGameplayTag::EmptyTag};

// GestureHand

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGestureHand{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GestureHand{FGameplayTag::EmptyTag};

// ReloadingType

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredReloadingType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ReloadingType{FGameplayTag::EmptyTag};

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

// FirearmFingerAction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFirearmFingerAction{ALSXTFirearmFingerActionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerAction{ALSXTFirearmFingerActionTags::None};

// FirearmFingerActionHand

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFirearmFingerActionHand{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerActionHand{FGameplayTag::EmptyTag};

// WeaponCarryPosition

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredWeaponCarryPosition{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponCarryPosition{FGameplayTag::EmptyTag};

// FirearmSightLocation

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFirearmSightLocation{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmSightLocation{FGameplayTag::EmptyTag};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> LookMouseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> RagdollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> RotationModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> ViewModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SwitchShoulderAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> FreelookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> ToggleCombatReadyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SecondaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SecondaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> LeanLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> LeanRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> HoldBreathAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SlideAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AcrobaticAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SwitchTargetLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SwitchTargetRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> ToggleWeaponReadyPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> ToggleWeaponFirearmStanceAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SwitchGripPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SwitchForegripPositionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SelectEmoteAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SelectGestureAction;

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

	// Input

public:
	UFUNCTION()
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

public:
	void InputBlock(const FInputActionValue& ActionValue);

private:
	void InputLookMouse(const FInputActionValue& ActionValue);

	void InputLook(const FInputActionValue& ActionValue);

	void InputMove(const FInputActionValue& ActionValue);

	void InputSprint(const FInputActionValue& ActionValue);

	void InputWalk();

	void InputJump(const FInputActionValue& ActionValue);

	void InputAim(const FInputActionValue& ActionValue);

	void InputRagdoll();

	void InputRoll();

	void InputRotationMode();

	void InputViewMode();

	void InputSwitchShoulder();

	void InputFreelook(const FInputActionValue& ActionValue);

	void InputToggleCombatReady();

	void InputHoldBreath(const FInputActionValue& ActionValue);

	void InputLeanLeft();

	void InputLeanRight();

	void InputAcrobatic();

	void InputSwitchTargetLeft();

	void InputSwitchTargetRight();

	void InputToggleWeaponFirearmStance();

	void InputToggleWeaponReadyPosition();

	void InputReload();

	void InputReloadWithRetention();

	void InputSwitchGripPosition();

	void InputSwitchForegripPosition();

	void InputSelectEmote(const FInputActionValue& ActionValue);

	void InputSelectGesture(const FInputActionValue& ActionValue);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputPrimaryAction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputSecondaryAction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputPrimaryInteraction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputSecondaryInteraction();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryAction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformSecondaryAction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryInteraction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformSecondaryInteraction() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnViewModeChanged(const FGameplayTag& PreviousViewModeTag);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetMovementModeLocked(bool bNewMovementModeLocked);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSetToViewMode(const FGameplayTag& ViewModeTag) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetUnarmedTraceLocations(const FGameplayTag& UnarmedAttackType, FVector& Start, FVector& End, float& Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetUnarmedAttackDamageInfo(const FGameplayTag& UnarmedAttackType, const FGameplayTag& UnarmedAttackStrength, float& BaseDamage, FGameplayTag& ImpactForm, FGameplayTag& DamageType) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetBlockingInfo(const FAttackDoubleHitResult& AttackDoubleHitResult, float& BaseDamage) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	bool IsHoldingItem() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetHeldItemTraceLocations(bool& Found, FVector& Start, FVector& End, float& Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetHeldItemAttackDamageInfo(const FGameplayTag& HeldItemAttackType, const FGameplayTag& HeldItemAttackStrength, float& BaseDamage, FGameplayTag& ImpactForm, FGameplayTag& DamageType) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	void GetHeldItemViewTarget(FTransform& Transform, float& FOV, float& VignetteIntensity, bool& Attachment) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	bool IsHoldingAimableItem() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Aim Down Sights")
	bool IsAimingDownSights() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|First Person Eye Focus")
	bool IsFirstPersonEyeFocusActive() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Stamina")
	float GetActionBaseStaminaCost(UPARAM(meta = (Categories = "Als.LocomotionAction"))const FGameplayTag& Action, UPARAM(meta = (Categories = "Als.Action Strength")) const FGameplayTag& ActionStrength) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Stamina")
	float GetAttackBaseStaminaCost(UPARAM(meta = (Categories = "Als.Attack Method")) const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength")) const FGameplayTag& ActionStrength) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Stamina")
	float GetDamageBaseStaminaCost(float Damage, UPARAM(meta = (Categories = "Als.Impact Form")) const FGameplayTag& ImpactForm) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSprint() const;
	void CanSprint_Implementation();

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

public:
	// Footstep Values

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
		FALSXTFootwearDetails GetFootwearDetails() const;

	// Footstep State

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTFootprintsState& GetFootprintsState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	void SetFootprintsState(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	FALSXTFootprintsState ProcessNewFootprintsState(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewFootprintsState(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetFootprintsState(const EALSXTFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState);

	UFUNCTION()
	void OnReplicate_FootprintsState(const FALSXTFootprintsState& PreviousFootprintsState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFootprintsStateChanged(const FALSXTFootprintsState& PreviousFootprintsState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanAimDownSights() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanAim() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanRoll() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanVault() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSlide() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanWallrun() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool TryVault() const;

protected:		
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartVault();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartWallrun();

	// Desired Freelooking

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FGameplayTag& GetDesiredFreelooking() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookingTag"))
	void SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
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
	UFUNCTION(Server, Reliable)
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
	void BeginFreelookTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void FreelookTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void EndFreelookTimer();

	// Attack Collision Trace
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void BeginAttackCollisionTrace(FALSXTCombatAttackTraceSettings TraceSettings);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void AttackCollisionTrace();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void EndAttackCollisionTrace();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnAttackCollision(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void GetCameraShakeInfoFromHit(FAttackDoubleHitResult Hit, TSubclassOf<UCameraShakeBase>& CameraShakeClass, float& Scale);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void GetFirearmCameraShakeInfo(TSubclassOf<UCameraShakeBase>& CameraShakeClass, float& Scale);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnAttackHit(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnImpactCollision(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnImpactHit(FDoubleHitResult Hit);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Als Character")
	void GetLocationFromBoneName(FName Hit, FGameplayTag& Location);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void GetFormFromHit(FDoubleHitResult Hit, FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength);

	// Desired HoldingBreath

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanHoldBreath() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredHoldingBreath() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHoldingBreathTag"))
	void SetDesiredHoldingBreath(const FGameplayTag& NewHoldingBreathTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredHoldingBreath(const FGameplayTag& NewHoldingBreathTag);

	// HoldingBreath

public:
	const FGameplayTag& GetHoldingBreath() const;

private:
	void SetHoldingBreath(const FGameplayTag& NewHoldingBreathTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnHoldingBreathChanged(const FGameplayTag& PreviousHoldingBreathTag);

	// Desired PhysicalAnimationMode

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredPhysicalAnimationMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewPhysicalAnimationModeTag"))
	void SetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName);

	// PhysicalAnimationMode

public:
	const FGameplayTag& GetPhysicalAnimationMode() const;

private:
	void SetPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const FName& BoneName);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnPhysicalAnimationModeChanged(const FGameplayTag& PreviousPhysicalAnimationModeTag);

	// Emote

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanEmote() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSelectEmote() const;

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

	// Desired ReloadingType

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredReloadingType() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewReloadingTypeTag"))
		void SetDesiredReloadingType(const FGameplayTag& NewReloadingTypeTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredReloadingType(const FGameplayTag& NewReloadingTypeTag);

	// ReloadingType

public:
	const FGameplayTag& GetReloadingType() const;

private:
	void SetReloadingType(const FGameplayTag& NewReloadingTypeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnReloadingTypeChanged(const FGameplayTag& PreviousReloadingTypeTag);

// Desired FirearmFingerAction

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredFirearmFingerAction() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFirearmFingerActionTag"))
	void SetDesiredFirearmFingerAction(const FGameplayTag& NewFirearmFingerActionTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredFirearmFingerAction(const FGameplayTag& NewFirearmFingerActionTag);

// FirearmFingerAction

public:
	const FGameplayTag& GetFirearmFingerAction() const;

private:
	void SetFirearmFingerAction(const FGameplayTag& NewFirearmFingerActionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFirearmFingerActionChanged(const FGameplayTag& PreviousFirearmFingerActionTag);

// Desired FirearmFingerActionHand

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredFirearmFingerActionHand() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFirearmFingerActionHandTag"))
	void SetDesiredFirearmFingerActionHand(const FGameplayTag& NewFirearmFingerActionHandTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredFirearmFingerActionHand(const FGameplayTag& NewFirearmFingerActionHandTag);

// FirearmFingerActionHand

public:
	const FGameplayTag& GetFirearmFingerActionHand() const;

private:
	void SetFirearmFingerActionHand(const FGameplayTag& NewFirearmFingerActionHandTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFirearmFingerActionHandChanged(const FGameplayTag& PreviousFirearmFingerActionHandTag);

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

// Desired FirearmSightLocation

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	const FGameplayTag& GetDesiredFirearmSightLocation() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFirearmSightLocationTag"))
	void SetDesiredFirearmSightLocation(const FGameplayTag& NewFirearmSightLocationTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredFirearmSightLocation(const FGameplayTag& NewFirearmSightLocationTag);

// FirearmSightLocation

public:
	const FGameplayTag& GetFirearmSightLocation() const;

private:
	void SetFirearmSightLocation(const FGameplayTag& NewFirearmSightLocationTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFirearmSightLocationChanged(const FGameplayTag& PreviousFirearmSightLocationTag);

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

};

inline const FALSXTPoseState& AALSXTCharacter::GetALSXTPoseState() const
{
	return ALSXTPoseState;
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

inline const FALSXTDefensiveModeState& AALSXTCharacter::GetDefensiveModeState() const
{
	return DefensiveModeState;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredHoldingBreath() const
{
	return DesiredHoldingBreath;
}

inline const FGameplayTag& AALSXTCharacter::GetHoldingBreath() const
{
	return HoldingBreath;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredPhysicalAnimationMode() const
{
	return DesiredPhysicalAnimationMode;
}

inline const FGameplayTag& AALSXTCharacter::GetPhysicalAnimationMode() const
{
	return PhysicalAnimationMode;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredReloadingType() const
{
	return DesiredReloadingType;
}

inline const FGameplayTag& AALSXTCharacter::GetReloadingType() const
{
	return ReloadingType;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredFirearmFingerAction() const
{
	return DesiredFirearmFingerAction;
}

inline const FGameplayTag& AALSXTCharacter::GetFirearmFingerAction() const
{
	return FirearmFingerAction;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredFirearmFingerActionHand() const
{
	return DesiredFirearmFingerActionHand;
}

inline const FGameplayTag& AALSXTCharacter::GetFirearmFingerActionHand() const
{
	return FirearmFingerActionHand;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredWeaponCarryPosition() const
{
	return DesiredWeaponCarryPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetWeaponCarryPosition() const
{
	return WeaponCarryPosition;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredFirearmSightLocation() const
{
	return DesiredFirearmSightLocation;
}

inline const FGameplayTag& AALSXTCharacter::GetFirearmSightLocation() const
{
	return FirearmSightLocation;
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