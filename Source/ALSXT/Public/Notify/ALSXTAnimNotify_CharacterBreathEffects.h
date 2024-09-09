// MIT

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "GameplayTagContainer.h"
#include "ALSXTAnimNotify_CharacterBreathEffects.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "ALSXT Character Breath Effects Animation Notify", Meta = (ShowWorldContextPin))
class ALSXT_API UALSXTAnimNotify_CharacterBreathEffects : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	/** Option to override Setting set in Character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Editor Preview")
	USoundBase* PreviewSound;

	/** Option to override Setting set in Character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Editor Preview")
	UNiagaraSystem* PreviewParticle;

	/** Option to override Setting set in Character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Optional Overrides")
	bool EnableCharacterBreathEffects{ true };

	/** By Default, Breath is determined by Stamina. Set this Tag to override the Stamina */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Optional Overrides", Meta = (Categories = "Als.Stamina"))
	FGameplayTag StaminaOverride{FGameplayTag::EmptyTag};
	
};
