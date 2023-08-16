// MIT


#include "Notify/ALSXTAnimNotify_CharacterMovementSound.h"

#include "AlsCharacter.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"

#include "Notify/ALSXTAnimNotify_FootstepEffects.h"


FString UALSXTAnimNotify_CharacterMovementSound::GetNotifyName_Implementation() const
{
	return FString("ALSXT Character Movement Sound Notify");
}

void UALSXTAnimNotify_CharacterMovementSound::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);

	if (!IsValid(Mesh) || !ALS_ENSURE(IsValid(CharacterMovementSoundNotifySettings)))
	{
		return;
	}

	const auto* Character{ Cast<AAlsCharacter>(Mesh->GetOwner()) };
	AALSXTCharacter* ALSXTCharacter{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };

	if (IsValid(ALSXTCharacter) && ALSXTCharacter->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}

	const auto CapsuleScale{ IsValid(ALSXTCharacter) ? ALSXTCharacter->GetCapsuleComponent()->GetComponentScale().Z : 1.0f };

	const auto* World{ Mesh->GetWorld() };
	const auto* AnimationInstance{ Mesh->GetAnimInstance() };
	const auto* ALSXTAnimationInstance{ Cast<UALSXTAnimationInstance>(Mesh->GetAnimInstance()) };

}