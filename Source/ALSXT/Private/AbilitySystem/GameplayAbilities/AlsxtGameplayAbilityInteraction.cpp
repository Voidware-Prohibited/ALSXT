// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityInteraction.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectComponent.h"
#include "SmartObjectRequestTypes.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorSubsystem.h"
#include "Abilities/GameplayAbility.h"
#include "SmartObjectBlueprintFunctionLibrary.h" // For MarkSlotAsOccupied


UAlsxtGameplayAbilityInteraction::UAlsxtGameplayAbilityInteraction()
{
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UAlsxtGameplayAbilityInteraction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	USmartObjectSubsystem* SOSubsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
    
	// 1. Identify the target Smart Object (typically passed via TriggerEventData or a Trace)
	const AActor* TargetActor = TriggerEventData ? TriggerEventData->Target : nullptr;
	if (!SOSubsystem || !TargetActor || !BehaviorDefinitionClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 2. Find a compatible slot
	FSmartObjectRequest Request;
	// Request.Filter.BehaviorDefinitionClass = BehaviorDefinitionClass;
	Request.Filter.BehaviorDefinitionClasses.Add(BehaviorDefinitionClass);
    
	FSmartObjectRequestResult RequestResult = SOSubsystem->FindSmartObject(Request);
    
	if (RequestResult.IsValid())
	{

		FConstStructView UserData = FConstStructView(); 
		
		// 3. NEW: Mark the slot as Claimed (Replaces deprecated Claim)
		CurrentClaimHandle = SOSubsystem->MarkSlotAsClaimed(RequestResult.SlotHandle, ESmartObjectClaimPriority::Normal, UserData);

		if (CurrentClaimHandle.IsValid())
		{
			// 4. NEW: Mark the slot as Occupied when starting the interaction
			if (SOSubsystem->MarkSlotAsOccupied(CurrentClaimHandle, BehaviorDefinitionClass))
			{
				// 5. Execute the linked Gameplay Behavior
				const UGameplayBehaviorSmartObjectBehaviorDefinition* Definition = 
					Cast<UGameplayBehaviorSmartObjectBehaviorDefinition>(SOSubsystem->GetBehaviorDefinition(CurrentClaimHandle, BehaviorDefinitionClass));

				if (Definition)
				{
					UGameplayBehaviorSubsystem* BehaviorSubsystem = UGameplayBehaviorSubsystem::GetCurrent(GetWorld());
					AActor* MutableActor = const_cast<AActor*>(TargetActor);
					// BehaviorSubsystem->StartBehavior(*Avatar, *Definition->GameplayBehaviorConfig, TargetActor);
					BehaviorSubsystem->TriggerBehavior(*Definition->GameplayBehaviorConfig, *Avatar, MutableActor);
				}
			}
		}
	}
}

void UAlsxtGameplayAbilityInteraction::OnTargetFound(FSmartObjectHandle SmartObjectHandle, FSmartObjectSlotHandle SlotHandle,
	FSmartObjectClaimHandle ClaimHandle)
{
	
}

void UAlsxtGameplayAbilityInteraction::ReleaseSlot()
{
	if (CurrentClaimHandle.IsValid())
	{
		if (USmartObjectSubsystem* SOSubsystem = USmartObjectSubsystem::GetCurrent(GetWorld()))
		{
			// SOSubsystem->ReleaseSlot(CurrentClaimHandle);
			SOSubsystem->MarkSlotAsFree(CurrentClaimHandle);
		}
		CurrentClaimHandle = FSmartObjectClaimHandle::InvalidHandle;
	}
}


void UAlsxtGameplayAbilityInteraction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ReleaseSlot();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
