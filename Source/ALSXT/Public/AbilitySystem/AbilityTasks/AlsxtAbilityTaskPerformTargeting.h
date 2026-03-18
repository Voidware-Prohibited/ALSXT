// MIT

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetingSystem/TargetingPreset.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AlsxtAbilityTaskPerformTargeting.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomTargetingDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtAbilityTaskPerformTargeting : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAlsxtAbilityTaskPerformTargeting(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FCustomTargetingDelegate OnTargetDataReady;

	/**
	 * Creates the custom targeting ability task.
	 * @param OwningAbility The ability that owns this task.
	 * @param TaskName The name of the task.
	 * @return The created ability task instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAlsxtAbilityTaskPerformTargeting* CustomTargetDataTask(UGameplayAbility* OwningAbility, FName TaskName);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	void PerformTargeting();

	// The actual targeting logic would go here. For this example, we simulate a simple line trace.
	FGameplayAbilityTargetDataHandle ProduceTargetData();
};
