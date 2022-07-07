#pragma once

#include "AIController.h"
#include "ALSXTAIController.generated.h"

UCLASS(DisplayName = "Als AI Controller")
class ALSXT_API AALSXTAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviourTree;

public:
	AALSXTAIController();

protected:
	virtual void OnPossess(APawn* NewPawn) override;

public:
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
};
