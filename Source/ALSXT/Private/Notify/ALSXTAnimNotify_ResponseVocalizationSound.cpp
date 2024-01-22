// MIT


#include "Notify/ALSXTAnimNotify_ResponseVocalizationSound.h"
#include "AlsCharacter.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterInterface.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimNotify_ResponseVocalizationSound)


FString UALSXTAnimNotify_ResponseVocalizationSound::GetNotifyName_Implementation() const
{
	return FString("ALSXT Impact Response Vocalization Sound Notify");
}

void UALSXTAnimNotify_ResponseVocalizationSound::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);

	if (!IsValid(Mesh))
	{
		return;
	}

	const auto* World{ Mesh->GetWorld() };
	const auto* Character{ Cast<AAlsCharacter>(Mesh->GetOwner()) };
	AALSXTCharacter* ALSXTCharacter{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };
	FGameplayTag Status{ IALSXTCharacterInterface::Execute_GetStatus(Mesh->GetOwner()) };

	if (Status == ALSXTStatusTags::Dead)
	{
		return;
	}

	if (IsValid(ALSXTCharacter) && ALSXTCharacter->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}

	if (World->WorldType != EWorldType::EditorPreview)
	{
		// IALSXTCharacterInterface::Execute_PlayCharacterMovementSound(ALSXTCharacter, EnableCharacterMovementAccentSound, EnableWeaponMovementSound, MovementType, WeightTag);
	}
}