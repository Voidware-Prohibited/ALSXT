#pragma once

#include "Settings/AlsxtGameStateSettings.h"
#include "AlsxtGameStateInterface.generated.h"



UINTERFACE(Blueprintable)
class UAlsxtGameStateInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IAlsxtGameStateInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game State Interface")
  FAlsxtLocomotionGameStateSettings GetServerLocomotionSettings() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game State Interface")
  void SetServerLocomotionSettings(FAlsxtLocomotionGameStateSettings NewGameStateSettings) const;
};