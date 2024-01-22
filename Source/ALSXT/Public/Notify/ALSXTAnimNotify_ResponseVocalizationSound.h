// MIT

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALSXTAnimNotify_ResponseVocalizationSound.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "ALSXT Impact Response Vocalization Sound Notify", AutoExpandCategories = ("Settings|Sound"), Meta = (ShowWorldContextPin))
class ALSXT_API UALSXTAnimNotify_ResponseVocalizationSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MovementType{FGameplayTag::EmptyTag};
	
};