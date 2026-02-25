// MIT

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "Abilities/GameplayAbility.h"
#include "AlsxtModularGameplayGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALSXTEXTRAS_API AAlsxtModularGameplayGameModeBase : public AModularGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	// Server owner that has been granted ServerOwnerAbility
	TSoftObjectPtr<APlayerState> ServerOwnerPlayerState {nullptr};

	// Admin Server Owner Gameplay Ability
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> ServerOwnerAbility; 
};
