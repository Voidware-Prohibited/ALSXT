#pragma once

#include "AlsCharacter.h"

#include "State/AlsLocomotionState.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTCharacter.generated.h"

class UALSXTAnimationInstance;
class UAlsCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(AutoExpandCategories = ("Settings|Als Character Example", "State|Als Character Example"))
class ALSXT_API AALSXTCharacter : public AAlsCharacter
{
	GENERATED_BODY()

private:
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

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSprint() const;
	void CanSprint_Implementation();

	// Debug

public:
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalLocation) override;

	/** BP implementable function for AI to check if they can Mantle or Vault obstacle */
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void OnAIJumpObstacle_Implementation();

	/** BP implementable function for AI to trace for Mantle or Vault obstacles */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void AIObstacleTrace();
	virtual void AIObstacleTrace_Implementation();

	/** BP implementable function that called when Roll starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnRoll();
	virtual void OnRoll_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool CanRoll() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanVault() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanSlide() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanWallrun() const;

	/** BP implementable function that called when Mantle starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnMantle();
	virtual void OnMantle_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool TryVault() const;

protected:		
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartVault();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void StartSlide();

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
};

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