// MIT

#include "Components/Character/ALSXTAcrobaticActionComponent.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UALSXTAcrobaticActionComponent::UALSXTAcrobaticActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTAcrobaticActionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	
}


// Called every frame
void UALSXTAcrobaticActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UALSXTAcrobaticActionComponent::TryAcrobaticAction()
{
	if (!GeneralAcrobaticActionSettings.bAcrobaticActions || Character->GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
	{
		return;
	}

	// if (Character->GetLocomotionAction() != FGameplayTag::EmptyTag)
	// {
	// 	return;
	// }

	FGameplayTag AcrobaticActionType;
	DetermineAcrobaticActionType(AcrobaticActionType);

	if (AcrobaticActionType == ALSXTAcrobaticActionTypeTags::Flip)
	{
		BeginFlip();
	}
	else if (AcrobaticActionType == ALSXTAcrobaticActionTypeTags::WallJump)
	{
		BeginWallJump();
	}
	else if (AcrobaticActionType == ALSXTAcrobaticActionTypeTags::WallRun)
	{
		BeginWallRun();
	}
	else
	{
		return;
	}
}

void UALSXTAcrobaticActionComponent::DetermineAcrobaticActionType(FGameplayTag& AcrobaticActionType)
{
	if (Character->GetLocomotionAction() != FGameplayTag::EmptyTag)
	{
		return;
	}
	const auto* Capsule{ Character->GetCapsuleComponent() };
	FVector ActorLocation = Character->GetActorLocation();
	float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float Velocity = Character->GetVelocity().Size();
	TArray<AActor*> ActorsToIgnore;

	FVector ForwardTraceHalfSize {CapsuleRadius, (CapsuleRadius*1.25), CapsuleHalfHeight};
	FVector LateralTraceHalfSize{ (CapsuleRadius * 1.25), CapsuleRadius, CapsuleHalfHeight };
	FVector ForwardVector = Character->GetCapsuleComponent()->GetForwardVector();
	FVector UpVector = Character->GetActorUpVector();
	FVector DownTraceStartPoint = ActorLocation + (UpVector * -CapsuleHalfHeight * 2.0f);
	FVector DownTraceEndPoint = DownTraceStartPoint + (UpVector * -GeneralAcrobaticActionSettings.DownTraceDistance);
	FVector ForwardTraceStartLocation = ActorLocation + (ForwardVector * 50.0f);
	FVector ForwardTraceEndLocation = ForwardTraceStartLocation + (ForwardVector * GeneralAcrobaticActionSettings.ForwardTraceDistance);

	FVector RightVector = Character->GetCapsuleComponent()->GetRightVector();

	FVector RightTraceStartLocation = ActorLocation + (RightVector * 50.0f) + (ForwardVector * -25.0f);
	FVector RightTraceEndLocation = RightTraceStartLocation + (RightVector * GeneralAcrobaticActionSettings.LateralTraceDistance);
	FVector LeftTraceStartLocation = ActorLocation + (RightVector * -50.0f) + (ForwardVector * -25.0f);
	FVector LeftTraceEndLocation = LeftTraceStartLocation + (RightVector * -GeneralAcrobaticActionSettings.LateralTraceDistance);
	
	TArray<FHitResult> ForwardOutHits;
	TArray<FHitResult> DownOutHits;
	TArray<FHitResult> LeftOutHits;
	TArray<FHitResult> RightOutHits;
	
	bool DownwardHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), DownTraceStartPoint, DownTraceEndPoint, ForwardTraceHalfSize, Character->GetActorRotation(), GeneralAcrobaticActionSettings.TraceObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, DownOutHits, true, FLinearColor::Red, FLinearColor::Green, GeneralAcrobaticActionSettings.DebugTraceDuration);
	bool Falling = Character->GetVelocity().Z < 0.0;
	bool HasEnoughSpaceForFlip = !DownwardHit || DownwardHit && !Falling;
	bool ForwardHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), ForwardTraceStartLocation, ForwardTraceEndLocation, ForwardTraceHalfSize, Character->GetActorRotation(), GeneralAcrobaticActionSettings.TraceObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, ForwardOutHits, true, FLinearColor::Red, FLinearColor::Green, GeneralAcrobaticActionSettings.DebugTraceDuration);
	bool LeftHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), LeftTraceStartLocation, LeftTraceEndLocation, LateralTraceHalfSize, Character->GetActorRotation(), GeneralAcrobaticActionSettings.TraceObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, LeftOutHits, true, FLinearColor::Red, FLinearColor::Green, GeneralAcrobaticActionSettings.DebugTraceDuration);
	bool RightHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), RightTraceStartLocation, RightTraceEndLocation, LateralTraceHalfSize, Character->GetActorRotation(), GeneralAcrobaticActionSettings.TraceObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, RightOutHits, true, FLinearColor::Red, FLinearColor::Green, GeneralAcrobaticActionSettings.DebugTraceDuration);

	if (HasEnoughSpaceForFlip && (!ForwardHit && !LeftHit && !RightHit) || Velocity < GeneralAcrobaticActionSettings.MinimumSpeedForWallJump)
	{
		AcrobaticActionType = ALSXTAcrobaticActionTypeTags::Flip;
		return;
	}

	if (HasEnoughSpaceForFlip && (LeftHit || RightHit) && Velocity < GeneralAcrobaticActionSettings.MinimumSpeedForWallRun)
	{
		AcrobaticActionType = ALSXTAcrobaticActionTypeTags::Flip;
		return;
	}
	
	if ((ForwardHit & (!RightHit && !LeftHit)) && (Velocity > GeneralAcrobaticActionSettings.MinimumSpeedForWallJump) && CanWallJump())
	{
		AcrobaticActionType = ALSXTAcrobaticActionTypeTags::WallJump;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Wall Jump");
		return;

	}
	
	if ((LeftHit || (LeftHit && ForwardHit)) && (Velocity > GeneralAcrobaticActionSettings.MinimumSpeedForWallRun) && CanWallRun())
	{
		AcrobaticActionType = ALSXTAcrobaticActionTypeTags::WallRunLeft;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Wall Run Left");
	}

	if ((RightHit || (RightHit && ForwardHit)) && (Velocity > GeneralAcrobaticActionSettings.MinimumSpeedForWallRun) && CanWallRun())
	{
		AcrobaticActionType = ALSXTAcrobaticActionTypeTags::WallRunRight;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Wall Run Right");
	}

	// TODO
	// If Wall Run (Forward and 1 Lateral Hit)
	// Check if Forward and Lateral is the same Angle

}

void UALSXTAcrobaticActionComponent::BeginFlip()
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerBeginFlip();
	}
}


void UALSXTAcrobaticActionComponent::ServerBeginFlip_Implementation()
{
	MulticastBeginFlip();
}

void UALSXTAcrobaticActionComponent::MulticastBeginFlip_Implementation()
{
	float Velocity = Character->GetVelocity().Size();
	FVector VNorm = Character->GetVelocity();
	VNorm.Normalize(0.0001f);
	float Direction =  FVector::DotProduct(Character->GetActorForwardVector(), VNorm);

	if (Direction < 0.0)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(GeneralAcrobaticActionSettings.BackflipMontage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}	
	else if (Velocity < GeneralAcrobaticActionSettings.MaximumVelocityForBackflip)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(GeneralAcrobaticActionSettings.BackflipMontage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}
	else
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(GeneralAcrobaticActionSettings.FlipMontage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}	
}

void UALSXTAcrobaticActionComponent::BeginWallJump()
{

}

void UALSXTAcrobaticActionComponent::BeginWallRun()
{

}

void UALSXTAcrobaticActionComponent::UpdateWallRun()
{

}

void UALSXTAcrobaticActionComponent::EndWallRun()
{

}
