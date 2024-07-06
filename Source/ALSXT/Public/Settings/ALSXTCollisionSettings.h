#pragma once

#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "ALSXTCollisionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCollisionEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Action Strength"))
	FGameplayTagContainer ImpactStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<UNiagaraSystem*> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	USoundBase* Sound{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<UMaterialInterface*> Decals;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCollisionEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FALSXTCollisionEffect> Effects;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCollisionEffectMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCollisionEffects> Effects;

};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTCollisionEffectsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCollisionEffectMap> CollisionEffectMap;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDamageResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float Force{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float BaseDamage{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float ShockDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float PunctureDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float BurnDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float ExplosiveDamage{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float Penetration{ 0.0f };
};