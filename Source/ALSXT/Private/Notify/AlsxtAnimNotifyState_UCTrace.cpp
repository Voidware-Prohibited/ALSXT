// MIT


#include "Notify/AlsxtAnimNotifyState_UCTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/AlsxtMeleeTraceEventData.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtCombatInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/AlsUtility.h"
#include "AbilitySystemComponent.h"

UAlsxtAnimNotifyState_UCTrace::UAlsxtAnimNotifyState_UCTrace()
{
	bIsNativeBranchingPoint = true;
}

FString UAlsxtAnimNotifyState_UCTrace::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Unarmed Attack Type: {0}"), {
							   FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(UnarmedAttackType).ToString(), false)
		});
}

void UAlsxtAnimNotifyState_UCTrace::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const float Duration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(Mesh, Animation, Duration, EventReference);

	if (Mesh && Mesh->GetOwner())
	{
		AActor* Owner = Mesh->GetOwner();
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

		if (ASC && MeleeCollisionTraceBeginEventTag.IsValid())
		{
			// Calculate actual start/end points based on character location/rotation/skeletal mesh
			// A common approach is to use a specific socket on the weapon or character for accurate positioning.
			FVector StartLoc = Mesh->GetSocketLocation(FName("WeaponStartSocket")) + TraceSettings.Start; // Example using a socket
			FVector EndLoc = Mesh->GetSocketLocation(FName("WeaponEndSocket")) + TraceSettings.End;   // Example using a socket

			FAlsxtMeleeTraceEventData EventData;
			EventData.StartLocation = StartLoc;
			EventData.EndLocation = EndLoc;
			EventData.Radius = TraceSettings.Radius;

			// Use the custom struct with FInstancedStruct for passing complex data
			FAlsxtMeleeTraceEventData PayloadData;
			PayloadData.Instigator = Owner;
			PayloadData.Target = nullptr; // Can set target if known, but trace will find targets
			PayloadData.EventTag = MeleeCollisionTraceBeginEventTag;
			PayloadData.InstigatorTags = FGameplayTagContainer();
			PayloadData.TargetData = FGameplayAbilityTargetDataHandle();
			// PayloadData.OptionalObject = EventData.GetInstancedStruct(); // Pass our data via OptionalPayload

			// Create the wrapper instance
			UAlsxtMeleeTraceEventDataObject* Wrapper = NewObject<UAlsxtMeleeTraceEventDataObject>(this);

			// Set your struct data
			Wrapper->MeleeTraceEventData = PayloadData;

			// Assign to OptionalObject
			PayloadData.OptionalObject = Wrapper;

			// Trigger the event in the ASC, which the ability will listen for
			FScopedPredictionWindow NewScopedWindow(ASC, true); // Use scoped prediction for networked events
			ASC->HandleGameplayEvent(MeleeCollisionTraceBeginEventTag, &PayloadData);
		}
	}

	
	const auto* World{ Mesh->GetWorld() };
	if (World->WorldType != EWorldType::EditorPreview)
	{
		if (IsValid(Mesh->GetOwner()))
		{
			TraceSettings.ImpactType = ALSXTImpactTypeTags::Hit;
			TraceSettings.AttackType = UnarmedAttackType;
			TraceSettings.ImpactForm = ALSXTImpactFormTags::Blunt;
			TraceSettings.AttackStrength = AttackStrength;
			if (Mesh->GetOwner()->Implements<UAlsxtCharacterInterface>())
			{
				TraceSettings.Overlay = IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(Mesh->GetOwner());

				IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(Mesh->GetOwner(), true);
			}
			if (Mesh->GetOwner()->Implements<UAlsxtCombatInterface>())
			{
				IAlsxtCombatInterface::Execute_GetCombatUnarmedTraceLocations(Mesh->GetOwner(), UnarmedAttackType, TraceSettings.Start, TraceSettings.End, TraceSettings.Radius);
				FString DebugMsg;
				IAlsxtCombatInterface::Execute_BeginCombatAttackCollisionTrace(Mesh->GetOwner(), TraceSettings);
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Invalid")));
		}
	}
}

void UAlsxtAnimNotifyState_UCTrace::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(Mesh, Animation, EventReference);

	const auto* World{ Mesh->GetWorld() };
	if (World->WorldType != EWorldType::EditorPreview)
	{
		if (IsValid(Mesh->GetOwner()))
		{
			if (Mesh->GetOwner()->Implements<UAlsxtCombatInterface>())
			{
				IAlsxtCombatInterface::Execute_EndCombatAttackCollisionTrace(Mesh->GetOwner());
			}
			if (Mesh->GetOwner()->Implements<UAlsxtCharacterInterface>())
			{
				IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(Mesh->GetOwner(), false);
			}

			AActor* Owner = Mesh->GetOwner();
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

			if (ASC && MeleeCollisionTraceEndEventTag.IsValid())
			{
				// Calculate actual start/end points based on character location/rotation/skeletal mesh
				// A common approach is to use a specific socket on the weapon or character for accurate positioning.
				FVector StartLoc = Mesh->GetSocketLocation(FName("WeaponStartSocket")) + TraceSettings.Start; // Example using a socket
				FVector EndLoc = Mesh->GetSocketLocation(FName("WeaponEndSocket")) + TraceSettings.End;   // Example using a socket

				FAlsxtMeleeTraceEventData EventData;
				EventData.StartLocation = StartLoc;
				EventData.EndLocation = EndLoc;
				EventData.Radius = TraceSettings.Radius;

				// Use the custom struct with FInstancedStruct for passing complex data
				FAlsxtMeleeTraceEventData PayloadData;
				PayloadData.Instigator = Owner;
				PayloadData.Target = nullptr; // Can set target if known, but trace will find targets
				PayloadData.EventTag = MeleeCollisionTraceEndEventTag;
				PayloadData.InstigatorTags = FGameplayTagContainer();
				PayloadData.TargetData = FGameplayAbilityTargetDataHandle();

				// Trigger the event in the ASC, which the ability will listen for
				FScopedPredictionWindow NewScopedWindow(ASC, true); // Use scoped prediction for networked events
				ASC->HandleGameplayEvent(MeleeCollisionTraceEndEventTag, &PayloadData);
			}
		}
	}
}