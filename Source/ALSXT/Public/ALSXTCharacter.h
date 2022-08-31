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

UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class ALSXT_API AALSXTCharacter : public AAlsCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTCharacterSettings> ALSXTSettings;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UALSXTAnimationInstance> XTAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FRotator MeshRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float MeshRotationYaw{0.0f};

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
	TObjectPtr<UInputAction> BlockAction;

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

public:
	AALSXTCharacter();

	virtual void NotifyControllerChanged() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Crouch(bool bClientSimulation = false) override;

	void ApplyDesiredStance() override;

	// Camera

protected:
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;

	// Input

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

private:
	void InputLookMouse(const FInputActionValue& ActionValue);

	void InputLook(const FInputActionValue& ActionValue);

	void InputMove(const FInputActionValue& ActionValue);

	void InputSprint(const FInputActionValue& ActionValue);

	void InputWalk();

	void InputCrouch();

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

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnViewModeChanged(const FGameplayTag& PreviousViewModeTag);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanSetToViewMode(const FGameplayTag& ViewModeTag) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	void GetHeldItemViewTarget(FTransform& Transform, float& FOV, bool& Attachment) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Aim Down Sights")
	bool IsHoldingAimableItem() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALS|Aim Down Sights")
	bool IsAimingDownSights() const;

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
	const FGameplayTag& GetDesiredFreelooking() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFreelookingTag"))
	void SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanFreelook() const;

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredFreelooking(const FGameplayTag& NewFreelookingTag);

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