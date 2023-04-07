#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTCharacterSoundSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterSoundParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Location{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTCharacterSoundSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Strength} {Stamina} {Sound}", AllowPrivateAccess))
	TArray<FALSXTCharacterActionSound> ActionSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Strength} {Stamina} {Sound}", AllowPrivateAccess))
	TArray<FALSXTCharacterActionSound> AttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Form} {Damage} {Sound}", AllowPrivateAccess))
	TArray<FALSXTCharacterDamageSound> DamageSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Form} {Damage} {Sound}", AllowPrivateAccess))
	TArray<FALSXTCharacterDamageSound> DeathSounds;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCharacterSoundSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCharacterSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableActionSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableAttackSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableDamageSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableDeathSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebugMode{ false };

};