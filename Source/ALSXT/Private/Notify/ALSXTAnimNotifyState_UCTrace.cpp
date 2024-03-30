// MIT


#include "Notify/ALSXTAnimNotifyState_UCTrace.h"

#include "Utility/ALSXTGameplayTags.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/AlsUtility.h"

UALSXTAnimNotifyState_UCTrace::UALSXTAnimNotifyState_UCTrace()
{
	bIsNativeBranchingPoint = true;
}

FString UALSXTAnimNotifyState_UCTrace::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Unarmed Attack Type: {0}"), {
							   FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(UnarmedAttackType).ToString(), false)
		});
}

void UALSXTAnimNotifyState_UCTrace::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const float Duration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(Mesh, Animation, Duration, EventReference);

	if (IsValid(Mesh->GetOwner()) && Mesh->GetOwner()->Implements<UALSXTCharacterInterface>() && Mesh->GetOwner()->Implements<UALSXTCombatInterface>())
	{
		
		TraceSettings.Overlay = IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(Mesh->GetOwner());
		TraceSettings.ImpactType = ALSXTImpactTypeTags::Hit;
		TraceSettings.AttackType = UnarmedAttackType;
		TraceSettings.ImpactForm = ALSXTImpactFormTags::Blunt;
		TraceSettings.AttackStrength = AttackStrength;

		IALSXTCombatInterface::Execute_GetCombatUnarmedTraceLocations(Mesh->GetOwner(), UnarmedAttackType, TraceSettings.Start, TraceSettings.End, TraceSettings.Radius);
		FString DebugMsg;
		IALSXTCombatInterface::Execute_BeginCombatAttackCollisionTrace(Mesh->GetOwner(), TraceSettings);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Invalid")));
	}
}

void UALSXTAnimNotifyState_UCTrace::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(Mesh, Animation, EventReference);

	if (IsValid(Mesh->GetOwner()) && Mesh->GetOwner()->Implements<UALSXTCharacterInterface>() && Mesh->GetOwner()->Implements<UALSXTCombatInterface>())
	{
		IALSXTCombatInterface::Execute_EndCombatAttackCollisionTrace(Mesh->GetOwner());
	}	
}