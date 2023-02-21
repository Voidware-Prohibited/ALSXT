// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ALSXTAnimNotifyState_UCTrace.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTAnimNotifyState_UCTrace : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	FALSXTAttackTraceSettings TraceSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag UnarmedAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag AttackStrength;

public:
	UALSXTAnimNotifyState_UCTrace();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                         float Duration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
	
};
