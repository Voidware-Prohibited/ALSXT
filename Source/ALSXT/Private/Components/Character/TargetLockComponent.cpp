// MIT


#include "Components/Character/TargetLockComponent.h"
#include "Utility/ALSXTStructs.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/ALSXTEnums.h"
#include "Math/Vector.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TargetLockInterface.h"

// Sets default values for this component's properties
UTargetLockComponent::UTargetLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
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

	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (Character && TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming() && IsValid(CurrentTarget.HitResult.GetActor()))
	{
		// if (CurrentTarget.HitResult.GetActor()->GetDistanceTo(Character) > MaxLockDistance)
		// {
		// 
		// }
		if (UnlockWhenTargetIsObstructed && !IsTartgetObstructed()) {
			if (CurrentTarget.Valid)
			{
				RotatePlayerToTarget(CurrentTarget);
			}
		}
		else
		{
			if (CurrentTarget.Valid)
			{
				RotatePlayerToTarget(CurrentTarget);
			}
		}	
	}
}

float UTargetLockComponent::GetAngle(FVector Target)
{
	float resultAngleInRadians = 0.0f;
	FVector PlayerLocation = Character->GetActorLocation();
	PlayerLocation.Normalize();
	Target.Normalize();

	auto crossProduct = PlayerLocation.Cross(Target);
	auto dotProduct = PlayerLocation.Dot(Target);
	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(Target, PlayerLocation);
	FRotator RotX = UKismetMathLibrary::MakeRotFromX(UnitVector);
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation(), RotX);

	if (crossProduct.Z > 0)
	{
		resultAngleInRadians = acosf(dotProduct);
	}
	else
	{
		resultAngleInRadians = -1 * acosf(dotProduct);
	}

	auto resultAngleInDegrees = FMath::RadiansToDegrees(resultAngleInRadians);
	//return resultAngleInDegrees;
	return FRotator::ClampAxis(Delta.Yaw);
}

bool UTargetLockComponent::IsTartgetObstructed()
{
	FVector CharLoc = Character->GetActorLocation();
	TArray<FHitResult> OutHits;

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : ObstructionTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}

	if (GetWorld()->LineTraceMultiByObjectType(OutHits, CharLoc, CurrentTarget.HitResult.GetActor()->GetActorLocation(), ObjectQueryParameters))
	{
		if (UnlockWhenTargetIsObstructed)
		{
			ClearCurrentTarget();
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UTargetLockComponent::TraceForTargets(TArray<FTargetHitResultEntry>& Targets)
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
		DrawDebugBox(GetWorld(), CenterLocation, TraceAreaHalfSize, ControlRotation.Quaternion(), FColor::Yellow, false, DebugDuration, 100, 2);
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

void UTargetLockComponent::GetClosestTarget()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming())
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != Character)
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
					}
					else
					{
						if (Hit.AngleFromCenter < FoundHit.AngleFromCenter)
						{
							FoundHit = Hit;
						}
					}
				}
			}
		}
		if (FoundHit.Valid && FoundHit.HitResult.GetActor())
		{
			SetCurrentTarget(FoundHit);
			USkeletalMeshComponent* HitMesh = Cast<AALSXTCharacter>(FoundHit.HitResult.GetActor())->GetMesh();
			TArray<UMaterialInterface*> CharMaterials = HitMesh->GetMaterials();
			if (CharMaterials[0]) 
			{
				for (int m = 0;  m < CharMaterials.Num(); m++)
				{
					UMaterialInstanceDynamic* CharDynMaterial = HitMesh->CreateAndSetMaterialInstanceDynamic(m);
					CharDynMaterial->SetScalarParameterValue(HighlightMaterialParameterName, 1.0f);
					HitMesh->SetMaterial(m, CharDynMaterial);
				}
			}	
			if (GEngine && DebugMode)
			{
				FString DebugMsg = FString::SanitizeFloat(FoundHit.AngleFromCenter);
				DebugMsg.Append(" Hit Result: ");
				DebugMsg.Append(FoundHit.HitResult.GetActor()->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMsg);
			}
		}
	}
}

void UTargetLockComponent::SetCurrentTarget(const FTargetHitResultEntry& NewTarget)
{
	CurrentTarget = NewTarget;
}

void UTargetLockComponent::ClearCurrentTarget()
{
	CurrentTarget.Valid = false;
	CurrentTarget.DistanceFromPlayer = 340282346638528859811704183484516925440.0f;
	CurrentTarget.AngleFromCenter = 361.0f;

	if (Cast<AALSXTCharacter>(CurrentTarget.HitResult.GetActor()))
	{
		USkeletalMeshComponent* HitMesh = Cast<AALSXTCharacter>(CurrentTarget.HitResult.GetActor())->GetMesh();
		TArray<UMaterialInterface*> CharMaterials = HitMesh->GetMaterials();
		if (CharMaterials[0])
		{
			for (int m = 0; m < CharMaterials.Num(); m++)
			{
				UMaterialInstanceDynamic* CharDynMaterial = HitMesh->CreateAndSetMaterialInstanceDynamic(m);
				CharDynMaterial->SetScalarParameterValue(HighlightMaterialParameterName, 0.0f);
				HitMesh->SetMaterial(m, CharDynMaterial);
			}
		}
	}
	CurrentTarget.HitResult = FHitResult(ForceInit);
}

void UTargetLockComponent::DisengageAllTargets()
{
	CurrentTarget.Valid = false;
	CurrentTarget.DistanceFromPlayer = 340282346638528859811704183484516925440.0f;
	CurrentTarget.AngleFromCenter = 361.0f;
	CurrentTarget.HitResult.Init();
}

void UTargetLockComponent::GetTargetLeft()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming())
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != Character && (Hit.AngleFromCenter < CurrentTarget.AngleFromCenter))
			{
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
				}
				else
				{
					if(Hit.AngleFromCenter > FoundHit.AngleFromCenter)
					{
						FoundHit = Hit;
					}
				}
			}
		}
		SetCurrentTarget(FoundHit);
	}
}

void UTargetLockComponent::GetTargetRight()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming())
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != Character && (Hit.AngleFromCenter > CurrentTarget.AngleFromCenter))
			{
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
				}
				else
				{
					if (Hit.AngleFromCenter < FoundHit.AngleFromCenter)
					{
						FoundHit = Hit;
					}
				}
			}
		}
		SetCurrentTarget(FoundHit);
	}
}

void UTargetLockComponent::RotatePlayerToTarget(FTargetHitResultEntry Target)
{
	if (IsValid(Character) && IsValid(Character->GetController()) && Target.Valid && IsValid(Target.HitResult.GetActor()))
	{
		FRotator CurrentPlayerRotation = Character->GetActorRotation();
		FRotator CurrentPlayerControlRotation = Character->GetController()->GetControlRotation();
		FVector TargetActorLocation = Target.HitResult.GetActor()->GetActorLocation();
		FRotator NewPlayerRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetActorLocation);
		NewPlayerRotation.Pitch = CurrentPlayerRotation.Pitch;
		NewPlayerRotation.Roll = CurrentPlayerRotation.Roll;
		
		if ((NewPlayerRotation != CurrentPlayerControlRotation) && IsValid(Target.HitResult.GetActor()))
		{
			// Character->SetActorRotation(NewPlayerRotation, ETeleportType::TeleportPhysics);
			Character->GetController()->SetControlRotation(NewPlayerRotation);
		}
	}
}
