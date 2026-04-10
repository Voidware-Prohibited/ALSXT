#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "AlsxtMeleeTraceEventData.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtMeleeTraceEventData : public FGameplayEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FGameplayTag AttackType {FGameplayTag::EmptyTag};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FVector StartLocation {FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FVector EndLocation {FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	float Radius {0.0f};

	// Helper to get FInstancedStruct from this data
	FInstancedStruct GetInstancedStruct() const
	{
		FInstancedStruct InstancedStruct = FInstancedStruct::Make<FAlsxtMeleeTraceEventData>();
		InstancedStruct.GetMutablePtr<FAlsxtMeleeTraceEventData>()->StartLocation = StartLocation;
		InstancedStruct.GetMutablePtr<FAlsxtMeleeTraceEventData>()->EndLocation = EndLocation;
		InstancedStruct.GetMutablePtr<FAlsxtMeleeTraceEventData>()->Radius = Radius;
		return InstancedStruct;
	}
};