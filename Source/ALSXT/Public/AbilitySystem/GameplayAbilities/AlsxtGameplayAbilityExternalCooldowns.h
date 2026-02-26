// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityExternalCooldowns.generated.h"


USTRUCT(BlueprintType)
struct FAlsxtCooldownEffectEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayEffect> CooldownGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTag MagnitudeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	float Magnitude {1.0f}; 
};

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityExternalCooldowns : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UFUNCTION()
	void ApplyExternalCooldownEffects(TArray<FAlsxtCooldownEffectEntry> CooldownEffects);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldowns|External", meta=(ToolTip="Cooldown Gameplay Effect applied when this Ability is Activated"))
	TArray<FAlsxtCooldownEffectEntry> OnAbilityActivatedCooldownEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldowns|External", meta=(ToolTip="Cooldown Gameplay Effect applied when this Ability is Activated"))
	TArray<FAlsxtCooldownEffectEntry> OnAbilityEndedCooldownEffects;
};
