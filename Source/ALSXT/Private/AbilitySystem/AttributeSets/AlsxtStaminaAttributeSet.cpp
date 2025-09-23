// MIT


#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGasGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtStaminaAttributeSet::UAlsxtStaminaAttributeSet()
{
	MaxStamina = 1.0f;
	CurrentStamina = 1.0f;
	CurrentStaminaRegenMagnitude = 1.0f;
	MaxStaminaRegenMagnitude = 10.0f;
	CurrentStaminaRegenDelay = 3.0f;
	MaxStaminaRegenDelay = 10.0f;
}

void UAlsxtStaminaAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentStaminaAttribute())
	{
		CheckMaxReachedForAttribute(MaxStamina, ALSXTGASGameplayTags::State::TAG_State_Max_Stamina.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStaminaAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtStaminaAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	if (Attribute == GetCurrentStaminaAttribute())
	{
		// This should be removed in favor of another method, as we're modifying 2 times the current stamina. (One right before post, one right after (here).
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
		return;
	}

	if (Attribute == GetCurrentStaminaRegenMagnitudeAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		return;
	}

	if (Attribute == GetCurrentStaminaRegenDelayAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		return;
	}
}

void UAlsxtStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	// Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, CurrentStamina, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, MaxStamina, Params);

	// Owner Only
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, CurrentStaminaRegenMagnitude, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, MaxStaminaRegenMagnitude, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, CurrentStaminaRegenDelay, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStaminaAttributeSet, MaxStaminaRegenDelay, Params);
}

void UAlsxtStaminaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Get the Ability System Component from the target actor
	UAbilitySystemComponent* TargetASC = GetOwningAbilitySystemComponentChecked();	
	// UAbilitySystemComponent* TargetASC = Data.Target.AbilityActorInfo.Get()->AbilitySystemComponent.Get();
	if (!TargetASC)
	{
		return;
	}

	// Check if the modified attribute was CurrentStamina
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCurrentStaminaAttribute modified!"));
		float NewStamina = GetCurrentStamina();
        
		// Add the tag if stamina is below the threshold
		if (NewStamina <= LowStaminaThreshold)
		{
			if (!TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("UAlsxtStaminaAttributeSet::PostGameplayEffectExecute: Apply Tag: State.Min.Stamina"));
				// TargetASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina")));
				TargetASC->AddReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina")));
			}
		}
		// Remove the tag if stamina is above the threshold
		else
		{
			if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("UAlsxtStaminaAttributeSet::PostGameplayEffectExecute: Remove Tag: State.Min.Stamina"));
				// TargetASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina")));
				TargetASC->RemoveReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Min.Stamina")));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCurrentStaminaAttribute not modified"));
	}
}

void UAlsxtStaminaAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, CurrentStamina, OldValue);
}

void UAlsxtStaminaAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, MaxStamina, OldValue);
}

void UAlsxtStaminaAttributeSet::OnRep_CurrentStaminaRegenMagnitude(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, CurrentStaminaRegenMagnitude, OldValue);
}

void UAlsxtStaminaAttributeSet::OnRep_MaxStaminaRegenMagnitude(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, MaxStaminaRegenMagnitude, OldValue);
}

void UAlsxtStaminaAttributeSet::OnRep_CurrentStaminaRegenDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, CurrentStaminaRegenDelay, OldValue);
}

void UAlsxtStaminaAttributeSet::OnRep_MaxStaminaRegenDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStaminaAttributeSet, MaxStaminaRegenDelay, OldValue);
}
