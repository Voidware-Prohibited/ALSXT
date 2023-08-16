// MIT

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALSXTAnimNotify_CharacterMovementSound.generated.h"

class USoundBase;

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTCharacterMovementSoundNotifySettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableVertexPaintTrace{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementAccentSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableWeaponMovementSound{ true };
};

/**
 * 
 */
UCLASS(DisplayName = "ALSXT Character Movement Sound Animation Notify",
	AutoExpandCategories = ("Settings|Sound", "Settings|Decal", "Settings|Particle System"), Meta = (ShowWorldContextPin))
class ALSXT_API UALSXTAnimNotify_CharacterMovementSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	UALSXTCharacterMovementSoundNotifySettings* CharacterMovementSoundNotifySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableVertexPaintTrace{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementAccentSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableWeaponMovementSound{ true };
	
};
