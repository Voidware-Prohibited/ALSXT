#include "ALSXTAIController.h"

AALSXTAIController::AALSXTAIController()
{
	bAttachToPawn = true;
}

void AALSXTAIController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	RunBehaviorTree(BehaviourTree);
}

FVector AALSXTAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	const auto* FocusedPawn{Cast<APawn>(Actor)};
	if (IsValid(FocusedPawn))
	{
		return FocusedPawn->GetPawnViewLocation();
	}

	return Super::GetFocalPointOnActor(Actor);
}
