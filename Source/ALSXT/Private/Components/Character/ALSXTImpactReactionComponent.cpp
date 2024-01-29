// MIT

#include "Components/Character/ALSXTImpactReactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "Engine/World.h"
#include "GameFrameWork/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Utility/AlsMacros.h"
#include "InputActionValue.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTCollisionInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UALSXTImpactReactionComponent::UALSXTImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void UALSXTImpactReactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CrowdNavigationPoseState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, BumpPoseState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ObstacleImpactHistory, Parameters)
}


// Called when the game starts
void UALSXTImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	if (Character)
	{
		AnimInstance = Character->GetMesh()->GetAnimInstance();
	}
	CrowdNavigationVelocityTimerDelegate.BindUFunction(this, "CrowdNavigationVelocityTimer");
	BumpVelocityTimerDelegate.BindUFunction(this, "BumpVelocityTimer");

	ImpactFallingTimerDelegate.BindUFunction(this, "ImpactFallingTimer");
	AttackFallingTimerDelegate.BindUFunction(this, "AttackFallingTimer");
	StabilizeTimerDelegate.BindUFunction(this, "StabilizeTimer");
	BraceForImpactTimerDelegate.BindUFunction(this, "BraceForImpactTimer");
	ImpactFallenTimerDelegate.BindUFunction(this, "ImpactFallenTimer");
	AttackFallenTimerDelegate.BindUFunction(this, "AttackFallenTimer");
	ClutchImpactPointTimerDelegate.BindUFunction(this, "ClutchImpactPointTimer");

	if (Character)
	{
		// CharacterCapsule = Character->GetCapsuleComponent();
		// CharacterCapsule->OnComponentHit.Add(this, &UALSXTImpactReactionComponent::OnCapsuleHit);
		// Character->OnActorHit.AddDynamic(this, &UALSXTImpactReactionComponent::OnCapsuleHit);
		// CharacterCapsule->OnComponentHit.AddDynamic(this, &UALSXTImpactReactionComponent::OnCapsuleHit);
	}
}


// Called every frame
void UALSXTImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Character->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity))
	{
		ObstacleTrace();
	}
	AnticipationTrace();
}

void UALSXTImpactReactionComponent::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != Character) && (OtherComp != NULL))
	{
		if (GEngine && ImpactReactionSettings.DebugMode) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));

		if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCharacterInterface::StaticClass()))
		{

		}

		if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCollisionInterface::StaticClass()))
		{
		
		}
	}
}

bool UALSXTImpactReactionComponent::GetImpactFallLocation(FVector& Location, FDoubleHitResult Hit)
{
	// Get Other Actors Mass and Velocity and add it to Impact Velocity to determine potential distance
	// then capsule sweep for obstacles to find actual end location
	// Calculate time to move actor to location based on length of animation 

	UAnimMontage* Montage = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallingMontages[0].Montage;
	float MontageLength = AnimInstance->Montage_Play(Montage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f);

		//if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
		//{
		//
		//}

	FVector ImpactVelocityVector = Hit.HitResult.Velocity;
	FGameplayTag ImpactVelocityTag = Hit.Strength;
	float ActorMass{ 0.0f };
	IALSXTCollisionInterface::Execute_GetActorMass(Hit.HitResult.HitResult.GetActor(), ActorMass);
	FVector ActorVelocity{ FVector::ZeroVector };
	IALSXTCollisionInterface::Execute_GetActorVelocity(Hit.HitResult.HitResult.GetActor(), ActorVelocity);
	FVector FallLocation = Character->GetActorLocation() * ImpactVelocityVector;

	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	const FVector StartLocation{ Character->GetActorLocation()};
	const FVector EndLocation{ StartLocation - UpVector * 100 };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);

	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		for (FHitResult HitResult : HitResults)
		{
			Location = HitResult.ImpactPoint;
			return true;
		}
		return true;
	}
	else
	{
		Location = FVector::ZeroVector;
		return false;
	}
}

bool UALSXTImpactReactionComponent::GetAttackFallLocation(FVector& Location, FAttackDoubleHitResult Hit)
{
	// Get Other Actors Mass and Velocity and add it to Impact Velocity to determine potential distance
	// then capsule sweep for obstacles to find actual end location
	// Calculate time to move actor to location based on length of animation 

	FFallenAnimation FallenAnimations = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations;

	if (FallenAnimations.FallingMontages.Num() > 0 && ALS_ENSURE(IsValid(GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.FallingMontages[0].Montage)))
	{
		UAnimMontage* Montage = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.FallingMontages[0].Montage;
		// float MontageLength = AnimInstance->Montage_Play(Montage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f);

		//if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
		//{
		//
		//}

		FVector ImpactVelocityVector = Hit.DoubleHitResult.HitResult.Velocity;
		FGameplayTag ImpactVelocityTag = Hit.DoubleHitResult.Strength;
		float ActorMass{ 0.0f };
		IALSXTCollisionInterface::Execute_GetActorMass(Hit.DoubleHitResult.HitResult.HitResult.GetActor(), ActorMass);
		FVector ActorVelocity{ FVector::ZeroVector };
		IALSXTCollisionInterface::Execute_GetActorVelocity(Hit.DoubleHitResult.HitResult.HitResult.GetActor(), ActorVelocity);
		FVector FallLocation = Character->GetActorLocation() * ImpactVelocityVector;
		const auto* Capsule{ Character->GetCapsuleComponent() };
		const auto CapsuleScale{ Capsule->GetComponentScale().Z };
		auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
		const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
		const FVector UpVector{ Character->GetActorUpVector() };
		const FVector ForwardVector{ Character->GetActorForwardVector() };
		const FVector StartLocation{ Character->GetActorLocation() + UpVector * 2.0f };
		const FVector PotentialLandLocation{ StartLocation - ForwardVector * 100.0f };
		TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
		BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForOneFrame;
		FHitResult HitResult;
		FHitResult DownTraceHitResult;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(Character);

		if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartLocation, PotentialLandLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
		{
			Location = HitResult.ImpactPoint;
			const FVector EndLocation{ StartLocation - UpVector * 100 };

			if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), PotentialLandLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, DownTraceHitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				if (Character->GetCharacterMovement()->IsWalkable(DownTraceHitResult))
				{
					Location = DownTraceHitResult.ImpactPoint;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			const FVector EndLocation{ PotentialLandLocation - UpVector * 100 };

			if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), PotentialLandLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, DownTraceHitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				if (Character->GetCharacterMovement()->IsWalkable(DownTraceHitResult))
				{
					Location = DownTraceHitResult.ImpactPoint;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}

		}
	}
	else
	{
		return false;
	}
	
}

float UALSXTImpactReactionComponent::GetImpactFallenMinimumTime(FDoubleHitResult Hit)
{
	return FGenericPlatformMath::Min(GetDynamicImpactFallenMinimumTime(), IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ImpactFallenMinimumTime);
}

float UALSXTImpactReactionComponent::GetAttackFallenMinimumTime(FAttackDoubleHitResult Hit)
{
	return FGenericPlatformMath::Min(GetDynamicImpactFallenMinimumTime(), IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->AttackFallenMinimumTime);
}

void UALSXTImpactReactionComponent::CrowdNavigationVelocityTimer()
{
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ Character->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.15 };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForOneFrame;
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);

	if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), Character->GetActorLocation(), Character->GetActorLocation(), CapsuleRadius, CapsuleHalfHeight, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		if (IsValid(AnimInstance))
		{
			FALSXTBumpPoseState NewCrowdNavigationPoseState = GetCrowdNavigationPoseState();
			NewCrowdNavigationPoseState.Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 5.0f), FVector2D(1.0f, 0.0f), HitResult.Distance);
			SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
		}
	}
	else
	{
		FALSXTBumpPoseState NewCrowdNavigationPoseState = GetCrowdNavigationPoseState();
		NewCrowdNavigationPoseState.Alpha = 0.0f;
		SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
		GetWorld()->GetTimerManager().ClearTimer(CrowdNavigationVelocityTimerHandle);
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			AnimInstance->Montage_Resume(AnimInstance->GetCurrentActiveMontage());
		}
	}
}

void UALSXTImpactReactionComponent::BumpVelocityTimer()
{
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ Character->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.15 };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForOneFrame;
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);

	if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), Character->GetActorLocation(), Character->GetActorLocation(), CapsuleRadius, CapsuleHalfHeight, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		if (IsValid(AnimInstance))
		{
			
			FALSXTBumpPoseState NewBumpPoseState = GetBumpPoseState();
			NewBumpPoseState.Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 5.0f), FVector2D(1.0f, 0.0f), HitResult.Distance);
			SetBumpPoseState(NewBumpPoseState);
		}
	}
	else
	{
		FALSXTBumpPoseState NewBumpPoseState = GetBumpPoseState();
		NewBumpPoseState.Alpha = 0.0f;
		SetBumpPoseState(NewBumpPoseState);
		GetWorld()->GetTimerManager().ClearTimer(BumpVelocityTimerHandle);
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			// AnimInstance->Montage_Resume(AnimInstance->GetActiveMontageInstance());
		}
	}
}

void UALSXTImpactReactionComponent::StartClutchImpactPointTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ClutchImpactPointTimerHandle, ClutchImpactPointTimerDelegate, 1.25f, false);
}

void UALSXTImpactReactionComponent::ClutchImpactPointTimer()
{
	Character->ResetDefensiveModeState();
	Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);

	if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		if (IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
		{
			// AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			// AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		}
	}
	else
	{
		if (IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
		{
			if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
			{
				ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
			}
		}
	}
}

void UALSXTImpactReactionComponent::StartImpactFallingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ImpactFallingTimerHandle, ImpactFallingTimerDelegate, 0.01f, true);
}

void UALSXTImpactReactionComponent::ImpactFallingTimer()
{
	//..
	FVector CharVel = Character->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, Character->GetVelocity().Length());
	FGameplayTag ImpactStrength = GetImpactReactionState().ImpactReactionParameters.ImpactHit.Strength;
	FVector ImpactVelocity = GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.Velocity;
	FVector EndLocation = StartLocation + CharVel * VelocityLength;

	// Predict Landing
	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
		{
			for (FHitResult HitResult : HitResults)
			{
				// Get Normal of predicted Impact surface
				// If floor use land animation
				// If wall use bump animation

				float ImpactSurfaceAngle = HitResult.ImpactNormal.CosineAngle2D(UpVector);

				if (ImpactSurfaceAngle > 45.0f || CharVelLength > 700)
				{
					BraceForImpact();
				}
				else
				{
					ImpactFallLand(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
				}
			}
		}	
}

void UALSXTImpactReactionComponent::StartAttackFallingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AttackFallingTimerHandle, AttackFallingTimerDelegate, 0.01f, true);
	// Character->StartRagdolling();
}

void UALSXTImpactReactionComponent::AttackFallingTimer()
{
	//..
	FVector CharVel = Character->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, Character->GetVelocity().Length());
	FGameplayTag ImpactStrength = GetImpactReactionState().ImpactReactionParameters.ImpactHit.Strength;
	FVector ImpactVelocity = GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.Velocity;
	FVector EndLocation = StartLocation + CharVel * VelocityLength;

	// Predict Landing
	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		for (FHitResult HitResult : HitResults)
		{
			// Get Normal of predicted Impact surface
			// If floor use land animation
			// If wall use bump animation

			float ImpactSurfaceAngle = HitResult.ImpactNormal.CosineAngle2D(UpVector);

			if (ImpactSurfaceAngle > 45.0f || CharVelLength > 700)
			{
				BraceForImpact();
			}
			else
			{
				// Character->TryStopRagdolling();
				AttackFallLand(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			}

		}
	}
}

void UALSXTImpactReactionComponent::StartStabilizeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StabilizeTimerHandle, StabilizeTimerDelegate, 0.01f, true);
}

void UALSXTImpactReactionComponent::StabilizeTimer()
{
	//..
}

void UALSXTImpactReactionComponent::StartBraceForImpactTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BraceForImpactTimerHandle, BraceForImpactTimerDelegate, 0.01f, true);
}

void UALSXTImpactReactionComponent::BraceForImpactTimer()
{
	//..
	FVector CharVel = Character->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, Character->GetVelocity().Length());
	FGameplayTag ImpactStrength = GetImpactReactionState().ImpactReactionParameters.ImpactHit.Strength;
	FVector ImpactVelocity = GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.Velocity;
	FVector EndLocation = StartLocation + CharVel * VelocityLength;

	// Predict Landing
	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		for (FHitResult HitResult : HitResults)
		{
			// Get Normal of predicted Impact surface
			// If floor use land animation
			// If wall use bump animation
			FDoubleHitResult DoubleHitResult;
			TArray<FHitResult> OriginHitResults;
			TArray<AActor*> OriginIgnoreActors;
			OriginIgnoreActors.Add(HitResult.GetActor());

			if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, OriginIgnoreActors, BumpDebugMode, OriginHitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				for (FHitResult OriginHitResult : OriginHitResults)
				{
					if (OriginHitResult.GetActor() == HitResult.GetActor())
					{
						FExtendedHitResult ExtendedHitResult;
						FExtendedHitResult OriginExtendedHitResult;
						ExtendedHitResult.HitResult = HitResult;
						OriginExtendedHitResult.HitResult = OriginHitResult;
						DoubleHitResult.HitResult = ExtendedHitResult;
						DoubleHitResult.OriginHitResult = OriginExtendedHitResult;
						float ImpactSurfaceAngle = HitResult.ImpactNormal.CosineAngle2D(UpVector);

						if (ImpactSurfaceAngle > 45.0f || CharVelLength > 700)
						{
							ImpactReaction(DoubleHitResult);
						}
						else
						{
							ImpactFallLand(DoubleHitResult);
						}
					}
				}
			}
			
		}
	}
}

void UALSXTImpactReactionComponent::StartImpactFallenTimer(FDoubleHitResult Hit)
{
	float FallenMinTime = GetImpactFallenMinimumTime(Hit);
	FALSXTImpactReactionState Params = GetImpactReactionState();
	Params.ImpactReactionParameters.FallenMinimumTime = FallenMinTime;
	Params.ImpactReactionParameters.CurrentFallenTime = FallenMinTime;
	SetImpactReactionState(Params);

	GetWorld()->GetTimerManager().SetTimer(ImpactFallenTimerHandle, ImpactFallenTimerDelegate, 0.25f, true);
}

void UALSXTImpactReactionComponent::ImpactFallenTimer()
{
	float CurrentTime = GetImpactReactionState().ImpactReactionParameters.CurrentFallenTime;
	if (CurrentTime > 0.0f )
	{
		FALSXTImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.CurrentFallenTime = FMath::Clamp(Params.ImpactReactionParameters.CurrentFallenTime - 0.25, 0.0f, 100.f);
		SetImpactReactionState(Params);
	}
	else
	{
		FALSXTImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.FallenMinimumTime = 0.0f;
		Params.ImpactReactionParameters.CurrentFallenTime = 0.0f;
		SetImpactReactionState(Params);
		GetWorld()->GetTimerManager().ClearTimer(ImpactFallenTimerHandle);

		if (ImpactReactionSettings.bEnableAutoGetUp)
		{
			ImpactGetUp(Params.ImpactReactionParameters.ImpactHit);
		}
	}
}

void UALSXTImpactReactionComponent::StartAttackFallenTimer(FAttackDoubleHitResult Hit)
{
	float FallenMinTime = GetAttackFallenMinimumTime(Hit);
	FALSXTImpactReactionState Params = GetImpactReactionState();
	Params.ImpactReactionParameters.FallenMinimumTime = 8.0f;
	Params.ImpactReactionParameters.CurrentFallenTime = 8.0f;
	SetImpactReactionState(Params);

	GetWorld()->GetTimerManager().SetTimer(AttackFallenTimerHandle, AttackFallenTimerDelegate, 0.25f, true);
}

void UALSXTImpactReactionComponent::AttackFallenTimer()
{
	float CurrentTime = GetImpactReactionState().ImpactReactionParameters.CurrentFallenTime;
	RefreshAttackFallReaction(Character->GetWorld()->GetDeltaSeconds());
	if (CurrentTime > 0.0f)
	{
		FALSXTImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.CurrentFallenTime = FMath::Clamp(Params.ImpactReactionParameters.CurrentFallenTime - 0.25, 0.0f, 100.f);
		SetImpactReactionState(Params);
	}
	else
	{
		FALSXTImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.FallenMinimumTime = 0.0f;
		Params.ImpactReactionParameters.CurrentFallenTime = 0.0f;
		SetImpactReactionState(Params);
		GetWorld()->GetTimerManager().ClearTimer(AttackFallenTimerHandle);

		if (ImpactReactionSettings.bEnableAutoGetUp && IALSXTCollisionInterface::Execute_CanGetUp(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldGetUp(GetOwner()))
		{
			Character->SetDesiredStatus(ALSXTStatusTags::Normal);
			AttackGetUp(Params.ImpactReactionParameters.AttackHit);
			// AttackGetUp(GetLastAttackImpact());
			// ServerGetUp();
		}
	}
}

void UALSXTImpactReactionComponent::OnCrowdNavigationReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
	{
		CrowdNavigationFall();
	}
}

void UALSXTImpactReactionComponent::OnBumpReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	// GetWorld()->GetTimerManager().ClearTimer(BumpVelocityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BumpVelocityTimerHandle, BumpVelocityTimerDelegate, 0.1f, true);
	if (IALSXTCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
	{
		CrowdNavigationFall();
	}
}

void UALSXTImpactReactionComponent::OnImpactReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldImpactFall(GetOwner()) && IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
	{
		Character->ResetDefensiveModeState();
		Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
		ImpactFall(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	}
}

void UALSXTImpactReactionComponent::OnAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	
	if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		if (IALSXTCollisionInterface::Execute_CanAttackFall(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
		{
			Character->ResetDefensiveModeState();
			Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
			AttackFall(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		}
		else
		{
			if (IALSXTCollisionInterface::Execute_ShouldStabilize(GetOwner()))
			{
				Stabilize(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
			}
			if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
			{
				ClutchImpactPoint(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
			}
			else if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.OriginHitResult.HitResult.GetActor()) && IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
			{
				StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			}
			else
			{
				return;
			}			
		}
	}
}

void UALSXTImpactReactionComponent::OnSyncedAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldSyncedAttackFall(GetOwner()))
	{

	}
}

void UALSXTImpactReactionComponent::OnClutchImpactPointBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		if (IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
		{
			AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		}
	}
	else
	{
		if (IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
		{
			if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
			{
				ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
			}
		}
	}
}
void UALSXTImpactReactionComponent::OnCrowdNavigationFallBlendOut(UAnimMontage* Montage, bool bInterrupted){}
void UALSXTImpactReactionComponent::OnImpactFallBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	ImpactFallIdle(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	// StartImpactFallenTimer();
}

void UALSXTImpactReactionComponent::OnAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	AttackFallIdle(GetImpactReactionState().ImpactReactionParameters.AttackHit);
	StartAttackFallenTimer(GetLastAttackImpact());
}

void UALSXTImpactReactionComponent::OnSyncedAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted){}
void UALSXTImpactReactionComponent::OnBraceForImpactBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UALSXTImpactReactionComponent::OnCrowdNavigationFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldPerformCrowdNavigationResponse(GetOwner()))
	{
		CrowdNavigationResponse();
	}
}

void UALSXTImpactReactionComponent::OnImpactFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
	{
		ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	}
}

void UALSXTImpactReactionComponent::OnAttackFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
	{
		// AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
	}
}

void UALSXTImpactReactionComponent::OnSyncedAttackFallGetUpBlendOut(UAnimMontage* Montage, bool bInterrupted){}
void UALSXTImpactReactionComponent::OnImpactResponseBlendOut(UAnimMontage* Montage, bool bInterrupted){}
void UALSXTImpactReactionComponent::OnAttackResponseBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UALSXTImpactReactionComponent::OnCrowdNavigationReactionEnded(UAnimMontage* Montage, bool bInterrupted) 
{
	if (AnimInstance)
	{
		OnCrowdNavigationReactionBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnBumpReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnBumpReactionBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnImpactReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactReactionBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackReactionBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnSyncedAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackReactionBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnClutchImpactPointEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnClutchImpactPointBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnCrowdNavigationFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnCrowdNavigationFallBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnImpactFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactFallBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnAttackFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackFallBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnSyncedAttackFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackFallBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnBraceForImpactEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnBraceForImpactBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnCrowdNavigationFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnCrowdNavigationFallGetupBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnImpactFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactFallGetupBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnAttackFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackFallGetupBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnSyncedAttackFallGetUpEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackFallGetUpBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnImpactResponseEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactResponseBlendOutDelegate.Unbind();
	}
}
void UALSXTImpactReactionComponent::OnAttackResponseEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackResponseBlendOutDelegate.Unbind();
	}
}

FGameplayTag UALSXTImpactReactionComponent::GetCharacterVelocity()
{
	if (Character->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		if (Character->GetVelocity().Length() < 175)
		{
			return ALSXTImpactVelocityTags::Slow;
		}
		else if (Character->GetVelocity().Length() >= 175 && Character->GetVelocity().Length() < 350)
		{
			return ALSXTImpactVelocityTags::Moderate;
		}
		else if (Character->GetVelocity().Length() >= 350 && Character->GetVelocity().Length() < 650)
		{
			return ALSXTImpactVelocityTags::Fast;
		}
		else if (Character->GetVelocity().Length() >= 650 && Character->GetVelocity().Length() < 800)
		{
			return ALSXTImpactVelocityTags::Faster;
		}
		else if (Character->GetVelocity().Length() >= 800)
		{
			return ALSXTImpactVelocityTags::TerminalVelocity;
		}
		else
		{
			return FGameplayTag::EmptyTag;
		}
	}
	else
	{
		FGameplayTag CharacterGait = Character->GetDesiredGait();
		if (CharacterGait == AlsGaitTags::Walking)
		{
			return ALSXTImpactVelocityTags::Slow;
		}
		else if (CharacterGait == AlsGaitTags::Walking)
		{
			return ALSXTImpactVelocityTags::Moderate;
		}
		else
		{
			return ALSXTImpactVelocityTags::Fast;
		}
	}
}

bool UALSXTImpactReactionComponent::ShouldRecieveVelocityDamage()
{
	return Character->GetVelocity().Length() >= 650;
}

float UALSXTImpactReactionComponent::GetBaseVelocityDamage()
{
	FVector2D VelocityRange{ 650.0, 2000.0 };
	FVector2D ConversionRange{ 0.0, 100.0 };
	return FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, Character->GetVelocity().Length());
}

bool UALSXTImpactReactionComponent::ValidateNewHit(AActor* ActorToCheck)
{
	double NewHitTime;
	NewHitTime = GetWorld()->GetTimeSeconds();
	bool RecentlyHit{ false };
	FImpactHistoryEntry EntryToCheck;
	EntryToCheck.Actor = ActorToCheck;

	if (ObstacleImpactHistory.Contains(EntryToCheck)) //Actor Already in History
	{
		double PreviousTime;
		int FoundIndex = ObstacleImpactHistory.Find(EntryToCheck);
		FImpactHistoryEntry FoundEntry = ObstacleImpactHistory[FoundIndex];
		PreviousTime = FoundEntry.Time;
		double TimeResult;
		TimeResult = NewHitTime - PreviousTime;
		RecentlyHit = TimeResult <= 0.33;

		if (GEngine && ImpactReactionSettings.DebugMode) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString((RecentlyHit)? TEXT("True"):TEXT("False")));
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::SanitizeFloat(NewHitTime));
		if (GEngine && ImpactReactionSettings.DebugMode) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(TimeResult));
		ObstacleImpactHistory.RemoveAt(FoundIndex);		
		if (ObstacleImpactHistory.Num() >= 6)
		{
			ObstacleImpactHistory.RemoveAt(0);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, GetWorld()->GetTimeSeconds() };
		ObstacleImpactHistory.Add(NewEntry);
		return !RecentlyHit;
	}
	else // Actor not in History. Add New Element
	{
		if (ObstacleImpactHistory.Num() >= 6)
		{
			ObstacleImpactHistory.RemoveAt(0);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, GetWorld()->GetTimeSeconds() };
		ObstacleImpactHistory.Add(NewEntry);
		if (GEngine && ImpactReactionSettings.DebugMode) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::SanitizeFloat(NewHitTime));
		return true;
	}
}

void UALSXTImpactReactionComponent::ObstacleTrace()
{
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2)};
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);
	float VelocityLength{ 0.0f };
	float TraceDistance {0.0f};
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	FVector RangedVelocity = Character->GetVelocity();
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, Character->GetVelocity().Length());

	if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()) && Character->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity))
	{
		if (Character->GetLocomotionAction() == AlsLocomotionActionTags::Sliding)
		{
			CapsuleRadius = CapsuleRadius * 2.0;
			TraceDistance = ImpactReactionSettings.MaxSlideToCoverDetectionDistance;
		}
		else if (!Character->GetMesh()->IsPlayingNetworkedRootMotionMontage())
		{
			TraceDistance = ImpactReactionSettings.MaxBumpDetectionDistance;
		}
		const FVector EndLocation{ StartLocation + (Character->GetVelocity() * TraceDistance) };

		if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
		{
			for (FHitResult HitResult : HitResults)
			{
				FHitResult OriginHitResult;
				TArray<AActor*> IgnoreActorsOrigin;
				IgnoreActorsOrigin.Add(HitResult.GetActor());

				if (ValidateNewHit(HitResult.GetActor()))
				{
					if (HitResult.GetComponent()->GetOwner() != GetOwner() && UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), HitResult.ImpactPoint, StartLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActorsOrigin, EDrawDebugTrace::None, OriginHitResult, false, FLinearColor::Green, FLinearColor::Red, 5.0f))
					{
						FALSXTImpactReactionState NewImpactReactionState;
						FDoubleHitResult DoubleHitResult;
						DoubleHitResult.HitResult.HitResult = HitResult;
						DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
						DoubleHitResult.OriginHitResult.ImpactGait = Character->GetDesiredGait();
						DoubleHitResult.HitResult.ImpactSide = LocationToActorImpactSide(HitResult.GetActor(), HitResult.ImpactPoint);

						if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
						{
							IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), DoubleHitResult.HitResult.Mass);
							IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), DoubleHitResult.HitResult.Velocity);
						}
						else
						{
							DoubleHitResult.HitResult.Mass = 100.00f;
							DoubleHitResult.HitResult.Velocity = HitResult.GetActor()->GetVelocity();
						}

						DoubleHitResult.Strength = ConvertVelocityToStrength(DoubleHitResult.HitResult.Velocity);
						FGameplayTag SideTag = LocationToImpactSide(HitResult.ImpactPoint);
						FGameplayTag OriginSideTag = LocationToImpactSide(OriginHitResult.ImpactPoint);
						TEnumAsByte<EPhysicalSurface> OriginPhysSurf = OriginHitResult.PhysMaterial->SurfaceType;
						TEnumAsByte<EPhysicalSurface> PhysSurf = HitResult.PhysMaterial->SurfaceType;
						FGameplayTag FormTag = ConvertPhysicalSurfaceToFormTag(OriginPhysSurf);
						DoubleHitResult.ImpactForm = FormTag;
						DoubleHitResult.ImpactSide = SideTag;
						IALSXTCollisionInterface::Execute_GetActorMass(Character, DoubleHitResult.OriginHitResult.Mass);
						IALSXTCollisionInterface::Execute_GetActorVelocity(Character, DoubleHitResult.OriginHitResult.Velocity);
						// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;

						if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCharacterInterface::StaticClass()) && IALSXTCharacterInterface::Execute_GetCombatStance(HitResult.GetActor()) == ALSXTCombatStanceTags::Neutral)
						{
							if (Character->GetVelocity().Length() < FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity))
							{
								// Use Static Pose instead
								FALSXTBumpPoseState NewCrowdNavigationPoseState;
								NewCrowdNavigationPoseState.Pose = SelectCrowdNavigationPose(DoubleHitResult.ImpactSide, DoubleHitResult.ImpactForm);
								SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);

								// Set Physical Animation Component Curves/Profile Here
								Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Bump, DoubleHitResult.HitResult.HitResult.BoneName);
							}
							else if ((Character->GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral && IALSXTCollisionInterface::Execute_ShouldPerformCrowdNavigationReaction(GetOwner())) || (Character->GetVelocity().Length() >= 650.0f && IALSXTCollisionInterface::Execute_ShouldPerformCrowdNavigationReaction(GetOwner())))
							{
								CrowdNavigationReaction(Character->GetDesiredGait(), OriginSideTag, FormTag);

								// Set Physical Animation Component Curves/Profile Here
								Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Bump, DoubleHitResult.HitResult.HitResult.BoneName);
							}
							NewImpactReactionState.ImpactReactionParameters.CrowdNavigationHit = DoubleHitResult;
							NewImpactReactionState.ImpactReactionParameters.ImpactType = ALSXTImpactTypeTags::CrowdNavigation;
							SetImpactReactionState(NewImpactReactionState);
							IALSXTCharacterInterface::Execute_CrowdNavigationReaction(HitResult.GetActor(), Character->GetDesiredGait(), DoubleHitResult, SideTag, FormTag);
						}
						else
						{
							NewImpactReactionState.ImpactReactionParameters.BumpHit = DoubleHitResult;
							NewImpactReactionState.ImpactReactionParameters.ImpactType = ALSXTImpactTypeTags::Bump;
							SetImpactReactionState(NewImpactReactionState);

							if (ImpactReactionSettings.DebugMode)
							{
								FString VelMsg = "Vel: ";
								VelMsg.Append(FString::SanitizeFloat(Character->GetVelocity().Length()));
								GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, VelMsg);
							}

							if (Character->GetVelocity().Length() < ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity)
							{
								// Use Static Pose instead
								FALSXTBumpPoseState NewBumpPoseState;
								NewBumpPoseState.Pose = SelectBumpPose(DoubleHitResult.ImpactSide, DoubleHitResult.ImpactForm);
								SetBumpPoseState(NewBumpPoseState);

								// Set Physical Animation Component Curves/Profile Here
								Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, DoubleHitResult.HitResult.HitResult.BoneName);

							}
							else if ((Character->GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral  && ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity) || (Character->GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral && Character->GetVelocity().Length() >= 650.0f))
							{
								BumpReaction(Character->GetDesiredGait(), SideTag, FormTag);

								// Set Physical Animation Component Curves/Profile Here
								Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, DoubleHitResult.HitResult.HitResult.BoneName);
							}

							if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
							{
								IALSXTCollisionInterface::Execute_ActorBumpCollision(HitResult.GetActor(), DoubleHitResult);
							}
						}
					}

					if (ImpactReactionSettings.DebugMode)
					{
						FString BumpHit = "Bump: ";
						BumpHit.Append(HitResult.GetActor()->GetName());
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BumpHit);
					}
				}							
			}						
		}
	}
	else
	{
		TraceDistance = 0.0f;
		if (Character->GetPhysicalAnimationMode() != ALSXTPhysicalAnimationModeTags::None)
		{
			Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "head");
		}
		return;
	}
}

void UALSXTImpactReactionComponent::AnticipationTrace()
{
	const auto* Capsule{ Character->GetCapsuleComponent() };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ Character->GetActorUpVector() };
	float TraceDistance{ 100.0f };
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	const FVector EndLocation{ StartLocation + (Character->GetActorForwardVector() * TraceDistance) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	// BumpDebugMode = EDrawDebugTrace::ForOneFrame;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);

	if (Character->GetDefensiveModeState().Mode != ALSXTDefensiveModeTags::ClutchImpactPoint)
	{
		bool isHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ImpactReactionSettings.AnticipationAreaHalfSize, Character->GetControlRotation(), ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f);
		if (isHit)
		{
			for (FHitResult HitResult : HitResults)
			{
				if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCharacterInterface::StaticClass()))
				{
					FVector ActorVelocity{ FVector::ZeroVector };
					float ActorMass{ 0.0f };
					FGameplayTag Velocity{ FGameplayTag::EmptyTag };
					FGameplayTag Form{ FGameplayTag::EmptyTag };
					FVector AnticipationPoint{ FVector::ZeroVector };
					FALSXTDefensiveModeState DefensiveModeState;
					FAnticipationPose Montage;
					FGameplayTag CharacterCombatStance = IALSXTCharacterInterface::Execute_GetCombatStance(HitResult.GetActor());
					IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
					IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);
					IALSXTCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationPoint);

					FGameplayTag DefensiveMode = IALSXTCombatInterface::Execute_Attacking(HitResult.GetActor()) ? DetermineDefensiveModeFromAttackingCharacter(Form, CharacterCombatStance) : DetermineDefensiveModeFromCharacter(Form, CharacterCombatStance);


					FGameplayTag Stance = Character->GetDesiredStance();
					FGameplayTag Side = LocationToImpactSide(AnticipationPoint);
					FGameplayTag Health = HealthToHealthTag(GetHealth());

					if (DefensiveMode == ALSXTDefensiveModeTags::Anticipation)
					{
						Montage = SelectAttackAnticipationMontage(CharacterCombatStance, Velocity, Stance, Side, Form, Health);
					}
					else
					{
						if (Character->GetDesiredStatus() == ALSXTStatusTags::Normal && DefensiveMode == ALSXTDefensiveModeTags::Blocking)
						{
							Montage = SelectDefensiveMontage(Velocity, Stance, Side, Form, Health);
						}
					}

					DefensiveModeState.Mode = DefensiveMode;
					DefensiveModeState.Montage = Montage.Pose;
					DefensiveModeState.Location = AnticipationPoint;
					Character->SetDefensiveModeState(DefensiveModeState);
					Character->SetDesiredDefensiveMode(DefensiveMode);
					return;
				}
				else
				{
					if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
					{
						FVector ActorVelocity{ FVector::ZeroVector };
						float ActorMass{ 0.0f };
						FGameplayTag Velocity{ FGameplayTag::EmptyTag };
						FGameplayTag Form{ FGameplayTag::EmptyTag };
						FVector AnticipationPoint{ FVector::ZeroVector };
						FALSXTDefensiveModeState DefensiveModeState = Character->GetDefensiveModeState();
						FAnticipationPose Montage;
						IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
						IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);
						IALSXTCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationPoint);
						FGameplayTag DefensiveMode = DetermineDefensiveMode(Form);
						FGameplayTag Stance = Character->GetDesiredStance();
						FGameplayTag Side = LocationToImpactSide(AnticipationPoint);
						FGameplayTag Health = HealthToHealthTag(GetHealth());

						if (DefensiveMode == ALSXTDefensiveModeTags::Anticipation)
						{
							Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
						}
						if (DefensiveMode == ALSXTDefensiveModeTags::Blocking)
						{
							Montage = SelectDefensiveMontage(Velocity, Stance, Side, Form, Health);
						}

						DefensiveModeState.Mode = DefensiveMode;
						DefensiveModeState.Montage = Montage.Pose;
						DefensiveModeState.Location = AnticipationPoint;
						Character->SetDefensiveModeState(DefensiveModeState);
						Character->SetDesiredDefensiveMode(DefensiveMode);
						return;
					}
				}
			}
		}
		else
		{
			if (!Character->IsBlocking() || Character->GetDefensiveModeState().Mode != ALSXTDefensiveModeTags::ClutchImpactPoint)
			{
				Character->ResetDefensiveModeState();
				Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
			}

		}
	}
}

void UALSXTImpactReactionComponent::ServerSetDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	Character->SetDefensiveModeState(NewDefensiveModeState);
}

// ImpactReaction State
void UALSXTImpactReactionComponent::SetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState)
{
	const auto PreviousImpactReactionState{ ImpactReactionState };

	ImpactReactionState = NewImpactReactionState;

	OnImpactReactionStateChanged(PreviousImpactReactionState);

	if ((Character->GetLocalRole() == ROLE_AutonomousProxy) && Character->IsLocallyControlled())
	{
		ServerSetImpactReactionState(NewImpactReactionState);
	}
}

void UALSXTImpactReactionComponent::ServerSetImpactReactionState_Implementation(const FALSXTImpactReactionState& NewImpactReactionState)
{
	SetImpactReactionState(NewImpactReactionState);
}

void UALSXTImpactReactionComponent::ServerProcessNewImpactReactionState_Implementation(const FALSXTImpactReactionState& NewImpactReactionState)
{
	ProcessNewImpactReactionState(NewImpactReactionState);
}

void UALSXTImpactReactionComponent::OnReplicate_ImpactReactionState(const FALSXTImpactReactionState& PreviousImpactReactionState)
{
	OnImpactReactionStateChanged(PreviousImpactReactionState);
}

FDoubleHitResult UALSXTImpactReactionComponent::GetLastImpact() const
{
	return PreviousImpacts.Last();
}

FAttackDoubleHitResult UALSXTImpactReactionComponent::GetLastAttackImpact() const
{
	return PreviousAttackImpacts.Last();
}

void UALSXTImpactReactionComponent::OnImpactReactionStateChanged_Implementation(const FALSXTImpactReactionState& PreviousImpactReactionState) {}

// CrowdNavigationPose State
void UALSXTImpactReactionComponent::SetCrowdNavigationPoseState(const FALSXTBumpPoseState& NewCrowdNavigationPoseState)
{
	const auto PreviousCrowdNavigationPoseState{ CrowdNavigationPoseState };

	CrowdNavigationPoseState = NewCrowdNavigationPoseState;

	OnCrowdNavigationPoseStateChanged(PreviousCrowdNavigationPoseState);

	if ((Character->GetLocalRole() == ROLE_AutonomousProxy) && Character->IsLocallyControlled())
	{
		ServerSetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
	}
}

void UALSXTImpactReactionComponent::ServerSetCrowdNavigationPoseState_Implementation(const FALSXTBumpPoseState& NewCrowdNavigationPoseState)
{
	SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
}

void UALSXTImpactReactionComponent::ServerProcessNewCrowdNavigationPoseState_Implementation(const FALSXTBumpPoseState& NewCrowdNavigationPoseState)
{
	ProcessNewCrowdNavigationPoseState(NewCrowdNavigationPoseState);
}

void UALSXTImpactReactionComponent::OnReplicate_CrowdNavigationPoseState(const FALSXTBumpPoseState& PreviousCrowdNavigationPoseState)
{
	OnCrowdNavigationPoseStateChanged(PreviousCrowdNavigationPoseState);
}

void UALSXTImpactReactionComponent::OnCrowdNavigationPoseStateChanged_Implementation(const FALSXTBumpPoseState& PreviousCrowdNavigationPoseState) {}

// BumpPose State
void UALSXTImpactReactionComponent::SetBumpPoseState(const FALSXTBumpPoseState& NewBumpPoseState)
{
	const auto PreviousBumpPoseState{ BumpPoseState };

	BumpPoseState = NewBumpPoseState;

	OnBumpPoseStateChanged(PreviousBumpPoseState);

	if ((Character->GetLocalRole() == ROLE_AutonomousProxy) && Character->IsLocallyControlled())
	{
		ServerSetBumpPoseState(NewBumpPoseState);
	}
}

void UALSXTImpactReactionComponent::ServerSetBumpPoseState_Implementation(const FALSXTBumpPoseState& NewBumpPoseState)
{
	SetBumpPoseState(NewBumpPoseState);
}

void UALSXTImpactReactionComponent::ServerProcessNewBumpPoseState_Implementation(const FALSXTBumpPoseState& NewBumpPoseState)
{
	ProcessNewBumpPoseState(NewBumpPoseState);
}

void UALSXTImpactReactionComponent::OnReplicate_BumpPoseState(const FALSXTBumpPoseState& PreviousBumpPoseState)
{
	OnBumpPoseStateChanged(PreviousBumpPoseState);
}

void UALSXTImpactReactionComponent::OnReplicate_ObstacleImpactHistory(const TArray<FImpactHistoryEntry> PreviousObstacleImpactHistory)
{
	// OnBumpPoseStateChanged(PreviousBumpPoseState);
}

void UALSXTImpactReactionComponent::OnBumpPoseStateChanged_Implementation(const FALSXTBumpPoseState& PreviousBumpPoseState) {}

// ENTRY FUNCTIONS

// Defensive Reaction
void UALSXTImpactReactionComponent::DefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	// ...
}

void UALSXTImpactReactionComponent::SyncedAnticipationReaction(FVector AnticipationPoint)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAnticipationReaction(AnticipationPoint);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAnticipationReaction(AnticipationPoint);
	}
}

// Crowd Navigation Reaction
void UALSXTImpactReactionComponent::CrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerCrowdNavigationReaction(Gait, Side, Form);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartCrowdNavigationReaction(Gait, Side, Form);
	}
}

// Bump Reaction
void UALSXTImpactReactionComponent::BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerBumpReaction(Gait, Side, Form);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartBumpReaction(Gait, Side, Form);
	}
}

// Impact Reaction
void UALSXTImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);

	// if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerImpactReaction(Hit);
	// }
	// else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	// {
	// 	StartImpactReaction(Hit);
	// }
}

void UALSXTImpactReactionComponent::AttackReaction(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackReaction(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackReaction(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackReaction(int Index)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackReaction(Index);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackReaction(Index);
	}
}

void UALSXTImpactReactionComponent::Stabilize(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerStabilize(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartStabilize(Hit);
	}
}

void UALSXTImpactReactionComponent::ClutchImpactPoint(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerClutchImpactPoint(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartClutchImpactPoint(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFall(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFall(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFall(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFallLand(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFallLand(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFallLand(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFallIdle(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFallIdle(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFallIdle(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFall(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFall(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		//MulticastAttackFall(Hit);
		StartAttackFall(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFallLand(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFallLand(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackFallLand(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFallIdle(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFallIdle(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackFallIdle(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackFall(int32 Index)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackFall(Index);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackFall(Index);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackFallIdle(int32 Index)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackFallIdle(Index);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackFallIdle(Index);
	}
}

void UALSXTImpactReactionComponent::BraceForImpact()
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerBraceForImpact();
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartBraceForImpact();
	}
}

void UALSXTImpactReactionComponent::ImpactGetUp(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactGetUp(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactGetUp(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackGetUp(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackGetUp(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackGetUp(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackGetUp(int32 Index)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackGetUp(Index);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackGetUp(Index);
	}
}

void UALSXTImpactReactionComponent::CrowdNavigationResponse()
{
	// if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerCrowdNavigation();
	// }
	// else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	// {
	// 	StartCrowdNavigation();
	// }
}

void UALSXTImpactReactionComponent::ImpactResponse(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactResponse(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactResponse(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackResponse(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackResponse(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackResponse(Hit);
		// MulticastAttackResponse(Hit);
		// ServerAttackResponse(Hit);
	}
}

void UALSXTImpactReactionComponent::BodyFallReaction(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactReaction(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactReaction(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactTimelineUpdate(float Value)
{
	//...
}

// Error Checks
bool UALSXTImpactReactionComponent::IsSyncedAnticipationReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsDefensiveReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsCrowdNavigationReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsBumpReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsAttackReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsSyncedAttackReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsClutchImpactPointAllowedToStart(const UAnimSequenceBase* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsCrowdNavigationFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsImpactFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsAttackFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsSyncedAttackFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsImpactResponseAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsAttackResponseAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

// Start Events
void UALSXTImpactReactionComponent::StartSyncedAnticipationReaction(FVector AnticipationPoint)
{
	// ...
}

void UALSXTImpactReactionComponent::StartDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	// ...
}

void UALSXTImpactReactionComponent::StartBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	FBumpReactionAnimation SelectedBumpReaction = SelectBumpReactionMontage(Gait, Side, Form);
	UAnimMontage* Montage = SelectedBumpReaction.Montage.Montage;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsBumpReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	TSubclassOf<AActor> ParticleActor = GetImpactReactionParticleActor(GetImpactReactionState().ImpactReactionParameters.BumpHit);
	UNiagaraSystem* Particle = GetImpactReactionParticle(GetImpactReactionState().ImpactReactionParameters.BumpHit);
	USoundBase* Audio = GetImpactReactionSound(GetImpactReactionState().ImpactReactionParameters.BumpHit).Sound.Sound;
	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartBumpReaction(SelectedBumpReaction.Montage, ParticleActor, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartBumpReaction(SelectedBumpReaction.Montage, ParticleActor, Particle, Audio);
		OnBumpReactionStarted();
	}
}

void UALSXTImpactReactionComponent::StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FALSXTImpactReactionState IRState = GetImpactReactionState();
	FBumpReactionAnimation SelectedCrowdNavigationReaction = SelectCrowdNavigationReactionMontage(IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactGait, IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactSide, IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactForm);
	UAnimMontage* Montage = SelectedCrowdNavigationReaction.Montage.Montage;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	USoundBase* Audio = GetImpactReactionSound(IRState.ImpactReactionParameters.CrowdNavigationHit).Sound.Sound;
	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartCrowdNavigationReaction(SelectedCrowdNavigationReaction.Montage, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartCrowdNavigationReaction(SelectedCrowdNavigationReaction.Montage, Audio);
		OnCrowdNavigationReactionStarted();
	}
}

void UALSXTImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage{ nullptr };
	TSubclassOf<AActor> ParticleActor = GetImpactReactionParticleActor(Hit);
	UNiagaraSystem* Particle{ nullptr };
	USoundBase* Audio{ nullptr };

	FImpactReactionAnimation ImpactReactionAnimation = SelectImpactReactionMontage(Hit);
	Montage = ImpactReactionAnimation.Montage.Montage;
	Particle = GetImpactReactionParticle(Hit);
	Audio = GetImpactReactionSound(Hit).Sound.Sound;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	Character->SetMovementModeLocked(true);
	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	ImpactReactionParameters.ImpactHit = Hit;
	ImpactReactionParameters.ImpactHit = Hit;
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackReaction.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	ImpactReactionParameters.ImpactType = Hit.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	FALSXTImpactReactionState NewImpactReactionState;
	NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	SetImpactReactionState(NewImpactReactionState);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		MulticastStartImpactReaction(Hit, Montage, ParticleActor, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartImpactReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
		ServerStartImpactReaction(Hit, Montage, ParticleActor, Particle, Audio);
		OnImpactReactionStarted(Hit);
	}
}

void UALSXTImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	if (IsValid(Hit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		UAnimMontage* Montage{ nullptr };
		FAttackReactionAnimation SelectedAttackReaction = SelectAttackReactionMontage(Hit);
		Montage = SelectedAttackReaction.Montage.Montage;
		PreviousAttackImpacts.Add(Hit);

		if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
			return;
		}

		UNiagaraSystem* Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
		TSubclassOf<AActor> ParticleActor = GetImpactReactionParticleActor(Hit.DoubleHitResult);
		USoundBase* Audio = GetImpactReactionSound(Hit.DoubleHitResult).Sound.Sound;
		const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

		Character->SetMovementModeLocked(true);

		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
			// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
		NewImpactReactionState.ImpactReactionParameters.AttackHit = Hit;
		NewImpactReactionState.ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
		NewImpactReactionState.ImpactReactionParameters.PlayRate = SelectedAttackReaction.Montage.PlayRate;
		// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		NewImpactReactionState.ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
		// ImpactReactionParameters.Stance = Stance;
		NewImpactReactionState.ImpactReactionParameters.ImpactVelocity = Hit.Strength;
		NewImpactReactionState.ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
		NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
		SetImpactReactionState(NewImpactReactionState);

		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

		if (Character->GetLocalRole() >= ROLE_Authority)
		{
			ServerStartAttackReaction(Hit, SelectedAttackReaction.Montage, ParticleActor, Particle, Audio);
		}
		else
		{
			Character->GetCharacterMovement()->FlushServerMoves();
			MulticastStartAttackReaction(Hit, SelectedAttackReaction.Montage, ParticleActor, Particle, Audio);
			OnAttackReactionStarted(Hit);
		}
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackReaction(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartStabilize(FDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	UAnimSequenceBase* Montage;
	FAnticipationPose SelectedClutchImpactPointReaction = SelectStablizationPose(Hit);
	Montage = SelectedClutchImpactPointReaction.Pose;
	FALSXTDefensiveModeState DefensiveModeState;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsClutchImpactPointAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}
	FGameplayTag Health = HealthToHealthTag(GetHealth());
	DefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
	DefensiveModeState.Montage = Montage;
	DefensiveModeState.Location = Hit.HitResult.HitResult.ImpactPoint;
	Character->SetDefensiveModeState(DefensiveModeState);
	Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::ClutchImpactPoint);
	StartClutchImpactPointTimer();

	// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };
	// 
	// // StartClutchImpactPointImplementation(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// 
	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	ServerStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// }
	// else
	// {
	// 	Character->GetCharacterMovement()->FlushServerMoves();
	// 	MulticastStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// 	OnImpactReactionStarted(Hit);
	// }
}

void UALSXTImpactReactionComponent::StartClutchImpactPoint(FDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	UAnimSequenceBase* Montage;
	FClutchImpactLocationAnimation SelectedClutchImpactPointReaction = SelectClutchImpactPointMontage(Hit);
	Montage = SelectedClutchImpactPointReaction.Pose;
	FALSXTDefensiveModeState DefensiveModeState;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsClutchImpactPointAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}
	FGameplayTag Health = HealthToHealthTag(GetHealth());
	DefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
	DefensiveModeState.Montage = Montage;
	DefensiveModeState.Location = Hit.HitResult.HitResult.ImpactPoint;
	Character->SetDefensiveModeState(DefensiveModeState);
	Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::ClutchImpactPoint);
	StartClutchImpactPointTimer();

	// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };
	// 
	// // StartClutchImpactPointImplementation(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// 
	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	ServerStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// }
	// else
	// {
	// 	Character->GetCharacterMovement()->FlushServerMoves();
	// 	MulticastStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
	// 	OnImpactReactionStarted(Hit);
	// }
}

void UALSXTImpactReactionComponent::StartImpactFall(FDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FFallenAnimation FallenAnimations = SelectImpactFallAnimations(Hit);
	FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
	NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations = FallenAnimations;
	NewImpactReactionState.ImpactReactionParameters.FallType = ALSXTFallTypeTags::Impact;
	SetImpactReactionState(NewImpactReactionState);

	FActionMontageInfo Montage{ nullptr };
	FActionMontageInfo SelectedAttackFall = SelectImpactFallMontage(Hit);
	Montage = SelectedAttackFall;

	if (!ALS_ENSURE(IsValid(Montage.Montage)) || !IsAttackFallAllowedToStart(Montage.Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	Character->SetMovementModeLocked(true);

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactFall(Hit, Montage, Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartImpactFall(Hit, Montage, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartImpactFallLand(FDoubleHitResult Hit)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactFallIdle(FDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FActionMontageInfo Montage{ nullptr };
	FActionMontageInfo SelectedAttackFall = SelectImpactFallMontage(Hit);
	Montage = SelectedAttackFall;

	if (!ALS_ENSURE(IsValid(Montage.Montage)) || !IsAttackFallAllowedToStart(Montage.Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	Character->SetMovementModeLocked(true);

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactFall(Hit, Montage, Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartImpactFall(Hit, Montage, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartAttackFall(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FFallenAnimation FallenAnimations = SelectAttackFallAnimations(Hit);
	FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
	NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations = FallenAnimations;
	NewImpactReactionState.ImpactReactionParameters.FallType = ALSXTFallTypeTags::Attack;
	SetImpactReactionState(NewImpactReactionState);
	FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
	FActionMontageInfo Montage = SelectedAttackFall;

	if (!ALS_ENSURE(IsValid(Montage.Montage)) || !IsAttackFallAllowedToStart(Montage.Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);
	Character->SetDesiredStatus(ALSXTStatusTags::KnockedDown);
	// Character->SetDesiredStance(AlsStanceTags::Crouching);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->SetDesiredStatus(ALSXTStatusTags::KnockedDown);
		// Character->SetDesiredStance(AlsStanceTags::Crouching);
		ServerStartAttackFall(Hit, Montage, Montage);
		// StartAttackFallImplementation(Hit, Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		Character->SetDesiredStatus(ALSXTStatusTags::KnockedDown);
		// Character->SetDesiredStance(AlsStanceTags::Crouching);
		MulticastStartAttackFall(Hit, Montage, Montage);
		// StartAttackFallImplementation(Hit, Montage);
		// StartAttackFallImplementation(Hit, Montage);
		// ServerStartAttackFall(Hit, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartAttackFallLand(FAttackDoubleHitResult Hit)
{
	StartAttackFallIdle(Hit);
}

void UALSXTImpactReactionComponent::StartAttackFallIdle(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
	FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
	// FActionMontageInfo Montage = SelectedAttackFall;
	UAnimMontage* Montage = NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations.FallenPose;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsAttackFallAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	Character->SetMovementModeLocked(true);
	Character->SetDesiredStatus(ALSXTStatusTags::KnockedDown);

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartAttackFallIdle(Montage, Hit);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartAttackFallIdle(Montage, Hit);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackFall(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallLand(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallIdle(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartBraceForImpact()
{
	const FGameplayTag Side{ALSXTImpactSideTags::Front};
	UAnimSequenceBase* Montage = SelectBraceForImpactPose(Side);
	FALSXTDefensiveModeState DefensiveModeState;

	if (!ALS_ENSURE(IsValid(Montage)) || !IsClutchImpactPointAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}
	
	DefensiveModeState.Mode = ALSXTDefensiveModeTags::BraceForImpact;
	DefensiveModeState.Montage = Montage;
	Character->SetDefensiveModeState(DefensiveModeState);
	Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::BraceForImpact);
	// StartClutchImpactPointTimer();
}

void UALSXTImpactReactionComponent::StartImpactGetUp(FDoubleHitResult Hit)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackGetUp(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }
	if (GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.GetUpMontages.IsValidIndex(0))
	{
		FActionMontageInfo SelectedAttackGetUp = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.GetUpMontages[0];
		FActionMontageInfo Montage = SelectedAttackGetUp;

		if (!ALS_ENSURE(IsValid(Montage.Montage)))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
			return;
		}
		const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
		// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
		// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
		// ImpactReactionParameters.Stance = Stance;
		// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
		// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
		// FALSXTImpactReactionState NewImpactReactionState;
		// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
		// SetImpactReactionState(NewImpactReactionState);

		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

		if (Character->GetLocalRole() >= ROLE_Authority)
		{
			ServerStartAttackGetUp(Hit, Montage);
		}
		else
		{
			Character->GetCharacterMovement()->FlushServerMoves();
			MulticastStartAttackGetUp(Hit, Montage);
			// OnImpactReactionStarted(Hit.DoubleHitResult);
		}
	}
	
}

void UALSXTImpactReactionComponent::StartSyncedAttackGetUp(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactResponse(FDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FResponseAnimation Montage;
	FResponseAnimation SelectedImpactResponse = SelectImpactResponseMontage(Hit);
	Montage = SelectedImpactResponse;

	if (!ALS_ENSURE(IsValid(Montage.Montage.Montage)) || !IsImpactResponseAllowedToStart(Montage.Montage.Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}
	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	Character->SetMovementModeLocked(true);

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	// ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactResponse(Hit, Montage.Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		// MulticastStartImpactResponse(Hit, Montage.Montage);
		StartImpactResponseImplementation(Hit, Montage.Montage);
	}

}

void UALSXTImpactReactionComponent::StartAttackResponse(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FResponseAnimation SelectedAttackResponse = SelectAttackResponseMontage(Hit);
	FResponseAnimation Montage = SelectedAttackResponse;

	if (!ALS_ENSURE(IsValid(Montage.Montage.Montage)) || !IsAttackResponseAllowedToStart(Montage.Montage.Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}
	
	if (!Hit.DoubleHitResult.OriginHitResult.HitResult.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Actor Invalid"));
		return;
	}
	
	FVector OtherActorLocation = Hit.DoubleHitResult.OriginHitResult.HitResult.GetActor()->GetActorLocation();
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), OtherActorLocation);
	const float StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(PlayerRot.Yaw)) };
	FRotator CurrentRotation = Character->GetActorRotation();
	CurrentRotation.Yaw = PlayerRot.Yaw;
	Character->SetActorRotation(CurrentRotation);
	Character->SetMovementModeLocked(true);

	// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartAttackResponse(Hit, Montage.Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartAttackResponse(Hit, Montage.Montage);
		// StartAttackResponseImplementation(Hit, Montage.Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::CrowdNavigationFall() {}

//Parameters
UALSXTImpactReactionSettings* UALSXTImpactReactionComponent::SelectImpactReactionSettings_Implementation()
{
	return nullptr;
}

UALSXTElementalInteractionSettings* UALSXTImpactReactionComponent::GetElementalInteractionSettings_Implementation()
{
	return nullptr;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectImpactAnticipationMontage_Implementation(const FGameplayTag& Velocity , const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAnticipationPose> Montages = SelectedImpactReactionSettings->ImpactAnticipationPoses;
	TArray<FAnticipationPose> FilteredMontages;
	FAnticipationPose SelectedImpactAnticipationPose;
	TArray<FGameplayTag> TagsArray = { ALSXTImpactVelocityTags::Slow, AlsStanceTags::Standing, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt, ALSXTHealthTags::All };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Pose)
	{
		return SelectedImpactAnticipationPose;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Velocity);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);
		CurrentTagsContainer.AppendTags(Montage.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Pose)
	{
		return SelectedImpactAnticipationPose;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedImpactAnticipationPose = FilteredMontages[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedImpactAnticipationPose;
	}
	else
	{
		SelectedImpactAnticipationPose = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedImpactAnticipationPose;
	}
	return SelectedImpactAnticipationPose;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectAttackAnticipationMontage_Implementation(const FGameplayTag& CharacterCombatStance, const FGameplayTag& Strength, const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAnticipationPose> Montages = SelectedImpactReactionSettings->AttackAnticipationPoses;
	TArray<FAnticipationPose> FilteredMontages;
	FAnticipationPose SelectedAttackAnticipationPose;
	TArray<FGameplayTag> TagsArray = { ALSXTActionStrengthTags::Light, AlsStanceTags::Standing, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt, ALSXTHealthTags::All };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Pose)
	{
		return SelectedAttackAnticipationPose;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Velocity);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);
		CurrentTagsContainer.AppendTags(Montage.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Pose)
	{
		return SelectedAttackAnticipationPose;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackAnticipationPose = FilteredMontages[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackAnticipationPose;
	}
	else
	{
		SelectedAttackAnticipationPose = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackAnticipationPose;
	}
	return SelectedAttackAnticipationPose;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectDefensiveMontage_Implementation(const FGameplayTag& Strength, const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAnticipationPose> Montages = SelectedImpactReactionSettings->DefensivePoses;
	TArray<FAnticipationPose> FilteredMontages;
	FAnticipationPose SelectedDefensivePose;
	TArray<FGameplayTag> TagsArray = { ALSXTActionStrengthTags::Light, AlsStanceTags::Standing, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt, ALSXTHealthTags::All };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Pose)
	{
		return SelectedDefensivePose;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Velocity);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);
		CurrentTagsContainer.AppendTags(Montage.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Pose)
	{
		return SelectedDefensivePose;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedDefensivePose = FilteredMontages[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedDefensivePose;
	}
	else
	{
		SelectedDefensivePose = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedDefensivePose;
	}
	return SelectedDefensivePose;
}

FBumpReactionAnimation UALSXTImpactReactionComponent::SelectBumpReactionMontage_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpReactionAnimation> Montages = SelectedImpactReactionSettings->BumpReactionAnimations;
	TArray<FBumpReactionAnimation> FilteredMontages;
	FBumpReactionAnimation SelectedBumpReactionAnimation;
	TArray<FGameplayTag> TagsArray = { ALSXTImpactVelocityTags::Slow, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt};
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Montage.Montage)
	{
		return SelectedBumpReactionAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Velocity);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage.Montage)
	{
		return SelectedBumpReactionAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedBumpReactionAnimation = FilteredMontages[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBumpReactionAnimation;
	}
	else
	{
		SelectedBumpReactionAnimation = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBumpReactionAnimation;
	}
	return SelectedBumpReactionAnimation;
}

UAnimSequenceBase* UALSXTImpactReactionComponent::SelectCrowdNavigationPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAnimSequenceBase* Pose { nullptr };
	return Pose;
}

FBumpReactionAnimation UALSXTImpactReactionComponent::SelectCrowdNavigationReactionMontage_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpReactionAnimation> Montages = SelectedImpactReactionSettings->CrowdNavigationReactionAnimations;
	TArray<FBumpReactionAnimation> FilteredMontages;
	FBumpReactionAnimation SelectedCrowdNavigationReactionAnimation;
	TArray<FGameplayTag> TagsArray = { ALSXTImpactVelocityTags::Slow, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Montage.Montage)
	{
		return SelectedCrowdNavigationReactionAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Velocity);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage.Montage)
	{
		return SelectedCrowdNavigationReactionAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedCrowdNavigationReactionAnimation = FilteredMontages[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedCrowdNavigationReactionAnimation;
	}
	else
	{
		SelectedCrowdNavigationReactionAnimation = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedCrowdNavigationReactionAnimation;
	}
	return SelectedCrowdNavigationReactionAnimation;
}

UAnimSequenceBase* UALSXTImpactReactionComponent::SelectBumpPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAnimSequenceBase* Pose { nullptr };
	return Pose;
}

FAttackReactionAnimation UALSXTImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAttackReactionAnimation> Montages = SelectedImpactReactionSettings->AttackReactionAnimations;
	TArray<FAttackReactionAnimation> FilteredMontages;
	FAttackReactionAnimation SelectedAttackReactionAnimation;
	TArray<FGameplayTag> TagsArray = { ALSXTActionStrengthTags::Light, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Montage.Montage)
	{
		return SelectedAttackReactionAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.ImpactStrength);
		CurrentTagsContainer.AppendTags(Montage.ImpactSide);
		CurrentTagsContainer.AppendTags(Montage.ImpactForm);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage.Montage)
	{
		return SelectedAttackReactionAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		if (FilteredMontages.Contains(LastAttackReactionAnimation))
		{
			int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
			FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackReactionAnimation = FilteredMontages[RandIndex];
		LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackReactionAnimation;
	}
	else
	{
		SelectedAttackReactionAnimation = FilteredMontages[0];
		LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackReactionAnimation;
	}
	return SelectedAttackReactionAnimation;
}

FImpactReactionAnimation UALSXTImpactReactionComponent::SelectImpactReactionMontage_Implementation(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FImpactReactionAnimation> Montages = SelectedImpactReactionSettings->ImpactReactionAnimations;
	TArray<FImpactReactionAnimation> FilteredMontages;
	FImpactReactionAnimation SelectedAttackReactionAnimation;
	// TArray<FGameplayTag> TagsArray = { Hit.Strength, Hit.DoubleHitResult.ImpactSide, Hit.DoubleHitResult.ImpactForm };
	TArray<FGameplayTag> TagsArray = { ALSXTActionStrengthTags::Light, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Montage.Montage)
	{
		return SelectedAttackReactionAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.ImpactVelocity);
		CurrentTagsContainer.AppendTags(Montage.ImpactSide);
		CurrentTagsContainer.AppendTags(Montage.ImpactForm);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage.Montage)
	{
		return SelectedAttackReactionAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		if (FilteredMontages.Contains(LastImpactReactionAnimation))
		{
			int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
			FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackReactionAnimation = FilteredMontages[RandIndex];
		LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackReactionAnimation;
	}
	else
	{
		SelectedAttackReactionAnimation = FilteredMontages[0];
		LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedAttackReactionAnimation;
	}
	return SelectedAttackReactionAnimation;
}

FSyncedAttackAnimation UALSXTImpactReactionComponent::GetSyncedMontage_Implementation(int Index)
{
	UALSXTCombatSettings* SelectedCombatSettings = IALSXTCombatInterface::Execute_GetCombatSettings(this);
	TArray<FSyncedAttackAnimation> Montages = SelectedCombatSettings->SyncedAttackAnimations;
	TArray<FSyncedAttackAnimation> FilteredMontages;

	if (ALS_ENSURE(IsValid(Montages[Index].SyncedMontage.TargetSyncedMontage.Montage)))
	{
		// FSyncedAttackAnimation SelectedSyncedAttackReactionAnimation = Montages[Index];
		return Montages[Index];
	}
	else
	{
		FSyncedAttackAnimation EmptySyncedAttackAnimation;
		return EmptySyncedAttackAnimation;
	}

}

FAnticipationPose UALSXTImpactReactionComponent::SelectStablizationPose_Implementation(FDoubleHitResult Hit)
{
	FAnticipationPose SelectedMontage;
	return SelectedMontage;
}

FClutchImpactLocationAnimation UALSXTImpactReactionComponent::SelectClutchImpactPointMontage_Implementation(FDoubleHitResult Hit)
{
	FClutchImpactLocationAnimation SelectedMontage;
	return SelectedMontage;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectSteadyMontage_Implementation(const FGameplayTag& Side)
{
	FAnticipationPose SelectedMontage;
	return SelectedMontage;
}

FFallenAnimation UALSXTImpactReactionComponent::SelectImpactFallAnimations_Implementation(FDoubleHitResult Hit)
{
	FFallenAnimation SelectedMontage;

	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallingMontages;
	FActionMontageInfo SelectedImpactFallAnimation;

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage)
	{
		return SelectedMontage;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactFallAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedImpactFallAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedMontage;
	}
	else
	{
		SelectedImpactFallAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedMontage;
	}
	return SelectedMontage;
}

FFallenAnimation UALSXTImpactReactionComponent::SelectAttackFallAnimations_Implementation(FAttackDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FGameplayTag> TagsArray = { ALSXTActionStrengthTags::Light, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
	TArray<FFallenAnimation> Montages = SelectedImpactReactionSettings->AttackFallenAnimations;
	TArray<FFallenAnimation> FilteredMontages;
	FFallenAnimation SelectedAttackFallAnimation;

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].FallingMontages[0].Montage)
	{
		return SelectedAttackFallAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.ImpactStrength);
		CurrentTagsContainer.AppendTags(Montage.ImpactSide);
		CurrentTagsContainer.AppendTags(Montage.ImpactForm);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].FallingMontages[0].Montage)
	{
		return SelectedAttackFallAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactFallAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackFallAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedAttackFallAnimation;
	}
	else
	{
		SelectedAttackFallAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedAttackFallAnimation;
	}
	return SelectedAttackFallAnimation;
}

FActionMontageInfo UALSXTImpactReactionComponent::SelectImpactFallMontage_Implementation(FDoubleHitResult Hit)
{
	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallingMontages;
	FActionMontageInfo SelectedImpactFallAnimation;

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage)
	{
		return SelectedImpactFallAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactFallAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedImpactFallAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedImpactFallAnimation;
	}
	else
	{
		SelectedImpactFallAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedImpactFallAnimation;
		return SelectedImpactFallAnimation;
	}
	return SelectedImpactFallAnimation;
}

FActionMontageInfo UALSXTImpactReactionComponent::SelectAttackFallMontage_Implementation(FAttackDoubleHitResult Hit)
{
	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.FallingMontages;
	FActionMontageInfo SelectedAttackFallAnimation;

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage)
	{
		return SelectedAttackFallAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactFallAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackFallAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedAttackFallAnimation;
		return SelectedAttackFallAnimation;
	}
	else
	{
		SelectedAttackFallAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedAttackFallAnimation;
		return SelectedAttackFallAnimation;
	}
	return SelectedAttackFallAnimation;
}

UAnimSequenceBase* UALSXTImpactReactionComponent::SelectBraceForImpactPose_Implementation(const FGameplayTag& Side)
{
	TArray<FAnticipationPose> BracePoses = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BraceForImpactPoses;
	TArray<FAnticipationPose> FilteredBracePoses;
	UAnimSequenceBase* SelectedPose{ nullptr };
	const FGameplayTag Health = HealthToHealthTag(GetHealth());
	TArray<FGameplayTag> TagsArray = { Side, Health };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (BracePoses.Num() < 1 || !BracePoses[0].Pose)
	{
		return SelectedPose;
	}

	// Filter Montages based on Tag parameters
	for (auto Pose : BracePoses)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Pose.Side);
		CurrentTagsContainer.AppendTags(Pose.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredBracePoses.Add(Pose);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredBracePoses.Num() < 1 || !FilteredBracePoses[0].Pose)
	{
		return SelectedPose;
	}

	// If more than one result, avoid duplicates
	if (FilteredBracePoses.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredBracePoses.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredBracePoses.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredBracePoses.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredBracePoses.Num() - 1));
		SelectedPose = FilteredBracePoses[RandIndex].Pose;
		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedPose;
	}
	else
	{
		SelectedPose = FilteredBracePoses[0].Pose;
		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedPose;
	}
	return SelectedPose;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectImpactFallenPose_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallenPose;
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectAttackFallenPose_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.FallenPose;
	return SelectedMontage;
}

FActionMontageInfo UALSXTImpactReactionComponent::SelectImpactGetUpMontage_Implementation(FDoubleHitResult Hit)
{
	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.GetUpMontages;
	FActionMontageInfo SelectedAttackGetUpAnimation;

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage)
	{
		return SelectedAttackGetUpAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactGetUpAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackGetUpAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedAttackGetUpAnimation;
		return SelectedAttackGetUpAnimation;
	}
	else
	{
		SelectedAttackGetUpAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedAttackGetUpAnimation;
		return SelectedAttackGetUpAnimation;
	}
	return SelectedAttackGetUpAnimation;
}

FActionMontageInfo UALSXTImpactReactionComponent::SelectAttackGetUpMontage_Implementation(FAttackDoubleHitResult Hit)
{
	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.GetUpMontages;
	FActionMontageInfo SelectedAttackGetUpAnimation;

	// Return if there are no filtered Montages
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage)
	{
		return SelectedAttackGetUpAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastmpactGetUpAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredMontages.Num() - 1));
		SelectedAttackGetUpAnimation = FilteredMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedSelectedAttackGetUpAnimation;
		return SelectedAttackGetUpAnimation;
	}
	else
	{
		SelectedAttackGetUpAnimation = FilteredMontages[0];
		// LastImpactReactionAnimation = SelectedSelectedAttackGetUpAnimation;
		return SelectedAttackGetUpAnimation;
	}
	return SelectedAttackGetUpAnimation;
}

FResponseAnimation UALSXTImpactReactionComponent::SelectImpactResponseMontage_Implementation(FDoubleHitResult Hit)
{
	FResponseAnimation SelectedMontage;
	return SelectedMontage;
}

FResponseAnimation UALSXTImpactReactionComponent::SelectAttackResponseMontage_Implementation(FAttackDoubleHitResult Hit)
{
	FResponseAnimation SelectedMontage;
	return SelectedMontage;
}

// RPCs

void UALSXTImpactReactionComponent::ServerCrouch_Implementation()
{
	Character->SetDesiredStance(AlsStanceTags::Crouching);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::ServerGetUp_Implementation()
{
	Character->SetDesiredStance(AlsStanceTags::Standing);
	Character->ForceNetUpdate();
}


void UALSXTImpactReactionComponent::ServerBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastBumpReaction(Gait, Side, Form);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	StartBumpReaction(Gait, Side, Form);
}

void UALSXTImpactReactionComponent::ServerCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastCrowdNavigationReaction(Gait, Side, Form);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	StartCrowdNavigationReaction(Gait, Side, Form);
}

void UALSXTImpactReactionComponent::ServerSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	MulticastSyncedAnticipationReaction(AnticipationPoint);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	StartSyncedAnticipationReaction(AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	MulticastDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	StartDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerImpactReaction_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactReaction(Hit);
	Character->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastImpactReaction_Implementation(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackReaction(Hit);
	StartAttackReaction(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackReaction_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackReaction(Hit);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackReaction_Implementation(int32 Index)
{
	// MulticastSyncedAttackReaction(Index);
	StartSyncedAttackReaction(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackReaction_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackReaction_Implementation(int32 Index)
{
	StartSyncedAttackReaction(Index);
}

//

void UALSXTImpactReactionComponent::ServerStabilize_Implementation(FDoubleHitResult Hit)
{
	MulticastStabilize(Hit);
	// StartClutchImpactPoint(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerStabilize_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStabilize_Implementation(FDoubleHitResult Hit)
{
	StartStabilize(Hit);
}

void UALSXTImpactReactionComponent::ServerClutchImpactPoint_Implementation(FDoubleHitResult Hit)
{
	MulticastClutchImpactPoint(Hit);
	// StartClutchImpactPoint(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerClutchImpactPoint_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastClutchImpactPoint_Implementation(FDoubleHitResult Hit)
{
	StartClutchImpactPoint(Hit);
}

void UALSXTImpactReactionComponent::ServerImpactFall_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFall(Hit);
	StartImpactFall(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactFall_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactFall_Implementation(FDoubleHitResult Hit)
{
	StartImpactFall(Hit);
}

void UALSXTImpactReactionComponent::ServerImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFall(Hit);
	StartImpactFallLand(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactFallLand_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	StartImpactFallLand(Hit);
}

void UALSXTImpactReactionComponent::ServerImpactFallIdle_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFallIdle(Hit);
	StartImpactFallIdle(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactFallIdle_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactFallIdle_Implementation(FDoubleHitResult Hit)
{
	StartImpactFallIdle(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackFall_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastAttackFall(Hit);
	// StartAttackFall(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackFall_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackFall_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFall(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFall(Hit);
	StartAttackFallLand(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackFallLand_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFallLand(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackFallIdle_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFallIdle(Hit);
	StartAttackFallIdle(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackFallIdle_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackFallIdle_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFallIdle(Hit);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackFall_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFall(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackFall_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackFall_Implementation(int32 Index)
{
	StartSyncedAttackFall(Index);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackFallLand_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFallLand(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackFallLand_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackFallLand_Implementation(int32 Index)
{
	StartSyncedAttackFallLand(Index);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackFallIdle_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFallIdle(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackFallIdle_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackFallIdle_Implementation(int32 Index)
{
	StartSyncedAttackFallIdle(Index);
}

void UALSXTImpactReactionComponent::ServerBraceForImpact_Implementation()
{
	// MulticastBraceForImpact();
	StartBraceForImpact();
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerBraceForImpact_Validate()
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastBraceForImpact_Implementation()
{
	StartBraceForImpact();
}

void UALSXTImpactReactionComponent::ServerImpactGetUp_Implementation(FDoubleHitResult Hit)
{
	// MulticastImpactGetUp(Hit);
	StartImpactGetUp(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactGetUp_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactGetUp_Implementation(FDoubleHitResult Hit)
{
	StartImpactGetUp(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackGetUp_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackGetUp(Hit);
	StartAttackGetUp(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackGetUp_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackGetUp_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackGetUp(Hit);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackGetUp_Implementation(int32 Index)
{
	// MulticastSyncedAttackGetUp(Index);
	StartSyncedAttackGetUp(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackGetUp_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackGetUp_Implementation(int32 Index)
{
	StartSyncedAttackGetUp(Index);
}

void UALSXTImpactReactionComponent::ServerImpactResponse_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactResponse(Hit);
	// StartImpactResponse(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactResponse_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactResponse_Implementation(FDoubleHitResult Hit)
{
	StartImpactResponse(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackResponse_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackResponse(Hit);
	StartAttackResponse(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackResponse_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackResponse_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackResponse(Hit);
}

// Start RPCs

void UALSXTImpactReactionComponent::ServerStartSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	// if (IsImpactReactionAllowedToStart(Montage.Montage))
	// {
	// 	MulticastStartSyncedAnticipationReaction(Montage, AnticipationPoint);
	// 	Character->ForceNetUpdate();
	// }
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAnticipationReaction_Validate(FVector AnticipationPoint)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	// StartSyncedAnticipationReactionImplementation(Montage, AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerStartDefensiveReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartDefensiveReaction(Montage, Audio, AnticipationPoint);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartDefensiveReaction_Validate(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartDefensiveReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	StartDefensiveReactionImplementation(Montage, Audio, AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerStartBumpReaction_Implementation(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsBumpReactionAllowedToStart(Montage.Montage))
	{
	MulticastStartBumpReaction(Montage, ParticleActor, Particle, Audio);
	Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartBumpReaction_Validate(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartBumpReaction_Implementation(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartBumpReactionImplementation(Montage, ParticleActor, Particle, Audio);
}

void UALSXTImpactReactionComponent::ServerStartCrowdNavigationReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartCrowdNavigationReaction(Montage, Audio);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartCrowdNavigationReaction_Validate(FActionMontageInfo Montage, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartCrowdNavigationReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio)
{
	StartCrowdNavigationReactionImplementation(Montage, Audio);
}

void UALSXTImpactReactionComponent::ServerStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, ParticleActor, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactReaction_Validate(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
}

void UALSXTImpactReactionComponent::ServerStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackReaction(Hit, Montage, ParticleActor, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackReaction_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartAttackReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackReaction_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
	MulticastStartSyncedAttackReaction(Montage);
	Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAttackReaction_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAttackReaction_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackReactionImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartStabilize_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Pose))
	{
		MulticastStartStabilize(Pose, ImpactPoint);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartStabilize_Validate(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartStabilize_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	StartStabilizeImplementation(Pose, ImpactPoint);
}

void UALSXTImpactReactionComponent::ServerStartClutchImpactPoint_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Pose))
	{
		MulticastStartClutchImpactPoint(Pose, ImpactPoint);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartClutchImpactPoint_Validate(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartClutchImpactPoint_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	StartClutchImpactPointImplementation(Pose, ImpactPoint);
}

void UALSXTImpactReactionComponent::ServerStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactFallAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFall(Hit, Montage, FallMontage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactFall_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	StartImpactFallImplementation(Hit, Montage, FallMontage);
}

bool UALSXTImpactReactionComponent::ServerStartImpactFallIdle_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactFallIdle_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactFallIdleImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartImpactFallIdle_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFallIdle(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

void UALSXTImpactReactionComponent::ServerStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		// MulticastStartAttackFall(Hit, Montage);
		StartAttackFallImplementation(Hit, Montage, FallMontage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackFall_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	StartAttackFallImplementation(Hit, Montage, FallMontage);
}

void UALSXTImpactReactionComponent::ServerStartAttackFallIdle_Implementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartAttackFallIdle(Montage, Hit);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackFallIdle_Validate(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackFallIdle_Implementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	StartAttackFallIdleImplementation(Montage, Hit);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackFall_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFall(Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAttackFall_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAttackFall_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackFallIdle_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFallIdle(Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAttackFallIdle_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAttackFallIdle_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallIdleImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartBraceForImpact_Implementation(UAnimMontage* Montage)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartBraceForImpact(Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartBraceForImpact_Validate(UAnimMontage* Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartBraceForImpact_Implementation(UAnimMontage* Montage)
{
	StartBraceForImpactImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartImpactFallLand_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFallLand(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactFallLand_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactFallLand_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactFallLandImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartAttackFallLand_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackFallLand(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackFallLand_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackFallLand_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackFallLandImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackFallLand_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFallLand(Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAttackFallLand_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAttackFallLand_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallLandImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartImpactGetUp_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactGetUp(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactGetUp_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactGetUp_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactGetUpImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartAttackGetUp_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackGetUp(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackGetUp_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackGetUp_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackGetUpImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackGetUp_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackGetUp(Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartSyncedAttackGetUp_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartSyncedAttackGetUp_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackGetUpImplementation(Montage);
}

void UALSXTImpactReactionComponent::ServerStartImpactResponse_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactResponse(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactResponse_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactResponse_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactResponseImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartAttackResponse_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackResponse(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackResponse_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackResponse_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackResponseImplementation(Hit, Montage);
}

// Start Implementations

void UALSXTImpactReactionComponent::StartSyncedAnticipationReactionImplementation(FActionMontageInfo Montage, FVector AnticipationPoint)
{
	// ...
}

void UALSXTImpactReactionComponent::StartDefensiveReactionImplementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	// ...
}

void UALSXTImpactReactionComponent::StartBumpReactionImplementation(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsBumpReactionAllowedToStart(Montage.Montage) && IsValid(GetImpactReactionState().ImpactReactionParameters.BumpHit.HitResult.HitResult.GetActor()))
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		// Set Timer to Pause Animation if Velocity Stops
		// GetWorld()->GetTimerManager().SetTimer(BumpVelocityTimerHandle, BumpVelocityTimerDelegate, 0.1f, true);

		if (AnimInstance)
		{
			OnBumpReactionBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnBumpReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnBumpReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

		UAudioComponent* AudioComponent{ nullptr };

		//Calculate Rotation from Normal Vector
		FVector UpVector = CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
		{
			ClutchImpactPoint(CurrentImpactReactionState.ImpactReactionParameters.BumpHit);
		}
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
	}
	else
	{
		return;
	}
}

void UALSXTImpactReactionComponent::StartCrowdNavigationReactionImplementation(FActionMontageInfo Montage, USoundBase* Audio)
{
	if (IsCrowdNavigationReactionAllowedToStart(Montage.Montage) && IsValid(GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor()))
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		// Set Timer to Pause Animation if Velocity Stops
		// GetWorld()->GetTimerManager().SetTimer(CrowdNavigationVelocityTimerHandle, CrowdNavigationVelocityTimerDelegate, 0.1f, true);


		if (AnimInstance)
		{
			OnCrowdNavigationReactionBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnCrowdNavigationReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnCrowdNavigationReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

		UAudioComponent* AudioComponent{ nullptr };

		//Calculate Rotation from Normal Vector
		FVector UpVector = CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		if (IALSXTCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
		{
			CrowdNavigationFall();
		}
		else
		{
			if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
			{
				ClutchImpactPoint(CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit);
			}
		}
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
	}
	else
	{
		return;
	}
}

void UALSXTImpactReactionComponent::StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	//if (IsImpactReactionAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f))
	if (IsImpactReactionAllowedToStart(Montage))
	{
		//Anticipation
		FALSXTDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = Character->GetDefensiveMode();
		DefensiveModeState.Location = Hit.HitResult.HitResult.Location;
		Character->SetDefensiveModeState(DefensiveModeState);
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f);

		if (AnimInstance)
		{
			OnImpactReactionBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnImpactReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnImpactReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

		UAudioComponent* AudioComponent{ nullptr };

		//Calculate Rotation from Normal Vector
		FVector UpVector = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = Hit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, Hit.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, Hit.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}
		if (UKismetSystemLibrary::IsValidClass(ParticleActor))
		{
			// ServerSpawnParticleActor(Hit, ParticleActor);
			// MulticastSpawnParticleActor(Hit, ParticleActor);
			SpawnParticleActorImplementation(Hit, ParticleActor);
		}
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, Hit.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, Hit.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartAttackReactionImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsAttackReactionAllowedToStart(Montage.Montage))
	{
		PreviousAttackImpacts.Add(Hit);
		//Anticipation
		FALSXTDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = Character->GetDefensiveMode();
		DefensiveModeState.Location = Hit.DoubleHitResult.HitResult.HitResult.Location;
		Character->SetDefensiveModeState(DefensiveModeState);
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance)
		{
			OnAttackReactionBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnAttackReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnAttackReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

		UAudioComponent* AudioComponent{ nullptr };

		//Calculate Rotation from Normal Vector
		FVector UpVector = Hit.DoubleHitResult.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = Hit.DoubleHitResult.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = Hit.DoubleHitResult.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}
		if (ImpactReactionSettings.EnableImpactPointParticles)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GetImpactPointParticle(Hit.DoubleHitResult), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, NewRotation, { 1.0f, 1.0f, 1.0f }, true, true, ENCPoolMethod::None, true);
		}
		if (ImpactReactionSettings.EnableParticles)
		{

		}

		if (UKismetSystemLibrary::IsValidClass(ParticleActor))
		{
			// ServerSpawnParticleActor(Hit, ParticleActor);
			// MulticastSpawnParticleActor(Hit, ParticleActor);
			SpawnParticleActorImplementation(Hit.DoubleHitResult, ParticleActor);
		}
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, Hit.DoubleHitResult.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 1000, Hit.DoubleHitResult.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		if (IALSXTCollisionInterface::Execute_CanAttackFall(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
		{
			AttackFall(Hit);
		}
		else
		{
			if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
			{
				ClutchImpactPoint(Hit.DoubleHitResult);
			}
		}

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackReactionImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartStabilizeImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Montage))
	{
		// Character->SetFacialExpression();

		FALSXTDefensiveModeState CurrentDefensiveModeState = Character->GetDefensiveModeState();
		CurrentDefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.Location = ImpactPoint;
		Character->SetDefensiveModeState(CurrentDefensiveModeState);
		Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::ClutchImpactPoint);
		// AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartClutchImpactPointImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Montage))
	{
		// Character->SetFacialExpression();

		FALSXTDefensiveModeState CurrentDefensiveModeState = Character->GetDefensiveModeState();
		CurrentDefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.Location = ImpactPoint;
		Character->SetDefensiveModeState(CurrentDefensiveModeState);
		Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::ClutchImpactPoint);
		// AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartImpactFallImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactFallAllowedToStart(Montage.Montage))
	{

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();

		FVector EndLocation{ FVector::ZeroVector };
		if (GetImpactFallLocation(EndLocation, Hit))
		{
			CurrentImpactReactionState.ImpactReactionParameters.FallLandLocation = EndLocation;
			SetImpactReactionState(CurrentImpactReactionState);
		}
		else
		{
			StartImpactFallingTimer();
		}

		if (AnimInstance)
		{
			OnImpactFallBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnImpactFallBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnImpactFallBlendOutDelegate);
		}

		//Calculate Rotation from Normal Vector
		FVector UpVector = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactFall);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Character->SetDesiredStance(AlsStanceTags::Crouching);
		Character->Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartImpactFallIdleImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance)
		{
			OnImpactFallBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnImpactFallBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnImpactFallBlendOutDelegate);
		}

		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();

		//Calculate Rotation from Normal Vector
		FVector UpVector = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);
		// Character->GetMesh()->AddImpulseAtLocation(Hit.HitResult.Impulse, Hit.HitResult.HitResult.ImpactPoint, Hit.HitResult.HitResult.BoneName);
		Character->GetMesh()->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);
		Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartAttackFallImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsAttackFallAllowedToStart(Montage.Montage))
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();		
		FVector EndLocation{ FVector::ZeroVector };

		if (GetAttackFallLocation(EndLocation, Hit))
		{
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), EndLocation, 90.0f, 30.0f, Character->GetControlRotation(), FLinearColor::Red, 1.0, 1.0);
			CurrentImpactReactionState.ImpactReactionParameters.FallLandLocation = EndLocation;
			SetImpactReactionState(CurrentImpactReactionState);
		}
		else
		{
			StartAttackFallingTimer();
		}

		if (AnimInstance)
		{
			OnAttackFallBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnAttackFallBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnAttackFallBlendOutDelegate);
		}

		if (IsValid(CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
		{
			//Calculate Rotation from Normal Vector
			FVector UpVector = CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
			FVector NormalVector = CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.ImpactNormal;
			FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
			RotationAxis.Normalize();
			float DotProduct = FVector::DotProduct(UpVector, NormalVector);
			float RotationAngle = acosf(DotProduct);
			FQuat Quat = FQuat(RotationAxis, RotationAngle);
			FQuat RootQuat = CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
			FQuat NewQuat = Quat * RootQuat;
			FRotator NewRotation = NewQuat.Rotator();

			// Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.BoneName);
			// AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactFall);
			// Character->GetMesh()->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.BoneName, false,  true);
			// Character->SetDesiredPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, "pelvis");
		}

		Character->SetDesiredStatus(ALSXTStatusTags::KnockedDown);
		OnFallStarted();
		// ServerCrouch();
		// Character->Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartAttackFallIdleImplementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f);

		if (AnimInstance)
		{
			OnAttackFallEndedDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnAttackFallEnded);
			// StartAttackGetUp();
			AnimInstance->Montage_SetEndDelegate(OnAttackFallEndedDelegate);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallIdleImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartBraceForImpactImplementation(UAnimMontage* Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactFallLandImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackFallLandImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallLandImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactGetUpImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackGetUpImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance && IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
		{
			OnAttackFallGetupBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnAttackFallGetupBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnAttackFallGetupBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		// FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackGetUpImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactResponseImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartAttackResponseImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsAttackResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsAttackReactionNOTAllowedToStart"));
	}
}

// Spawn Particle Actor

bool UALSXTImpactReactionComponent::ServerSpawnParticleActor_Validate(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor)
{
	return true;
}

void UALSXTImpactReactionComponent::ServerSpawnParticleActor_Implementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor)
{
	SpawnParticleActorImplementation(Hit, ParticleActor);
}

void UALSXTImpactReactionComponent::MulticastSpawnParticleActor_Implementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor)
{
	if (UKismetSystemLibrary::IsValidClass(ParticleActor))
	{

		//Calculate Rotation from Normal Vector
		FVector UpVector = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = Hit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		FTransform SpawnTransform = FTransform(NewRotation, Hit.HitResult.HitResult.Location, { 1.0f, 1.0f, 1.0f });
		AActor* SpawnedActor;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// GetWorld()->SpawnActor<AActor>(ParticleActor->StaticClass(), SpawnTransform, SpawnInfo);
		SpawnedActor = GetWorld()->SpawnActor<AActor>(ParticleActor->StaticClass(), SpawnTransform, SpawnInfo);

		if (IsValid(SpawnedActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, SpawnedActor->GetActorLocation().ToString());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SpawnedActor Not Valid"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ParticleActor Invalid"));
	}
}

void UALSXTImpactReactionComponent::SpawnParticleActorImplementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor)
{
	if (UKismetSystemLibrary::IsValidClass(ParticleActor))
	{

		//Calculate Rotation from Normal Vector
		FVector UpVector = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = Hit.HitResult.HitResult.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = Hit.HitResult.HitResult.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		FTransform SpawnTransform = FTransform(NewRotation, Hit.HitResult.HitResult.Location, { 1.0f, 1.0f, 1.0f });
		AActor* SpawnedActor;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedActor = GetWorld()->SpawnActor<AActor>(ParticleActor, SpawnTransform, SpawnInfo);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ParticleActor Invalid"));
	}
}

// Refresh

void UALSXTImpactReactionComponent::RefreshSyncedAnticipationReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopSyncedAnticipationReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAnticipationReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshSyncedAnticipationReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshDefensiveReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopDefensiveReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshDefensiveReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshDefensiveReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshCrowdNavigationReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::CrowdNavigationReaction)
	{
		StopCrowdNavigationReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshCrowdNavigationReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshCrowdNavigationReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshBumpReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactReaction)
	{
		StopBumpReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshBumpReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshBumpReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshImpactReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactReaction)
	{
		StopImpactReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshImpactReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshImpactReactionPhysics(const float DeltaTime)
{
	float Offset = ImpactReactionSettings.RotationOffset;
	auto ComponentRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	TargetRotation.Pitch = ComponentRotation.Pitch;
	TargetRotation.Roll = ComponentRotation.Roll;

	if (ImpactReactionSettings.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = ImpactReactionState.ImpactReactionParameters.TargetYawAngle;

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			ImpactReactionState.ImpactReactionParameters.TargetYawAngle, DeltaTime,
			ImpactReactionSettings.RotationInterpolationSpeed);

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

void UALSXTImpactReactionComponent::RefreshAttackReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactReaction)
	{
		StopAttackReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshAttackReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshAttackReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshSyncedAttackReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::SyncedAttackReaction)
	{
		StopSyncedAttackReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAttackReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshSyncedAttackReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshCrowdNavigationFallReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactFall)
	{
		StopCrowdNavigationFallReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshCrowdNavigationFallReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshCrowdNavigationFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshImpactFallReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactFall)
	{
		StopImpactFallReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshImpactFallReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshImpactFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshAttackFallReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactFall)
	{
		StopAttackFallReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshAttackFallReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshAttackFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::RefreshSyncedAttackFallReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactFall)
	{
		StopSyncedAttackFallReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAttackFallReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshSyncedAttackFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UALSXTImpactReactionComponent::StopSyncedAnticipationReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAnticipationReactionEnded();
}

void UALSXTImpactReactionComponent::StopDefensiveReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnDefensiveReactionEnded();
}

void UALSXTImpactReactionComponent::StopCrowdNavigationReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationReactionEnded();
}

void UALSXTImpactReactionComponent::StopBumpReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnBumpReactionEnded();
}

void UALSXTImpactReactionComponent::StopImpactReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactReactionEnded();
}

void UALSXTImpactReactionComponent::StopAttackReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackReactionEnded();
}

void UALSXTImpactReactionComponent::StopSyncedAttackReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackReactionEnded();
}

void UALSXTImpactReactionComponent::StopCrowdNavigationFallReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationFallEnded();
}

void UALSXTImpactReactionComponent::StopImpactFallReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactFallEnded();
}

void UALSXTImpactReactionComponent::StopAttackFallReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackFallEnded();
}

void UALSXTImpactReactionComponent::StopSyncedAttackFallReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackFallEnded();
}

void UALSXTImpactReactionComponent::OnImpactReactionEnded_Implementation() {}
