#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayEffectBase.h"
#include "GameplayTagContainer.h"
#include "AlsxtGameplayEffectStatusEffectBase.generated.h"

/**
 * Custom GameplayEffect class to hold additional status effect data.
 */
UCLASS()
class ALSXT_API UAlsxtGameplayEffectStatusEffectBase : public UAlsxtGameplayEffectBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayEffectStatusEffectBase();

	/** Priority of the status effect, higher value means more important for sorting. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status Effect")
	float Priority;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status Effect")
	FGameplayTag Severity;

	/** Tag specifying the locomotion variant this effect causes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status Effect")
	FGameplayTag LocomotionVariantTag;

	/** Optional: Tag indicating the type of status (e.g., Injury, Temperature, Exhaustion) for secondary sorting. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status Effect")
	FGameplayTag StatusTypeTag;
};