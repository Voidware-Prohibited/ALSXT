#pragma once

#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "AlsxtGameStateSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionGameStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Action Strength"))
	FGameplayTagContainer AllowedLocomotionModes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Stances"))
	FGameplayTagContainer AllowedStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Gait"))
	FGameplayTagContainer AllowedGaits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Gait"))
	FGameplayTagContainer AllowedReadiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Gait"))
	FGameplayTagContainer AllowedLocomotionActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<UNiagaraSystem*> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	USoundBase* Sound{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<UMaterialInterface*> Decals;

};
