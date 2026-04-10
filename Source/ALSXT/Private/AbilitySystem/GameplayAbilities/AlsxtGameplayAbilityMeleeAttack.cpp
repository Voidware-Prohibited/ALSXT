// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityMeleeAttack.h"

#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "AbilitySystem/Data/AlsxtMeleeTraceEventData.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Notify/AlsxtAnimNotifyState_UCTrace.h"


UAlsxtGameplayAbilityMeleeAttack::UAlsxtGameplayAbilityMeleeAttack()
{
	// Create Wait Task for Melee Attack Trace Begin Event Tag
	UAbilityTask_WaitGameplayEvent* OnMeleeCollisionTraceBeginWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Trace.Melee.Begin"));
	OnMeleeCollisionTraceBeginWaitTask->EventReceived.AddDynamic(this, &UAlsxtGameplayAbilityMeleeAttack::OnMeleeCollisionTraceBeginEventReceived);
	OnMeleeCollisionTraceBeginWaitTask->ReadyForActivation();

	// Create Wait Task for Melee Attack Trace End Event Tag
	UAbilityTask_WaitGameplayEvent* OnMeleeCollisionTraceEndWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Trace.Melee.End"));
	OnMeleeCollisionTraceEndWaitTask->EventReceived.AddDynamic(this, &UAlsxtGameplayAbilityMeleeAttack::OnMeleeCollisionTraceEndEventReceived);
	OnMeleeCollisionTraceEndWaitTask->ReadyForActivation();
}

bool UAlsxtGameplayAbilityMeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                          const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}

	// Get the Stamina Attribute Set
	const UAlsxtStaminaAttributeSet* StaminaSet = ASC->GetSet<UAlsxtStaminaAttributeSet>();
	if (!StaminaSet)
	{
		return false;
	}

	// Calculate the jump cost from the Gameplay Effect
	
	// const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	float CurrentStamina = StaminaSet->GetCurrentStamina();
	if (CostGameplayEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle CostSpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffectClass, 1.0f, EffectContext);
		if (CostSpecHandle.IsValid())
		{
			for (const FGameplayModifierInfo& Modifier : CostSpecHandle.Data->Def->Modifiers)
			{
				if (Modifier.Attribute == UAlsxtStaminaAttributeSet::GetCurrentStaminaAttribute() && Modifier.ModifierOp == EGameplayModOp::Additive)
				{
					float ActionCost = 0.0f;
					ActionCost -= CostSpecHandle.Data->GetModifierMagnitude(0);
					// return CurrentStamina >= FMath::Abs(ActionCost) && Character->CanJump(); // Use FMath::Abs as cost is likely negative
					return CurrentStamina >= FMath::Abs(ActionCost); // Use FMath::Abs as cost is likely negative
				}
			}
		}
		// return Character->CanJump();
		return false;
	}
	// return Character->CanJump();
	return false;
}

void UAlsxtGameplayAbilityMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAlsxtGameplayAbilityMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAlsxtGameplayAbilityMeleeAttack::OnMeleeCollisionTraceBeginEventReceived(FGameplayEventData Payload)
{
	// Extract our custom data from the payload's optional payload
	if (Payload.OptionalObject.Get())
	{
		// const FAlsxtMeleeTraceEventData* CustomDataPtr = Cast<FAlsxtMeleeTraceEventData>(Payload.OptionalObject.Get());
		//const FAlsxtMeleeTraceEventData* CustomDataPtr = Payload.OptionalObject.GetAccessTrackedObjectPtr()<FAlsxtMeleeTraceEventData>();

		const UAlsxtMeleeTraceEventDataObject* TraceData = Cast<UAlsxtMeleeTraceEventDataObject>(Payload.OptionalObject.Get());
		{
			BeginMeleeTrace(TraceData->GetInstancedStruct().StartLocation, TraceData->GetInstancedStruct().EndLocation, TraceData->GetInstancedStruct().Radius);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Melee event received without valid FMeleeTraceEventData payload"));
	}

	// End the ability when all is done (or wait for montage complete event, etc.)
	// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UAlsxtGameplayAbilityMeleeAttack::OnMeleeCollisionTraceEndEventReceived(FGameplayEventData Payload)
{
	// Extract our custom data from the payload's optional payload
	if (Payload.OptionalObject.Get())
	{
		// const FAlsxtMeleeTraceEventData* CustomDataPtr = Cast<FAlsxtMeleeTraceEventData>(Payload.OptionalObject.Get());
		//const FAlsxtMeleeTraceEventData* CustomDataPtr = Payload.OptionalObject.GetAccessTrackedObjectPtr()<FAlsxtMeleeTraceEventData>();

		const UAlsxtMeleeTraceEventDataObject* TraceData = Cast<UAlsxtMeleeTraceEventDataObject>(Payload.OptionalObject.Get());
		{
			BeginMeleeTrace(TraceData->GetInstancedStruct().StartLocation, TraceData->GetInstancedStruct().EndLocation, TraceData->GetInstancedStruct().Radius);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Melee event received without valid FMeleeTraceEventData payload"));
	}

	EndMeleeTrace();

	// End the ability when all is done (or wait for montage complete event, etc.)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UAlsxtGameplayAbilityMeleeAttack::BeginMeleeTrace(FVector Start, FVector End, float Radius)
{
	
}

void UAlsxtGameplayAbilityMeleeAttack::EndMeleeTrace()
{
	// Clear Timer/Task
	// GetTimerManager().ClearTimer(AttackTraceTimerHandle);

	// Reset Attack Trace Settings
	// CurrentAttackTraceSettings.Start = { 0.0f, 0.0f, 0.0f };
	// CurrentAttackTraceSettings.End = { 0.0f, 0.0f, 0.0f };
	// CurrentAttackTraceSettings.Radius = { 0.0f };

	// Empty AttackTraceLastHitActors Array
	AttackTraceLastHitActors.Empty();
	
	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetAvatarActorFromActorInfo(), false);
	}
}
