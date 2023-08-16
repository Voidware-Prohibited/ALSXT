// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "Components/AudioComponent.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "Utility/ALSXTStructs.h"
#include "Settings/ALSXTCharacterSoundSettings.h"
#include "ALSXTCharacterSoundComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTCharacterSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTCharacterSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Als Character")
	UALSXTCharacterSoundSettings* SelectCharacterSoundSettings();

	UFUNCTION(BlueprintImplementableEvent, Category = "Als Character")
	UALSXTWeaponSoundSettings* SelectWeaponSoundSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool ShouldPlayDeathSoundModeration();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FName VoiceSocketName { "head" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D ActionSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D AttackSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D DamageSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool DebugMode { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralCharacterSoundSettings GeneralCharacterSoundSettings;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayCharacterMovementSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayWeaponMovementSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayWeaponActionSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayActionSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayAttackSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayDamageSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayDeathSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool ShouldPlayMovementAccentSound(const FGameplayTag& Type, const FGameplayTag& Strength);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool ShouldPlayWeaponMovementSound(const FGameplayTag& Type, const FGameplayTag& Strength);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Movement Sound")
	FName GetSocketForMovement(const FGameplayTag MovementType);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FGameplayTag ConvertWeightTagToStrengthTag(const FGameplayTag Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FGameplayTag ConvertStaminaToStaminaTag(const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterMovementSound SelectCharacterMovementSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterMovementSound SelectCharacterMovementAccentSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTWeaponMovementSound SelectWeaponMovementSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTWeaponActionSound SelectWeaponActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterActionSound SelectActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterActionSound SelectAttackSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterDamageSound SelectDamageSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterDamageSound SelectDeathSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type, Weight"))
	void PlayCharacterMovementSound(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type"))
	void PlayWeaponMovementSound(UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type"))
	void PlayWeaponActionSound(UPARAM(meta = (Categories = "Als.Weapon Action"))const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength"))
	void PlayActionSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength, AttackMode"))
	void PlayAttackSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength, AttackForm"))
	void PlayDamageSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength, AttackForm"))
	void PlayDeathSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

private:
	FTimerHandle TimeSinceLastCharacterMovementSoundTimer;
	float TimeSinceLastCharacterMovementSound{ 10.0f };
	FTimerHandle TimeSinceLastActionSoundTimer;
	float TimeSinceLastActionSound{ 10.0f };
	FTimerHandle TimeSinceLastAttackSoundTimer;
	float TimeSinceLastAttackSound{ 10.0f };
	FTimerHandle TimeSinceLastDamageSoundTimer;
	float TimeSinceLastDamageSound{ 10.0f };
	float TargetCharacterMovementSoundDelay{ 1.0f };
	float CurrentCharacterMovementSoundDelay{ 0.0f };
	float TargetActionSoundDelay{ 1.0f };
	float CurrentActionSoundDelay{ 0.0f };
	float TargetAttackSoundDelay{ 1.0f };
	float CurrentAttackSoundDelay{ 0.0f };
	float TargetDamageSoundDelay{ 1.0f };
	float CurrentDamageSoundDelay{ 0.0f };

	// USoundBase* ActionSound{ nullptr };
	// TArray<FAudioParameter> ActionSounds;
	// ActionSound->GetAllDefaultParameters(ActionSounds);

	FALSXTCharacterMovementSound LastCharacterMovementSound;
	FALSXTWeaponMovementSound LastWeaponMovementSound;
	FALSXTWeaponActionSound LastWeaponActionSound;
	FALSXTCharacterActionSound LastCharacterActionSound;
	FALSXTCharacterActionSound LastCharacterAttackSound;
	FALSXTCharacterDamageSound LastCharacterDamageSound;

	bool ShouldPlayActionSound(const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayAttackSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayDamageSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);	
	bool ShouldPlayDeathSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

	void StartTimeSinceLastCharacterMovementSoundTimer(const float Delay);
	void IncrementTimeSinceLastCharacterMovementSound();
	void ResetTimeSinceLastCharacterMovementSoundTimer();

	void StartTimeSinceLastActionSoundTimer(const float Delay);
	void IncrementTimeSinceLastActionSound();
	void ResetTimeSinceLastActionSoundTimer();
	void StartTimeSinceLastAttackSoundTimer(const float Delay);
	void IncrementTimeSinceLastAttackSound();
	void ResetTimeSinceLastAttackSoundTimer();
	void StartTimeSinceLastDamageSoundTimer(const float Delay);
	void IncrementTimeSinceLastDamageSound();
	void ResetTimeSinceLastDamageSoundTimer();
	void PlaySound(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch);

	UFUNCTION(Server, Reliable)
	void ServerPlaySound(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch);
};
