#pragma once

#include "ALSXTControllerInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTControllerInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTControllerInterface {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Character")
	APawn* GetControlledPawn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Character")
	APawn* GetControlledCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Character")
	bool CanControlPawn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Camera Effects Component Interface|Character")
	bool CanControlCharacter();
};