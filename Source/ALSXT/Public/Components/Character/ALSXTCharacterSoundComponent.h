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
#include "Components/AudioComponent.h"
#include "ALSXTCharacterSoundComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTCharacterSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTCharacterSoundComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Als Character")
	UALSXTCharacterSoundSettings* SelectCharacterSoundSettings();

	UFUNCTION(BlueprintImplementableEvent, Category = "Als Character")
	UALSXTWeaponSoundSettings* SelectWeaponSoundSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool ShouldPlayDeathSoundModeration();

private:
	FVector VoiceSocketLocation{ FVector::ZeroVector };
	FRotator VoiceSocketRotation{ FRotator::ZeroRotator };

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	FALSXTCharacterBreathEffectsSettings BreathParticleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.Voice Variant", AllowPrivateAccess))
	FGameplayTag VoiceVariant {ALSXTVoiceVariantTags::Default};

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentStamina {1.0f};

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FGameplayTag CurrentStaminaTag {ALSXTStaminaTags::Optimal};

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FGameplayTag CurrentBreathType{ ALSXTBreathTypeTags::Regular };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	UAudioComponent* CharacterMovementSoundMixer{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	UAudioComponent* VocalizationMixerAudioComponent{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	UAudioComponent* WeaponMovementAudioComponent{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	UAudioComponent* WeaponActionAudioComponent{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentBreathSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<FALSXTBreathSound> CurrentBreathSounds;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<FALSXTBreathSound> CurrentHoldingBreathSounds;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<FALSXTBreathSound> CurrentExhalingSounds;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	UNiagaraSystem* CurrentBreathParticle;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UNiagaraSystem*> CurrentBreathParticles;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentVocalizationSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentCharacterMovementSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentCharacterMovementAccentSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentWeaponMovementSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FSound CurrentWeaponActionSound;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousBreathsAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousHoldBreathAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousExhaleAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousBreathsParticleAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousVocalizationsAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousCharacterMovementAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousCharacterMovementAccentAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousWeaponMovementAssets;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<UObject*> PreviousWeaponActionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralCharacterSoundSettings GeneralCharacterSoundSettings;

	// UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	// FOnAudioPlayStateChanged OnAudioPlayStateChanged();

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void UpdateStaminaThresholds();

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void UpdateStamina(bool& StaminaTagChanged);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void UpdateVoiceSocketLocation();

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void UpdateVoiceSocketRotation();

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void DetermineNewSound(TArray<FSound> Sounds, TArray<UObject*> PreviousAssetsReferences, FSound& ResultSound);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	UNiagaraSystem* DetermineNewBreathParticle();

	UFUNCTION(NetMulticast, Reliable)
	void ServerPlayBreathParticle(UNiagaraSystem* NiagaraSystem);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void SetNewSound(UObject* Sound, TArray<UObject*> PreviousAssetsReferences, int NoRepeats);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void PlayCharacterBreathEffects(const FGameplayTag& StaminaOverride);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	void OnVocalization(FSound Vocalization);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	void OnMovementSound(FSound MovementSound);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayBreathSound();
	
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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement Sound")
	FGameplayTag GetBreathType();

	UFUNCTION(BlueprintCallable, Category = "Movement Sound")
	FName GetSocketForMovement(const FGameplayTag MovementType);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FGameplayTag ConvertWeightTagToStrengthTag(const FGameplayTag Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FGameplayTag ConvertStaminaToStaminaTag(const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTBreathSound> SelectBreathSoundsNew(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& BreathType, const FGameplayTag& Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<UNiagaraSystem*> SelectBreathParticles(const FGameplayTag& BreathType, const FGameplayTag& Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTBreathSound> SelectBreathSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& BreathType, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterMovementSound> SelectCharacterMovementSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterMovementSound> SelectCharacterMovementAccentSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTWeaponMovementSound> SelectWeaponMovementSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Weapon, const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTWeaponActionSound SelectWeaponActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterActionSound> SelectActionSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterActionSound> SelectAttackSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterDamageSound> SelectDamageSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Form, const FGameplayTag& Strength);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	TArray<FALSXTCharacterDamageSound> SelectDeathSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength);
	
	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type, Weight"))
	void PlayCharacterMovementSound(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type"))
	void PlayWeaponMovementSound(UPARAM(meta = (Categories = "Als.Weapon"))const FGameplayTag& Weapon, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type"))
	void PlayWeaponActionSound(UPARAM(meta = (Categories = "Als.Weapon Action"))const FGameplayTag& Type);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Type, Sex, Variant, Overlay, Strength"))
	void PlayActionSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength, AttackMode"))
	void PlayAttackSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, AttackMethod, Strength, AttackForm"))
	void PlayDamageSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Action Sound", Meta = (AutoCreateRefTerm = "Sex, Variant, Overlay, Strength, AttackForm"))
	void PlayDeathSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& Sex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

private:
	float StaminaOptimalThreshold = 0.75;
	float StaminaLowThreshold = 0.25;
	
	FTimerHandle TimeSinceLastCharacterMovementSoundTimer;
	float TimeSinceLastCharacterMovementSound{ 10.0f };
	FTimerHandle TimeSinceLastWeaponMovementSoundTimer;
	float TimeSinceLastWeaponMovementSound{ 10.0f };
	FTimerHandle TimeSinceLastActionSoundTimer;
	float TimeSinceLastActionSound{ 10.0f };
	FTimerHandle TimeSinceLastAttackSoundTimer;
	float TimeSinceLastAttackSound{ 10.0f };
	FTimerHandle TimeSinceLastDamageSoundTimer;
	float TimeSinceLastDamageSound{ 10.0f };
	float TargetCharacterMovementSoundDelay{ 1.0f };
	float CurrentCharacterMovementSoundDelay{ 0.0f };
	float TargetWeaponMovementSoundDelay{ 1.0f };
	float CurrentWeaponMovementSoundDelay{ 0.0f };
	float TargetActionSoundDelay{ 1.0f };
	float CurrentActionSoundDelay{ 0.0f };
	float TargetAttackSoundDelay{ 1.0f };
	float CurrentAttackSoundDelay{ 0.0f };
	float TargetDamageSoundDelay{ 1.0f };
	float CurrentDamageSoundDelay{ 0.0f };

	UNiagaraSystem* LastBreathParticle {nullptr};
	FALSXTCharacterMovementSound LastCharacterMovementSound;
	FALSXTWeaponMovementSound LastWeaponMovementSound;
	FALSXTWeaponActionSound LastWeaponActionSound;
	FALSXTBreathSound LastCharacterBreathSound;
	FALSXTBreathSound LastCharacterHoldingBreathSound;
	FALSXTBreathSound LastCharacterExhaleSound;
	FALSXTCharacterActionSound LastCharacterActionSound;
	FALSXTCharacterActionSound LastCharacterAttackSound;
	FALSXTCharacterDamageSound LastCharacterDamageSound;
	FALSXTCharacterDamageSound LastCharacterDeathSound;

	bool ShouldPlayBreathSound();
	bool ShouldPlayActionSound(const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayAttackSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayDamageSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);	
	bool ShouldPlayDeathSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);
	
	void StartTimeSinceLastCharacterMovementSoundTimer(const float Delay);
	void IncrementTimeSinceLastCharacterMovementSound();
	void ResetTimeSinceLastCharacterMovementSoundTimer();

	void StartTimeSinceLastWeaponMovementSoundTimer(const float Delay);
	void IncrementTimeSinceLastWeaponMovementSound();
	void ResetTimeSinceLastWeaponMovementSoundTimer();

	void StartTimeSinceLastActionSoundTimer(const float Delay);
	void IncrementTimeSinceLastActionSound();
	void ResetTimeSinceLastActionSoundTimer();
	void StartTimeSinceLastAttackSoundTimer(const float Delay);
	void IncrementTimeSinceLastAttackSound();
	void ResetTimeSinceLastAttackSoundTimer();
	void StartTimeSinceLastDamageSoundTimer(const float Delay);
	void IncrementTimeSinceLastDamageSound();
	void ResetTimeSinceLastDamageSoundTimer();
	void PlaySound(FMotionSounds MotionSounds);

	UFUNCTION(Server, Unreliable)
	void ServerPlaySound(FMotionSounds MotionSounds);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySound(FMotionSounds MotionSounds);

	void SpawnAudioComponent(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket);

	UFUNCTION(Server, Reliable)
	void ServerSpawnAudioComponent(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket);

	UFUNCTION(Client, Reliable)
	void ClientSpawnAudioComponent(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAudioComponent(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket);
};
