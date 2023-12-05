#pragma once

#include "NativeGameplayTags.h"
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	FALSXTCharacterBreathEffectsSettings GetBreathEffectsSettings();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FALSXTCharacterVoiceParameters GetVoiceParameters();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FVector GetVoiceSocketLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FRotator GetVoiceSocketRotation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ALSXTCharacter Interface")
	FGameplayTag GetBreathType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement", Meta = (AutoCreateRefTerm = "StaminaOverride"))
	void PlayBreathEffects(UPARAM(meta = (Categories = "Als.Stamina"))const FGameplayTag& StaminaOverride);
};