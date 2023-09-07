// MIT


#include "Notify/ALSXTAnimNotify_CharacterMovementSound.h"
#include "AlsCharacter.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterInterface.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimNotify_CharacterMovementSound)


FString UALSXTAnimNotify_CharacterMovementSound::GetNotifyName_Implementation() const
{
	return FString("ALSXT Character Movement Sound Notify");
}

void UALSXTAnimNotify_CharacterMovementSound::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
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

	if (IsValid(ALSXTCharacter) && ALSXTCharacter->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}
	// FGameplayTag WeightTag = IALSXTCharacterInterface::Execute_GetWeightTag(ALSXTCharacter);

	if (World->WorldType != EWorldType::EditorPreview)
	{
	FGameplayTag WeightTag = IALSXTCharacterInterface::Execute_GetWeightTag(ALSXTCharacter);	
	IALSXTCharacterInterface::Execute_PlayCharacterMovementSound(ALSXTCharacter, EnableCharacterMovementAccentSound, EnableWeaponMovementSound, MovementType, WeightTag);
	}
}