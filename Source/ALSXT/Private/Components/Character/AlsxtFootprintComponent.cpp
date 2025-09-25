// MIT


#include "Components/Character/AlsxtFootprintComponent.h"

#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAlsxtFootprintComponent::UAlsxtFootprintComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAlsxtFootprintComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LeftFootSaturationTimeline->SetFloatCurve(LeftFootSaturationTimelineCurve, FName("Alpha"));
	RightFootSaturationTimeline->SetFloatCurve(RightFootSaturationTimelineCurve, FName("Alpha"));
	
}

void UAlsxtFootprintComponent::ClientSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
}

void UAlsxtFootprintComponent::UpdateFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& Target, const float& Alpha)
{
}

void UAlsxtFootprintComponent::StartFootSaturation(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& TargetState)
{
}

void UAlsxtFootprintComponent::ResetFootSaturationTimeline_Implementation(const EAlsFootBone& Foot)
{
}

void UAlsxtFootprintComponent::MulticastSetFootprintsState_Implementation(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
}

void UAlsxtFootprintComponent::OnFootprintsStateChanged_Implementation(const FAlsxtFootprintsState& PreviousFootprintsState)
{
}


UTimelineComponent* UAlsxtFootprintComponent::GetFootTimeline(const EAlsFootBone& Foot)
{
	UTimelineComponent*  TimelineOut {nullptr};
	return TimelineOut;
}

// Called every frame
void UAlsxtFootprintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAlsxtFootprintComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;
	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FootprintsState, Parameters)
}

const FAlsxtFootprintsState& UAlsxtFootprintComponent::GetFootprintsState() const
{
	return FootprintsState;
}

void UAlsxtFootprintComponent::SetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
}

FAlsxtFootprintsState UAlsxtFootprintComponent::ProcessNewFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	FAlsxtFootprintsState OutFootprintsState;
	return OutFootprintsState;
}

void UAlsxtFootprintComponent::SetFootprintNewSurface(const FAlsxtFootprintsState& UpdatedTargetState,
	const FAlsxtFootprintsState& NewFootprintsState, const EAlsFootBone& Foot)
{
}

void UAlsxtFootprintComponent::OnReplicate_FootprintsState(const FAlsxtFootprintsState& PreviousFootprintsState)
{
}

void UAlsxtFootprintComponent::ServerProcessNewFootprintsState_Implementation(const EAlsFootBone& Foot,
                                                                              const FAlsxtFootprintsState& NewFootprintsState)
{
}

