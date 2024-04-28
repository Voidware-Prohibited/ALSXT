// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterCameraEffectsComponentInterface.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Interfaces/ALSXTAcrobaticActionComponentInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTFirearmInterface.h"
#include "AlsCameraComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "Components/Character/ALSXTCombatComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTAcrobaticActionComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterAdvanced.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AALSXTCharacterAdvanced : public AALSXTCharacter, public IALSXTCharacterCameraEffectsComponentInterface, public IALSXTHeadLookAtInterface, public IALSXTAcrobaticActionComponentInterface, public IALSXTTargetLockInterface, public IALSXTCombatInterface, public IALSXTFirearmInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> PrimaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SecondaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AcrobaticAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectEmoteAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectGestureAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchTargetLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SwitchTargetRightAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCombatComponent* Combat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTAcrobaticActionComponent* AcrobaticActions;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTEmoteComponent* Emotes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTGestureComponent* Gestures;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCameraEffectsComponent* CameraEffects;

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
	void BeginCombatAttackCollisionTrace_Implementation(FALSXTCombatAttackTraceSettings TraceSettings) override;
	void EndCombatAttackCollisionTrace_Implementation() override;

private:
	void InputAcrobaticAction(const FInputActionValue& ActionValue);

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputPrimaryAction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	void InputSecondaryAction();

private:

	void InputSwitchTargetLeft();

	void InputSwitchTargetRight();

	void InputReload();

	void InputReloadWithRetention();

protected:
	FTimerHandle HoldBreathTimerHandle;	// Timer Handle for Hold Breath
	FTimerDelegate HoldBreathTimerDelegate; // Delegate to bind function with parameters

	// Hold Breath
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void BeginHoldBreathTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void HoldBreathTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void EndHoldBreathTimer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformPrimaryAction() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Als|Input Actions")
	bool CanPerformSecondaryAction() const;

	void OnDesiredAimingChanged_Implementation(bool bPreviousDesiredAiming) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	bool CanPerformAcrobaticAction() const;

	// void UpdateADSTransforms() const;
	
};
