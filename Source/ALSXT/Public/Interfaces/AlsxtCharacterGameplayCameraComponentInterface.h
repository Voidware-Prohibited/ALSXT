#pragma once

#include "GameFramework/GameplayCameraComponent.h"
#include "AlsxtCharacterGameplayCameraComponentInterface.generated.h"

UINTERFACE()
class UAlsxtCharacterGameplayCameraComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IAlsxtCharacterGameplayCameraComponentInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  TSoftObjectPtr<UGameplayCameraComponent> GetCharacterGameplayCameraComponent() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  bool GetCharacterIsCameraRightShoulder() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  bool CanCharacterSwitchCameraRightShoulder() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface")
  void SetCharacterCameraRightShoulder(const bool NewCameraRightShoulder);
};