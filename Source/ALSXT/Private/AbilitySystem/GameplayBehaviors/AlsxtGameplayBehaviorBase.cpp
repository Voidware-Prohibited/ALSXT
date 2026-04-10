// MIT


#include "AbilitySystem/GameplayBehaviors/AlsxtGameplayBehaviorBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"


bool UAlsxtGameplayBehaviorBase::Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner)
{
	// return Super::Trigger(Avatar, Config, SmartObjectOwner);

	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(&Avatar)) {
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		// Send a tag that triggers a specific Gameplay Ability
		FGameplayEventData Payload;
		Payload.Instigator = &Avatar;
		ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Interaction.Activate"), &Payload);
		return true;
	}
	return false;
}
