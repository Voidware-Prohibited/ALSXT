#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacterCustomizationComponentInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCharacterCustomizationComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCharacterCustomizationComponentInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Customization Component Interface")
  void SetCustomizationMorphTarget(FALSXTCharacterMorphParameter MorphTargetName);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Customization Component Interface")
  float GetCustomizationMorphTarget(FName MorphTargetName);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Customization Component Interface")
  FALSXTCharacterMorphParameters GetAllCustomMorphTargets();
};