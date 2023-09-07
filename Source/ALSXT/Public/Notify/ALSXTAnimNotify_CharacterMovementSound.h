// MIT

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALSXTAnimNotify_CharacterMovementSound.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MovementType{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementAccentSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableWeaponMovementSound{ true };
	
};
