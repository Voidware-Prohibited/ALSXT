#pragma once

#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "State/AlsLocomotionState.h"
#include "Utility/ALSXTGameplayTags.h"
#include "State/ALSXTFootstepState.h"
#include "State/ALSXTSlidingState.h"
#include "ALSXTCharacter.generated.h"

class UALSXTAnimationInstance;
class UALSXTCharacterSettings;
class UAlsCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetupPlayerInputComponentDelegate);

UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class ALSXT_API AALSXTCharacter : public AAlsCharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTCharacterSettings> ALSXTSettings;

	UPROPERTY(BlueprintAssignable)
	FSetupPlayerInputComponentDelegate OnSetupPlayerInputComponentUpdated;

	UPROPERTY(BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{Cast<AAlsCharacter>(GetParentActor())};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UALSXTAnimationInstance> XTAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector MovementInput;

	// Footstep State

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_FootprintsState", Meta = (AllowPrivateAccess))
	FALSXTFootprintsState FootprintsState;

	// Freelooking

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFreelooking{ALSXTFreelookingTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Freelooking{ALSXTFreelookingTags::False};

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

	// Blocking

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredBlocking{ALSXTBlockingTags::NotBlocking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Blocking{ALSXTBlockingTags::NotBlocking};

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

	// HitReaction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredHitReaction{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HitReaction{FGameplayTag::EmptyTag};

	// HitSurface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredHitSurface{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HitSurface{FGameplayTag::EmptyTag};

	// BumpReaction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredBumpReaction{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag BumpReaction{FGameplayTag::EmptyTag};

	// BumpSurface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredBumpSurface{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag BumpSurface{FGameplayTag::EmptyTag};

	// HoldingBreath

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredHoldingBreath{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HoldingBreath{FGameplayTag::EmptyTag};

// Emote

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredEmote{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Emote{FGameplayTag::EmptyTag};

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

// ImpactType

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredImpactType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactType{FGameplayTag::EmptyTag};

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

	void InputBlock(const FInputActionValue& ActionValue);

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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnViewModeChanged(const FGameplayTag& PreviousViewModeTag);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetMovementModeLocked(bool bNewMovementModeLocked);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSetToViewMode(const FGameplayTag& ViewModeTag) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetUnarmedTraceLocations(const FGameplayTag& UnarmedAttackType, FVector& Start, FVector& End, float& Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	bool IsHoldingItem() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Held Item")
	void GetHeldItemTraceLocations(bool& Found, FVector& Start, FVector& End, float& Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	void GetHeldItemViewTarget(FTransform& Transform, float& FOV, float& VignetteIntensity, bool& Attachment) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	bool IsHoldingAimableItem() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Aim Down Sights")
	bool IsAimingDownSights() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|First Person Eye Focus")
	bool IsFirstPersonEyeFocusActive() const;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSprint() const;
	void CanSprint_Implementation();

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

	// Desired Blocking

public:
	const FGameplayTag& GetDesiredBlocking() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanBlock() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	bool IsBlocking() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBlockingTag"))
	void SetDesiredBlocking(const FGameplayTag& NewBlockingTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredBlocking(const FGameplayTag& NewBlockingTag);

	// Blocking

public:
	const FGameplayTag& GetBlocking() const;

private:
	void SetBlocking(const FGameplayTag& NewBlockingTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnBlockingChanged(const FGameplayTag& PreviousBlockingTag);

	// Desired StationaryMode

public:
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
	const FGameplayTag& GetDesiredStatus() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStatusTag"))
		void SetDesiredStatus(const FGameplayTag& NewStatusTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredStatus(const FGameplayTag& NewStatusTag);

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

	// Desired HitReaction

public:
	const FGameplayTag& GetDesiredHitReaction() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHitReactionTag"))
		void SetDesiredHitReaction(const FGameplayTag& NewHitReactionTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredHitReaction(const FGameplayTag& NewHitReactionTag);

	// HitReaction

public:
	const FGameplayTag& GetHitReaction() const;

private:
	void SetHitReaction(const FGameplayTag& NewHitReactionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnHitReactionChanged(const FGameplayTag& PreviousHitReactionTag);

	// Desired HitSurface

public:
	const FGameplayTag& GetDesiredHitSurface() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewHitSurfaceTag"))
		void SetDesiredHitSurface(const FGameplayTag& NewHitSurfaceTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredHitSurface(const FGameplayTag& NewHitSurfaceTag);

	// HitSurface

public:
	const FGameplayTag& GetHitSurface() const;

private:
	void SetHitSurface(const FGameplayTag& NewHitSurfaceTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnHitSurfaceChanged(const FGameplayTag& PreviousHitSurfaceTag);

	// Desired BumpReaction

public:
	const FGameplayTag& GetDesiredBumpReaction() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBumpReactionTag"))
		void SetDesiredBumpReaction(const FGameplayTag& NewBumpReactionTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredBumpReaction(const FGameplayTag& NewBumpReactionTag);

	// BumpReaction

public:
	const FGameplayTag& GetBumpReaction() const;

private:
	void SetBumpReaction(const FGameplayTag& NewBumpReactionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnBumpReactionChanged(const FGameplayTag& PreviousBumpReactionTag);

	// Desired BumpSurface

public:
	const FGameplayTag& GetDesiredBumpSurface() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewBumpSurfaceTag"))
		void SetDesiredBumpSurface(const FGameplayTag& NewBumpSurfaceTag);

private:
	UFUNCTION(Server, Reliable)
		void ServerSetDesiredBumpSurface(const FGameplayTag& NewBumpSurfaceTag);

	// BumpSurface

public:
	const FGameplayTag& GetBumpSurface() const;

private:
	void SetBumpSurface(const FGameplayTag& NewBumpSurfaceTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
		void OnBumpSurfaceChanged(const FGameplayTag& PreviousBumpSurfaceTag);

	// Desired HoldingBreath

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanHoldBreath() const;

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

	// Desired Emote

public:
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
	void SetEmote(const FGameplayTag& NewEmoteTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnEmoteChanged(const FGameplayTag& PreviousEmoteTag);

	// Desired Gesture

public:
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
	const FGameplayTag& GetDesiredGestureHand() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewGestureHandTag"))
	void SetDesiredGestureHand(const FGameplayTag& NewGestureHandTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGestureHand(const FGameplayTag& NewGestureHandTag);

// GestureHand

public:
	const FGameplayTag& GetGestureHand() const;

private:
	void SetGestureHand(const FGameplayTag& NewGestureHandTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGestureHandChanged(const FGameplayTag& PreviousGestureHandTag);

	// Desired ReloadingType

public:
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

// Desired ImpactType

public:
	const FGameplayTag& GetDesiredImpactType() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewImpactTypeTag"))
	void SetDesiredImpactType(const FGameplayTag& NewImpactTypeTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredImpactType(const FGameplayTag& NewImpactTypeTag);

// ImpactType

public:
	const FGameplayTag& GetImpactType() const;

private:
	void SetImpactType(const FGameplayTag& NewImpactTypeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnImpactTypeChanged(const FGameplayTag& PreviousImpactTypeTag);

// Desired WeaponCarryPosition

public:
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

inline const FALSXTFootprintsState& AALSXTCharacter::GetFootprintsState() const
{
	return FootprintsState;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredBlocking() const
{
	return DesiredBlocking;
}

inline const FGameplayTag& AALSXTCharacter::GetBlocking() const
{
	return Blocking;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredHitReaction() const
{
	return DesiredHitReaction;
}

inline const FGameplayTag& AALSXTCharacter::GetHitReaction() const
{
	return HitReaction;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredHitSurface() const
{
	return DesiredHitSurface;
}

inline const FGameplayTag& AALSXTCharacter::GetHitSurface() const
{
	return HitSurface;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredBumpReaction() const
{
	return DesiredBumpReaction;
}

inline const FGameplayTag& AALSXTCharacter::GetBumpReaction() const
{
	return BumpReaction;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredBumpSurface() const
{
	return DesiredBumpSurface;
}

inline const FGameplayTag& AALSXTCharacter::GetBumpSurface() const
{
	return BumpSurface;
}

inline const FGameplayTag& AALSXTCharacter::GetDesiredHoldingBreath() const
{
	return DesiredHoldingBreath;
}

inline const FGameplayTag& AALSXTCharacter::GetHoldingBreath() const
{
	return HoldingBreath;
}

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

inline const FGameplayTag& AALSXTCharacter::GetDesiredReloadingType() const
{
	return DesiredReloadingType;
}

inline const FGameplayTag& AALSXTCharacter::GetReloadingType() const
{
	return ReloadingType;
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

inline const FGameplayTag& AALSXTCharacter::GetDesiredImpactType() const
{
	return DesiredImpactType;
}

inline const FGameplayTag& AALSXTCharacter::GetImpactType() const
{
	return ImpactType;
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