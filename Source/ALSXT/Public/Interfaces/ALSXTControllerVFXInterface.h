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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Character")
    APlayerController* GetPlayerController();

    // UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Character")
    // AALSXTCharacter* GetCharacter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Character")
    APlayerCameraManager* GetPlayerCameraManager();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Camera")
    UAlsCameraComponent* GetCamera();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Camera")
    FVector GetCameraLocation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Camera")
    FRotator GetCameraRotation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Sprinting")
	void SetRadialBlurEffect(float Amount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Sprinting")
	void ResetRadialBlurEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Focus")
	void SetFocusEffect(bool Focus);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Focus")
	void ResetFocusEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Drunken")
	void AddDrunkEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Drunken")
	void ResetDrunkEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|High")
	void AddHighEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|High")
	void ResetHighEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Suppression")
	void AddSuppressionEffect(float Magnitude, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Suppression")
	void ResetSuppressionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Blinded")
	void AddBlindnessEffect(float Magnitude, float Length, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Blinded")
	void ResetBlindnessEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Fly By")
	void AddProjectileFlyByEffect(USoundBase* Sound, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Concussion")
	void AddConcussionEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Concussion")
	void ResetConcussionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void AddDamageEffect(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void ResetDamageEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void AddNearDeathEffect(float Damage, const FGameplayTag& DamageType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void ResetNearDeathEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void AddDeathEffect(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller VFX|Damage")
	void ResetDeathEffect();
};