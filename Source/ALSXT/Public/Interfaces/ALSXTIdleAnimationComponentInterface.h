#pragma once

#include "Settings/ALSXTIdleAnimationSettings.h"
#include "ALSXTIdleAnimationComponentInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTIdleAnimationComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTIdleAnimationComponentInterface {
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  bool ShouldIdle() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  UALSXTIdleAnimationSettings* SelectIdleSettings();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  FALSXTIdleState GetIdleState() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  void EnableIdleAnimation(bool Enable);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  bool IsIdleCameraRotationActive() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  float GetIdleCameraRotation() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  AActor* GetGazeCameraTarget() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  void ResetTimer();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Idle Animation Component Interface")
  void StopIdleAnimation();

};