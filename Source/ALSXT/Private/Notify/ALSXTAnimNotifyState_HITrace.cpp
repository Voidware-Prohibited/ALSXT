// MIT


#include "Notify/ALSXTAnimNotifyState_HITrace.h"

#include "ALSXTCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/AlsUtility.h"

FString UALSXTAnimNotifyState_HITrace::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Als Set Locomotion Action: {0}"), {
							   FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(OverlayMode).ToString(), false)
		});
}

void UALSXTAnimNotifyState_HITrace::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const float Duration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(Mesh, Animation, Duration, EventReference);

	auto* Character{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };
	if (IsValid(Character))
	{
		bool Found;
		FVector Start;
		FVector End;
		float Radius;

		Character->GetHeldItemTraceLocations(Found, Start, End, Radius);
	}
}

void UALSXTAnimNotifyState_HITrace::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(Mesh, Animation, EventReference);

	auto* Character{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };

	if (IsValid(Character))
	{
		// ..
	}
}