// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AlsxtGameplayAbilityMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityMeleeAttack : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayAbilityMeleeAttack();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Function to call when the Anim Notify event is received
	UFUNCTION()
	void OnMeleeCollisionTraceBeginEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnMeleeCollisionTraceEndEventReceived(FGameplayEventData Payload);

	// Function to perform the actual trace, can be overridden in Blueprints or child C++
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void BeginMeleeTrace(FVector Start, FVector End, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void EndMeleeTrace();

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitEventTask;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTag HitTraceEventTag; // Set to GameplayEvent.Melee.HitTrace in Blueprint/Defaults

	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	FGameplayTag CooldownMagnitudeTag;

	// The Gameplay Effects for Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TArray<TSubclassOf<UGameplayEffect>> DamageCostEffects;

	// Tag to set the magnitude of damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageMagnitudeTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	TArray<AActor*> AttackTraceLastHitActors;
	
};
