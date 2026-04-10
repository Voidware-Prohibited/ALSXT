// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Settings/AlsxtCombatSettings.h"
#include "AbilitySystem/Data/AlsxtMeleeTraceEventData.h"
#include "AlsxtAnimNotifyState_UCTrace.generated.h"

UCLASS(BlueprintType)
class UAlsxtMeleeTraceEventDataObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FAlsxtMeleeTraceEventData MeleeTraceEventData;

	// Helper to get the struct
	const FAlsxtMeleeTraceEventData& GetInstancedStruct() const { return MeleeTraceEventData; }
};

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtAnimNotifyState_UCTrace : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	FAlsxtCombatAttackTraceSettings TraceSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FGameplayTag MeleeCollisionTraceBeginEventTag {FGameplayTag::RequestGameplayTag(FName("Event.Trace.Melee.Begin"))};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FGameplayTag MeleeCollisionTraceEndEventTag {FGameplayTag::RequestGameplayTag(FName("Event.Trace.Melee.End"))};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag UnarmedAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag AttackStrength;

public:
	UAlsxtAnimNotifyState_UCTrace();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                         float Duration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
	
};
