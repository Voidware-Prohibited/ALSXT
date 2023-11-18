#pragma once

#include "Settings/ALSXTMeshRenderSettings.h"
#include "ALSXTControllerRenderInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTControllerRenderInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTControllerRenderInterface {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller Render Interface")
	FALSXTGeneralALSXTMeshRenderSettings GetMeshRenderSettings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller Render Interface")
    bool IsSeparateFirstPersonFOVEnabled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller Render Interface")
	bool IsViewModelOnSkeletalMeshEnabled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller Render Interface")
	bool IsViewModelOnStaticMeshEnabled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Controller Render Interface")
	bool IsViewModelBlendOnCharacterActionEnabled();
};