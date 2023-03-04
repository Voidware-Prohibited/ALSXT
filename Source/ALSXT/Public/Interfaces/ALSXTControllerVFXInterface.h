#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTControllerVFXInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTControllerVFXInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTControllerVFXInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Sprinting")
	void AddSprintingEffect(float Magnitude, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Sprinting")
	void ResetSprintingEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Focus")
	void SetFocusEffect(bool Focus);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Focus")
	void ResetFocusEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Suppression")
	void AddSuppressionEffect(float Magnitude, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Suppression")
	void ResetSuppressionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Blinded")
	void AddBlindedEffect(float Magnitude, float Length, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Blinded")
	void ResetBlindedEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Flashbang")
	void AddFlashbangEffect(float Magnitude, float Length, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Flashbang")
	void ResetFlashbangEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Fly By")
	void AddProjectileFlyByEffect(USoundBase* Sound, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void AddDamageEffect(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void ResetDamageEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Concussion")
	void AddConcussionEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Concussion")
	void ResetConcussionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Drunken")
	void AddDrunkEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Drunken")
	void ResetDrunkEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|High")
	void AddHighEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|High")
	void ResetHighEffect();
};