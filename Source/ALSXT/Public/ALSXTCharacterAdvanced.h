// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/ALSXTCharacterAdvancedSettings.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterCameraEffectsComponentInterface.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Interfaces/ALSXTAcrobaticActionComponentInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTFirearmInterface.h"
#include "AlsCameraComponent.h"
#include "CineCameraComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "Components/Character/ALSXTCombatComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTAcrobaticActionComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterAdvanced.generated.h"

class UALSXTCharacterAdvancedSettings;

UCLASS()
class ALSXT_API AALSXTCharacterAdvanced : public AALSXTCharacter, public IALSXTCharacterCameraEffectsComponentInterface, public IALSXTHeadLookAtInterface, public IALSXTAcrobaticActionComponentInterface, public IALSXTCombatInterface, public IALSXTFirearmInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Advanced", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTCharacterAdvancedSettings> ALSXTAdvancedSettings;

	void CalcADSCamera(FMinimalViewInfo& ViewInfo);
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> HoldBreathAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> PrimaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SecondaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AcrobaticAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> TargetLockAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchTargetLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchTargetRightAction;

	// Components

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UCineCameraComponent> BodyCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCombatComponent* Combat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTAcrobaticActionComponent* AcrobaticActions;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCameraEffectsComponent* CameraEffects;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State|Aim")
	FTransform GetADSRelativeTransform() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State|Aim")
	FRotator CalculateRecoilControlRotation(FRotator AdditiveControlRotation) const;

// INPUTS

private:
	void InputHoldBreath(const FInputActionValue& ActionValue);
	void InputAcrobaticAction(const FInputActionValue& ActionValue);
	void InputTargetLock(const FInputActionValue& ActionValue);
	void InputSwitchTargetLeft();
	void InputSwitchTargetRight();
	void InputChargeFirearm();
	void InputReload();
	void InputReloadWithRetention();
	void ClearWeaponMalfunction();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputPrimaryAction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputSecondaryAction();

// Input Action Criteria

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryAction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformSecondaryAction() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	bool CanPerformAcrobaticAction() const;

	void OnDesiredAimingChanged_Implementation(bool bPreviousDesiredAiming) override;

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FALSXTFirearmAimState FirearmAimState;

	// Timers

	FTimerHandle HoldBreathTimerHandle;	// Timer Handle for Hold Breath
	FTimerDelegate HoldBreathTimerDelegate; // Delegate to bind function with parameters

	// Hold Breath
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	float CalculateHoldBreathTimer();

	// Hold Breath
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void BeginHoldBreathTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void HoldBreathTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	FGameplayTag CalculateBreathReleaseMode() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void EndHoldBreathTimer();
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

	// void UpdateADSTransforms() const;

private:
	// ReloadingType

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredReloadingType{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ReloadingType{ FGameplayTag::EmptyTag };

	// FirearmFingerAction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFirearmFingerAction{ ALSXTFirearmFingerActionTags::None };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerAction{ ALSXTFirearmFingerActionTags::None };

	// FirearmFingerActionHand

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredFirearmFingerActionHand{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerActionHand{ FGameplayTag::EmptyTag };

	// HoldingBreath

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredHoldingBreath{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HoldingBreath{ FGameplayTag::EmptyTag };

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

public:
	virtual void OnAttackCollision_Implementation(FAttackDoubleHitResult Hit) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void GetFirearmCameraShakeInfo(TSubclassOf<UCameraShakeBase>& CameraShakeClass, float& Scale);

	// Character Sound Component Interface Functions
	virtual void PlayAttackSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina) override;

// INTERFACE FUNCTIONS

public:
	// Character Interface
	virtual FGameplayTag GetCharacterHoldingBreath_Implementation() const override;

	// Held Item Interface Functions
	virtual FGameplayTag GetCharacterReloadingType_Implementation() const override;
	virtual FGameplayTag GetCharacterFirearmFingerAction_Implementation() const override;
	virtual FGameplayTag GetCharacterFirearmFingerActionHand_Implementation() const override;
	
	//Character Camera Effects Component Interface
	UAlsCameraComponent* GetCameraComponent_Implementation() const override;
	UALSXTCharacterCameraEffectsComponent* GetCameraEffectsComponent_Implementation() const override;
	void SetRadialBlurEffect_Implementation(float Amount) override;
	void ResetRadialBlurEffect_Implementation() override;
	void SetFocusEffect_Implementation(bool NewFocus) override;
	void AddDrunkEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength) override;
	void ResetDrunkEffect_Implementation() override;
	void AddHighEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength) override;
	void ResetHighEffect_Implementation() override;
	void AddSuppressionEffect_Implementation(float Magnitude, float PostDelay) override;
	void ResetSuppressionEffect_Implementation() override;
	void AddBlindnessEffect_Implementation(float Magnitude, float Length, float FadeOutLength) override;
	void ResetBlindnessEffect_Implementation() override;
	void AddProjectileFlyByEffect_Implementation(USoundBase* Sound, FVector Location, FRotator Rotation) override;
	void AddConcussionEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength) override;
	void ResetConcussionEffect_Implementation() override;
	void AddDamageEffect_Implementation(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay) override;
	void ResetDamageEffect_Implementation() override;
	void AddNearDeathEffect_Implementation(float Damage, const FGameplayTag& DamageType) override;
	void ResetNearDeathEffect_Implementation() override;
	void AddDeathEffect_Implementation(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay) override;
	void ResetDeathEffect_Implementation() override;

	//Head Look At Interface
	void IsInFrontOf_Implementation(bool& IsInFrontOf, FVector LookAtActorLocation) const override;

	// Combat Interface
	FALSXTGeneralCombatSettings GetGeneralCombatSettings_Implementation() override;
	FALSXTCombatAttackTraceSettings GetCombatAttackTraceSettings_Implementation() override;
	void BeginCombatAttackCollisionTrace_Implementation(FALSXTCombatAttackTraceSettings TraceSettings) override;
	void EndCombatAttackCollisionTrace_Implementation() override;

	//Firearm Interface
	virtual FALSXTFirearmAimState GetFirearmAimState_Implementation() const override;
};

inline const FGameplayTag& AALSXTCharacterAdvanced::GetDesiredReloadingType() const
{
	return DesiredReloadingType;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetReloadingType() const
{
	return ReloadingType;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetDesiredFirearmFingerAction() const
{
	return DesiredFirearmFingerAction;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetFirearmFingerAction() const
{
	return FirearmFingerAction;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetDesiredFirearmFingerActionHand() const
{
	return DesiredFirearmFingerActionHand;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetFirearmFingerActionHand() const
{
	return FirearmFingerActionHand;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetDesiredHoldingBreath() const
{
	return DesiredHoldingBreath;
}

inline const FGameplayTag& AALSXTCharacterAdvanced::GetHoldingBreath() const
{
	return HoldingBreath;
}
