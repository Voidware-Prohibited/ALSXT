// MIT


#include "Components/Character/ALSXTStationaryModeComponent.h"

// Sets default values for this component's properties
UALSXTStationaryModeComponent::UALSXTStationaryModeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTStationaryModeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UALSXTStationaryModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UALSXTStationaryModeComponent::TryTraceForSeat(){}

void UALSXTStationaryModeComponent::GetObjectStationaryModeInfo() {}

void UALSXTStationaryModeComponent::GetEnterStationaryModeAnimation() {}

void UALSXTStationaryModeComponent::EnterStationaryMode() {}

void UALSXTStationaryModeComponent::GetExitStationaryModeAnimation() {}

void UALSXTStationaryModeComponent::ExitStationaryMode() {}
