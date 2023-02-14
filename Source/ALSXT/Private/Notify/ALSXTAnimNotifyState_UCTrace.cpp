// MIT


#include "Notify/ALSXTAnimNotifyState_UCTrace.h"

#include "ALSXTCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/AlsUtility.h"

FString UALSXTAnimNotifyState_UCTrace::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Als Set Locomotion Action: {0}"), {
							   FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(UnarmedAttackType).ToString(), false)
		});
}

void UALSXTAnimNotifyState_UCTrace::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const float Duration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(Mesh, Animation, Duration, EventReference);

	auto* Character{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };
	if (IsValid(Character))
	{
		FAttackTraceSettings TraceSettings;
		TraceSettings.Active = true;
		TraceSettings.ImpactType = ImpactType;
		TraceSettings.AttackStrength = AttackStrength;

		Character->GetUnarmedTraceLocations(UnarmedAttackType, TraceSettings.Start, TraceSettings.End, TraceSettings.Radius);
		Character->BeginAttackCollisionTrace(TraceSettings);
	}
}

void UALSXTAnimNotifyState_UCTrace::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(Mesh, Animation, EventReference);

	auto* Character{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };

	if (IsValid(Character))
	{
		Character->EndAttackCollisionTrace();
	}
}