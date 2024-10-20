#pragma once
#include "Settings/ALSXTImpactReactionSettings.h"
#include "ALSXTImpactReactionState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactReactionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FALSXTImpactReactionParameters ImpactReactionParameters;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTPhysicalAnimationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FGameplayTag Mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FName ProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	TArray<FName> AffectedBonesBelow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float Alpha;
};