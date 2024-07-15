#pragma once

#include "NativeGameplayTags.h"
#include "Settings/ALSXTCharacterSoundSettings.h"
#include "Utility/ALSXTStructs.h"
#include "Settings/ALSXTCharacterBreathEffectsSettings.h"
#include "ALSXTCharacterSoundComponentInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UALSXTCharacterSoundComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterSoundComponentInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Sound Component Interface|Parameters")
	UALSXTCharacterSoundSettings* SelectCharacterSoundSettings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Sound Component Interface|Parameters")
	UALSXTWeaponSoundSettings* SelectWeaponSoundSettings();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Sound Component Interface|Breath Effects")
	FALSXTCharacterBreathEffectsSettings GetBreathEffectsSettings();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Sound Component Interface|Parameters")
	FALSXTCharacterVoiceParameters GetVoiceParameters();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Sound Component Interface|Breath Effects")
	bool CanPlayBreathSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Sound Component Interface|Parameters")
	bool CanPlayActionSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	bool CanPlayCharacterMovementSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	bool CanPlayWeaponMovementSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	bool CanPlayDamageSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	bool ShouldPlayWeaponMovementSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	bool ShouldPlayDamageSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	FVector GetVoiceSocketLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	FRotator GetVoiceSocketRotation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	FGameplayTag GetCurrentBreathType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Parameters")
	FGameplayTag GetHoldingBreath();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Sound Component Interface|Breath Effects|Events", Meta = (AutoCreateRefTerm = "StaminaOverride"))
	void PlayBreathEffects(UPARAM(meta = (Categories = "Als.Stamina"))const FGameplayTag& StaminaOverride);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Events", Meta = (AutoCreateRefTerm = "Type, SoundSex, Variant, Overlay, Strength"))
	void PlayActionSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Events", Meta = (AutoCreateRefTerm = "SoundSex, Variant, Overlay, Strength, AttackMode"))
	void PlayAttackSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Events", Meta = (AutoCreateRefTerm = "SoundSex, Variant, Overlay, AttackMethod, Strength, AttackForm"))
	void PlayDamageSound(bool MovementSound, bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Breath Effects Interface|Events", Meta = (AutoCreateRefTerm = "SoundSex, Variant, Overlay, Strength, AttackForm"))
	void PlayDeathSound(UPARAM(meta = (Categories = "Als.Sex"))const FGameplayTag& SoundSex, UPARAM(meta = (Categories = "Als.Voice Variant"))const FGameplayTag& Variant, UPARAM(meta = (Categories = "Als.OverlayMode"))const FGameplayTag& Overlay, UPARAM(meta = (Categories = "Als.Attack Method"))const FGameplayTag& AttackMethod, UPARAM(meta = (Categories = "Als.Action Strength"))const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);
};