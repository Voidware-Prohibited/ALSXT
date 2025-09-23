#pragma once

#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtStatusState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStatusState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Status"))
		FGameplayTag CurrentStatus { ALSXTStatusTags::Normal };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Status, Als.Breath Type, Als.Holding Breath, Als.Action Strength, Als.Attack Method, Als.Impact Form"))
		FGameplayTagContainer StatusTags;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float CurrentHealth { 0.0f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float CurrentStamina { 0.0f};

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 300))
		float CurrentWeight { 0.0f};

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LastDamageReceived { 0.0f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina"))
		FGameplayTag CurrentStaminaTag { ALSXTStaminaTags::Full };

		bool operator==(const FAlsxtStatusState& other) const
		{
			return (other.CurrentStatus == CurrentStatus) && (other.StatusTags == StatusTags) && (other.CurrentHealth == CurrentHealth) && (other.CurrentStamina == CurrentStamina) && (other.LastDamageReceived == LastDamageReceived);
		}
};

