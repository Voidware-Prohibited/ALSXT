// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityServerOwner.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityServerOwner : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayAbilityServerOwner();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Functions to call from ActivateAbility (ensure they are called on the server)
	UFUNCTION(BlueprintCallable, Category = "Admin Ability")
	void GrantPlayerAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Admin Ability")
	void RemovePlayerAbility(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Admin Ability")
	void ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> EffectClass, AActor* TargetActor, float Magnitude = 0.0f, FGameplayTag MagnitudeTag = FGameplayTag());

	UFUNCTION(BlueprintCallable, Category = "Admin Ability")
	void RemoveGameplayEffectFromTarget(AActor* TargetActor, FGameplayTag GameplayEffectTag);

	UFUNCTION(BlueprintCallable, Category = "Admin Ability")
	void SetBaseAttributeValue(AActor* TargetActor, FGameplayAttribute Attribute, float NewValue);

	// Tag to indicate which state to move to
	UPROPERTY(EditAnywhere, Category = "Game State")
	FGameplayTag TargetStateTag;

	// Optional map to load if this moves to a new level
	UPROPERTY(EditAnywhere, Category = "Game State")
	FString TargetMapName;
};
