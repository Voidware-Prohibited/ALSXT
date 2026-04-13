#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "Chooser.h"
#include "AlsxtGameplayAbilityBreathing.generated.h"

class UGameplayEffect;

/**
 * Gameplay Ability to manage the character's breathing rate.
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityBreathing : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayAbilityBreathing();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> BreathingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	FName VoiceSocketName = "HeadSocket";

	FActiveGameplayEffectHandle ActiveEffectHandle;

	// Chooser Table asset to define BreathState based on Stamina
	UPROPERTY(EditDefaultsOnly, Category = "Breathing")
	UChooserTable* BreathingChooser; 

	// MetaSound asset
	UPROPERTY(EditDefaultsOnly, Category = "Breathing")
	USoundBase* BreathingMetaSound;

	// Tags to deactivate breath
	UPROPERTY(EditDefaultsOnly, Category = "Breathing")
	FGameplayTagContainer HoldingBreathTags;

	// Tags to deactivate breath
	UPROPERTY(EditDefaultsOnly, Category = "Breathing")
	FGameplayTagContainer HoldingBreathUnderwaterTags;

	// Tags to deactivate breath
	UPROPERTY(EditDefaultsOnly, Category = "Breathing")
	FGameplayTagContainer DeactivationTags;

private:
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnDeactivationTagChanged(const FOnAttributeChangeData& Data);
	void RefreshBreathingSounds(float CurrentStamina);
    
	// Ability Task handle for the persistent sound
	UPROPERTY()
	class UAudioComponent* ActiveBreathComponent;
    
	FDelegateHandle StaminaChangedDelegateHandle;
};

inline void UAlsxtGameplayAbilityBreathing::OnDeactivationTagChanged(const FOnAttributeChangeData& Data)
{
	
}
