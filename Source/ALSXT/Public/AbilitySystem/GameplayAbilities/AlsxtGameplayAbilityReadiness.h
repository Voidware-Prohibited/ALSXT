// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityReadiness.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityReadiness : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UAlsxtGameplayAbilityReadiness();
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag AimingTag;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag ReadyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag NeutralTag;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> ReadyGameplayEffect;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> AimingGameplayEffect;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> FinalReplacementGameplayEffect;

	// Use an Ability Task to listen for the input action's ongoing state
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* AimInputAction = nullptr;

	UFUNCTION()
	void OnInputReleased(float TimeHeld);
    
	UFUNCTION()
	void ApplyTagGameplayEffect(TSubclassOf<UGameplayEffect> EffectClass);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag InputTag; // Tag corresponding to your Enhanced Input Action
};
