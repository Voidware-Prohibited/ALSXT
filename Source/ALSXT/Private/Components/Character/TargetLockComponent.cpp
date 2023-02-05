// MIT


#include "Components/Character/TargetLockComponent.h"
#include "Utility/ALSXTStructs.h"

// Sets default values for this component's properties
UTargetLockComponent::UTargetLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	Camera = Character->Camera;
	
}


// Called every frame
void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetLockComponent::GetClosestTarget(const TArray<FHitResult>& HitResults, FTargetHitResultEntry& Target) const
{
	// ...
}

void UTargetLockComponent::DisengageAllTargets()
{
	// ...
}

void UTargetLockComponent::GetTargetLeft(const TArray<FHitResult>& HitResults, FTargetHitResultEntry& Target) const
{
	// ...
}

void UTargetLockComponent::GetTargetRight(const TArray<FHitResult>& HitResults, FTargetHitResultEntry& Target) const
{
	// ...
}

