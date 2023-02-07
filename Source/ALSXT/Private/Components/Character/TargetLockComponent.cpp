// MIT


#include "Components/Character/TargetLockComponent.h"
#include "Utility/ALSXTStructs.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

void UTargetLockComponent::TraceForTargets(bool DisplayDebug, float DebugDuration, TArray<FHitResult>& Targets)
{
	// ...
	FRotator ControlRotation = Character->GetControlRotation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector CameraLocation = Character->Camera->GetFirstPersonCameraLocation();
	FVector StartLocation = ForwardVector * 150 + CameraLocation;
	FVector EndLocation = ForwardVector * 200 + StartLocation;
	// FVector CenterLocation = (StartLocation - EndLocation) / 2 + StartLocation;
	FVector CenterLocation = (StartLocation - EndLocation) / 8 + StartLocation;
	FVector HalfSize = { 400.0f, 400.0f, 150.0f };
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
	// draw collision sphere
	if (DisplayDebug)
	{
		DrawDebugBox(GetWorld(), CenterLocation, HalfSize, ControlRotation.Quaternion(), FColor::Green, false, DebugDuration, 100, 2);
		// DrawDebugBox(GetWorld(), StartLocation, HalfSize, FColor::Purple, ControlRotation, 50, 6, 1);
	}
	TArray<FHitResult> OutHits;
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, StartLocation, EndLocation, ControlRotation.Quaternion(), ECollisionChannel::ECC_Camera, CollisionShape);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			if (GEngine)
			{
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}
		}
	}
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

