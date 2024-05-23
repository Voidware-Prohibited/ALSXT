// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/ALSXTCombatSettings.h"
#include "Settings/ALSXTFirearmSettings.h"
#include "ALSXTCharacterAdvancedSettings.generated.h"

UCLASS()
class ALSXT_API UALSXTCharacterAdvancedSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FALSXTGeneralCombatSettings Combat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sights")
	FALSXTGeneralSightsSettings Sights;

	UALSXTCharacterAdvancedSettings();
	
};
