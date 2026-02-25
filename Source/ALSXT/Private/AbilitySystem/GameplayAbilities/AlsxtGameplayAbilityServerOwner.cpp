// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityServerOwner.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"

UAlsxtGameplayAbilityServerOwner::UAlsxtGameplayAbilityServerOwner()
{
    // Set up ability tags, replication settings, etc.
    // bServerRespectsRemoteActivation = true; // Allow client to ask server to activate
	// Ensure this only runs on the server
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UAlsxtGameplayAbilityServerOwner::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo) == false)
    {
        return;
    }
    
    // Logic to open an admin UI, listen for events, or execute immediate actions
    // For a functional UI, you'd likely use Ability Tasks to handle asynchronous input and UI interactions.
    // Example: Grant a specific ability immediately upon activation for demonstration
    // GrantPlayerAbility(SomeAbilityClass, 1);

	if (HasAuthority(&CurrentActivationInfo))
	{
		// 1. Update GameState Tag
		AGameStateBase* GS = GetWorld()->GetGameState();
		// if (AMyGameState* MyGS = Cast<AMyGameState>(GS))
		// {
		// 	MyGS->SetCurrentGameTags(TargetStateTag);
		// }

		// 2. Change Level/Map if specified
		if (!TargetMapName.IsEmpty())
		{
			GetWorld()->ServerTravel(TargetMapName);
		}

		// 3. Update Level Parameters via GameMode
		AGameModeBase* GM = GetWorld()->GetAuthGameMode();
		// if (AMyGameMode* MyGM = Cast<AMyGameMode>(GM))
		// {
		// 	MyGM->UpdateLevelParameters(); // Custom function
		// }
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAlsxtGameplayAbilityServerOwner::GrantPlayerAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo()))
    {
        if (AbilityClass)
        {
            ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, Level));
        }
    }
}

void UAlsxtGameplayAbilityServerOwner::RemovePlayerAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo()))
    {
    	// This function should ideally only be called on the server, as per GAS documentation.
    	if (!GetOwningActorFromActorInfo()->HasAuthority())
    	{
    		// Optionally add a log message or server RPC call here if needed
    		return;
    	}

    	// 1. Find the ability spec handle associated with the class
    	// Iterate over all the activatable abilities in the component
    	FGameplayAbilitySpecHandle FoundHandle;
    	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
    	{
    		if (Spec.Ability->GetClass() == AbilityClass)
    		{
    			FoundHandle = Spec.Handle;
    			break; // Found the ability, can exit loop
    		}
    	}

    	// If a valid handle was found, remove the ability
    	if (FoundHandle.IsValid())
    	{
    		ASC->ClearAbility(FoundHandle);
    	}
    	
    }
}

void UAlsxtGameplayAbilityServerOwner::ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> EffectClass, AActor* TargetActor, float Magnitude, FGameplayTag MagnitudeTag)
{
    if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
    {
        if (EffectClass)
        {
            FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, 1.0f, FGameplayEffectContextHandle());
            
            // If a tag is provided, set the magnitude dynamically using "Set By Caller"
            if (MagnitudeTag.IsValid())
            {
                SpecHandle.Data.Get()->SetSetByCallerMagnitude(MagnitudeTag, Magnitude);
            }
            
            TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void UAlsxtGameplayAbilityServerOwner::RemoveGameplayEffectFromTarget(AActor* TargetActor, FGameplayTag GameplayEffectTag)
{
    if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
    {
    	FGameplayTagContainer TagContainer;
    	TagContainer.AddTag(GameplayEffectTag);

    	// Remove all active effects that have this tag
    	// The -1 removes all stacks
    	TargetASC->RemoveActiveEffectsWithTags(TagContainer);
    }
}

void UAlsxtGameplayAbilityServerOwner::SetBaseAttributeValue(AActor* TargetActor, FGameplayAttribute Attribute, float NewValue)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		// This sets the base value, effects will still apply on top of this.
		// ASC->SetGameplayAttributeValue(Attribute, NewValue);
		if (ASC->HasAttributeSetForAttribute(Attribute))
		{
			ASC->SetNumericAttributeBase(Attribute, NewValue);
		}
	}
}
