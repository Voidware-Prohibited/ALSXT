// MIT


#include "Components/Character/TargetLockComponent.h"
#include "Utility/ALSXTStructs.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/ALSXTEnums.h"
#include "Math/Vector.h"
#include "Interfaces/TargetLockInterface.h"

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

float UTargetLockComponent::GetAngle(FVector Target)
{
	float resultAngleInRadians = 0.0f;
	FVector PlayerLocation = Character->GetActorLocation();
	PlayerLocation.Normalize();
	Target.Normalize();

	auto crossProduct = PlayerLocation.Cross(Target);
	auto dotProduct = PlayerLocation.Dot(Target);

	if (crossProduct.Z > 0)
	{
		resultAngleInRadians = acosf(dotProduct);
	}
	else
	{
		resultAngleInRadians = -1 * acosf(dotProduct);
	}

	auto resultAngleInDegrees = FMath::RadiansToDegrees(resultAngleInRadians);
	return resultAngleInDegrees;
}

void UTargetLockComponent::TraceForTargets(bool DisplayDebug, float DebugDuration, TArray<FTargetHitResultEntry>& Targets)
{
	FRotator ControlRotation = Character->GetControlRotation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector CameraLocation = Character->Camera->GetFirstPersonCameraLocation();
	FVector StartLocation = ForwardVector * 150 + CameraLocation;
	FVector EndLocation = ForwardVector * 200 + StartLocation;
	// FVector CenterLocation = (StartLocation - EndLocation) / 2 + StartLocation;
	FVector CenterLocation = (StartLocation - EndLocation) / 8 + StartLocation;
	FVector HalfSize = { 400.0f, 400.0f, 150.0f };
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
	TArray<FHitResult> OutHits;

	// Display Debug Shape
	if (DebugMode)
	{
		DrawDebugBox(GetWorld(), CenterLocation, HalfSize, ControlRotation.Quaternion(), FColor::Green, false, DebugDuration, 100, 2);
	}
	
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, StartLocation, EndLocation, ControlRotation.Quaternion(), ECollisionChannel::ECC_Camera, CollisionShape);

	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UTargetLockInterface::StaticClass()) && Hit.GetActor() != Character)
			{
				FTargetHitResultEntry HitResultEntry;
				HitResultEntry.Valid = true;
				HitResultEntry.DistanceFromPlayer = FVector::Distance(Character->GetActorLocation(), Hit.Location);
				HitResultEntry.AngleFromCenter = GetAngle(Hit.Location);
				HitResultEntry.HitResult = Hit;
				Targets.Add(HitResultEntry);
			}
		}
	}
}

void UTargetLockComponent::GetClosestTarget(const TArray<FTargetHitResultEntry>& HitResults, FTargetHitResultEntry& Target)
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(true, 6, OutHits);
	FTargetHitResultEntry FoundHit;

	for (auto& Hit : OutHits)
	{
		if (Hit.HitResult.GetActor()->GetClass()->ImplementsInterface(UTargetLockInterface::StaticClass()) && Hit.HitResult.GetActor() != Character)
		{
			FTargetHitResultEntry HitResultEntry;
			HitResultEntry.Valid = true;
			HitResultEntry.DistanceFromPlayer = Hit.DistanceFromPlayer;
			HitResultEntry.AngleFromCenter = Hit.AngleFromCenter;
			HitResultEntry.HitResult = Hit.HitResult;
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && (HitResultEntry.DistanceFromPlayer < CurrentTarget.DistanceFromPlayer))
			{
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
					if (GEngine && DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
					}
				}
				else
				{
					if (Hit.AngleFromCenter < FoundHit.AngleFromCenter)
					{
						FoundHit = Hit;
						if (GEngine && DebugMode)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
						}
					}
				}
			}
		}
	}
	Target = FoundHit;
}

void UTargetLockComponent::DisengageAllTargets()
{
	// ...
}

void UTargetLockComponent::GetTargetLeft(const TArray<FTargetHitResultEntry>& HitResults, FTargetHitResultEntry& Target)
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(true, 6, OutHits);
	FTargetHitResultEntry FoundHit;
	
	for (auto& Hit : OutHits)
	{
		if (Hit.HitResult.GetActor()->GetClass()->ImplementsInterface(UTargetLockInterface::StaticClass()) && Hit.HitResult.GetActor() != Character)
		{
			FTargetHitResultEntry HitResultEntry;
			HitResultEntry.Valid = true;
			HitResultEntry.DistanceFromPlayer = Hit.DistanceFromPlayer;
			HitResultEntry.AngleFromCenter = Hit.AngleFromCenter;
			HitResultEntry.HitResult = Hit.HitResult;
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && (HitResultEntry.AngleFromCenter < CurrentTarget.AngleFromCenter))
			{
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
					if (GEngine && DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
					}
				}
				else
				{
					if (Hit.AngleFromCenter < FoundHit.AngleFromCenter) 
					{
						FoundHit = Hit;
						if (GEngine && DebugMode)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
						}
					}
				}
			}
		}
	}
	Target = FoundHit;
}

void UTargetLockComponent::GetTargetRight(const TArray<FTargetHitResultEntry>& HitResults, FTargetHitResultEntry& Target)
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(true, 6, OutHits);
	FTargetHitResultEntry FoundHit;

	for (auto& Hit : OutHits)
	{
		if (Hit.HitResult.GetActor()->GetClass()->ImplementsInterface(UTargetLockInterface::StaticClass()) && Hit.HitResult.GetActor() != Character)
		{
			FTargetHitResultEntry HitResultEntry;
			HitResultEntry.Valid = true;
			HitResultEntry.DistanceFromPlayer = Hit.DistanceFromPlayer;
			HitResultEntry.AngleFromCenter = Hit.AngleFromCenter;
			HitResultEntry.HitResult = Hit.HitResult;
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && (HitResultEntry.AngleFromCenter > CurrentTarget.AngleFromCenter))
			{
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
					if (GEngine && DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
					}
				}
				else
				{
					if (Hit.AngleFromCenter < FoundHit.AngleFromCenter)
					{
						FoundHit = Hit;
						if (GEngine && DebugMode)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.HitResult.GetActor()->GetName()));
						}
					}
				}
			}
		}
	}
	Target = FoundHit;
}

void UTargetLockComponent::RotatePlayerToTarget(FTargetHitResultEntry Target)
{
	FRotator PlayerRotation = UKismetMathLibrary::FindLookAtRotation(Target.HitResult.Location, Character->GetActorLocation());
	Character->SetActorRotation(PlayerRotation, ETeleportType::TeleportPhysics);
}
