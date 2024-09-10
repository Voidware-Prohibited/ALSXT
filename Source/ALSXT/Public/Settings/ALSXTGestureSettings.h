#pragma once

#include "GameplayTagContainer.h"
#include "ALSXTGestureSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGestureMontages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> LeftMontage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> RightMontage{ nullptr };
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTGestureSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FALSXTGestureMontages> Gestures;
};