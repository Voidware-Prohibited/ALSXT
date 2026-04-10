// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AlsxtAnimNotifyState_HITrace.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtAnimNotifyState_HITrace : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag OverlayMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace Data")
	FGameplayTag MeleeEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag ImpactType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag AttackStrength;

public:
	UAlsxtAnimNotifyState_HITrace();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                         float Duration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
	
};
