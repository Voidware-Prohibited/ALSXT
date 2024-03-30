#pragma once

#include "AlsCameraComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterCameraEffectsComponentInterface.generated.h"

UINTERFACE()
class UALSXTCharacterCameraEffectsComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterCameraEffectsComponentInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  UAlsCameraComponent* GetCameraComponent() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  UALSXTCharacterCameraEffectsComponent* GetCameraEffectsComponent() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Character")
  APlayerCameraManager* GetPlayerCameraManager();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Sprinting")
  void SetRadialBlurEffect(float Amount);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Sprinting")
  void ResetRadialBlurEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Focus")
  void SetFocusEffect(bool Focus);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Focus")
  void ResetFocusEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Drunken")
  void AddDrunkEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Drunken")
  void ResetDrunkEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|High")
  void AddHighEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|High")
  void ResetHighEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Suppression")
  void AddSuppressionEffect(float Magnitude, float PostDelay);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Suppression")
  void ResetSuppressionEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Blinded")
  void AddBlindnessEffect(float Magnitude, float Length, float FadeOutLength);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Blinded")
  void ResetBlindnessEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Fly By")
  void AddProjectileFlyByEffect(USoundBase* Sound, FVector Location, FRotator Rotation);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Concussion")
  void AddConcussionEffect(float Magnitude, float Length, float FadeInLength, float FadeOutLength);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Concussion")
  void ResetConcussionEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void AddDamageEffect(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void ResetDamageEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void AddNearDeathEffect(float Damage, const FGameplayTag& DamageType);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void ResetNearDeathEffect();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void AddDeathEffect(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Damage")
  void ResetDeathEffect();
};