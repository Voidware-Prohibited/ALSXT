// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AlsxtGASMovementStaminaDrainCostSettings.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtGASMovementStaminaDrainCostConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cost = 5.0f;
	float Rate = 0.10f;
};

// Nested Structure: Locomotion -> Stance -> Gait -> Rate
USTRUCT(BlueprintType)
struct FAlsxtGASMovementStaminaDrainCostGaitMap
{
	GENERATED_BODY()
	// Key: Gait Tags (e.g., Movement.Gait.Run)
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FAlsxtGASMovementStaminaDrainCostConfig> GaitMap;
};

USTRUCT(BlueprintType)
struct FAlsxtGASMovementStaminaDrainCostStanceMap
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FAlsxtGASMovementStaminaDrainCostGaitMap> StanceMap;
};

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGASMovementStaminaDrainCostSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Key: Locomotion Tags (e.g., Movement.Mode.Grounded)
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	TMap<FGameplayTag, FAlsxtGASMovementStaminaDrainCostGaitMap> MovementStaminaDrainCosts;

	float GetDrainRate(FGameplayTag LocomotionMode, FGameplayTag Stance, FGameplayTag Gait) const;
};

UCLASS()
class ALSXT_API UAlsxtGASLocomotionActionStaminaCostSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Key: Locomotion Tags (e.g., Movement.Mode.Grounded)
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	TMap<FGameplayTag, float> LocomotionActionStaminaCosts;

	float GetLocomotionActionStaminaCost(FGameplayTag LocomotionAction, FGameplayTag Stance, FGameplayTag Gait) const;
};
