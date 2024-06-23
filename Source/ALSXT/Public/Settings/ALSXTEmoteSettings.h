#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTEmoteSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTEmote
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TObjectPtr<UAnimMontage> Montage;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTEmoteSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FALSXTEmote> Emotes;
};