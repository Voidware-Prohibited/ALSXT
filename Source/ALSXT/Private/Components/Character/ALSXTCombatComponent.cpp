// MIT

#include "Components/Character/ALSXTCombatComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Utility/AlsUtility.h"
#include "Utility/AlsMacros.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/ALSXTEnums.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Math/Vector.h"
#include "GameFramework/Character.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTTargetLockInterface.h"
#include "Interfaces/ALSXTHeldItemInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTCharacterSoundComponentInterface.h"
#include "ALSXTBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
UALSXTCombatComponent::UALSXTCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

// Called when the game starts
void UALSXTCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());

	UEnhancedInputComponent* EnhancedInput{ Cast<UEnhancedInputComponent>(Character) };
	if (IsValid(EnhancedInput))
	{
		//FSetupPlayerInputComponentDelegate Del = Character->OnSetupPlayerInputComponentUpdated;
		//Del.AddUniqueDynamic(this, &UALSXTCombatComponent::SetupInputComponent(EnhancedInput));
	}
	TargetTraceTimerDelegate.BindUFunction(this, "TryTraceForTargets");
	AttackTraceTimerDelegate.BindUFunction(this, "AttackCollisionTrace");
}

// Called every frame
void UALSXTCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshAttack(DeltaTime);
}

float UALSXTCombatComponent::GetAngle(FVector Target)
{
	float resultAngleInRadians = 0.0f;
	FVector PlayerLocation = GetOwner()->GetActorLocation();
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

bool UALSXTCombatComponent::IsTartgetObstructed()
{
	FVector CharLoc = GetOwner()->GetActorLocation();
	TArray<FHitResult> OutHits;

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : CombatSettings.ObstructionTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}
	FCollisionQueryParams CollisionQueryParameters;
	CollisionQueryParameters.AddIgnoredActor(GetOwner());
	CollisionQueryParameters.AddIgnoredActor(CurrentTarget.HitResult.GetActor());

	if (GetOwner()->GetWorld()->LineTraceMultiByObjectType(OutHits, CharLoc, CurrentTarget.HitResult.GetActor()->GetActorLocation(), ObjectQueryParameters, CollisionQueryParameters))
	{
		bool ValidObstruction = false;
		for (FHitResult Hit : OutHits)
		{		
			ValidObstruction = true;
			if (CombatSettings.UnlockWhenTargetIsObstructed)
			{
				ClearCurrentTarget();
			}
			return true;

		}
		return ValidObstruction;		
	}
	else
	{
		return false;
	}
}

void UALSXTCombatComponent::TryTraceForTargets()
{
	FGameplayTagContainer TargetableOverlayModes = IALSXTCombatInterface::Execute_GetTargetableOverlayModes(GetOwner());

	if (Character && TargetableOverlayModes.HasTag(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner())) && Character->IsDesiredAiming() && IsValid(CurrentTarget.HitResult.GetActor()))
	{
		if (Character->GetDistanceTo(CurrentTarget.HitResult.GetActor()) < CombatSettings.MaxInitialLockDistance)
		{			
			if (CombatSettings.UnlockWhenTargetIsObstructed && !CombatSettings.ObstructionTraceObjectTypes.IsEmpty()) {
				if (CombatSettings.UnlockWhenTargetIsObstructed && !IsTartgetObstructed()) {
					if (CurrentTarget.Valid)
					{
						RotatePlayerToTarget(CurrentTarget);
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Obstructed");
					DisengageAllTargets();
					OnTargetObstructed();
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
		else
		{
			DisengageAllTargets();
		}
	}
}

void UALSXTCombatComponent::TraceForTargets(TArray<FTargetHitResultEntry>& Targets)
{
	FRotator ControlRotation = Cast<ACharacter>(GetOwner())->GetControlRotation();
	FVector CharLoc = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector CameraLocation = IALSXTCharacterInterface::Execute_GetCharacterCamera(GetOwner())->GetFirstPersonCameraLocation();
	FVector StartLocation = ForwardVector * 150 + CameraLocation;
	FVector EndLocation = ForwardVector * 200 + StartLocation;
	FVector CenterLocation = (StartLocation - EndLocation) / 8 + StartLocation;
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(CombatSettings.TraceAreaHalfSize);
	TArray<FHitResult> OutHits;

	// Display Debug Shape
	if (CombatSettings.DebugMode)
	{
		DrawDebugBox(GetOwner()->GetWorld(), CenterLocation, CombatSettings.TraceAreaHalfSize, ControlRotation.Quaternion(), FColor::Yellow, false, CombatSettings.DebugDuration, 100, 2);
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : CombatSettings.TargetTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}

	bool isHit = GetOwner()->GetWorld()->SweepMultiByObjectType(OutHits, StartLocation, EndLocation, ControlRotation.Quaternion(), ObjectQueryParameters, CollisionShape);

	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UALSXTTargetLockInterface::StaticClass()) && Hit.GetActor() != GetOwner() && GetOwner()->GetDistanceTo(Hit.GetActor()) < CombatSettings.MaxLockDistance)
			{
				FTargetHitResultEntry HitResultEntry;
				HitResultEntry.Valid = true;
				HitResultEntry.DistanceFromPlayer = FVector::Distance(GetOwner()->GetActorLocation(), Hit.Location);
				HitResultEntry.AngleFromCenter = GetAngle(Hit.Location);
				HitResultEntry.HitResult = Hit;
				Targets.Add(HitResultEntry);
			}
		}
	}
}

void UALSXTCombatComponent::GetClosestTarget()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	AALSXTCharacter* ALSXTChar = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());
	FGameplayTagContainer TargetableOverlayModes = IALSXTCombatInterface::Execute_GetTargetableOverlayModes(GetOwner());

	if (TargetableOverlayModes.HasTag(ALSXTChar->GetOverlayMode()) && ALSXTChar->IsDesiredAiming())
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != GetOwner())
			{
				FTargetHitResultEntry HitResultEntry;
				HitResultEntry.Valid = true;
				HitResultEntry.DistanceFromPlayer = Hit.DistanceFromPlayer;
				HitResultEntry.AngleFromCenter = Hit.AngleFromCenter;
				HitResultEntry.HitResult = Hit.HitResult;
				
				if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner())->TargetableCharacterStatuses.HasTag(IALSXTCharacterInterface::Execute_GetCharacterStatus(Hit.HitResult.GetActor())) && (HitResultEntry.DistanceFromPlayer < CurrentTarget.DistanceFromPlayer))
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
					GetWorld()->GetTimerManager().SetTimer(TargetTraceTimerHandle, TargetTraceTimerDelegate, 0.001f, true);
				}
			}
		}
		if (FoundHit.Valid && FoundHit.HitResult.GetActor())
		{
			SetCurrentTarget(FoundHit);
			if (GEngine && CombatSettings.DebugMode)
			{
				FString DebugMsg = FString::SanitizeFloat(FoundHit.AngleFromCenter);
				DebugMsg.Append(" Hit Result: ");
				DebugMsg.Append(FoundHit.HitResult.GetActor()->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMsg);
			}
		}
	}
}

void UALSXTCombatComponent::SetCurrentTarget(const FTargetHitResultEntry& NewTarget)
{
	ClearCurrentTarget();
	CurrentTarget = NewTarget;
	IALSXTCombatInterface::Execute_OnNewTarget(GetOwner(), NewTarget);
	USkeletalMeshComponent* HitMesh;
	AALSXTCharacter* ALSXTChar = IALSXTCharacterInterface::Execute_GetCharacter(CurrentTarget.HitResult.GetActor());
	

	if (ALSXTChar)
	{
		HitMesh = ALSXTChar->GetMesh();
		TArray<UMaterialInterface*> CharMaterials = HitMesh->GetMaterials();	

		if (CharMaterials[0])
		{
			for (int m = 0; m < CharMaterials.Num(); m++)
			{
				UMaterialInstanceDynamic* CharDynMaterial = HitMesh->CreateAndSetMaterialInstanceDynamic(m);
				CharDynMaterial->SetScalarParameterValue(CombatSettings.HighlightMaterialParameterName, 1.0f);
				HitMesh->SetMaterial(m, CharDynMaterial);
				TargetDynamicMaterials.Add(CharDynMaterial);
			}
		}
		UMaterialInterface* HeadMaterial = ALSXTChar->Head->GetMaterial(0);
		UMaterialInstanceDynamic* HeadDynMaterial = ALSXTChar->Head->CreateAndSetMaterialInstanceDynamic(0);
		HeadDynMaterial->SetScalarParameterValue(CombatSettings.HighlightMaterialParameterName, 1.0f);
		ALSXTChar->Head->SetMaterial(0, HeadDynMaterial);
		TargetDynamicMaterials.Add(HeadDynMaterial);
	}	
}

void UALSXTCombatComponent::ClearCurrentTarget()
{
	if (CurrentTarget.Valid = true && CurrentTarget.HitResult.GetActor())
	{
		CurrentTarget.Valid = false;
		CurrentTarget.DistanceFromPlayer = 340282346638528859811704183484516925440.0f;
		CurrentTarget.AngleFromCenter = 361.0f;

		if (Cast<AALSXTCharacter>(CurrentTarget.HitResult.GetActor()))
		{
			USkeletalMeshComponent* HitMesh = Cast<AALSXTCharacter>(CurrentTarget.HitResult.GetActor())->GetMesh();
			TArray<UMaterialInterface*> CharMaterials = HitMesh->GetMaterials();
			if (TargetDynamicMaterials[0])
			{
				for (int m = 0; m < TargetDynamicMaterials.Num(); m++)
				{
					TargetDynamicMaterials[m]->SetScalarParameterValue(CombatSettings.HighlightMaterialParameterName, 0.0f);
					// HitMesh->SetMaterial(m, CharDynMaterial);
				}
				TargetDynamicMaterials.Empty();
			}
		}
		CurrentTarget.HitResult = FHitResult(ForceInit);
	}
}

void UALSXTCombatComponent::DisengageAllTargets()
{
	ClearCurrentTarget();

	// Clear Trace Timer
	GetWorld()->GetTimerManager().ClearTimer(TargetTraceTimerHandle);
}

void UALSXTCombatComponent::GetTargetLeft()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	AALSXTCharacter* ALSXTChar = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());
	FGameplayTagContainer TargetableOverlayModes = IALSXTCombatInterface::Execute_GetTargetableOverlayModes(GetOwner());
	// 
	// // IALSXTCharacterInterface::GetCharacter(this)->IsDesiredAiming();
	// IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner())
	// 
	if (TargetableOverlayModes.HasTag(ALSXTChar->GetOverlayMode()) && ALSXTChar->IsDesiredAiming())
	{
		if (OutHits.Num() == 0 || OutHits.Num() == 1 && OutHits[0].HitResult.GetActor() == CurrentTarget.HitResult.GetActor())
		{
			return;
		}
		
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && Hit.HitResult.GetActor() != GetOwner() && (Hit.AngleFromCenter < CurrentTarget.AngleFromCenter))
			{
				
				if (!FoundHit.Valid)
				{
					FoundHit = Hit;
				}
				else
				{
					if (Hit.AngleFromCenter > FoundHit.AngleFromCenter)
					{
						FoundHit = Hit;
					}
				}
			}
		}
		if (FoundHit.Valid && FoundHit.HitResult.GetActor())
		{
			SetCurrentTarget(FoundHit);
		}
	}
}

void UALSXTCombatComponent::GetTargetRight()
{
	TArray<FTargetHitResultEntry> OutHits;
	TraceForTargets(OutHits);
	FTargetHitResultEntry FoundHit;
	AALSXTCharacter* ALSXTChar = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());
	FGameplayTagContainer TargetableOverlayModes = IALSXTCombatInterface::Execute_GetTargetableOverlayModes(GetOwner());
	
	if (TargetableOverlayModes.HasTag(ALSXTChar->GetOverlayMode()) && ALSXTChar->IsDesiredAiming())
	{
		if (OutHits.Num() == 0 || OutHits.Num() == 1 && OutHits[0].HitResult.GetActor() == CurrentTarget.HitResult.GetActor())
		{
			return;
		}
		
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && Hit.HitResult.GetActor() != GetOwner() && (Hit.AngleFromCenter > CurrentTarget.AngleFromCenter))
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
		if (FoundHit.Valid && FoundHit.HitResult.GetActor())
		{
			SetCurrentTarget(FoundHit);
		}
	}
}

void UALSXTCombatComponent::RotatePlayerToTarget(FTargetHitResultEntry Target)
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
			Character->GetController()->SetControlRotation(NewPlayerRotation);
		}
	}
}

void UALSXTCombatComponent::DashToTarget()
{

}

// Combat State
void UALSXTCombatComponent::SetCombatState(const FALSXTCombatState& NewCombatState)
{
	const auto PreviousCombatState{ CombatState };

	CombatState = NewCombatState;

	OnCombatStateChanged(PreviousCombatState);

	if ((Character->GetLocalRole() == ROLE_AutonomousProxy) && Character->IsLocallyControlled())
	{
		ServerSetCombatState(NewCombatState);
	}
}

void UALSXTCombatComponent::ServerSetCombatState_Implementation(const FALSXTCombatState& NewCombatState)
{
	SetCombatState(NewCombatState);
}

void UALSXTCombatComponent::ServerProcessNewCombatState_Implementation(const FALSXTCombatState& NewCombatState)
{
	ProcessNewCombatState(NewCombatState);
}

void UALSXTCombatComponent::OnReplicate_CombatState(const FALSXTCombatState& PreviousCombatState)
{
	OnCombatStateChanged(PreviousCombatState);
}

void UALSXTCombatComponent::OnCombatStateChanged_Implementation(const FALSXTCombatState& PreviousCombatState) {}

// Attack

AActor* UALSXTCombatComponent::TraceForPotentialAttackTarget(float Distance)
{
	TArray<FHitResult> OutHits;
	FVector SweepStart = GetOwner()->GetActorLocation();
	FVector SweepEnd = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * Distance;
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(50.0f);
	TArray<AActor*> InitialIgnoredActors;
	TArray<AActor*> OriginTraceIgnoredActors;
	InitialIgnoredActors.Add(Character);	// Add Self to Initial Trace Ignored Actors
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;
	AttackTraceObjectTypes = CombatSettings.AttackTraceObjectTypes;
	EDrawDebugTrace::Type ShowDebugTrace {EDrawDebugTrace::None};	
	CombatSettings.DebugMode ? ShowDebugTrace = EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SweepStart, SweepEnd, 50, CombatSettings.AttackTraceObjectTypes, false, InitialIgnoredActors, ShowDebugTrace, OutHits, true, FLinearColor::White, FLinearColor::Blue, 0.0f);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor() != Character && UKismetSystemLibrary::DoesImplementInterface(Hit.GetActor(), UALSXTCombatInterface::StaticClass()))
			{
				if (GEngine && CombatSettings.DebugMode)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Potential Target: %s"), *Hit.GetActor()->GetName()));
				}
				return Hit.GetActor();
			}
		}
	}
	return nullptr;
}

void UALSXTCombatComponent::BeginAttackCollisionTrace(FALSXTCombatAttackTraceSettings TraceSettings)
{
	CurrentAttackTraceSettings = TraceSettings;
	GetWorld()->GetTimerManager().SetTimer(AttackTraceTimerHandle, AttackTraceTimerDelegate, 0.001f, true);
}

void UALSXTCombatComponent::AttackCollisionTrace()
{
	if (!UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UALSXTCombatInterface::StaticClass()))
	{
		return;
	}

	// Setup Initial Trace
	IALSXTCombatInterface::Execute_GetCombatUnarmedTraceLocations(GetOwner(), CurrentAttackTraceSettings.AttackType, CurrentAttackTraceSettings.Start, CurrentAttackTraceSettings.End, CurrentAttackTraceSettings.Radius);
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes = CombatSettings.AttackTraceObjectTypes;
	TArray<AActor*> InitialIgnoredActors;
	TArray<AActor*> OriginTraceIgnoredActors;
	TArray<FHitResult> HitResults;
	InitialIgnoredActors.Add(GetOwner());	// Add Self to Initial Trace Ignored Actors
	EDrawDebugTrace::Type ShowDebugTrace {EDrawDebugTrace::None};
	CombatSettings.DebugMode ? ShowDebugTrace = EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	// Initial Trace
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner()->GetWorld(), CurrentAttackTraceSettings.Start, CurrentAttackTraceSettings.End, CurrentAttackTraceSettings.Radius, AttackTraceObjectTypes, false, InitialIgnoredActors, ShowDebugTrace, HitResults, true, FLinearColor::Green, FLinearColor::Yellow, 0.0f);
	if (isHit)
	{
		// Loop through HitResults Array
		for (auto& HitResult : HitResults)
		{
			// Check if not in AttackedActors Array
			if (!AttackTraceLastHitActors.Contains(HitResult.GetActor()))
			{
				AttackTraceLastHitActors.AddUnique(HitResult.GetActor());
				// Declare Local Vars
				FAttackDoubleHitResult CurrentHitResult;
				FGameplayTag ImpactLoc;
				FGameplayTag ImpactStrength;
				// FGameplayTag ImpactSide;
				FGameplayTag ImpactForm;
				AActor* HitActor = HitResult.GetActor();
				FString HitActorname;
				FVector HitActorVelocity{ FVector::ZeroVector };
				float HitActorMass{ 0.0f };
				float HitActorAttackVelocity{ 0.0f };
				float HitActorAttackMass{ 0.0f };

				// Populate Hit
				CurrentHitResult.DoubleHitResult.HitResult.HitResult = HitResult;
				CurrentHitResult.Type = CurrentAttackTraceSettings.AttackType;
				CurrentHitResult.Strength = CurrentAttackTraceSettings.AttackStrength;
				CurrentHitResult.DoubleHitResult.HitResult.ImpactStrength = CurrentAttackTraceSettings.AttackStrength;
				HitActor = CurrentHitResult.DoubleHitResult.HitResult.HitResult.GetActor();
				HitActorname = HitActor->GetName();

				// Physics
				if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCharacterInterface::StaticClass()))
				{
					IALSXTCharacterInterface::Execute_GetCombatAttackPhysics(HitActor, HitActorAttackMass, HitActorAttackVelocity);
				}
				else
				{
					if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCollisionInterface::StaticClass()))
					{
						IALSXTCollisionInterface::Execute_GetActorVelocity(HitActor, HitActorVelocity);
						IALSXTCollisionInterface::Execute_GetActorMass(HitActor, HitActorMass);
					}
				}
				// TotalImpactEnergy = 50.0f + (HitActorVelocity * HitActorMass) + (HitActorAttackVelocity * HitActorAttackMass);
				FVector TotalImpactEnergy = (HitActorVelocity * HitActorMass) + (HitActorAttackVelocity * HitActorAttackMass);
				FVector HitDirection = HitResult.ImpactPoint - GetOwner()->GetActorLocation();
				HitDirection.Normalize();
				CurrentHitResult.DoubleHitResult.HitResult.Direction = HitDirection;
				CurrentHitResult.DoubleHitResult.HitResult.Impulse = HitResult.Normal * TotalImpactEnergy;

				// Impact Location
				if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCollisionInterface::StaticClass()))
				{
					IALSXTCollisionInterface::Execute_GetLocationFromBoneName(HitActor, CurrentHitResult.DoubleHitResult.HitResult.HitResult.BoneName, ImpactLoc);
				}
				CurrentHitResult.DoubleHitResult.HitResult.ImpactLocation = ImpactLoc;
				// CurrentHitResult.DoubleHitResult.HitResult.ImpactSide = ImpactSide;

				// Setup Origin Trace
				FHitResult OriginHitResult;
				OriginTraceIgnoredActors.Add(HitResult.GetActor());	// Add Hit Actor to Origin Trace Ignored Actors

				// Perform Origin Hit Trace to get PhysMat etc for ImpactLocation
				bool isOriginHit {false};
				isOriginHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetOwner()->GetWorld(), HitResult.Location, HitResult.TraceStart, CurrentAttackTraceSettings.Radius, AttackTraceObjectTypes, false, OriginTraceIgnoredActors, EDrawDebugTrace::None, OriginHitResult, true, FLinearColor::Green, FLinearColor::Yellow, 4.0f);
				if (isOriginHit)
				{
					// Populate Origin Hit
					CurrentHitResult.DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
					UALSXTBlueprintFunctionLibrary::GetSideFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.DoubleHitResult.ImpactSide);
					UALSXTBlueprintFunctionLibrary::GetStrengthFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.Strength);
					CurrentHitResult.DoubleHitResult.HitResult.ImpactStrength = CurrentAttackTraceSettings.AttackStrength;
					FString OriginHitActorname = OriginHitResult.GetActor()->GetName();

					// Populate Values based if Holding Item
					if (IALSXTHeldItemInterface::Execute_IsHoldingItem(GetOwner()))
					{
						IALSXTCombatInterface::Execute_GetCombatHeldItemAttackDamageInfo(GetOwner(), CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.HitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
					}
					else
					{
						IALSXTCombatInterface::Execute_GetCombatUnarmedAttackDamageInfo(GetOwner(), CurrentHitResult.Type, CurrentHitResult.Strength, CurrentHitResult.BaseDamage, CurrentHitResult.DoubleHitResult.HitResult.ImpactForm, CurrentHitResult.DoubleHitResult.HitResult.DamageType);
					}
					// GetFormFromHit(CurrentHitResult.DoubleHitResult, CurrentHitResult.DoubleHitResult.ImpactForm);
					// CurrentHitResult.DoubleHitResult.ImpactForm = 		
				}

				if (UKismetSystemLibrary::DoesImplementInterface(HitActor, UALSXTCollisionInterface::StaticClass()))
				{
					IALSXTCollisionInterface::Execute_OnActorAttackCollision(HitActor, CurrentHitResult);
				}

				// Play Effects for Attacker 
				IALSXTCombatInterface::Execute_OnAttackCollision(GetOwner(), CurrentHitResult);
			}
		}
	}
}

void UALSXTCombatComponent::EndAttackCollisionTrace()
{
	// Clear Attack Trace Timer
	GetWorld()->GetTimerManager().ClearTimer(AttackTraceTimerHandle);

	// Reset Attack Trace Settings
	CurrentAttackTraceSettings.Start = { 0.0f, 0.0f, 0.0f };
	CurrentAttackTraceSettings.End = { 0.0f, 0.0f, 0.0f };
	CurrentAttackTraceSettings.Radius = { 0.0f };

	// Empty AttackTraceLastHitActors Array
	AttackTraceLastHitActors.Empty();
}

void UALSXTCombatComponent::Attack(const FGameplayTag& ActionType, const FGameplayTag& AttackType, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate)
{
	if (Character->GetLocomotionAction() == AlsLocomotionActionTags::PrimaryAction)
	{
		return;
	}
	
	FGameplayTag NewStance = ALSXTActionStanceTags::Standing;

	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::InAir)
	{
		NewStance = ALSXTActionStanceTags::InAir;
	}
	else
	{
		if (Character->GetDesiredStance() == AlsStanceTags::Standing)
		{
			NewStance = ALSXTActionStanceTags::Standing;
		}
		else
		{
			NewStance = ALSXTActionStanceTags::Crouched;
		}
	}

	AActor* PotentialAttackTarget = TraceForPotentialAttackTarget(100.0f);
	FALSXTCombatState CurrentCombatState = GetCombatState();
	CurrentCombatState.CombatParameters.Target = PotentialAttackTarget;
	CurrentCombatState.CombatParameters.Form = ALSXTImpactFormTags::Blunt;
	CurrentCombatState.CombatParameters.AttackType = AttackType;
	CurrentCombatState.CombatParameters.Strength = Strength;
	CurrentCombatState.CombatParameters.BaseDamage = BaseDamage;
	CurrentCombatState.CombatParameters.PlayRate = PlayRate;
	FGameplayTag AttackMethod;
	if (IsValid(PotentialAttackTarget))
	{
		DetermineAttackMethod(AttackMethod, ActionType, AttackType, NewStance, Strength, BaseDamage, PotentialAttackTarget);
	}
	else
	{
		AttackMethod = ALSXTAttackMethodTags::Regular;
	}

	if (AttackMethod == ALSXTAttackMethodTags::Regular || AttackMethod == ALSXTAttackMethodTags::Riposte)
	{
		StartAttack(AttackType, NewStance, Strength, BaseDamage, PlayRate, CombatSettings.bRotateToInputOnStart && Character->GetLocomotionState().bHasInput
			? Character->GetLocomotionState().InputYawAngle
			: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)));
	}
	else if (AttackMethod == ALSXTAttackMethodTags::Unique || AttackMethod == ALSXTAttackMethodTags::TakeDown)
	{
		StartSyncedAttack(Character->GetOverlayMode(), AttackType, NewStance, Strength, AttackMethod, BaseDamage, PlayRate, CombatSettings.bRotateToInputOnStart && Character->GetLocomotionState().bHasInput
			? Character->GetLocomotionState().InputYawAngle
			: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)), 0);
	}
	CurrentCombatState.CombatParameters.AttackMethod = AttackMethod;
	SetCombatState(CurrentCombatState);
}

void UALSXTCombatComponent::SetupInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	if (PrimaryAction)
	{
		PlayerInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &UALSXTCombatComponent::InputPrimaryAction);
	}
}

void UALSXTCombatComponent::InputPrimaryAction()
{
	if ((Character->GetOverlayMode() == AlsOverlayModeTags::Default) && ((Character->GetCombatStance() == ALSXTCombatStanceTags::Ready) || (Character->GetCombatStance() == ALSXTCombatStanceTags::Aiming)) && IALSXTCombatInterface::Execute_CanAttack(GetOwner()))
	{
		Attack(ALSXTActionTypeTags::Primary, ALSXTAttackTypeTags::RightFist, ALSXTActionStrengthTags::Medium, 0.10f, 1.3f);
	}
}

bool UALSXTCombatComponent::IsAttackAllowedToStart(const UAnimMontage* Montage) const
{
	return !Character->GetLocomotionAction().IsValid() ||
		// ReSharper disable once CppRedundantParentheses
		(Character->GetLocomotionAction() == AlsLocomotionActionTags::PrimaryAction &&
			!Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage));
}


void UALSXTCombatComponent::StartAttack(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate, const float TargetYawAngle)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	FAttackAnimation Montage{ SelectAttackMontage(AttackType, Stance, Strength, BaseDamage) };

	if (!IsValid(Montage.Montage.Montage) || !IsAttackAllowedToStart(Montage.Montage.Montage))
	{
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	Character->SetMovementModeLocked(true);
	// Character->GetCharacterMovement()->SetMovementMode(MOVE_Custom);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		CombatParameters.BaseDamage = BaseDamage;
		CombatParameters.PlayRate = PlayRate;
		CombatParameters.TargetYawAngle = TargetYawAngle;
		CombatParameters.AttackType = AttackType;
		CombatParameters.Stance = Stance;
		CombatParameters.Strength = Strength;
		CombatParameters.CombatAnimation = Montage.Montage;

		FALSXTCombatState NewCombatState;
		NewCombatState.CombatParameters = CombatParameters;
		SetCombatState(NewCombatState);
		MulticastStartAttack(Montage.Montage.Montage, PlayRate, StartYawAngle, TargetYawAngle);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();

		CombatParameters.BaseDamage = BaseDamage;
		CombatParameters.PlayRate = PlayRate;
		CombatParameters.TargetYawAngle = TargetYawAngle;
		CombatParameters.AttackType = AttackType;
		CombatParameters.Stance = Stance;
		CombatParameters.Strength = Strength;
		CombatParameters.CombatAnimation = Montage.Montage;

		FALSXTCombatState NewCombatState;
		NewCombatState.CombatParameters = CombatParameters;
		SetCombatState(NewCombatState);

		ServerStartAttack(Montage.Montage.Montage, PlayRate, StartYawAngle, TargetYawAngle);
		// OnAttackStarted(AttackType, Stance, Strength, BaseDamage);
		OnAttackStartedDelegate.Broadcast(AttackType, Stance, Strength, BaseDamage);
	}
}

void UALSXTCombatComponent::StartSyncedAttack(const FGameplayTag& Overlay, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float BaseDamage, const float PlayRate, const float TargetYawAngle, int Index)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	int SelectSyncedAttackMontageIndex{ -1 };
	FSyncedAttackAnimation Montage{ SelectSyncedAttackMontage(AttackType, Stance, Strength, BaseDamage, SelectSyncedAttackMontageIndex) };

	if (!ALS_ENSURE(IsValid(Montage.SyncedMontage.InstigatorSyncedMontage.Montage)) || !IsAttackAllowedToStart(Montage.SyncedMontage.InstigatorSyncedMontage.Montage))
	{
		return;
	}

	// GetSyncedAttackMontage();

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	Character->SetMovementModeLocked(true);
	// Character->GetCharacterMovement()->SetMovementMode(MOVE_Custom);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		CombatParameters.BaseDamage = BaseDamage;
		CombatParameters.PlayRate = PlayRate;
		CombatParameters.TargetYawAngle = TargetYawAngle;
		CombatParameters.AttackType = AttackType;
		CombatParameters.Stance = Stance;
		CombatParameters.Strength = Strength;
		CombatParameters.CombatAnimation = Montage.SyncedMontage.InstigatorSyncedMontage;

		FALSXTCombatState NewCombatState;
		NewCombatState.CombatParameters = CombatParameters;
		SetCombatState(NewCombatState);
		MulticastStartSyncedAttack(Montage.SyncedMontage.InstigatorSyncedMontage.Montage, SelectSyncedAttackMontageIndex, PlayRate, StartYawAngle, TargetYawAngle);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();

		StartSyncedAttackImplementation(Montage.SyncedMontage.InstigatorSyncedMontage.Montage, SelectSyncedAttackMontageIndex, PlayRate, StartYawAngle, TargetYawAngle);
		ServerStartSyncedAttack(Montage.SyncedMontage.InstigatorSyncedMontage.Montage, SelectSyncedAttackMontageIndex, PlayRate, StartYawAngle, TargetYawAngle);
		OnSyncedAttackStarted(AttackType, Stance, Strength, BaseDamage);
	}
}

void UALSXTCombatComponent::DetermineAttackMethod_Implementation(FGameplayTag& AttackMethod, const FGameplayTag& ActionType, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const AActor* Target)
{
	if (UKismetSystemLibrary::DoesImplementInterface(GetCombatState().CombatParameters.Target, UALSXTCombatInterface::StaticClass()))
	{
		if (ActionType == ALSXTActionTypeTags::Secondary)
		{
			// if (IALSXTCombatInterface::Execute_CanBeTakenDown(GetCombatState().CombatParameters.Target) && CanPerformTakedown())
			if (IALSXTCombatInterface::Execute_CanPerformTakedown(GetOwner()))
			{
				AttackMethod = ALSXTAttackMethodTags::TakeDown;
				return;
			}
			else if (IALSXTCombatInterface::Execute_CanGrapple(GetOwner()))
			{
				if (IALSXTCombatInterface::Execute_CanThrow(GetOwner()))
				{
					AttackMethod = ALSXTAttackMethodTags::Throw;
					return;
				}
				else
				{
					AttackMethod = ALSXTAttackMethodTags::Grapple;
					return;
				}
			}
			else
			{
				AttackMethod = ALSXTAttackMethodTags::Cancelled;
				return;
			}
		}
		else
		{
			if (LastTargets.Num() > 0)
			{
				for (auto& LastTarget : LastTargets)
				{
					if (LastTarget.Target == Target)
					{
						if (LastTarget.LastBlockedAttack < 2.0f)
						{
							AttackMethod = ALSXTAttackMethodTags::Riposte;
							return;
						}
						else if (LastTarget.ConsecutiveHits >= IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner())->ConsecutiveHitsForSpecialAttack)
						{
							if (IALSXTCombatInterface::Execute_CanPerformUniqueAttack(GetOwner()))
							{
								AttackMethod = ALSXTAttackMethodTags::Unique;
								return;
							}
							else if (IALSXTCombatInterface::Execute_CanPerformTakedown(GetOwner()))
							{
								AttackMethod = ALSXTAttackMethodTags::TakeDown;
								return;
							}
							else if (IALSXTCombatInterface::Execute_CanGrapple(GetOwner()))
							{
								if (IALSXTCombatInterface::Execute_CanThrow(GetOwner()))
								{
									AttackMethod = ALSXTAttackMethodTags::Throw;
									return;
								}
								else
								{
									AttackMethod = ALSXTAttackMethodTags::Grapple;
									return;
								}
							}
							else
							{
								AttackMethod = ALSXTAttackMethodTags::Regular;
								return;
							}
						}
						else
						{
							AttackMethod = ALSXTAttackMethodTags::Regular;
							return;
						}
					}
				}
			}
			else
			{
				AttackMethod = ALSXTAttackMethodTags::Regular;
				return;
			}
		}
	}
	else
	{
		AttackMethod = ALSXTAttackMethodTags::Regular;
		return;
	}
}

FAttackAnimation UALSXTCombatComponent::SelectAttackMontage_Implementation(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage)
{
	FAttackAnimation SelectedAttackAnimation;
	UALSXTCombatSettings* Settings = IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner());
	TArray<FAttackAnimation> Montages = Settings->AttackAnimations;
	TArray<FAttackAnimation> FilteredMontages;
	TArray<FGameplayTag> TagsArray = { AttackType, Stance, Strength };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
	
	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].Montage.Montage)
	{
		return SelectedAttackAnimation;
	}

	// Filter sounds based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.AttackStrengths);
		CurrentTagsContainer.AppendTags(Montage.AttackStances);
		CurrentTagsContainer.AppendTags(Montage.AttackType);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered sounds
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].Montage.Montage)
	{
		return SelectedAttackAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackAnimation, remove it from FilteredMontages array to avoid duplicates
		if (FilteredMontages.Contains(LastAttackAnimation))
		{
			int IndexToRemove = FilteredMontages.Find(LastAttackAnimation);
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
		SelectedAttackAnimation = FilteredMontages[RandIndex];
		LastAttackAnimation = SelectedAttackAnimation;
		return SelectedAttackAnimation;
	}
	else
	{
		SelectedAttackAnimation = FilteredMontages[0];
		LastAttackAnimation = SelectedAttackAnimation;
		return SelectedAttackAnimation;
	}
	return SelectedAttackAnimation;
}

FSyncedAttackAnimation UALSXTCombatComponent::SelectSyncedAttackMontage_Implementation(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, int& Index)
{
	FSyncedAttackAnimation SelectedSyncedAttackAnimation;
	UALSXTCombatSettings* Settings = IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner());
	TArray<FSyncedAttackAnimation> Montages = Settings->SyncedAttackAnimations;
	TArray<FSyncedAttackAnimation> FilteredMontages;
	TArray<FGameplayTag> TagsArray = { AttackType, Stance, Strength };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Montages.Num() < 1 || !Montages[0].SyncedMontage.InstigatorSyncedMontage.Montage)
	{
		return SelectedSyncedAttackAnimation;
	}

	// Filter sounds based on Tag parameters
	for (auto Montage : Montages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.AttackStrength);
		CurrentTagsContainer.AppendTags(Montage.AttackStance);
		CurrentTagsContainer.AppendTags(Montage.AttackType);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredMontages.Add(Montage);
		}
	}

	// Return if there are no filtered sounds
	if (FilteredMontages.Num() < 1 || !FilteredMontages[0].SyncedMontage.InstigatorSyncedMontage.Montage)
	{
		return SelectedSyncedAttackAnimation;
	}

	// If more than one result, avoid duplicates
	if (FilteredMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackAnimation, remove it from FilteredMontages array to avoid duplicates
		if (FilteredMontages.Contains(LastSyncedAttackAnimation))
		{
			int IndexToRemove = FilteredMontages.Find(LastSyncedAttackAnimation);
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
		SelectedSyncedAttackAnimation = FilteredMontages[RandIndex];
		Index = Montages.Find(SelectedSyncedAttackAnimation);
		LastSyncedAttackAnimation = SelectedSyncedAttackAnimation;
		return SelectedSyncedAttackAnimation;
	}
	else
	{
		SelectedSyncedAttackAnimation = FilteredMontages[0];
		Index = Montages.Find(SelectedSyncedAttackAnimation);
		LastSyncedAttackAnimation = SelectedSyncedAttackAnimation;
		return SelectedSyncedAttackAnimation;
	}
	return SelectedSyncedAttackAnimation;
}

FAnticipationPose UALSXTCombatComponent::SelectBlockingkMontage_Implementation(const FGameplayTag& Strength, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	FAnticipationPose SelectedAnticipationPose;
	return SelectedAnticipationPose;
}

FSyncedActionAnimation UALSXTCombatComponent::GetSyncedAttackMontage_Implementation(int32 Index)
{
	UALSXTCombatSettings* Settings = IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner());
	TArray<FSyncedAttackAnimation> Montages = Settings->SyncedAttackAnimations;
	return Montages[Index].SyncedMontage;
}

void UALSXTCombatComponent::ServerStartAttack_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsAttackAllowedToStart(Montage))
	{
		MulticastStartAttack(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		Character->ForceNetUpdate();
	}
}

void UALSXTCombatComponent::MulticastStartAttack_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	StartAttackImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
}

void UALSXTCombatComponent::StartAttackImplementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	
	if (IsAttackAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate))
	{
		CombatState.CombatParameters.TargetYawAngle = TargetYawAngle;
		Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
		Character->SetLocomotionAction(AlsLocomotionActionTags::PrimaryAction);

		// Check if Player is moving forward or strafing
		float ForwardSpeed = FVector::DotProduct(Character->GetVelocity(), Character->GetActorForwardVector());
		float RightSpeed = FVector::DotProduct(Character->GetVelocity(), Character->GetActorRightVector());
		bool IsPlayerMoving = (ForwardSpeed > 200.0f || ForwardSpeed > 200.0f && ((RightSpeed > 200.0f) || (RightSpeed < 200.0f)));

		if (CombatSettings.bEnableMoveToTarget && IsValid(CurrentTarget.HitResult.GetActor()) && IsPlayerMoving)
		{
			float DistanceToTarget = FVector::Dist(CurrentTarget.HitResult.GetActor()->GetActorLocation(), Character->GetActorLocation());
			
			if (CombatSettings.DebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(DistanceToTarget));
			}	

			if ((DistanceToTarget < IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner())->MoveToTargetMaxDistance && DistanceToTarget > 60.0f) && !IsTartgetObstructed())
			{
				//Get Direction from Target to Player
				FVector Direction = CurrentTarget.HitResult.GetActor()->GetActorLocation() - Character->GetActorLocation();
				Direction.Normalize();

				//Calculate the new location depending on how far we are allowed to travel away.
				FVector NewLocation = CurrentTarget.HitResult.GetActor()->GetActorLocation() + IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner())->MoveToTargetMaxDistance * Direction;

				//Calculate Speed of MoveSmooth based on Min and Max values
				float MovementSpeed = ((DistanceToTarget - 60.0f) / (IALSXTCombatInterface::Execute_SelectCombatSettings(GetOwner())->MoveToTargetMaxDistance - 60.0f)) * 3.0f;

				//vector from our current location to the target which is slightly further away from the target
				FVector SmoothMoveVector = NewLocation - Character->GetActorLocation();
				FStepDownResult* StepdownResult{ nullptr };
				IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MoveSmooth(SmoothMoveVector, MovementSpeed, StepdownResult);
			}
		}
		
		// Crouch(); //Hack
		FALSXTCharacterVoiceParameters VoiceParameters = IALSXTCharacterSoundComponentInterface::Execute_GetVoiceParameters(GetOwner());
		IALSXTCharacterSoundComponentInterface::Execute_PlayAttackSound(GetOwner(), true, true, true, VoiceParameters.Sex, VoiceParameters.Variant, Character->GetOverlayMode(), CombatState.CombatParameters.Strength, CombatState.CombatParameters.AttackType, IALSXTCharacterInterface::Execute_GetStamina(GetOwner()));
	}
}

void UALSXTCombatComponent::RefreshAttack(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::PrimaryAction)
	{
		StopAttack();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshAttackPhysics(DeltaTime);
	}
}

void UALSXTCombatComponent::RefreshAttackPhysics(const float DeltaTime)
{
	// float Offset = CombatSettings->Combat.RotationOffset;
	auto ComponentRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	// TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	// TargetRotation.Yaw = TargetRotation.Yaw;
	// TargetRotation.Pitch = ComponentRotation.Pitch;
	// TargetRotation.Roll = ComponentRotation.Roll;

	// if (CombatSettings.RotationInterpolationSpeed <= 0.0f)
	// {
	// 	TargetRotation.Yaw = CombatState.TargetYawAngle;
	// 
	// 	Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	// }
	// else
	// {
	// 	TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
	// 		CombatState.TargetYawAngle, DeltaTime,
	// 		CombatSettings->Combat.RotationInterpolationSpeed);
	// 
	// 	Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	// }
}

void UALSXTCombatComponent::StopAttack()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	if (IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MovementMode == EMovementMode::MOVE_Custom)
	{
		IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), false);
		// Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		// OnAttackEnded();
		OnAttackEndedDelegate.Broadcast();
	}
	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), false);
}

void UALSXTCombatComponent::ServerStartSyncedAttack_Implementation(UAnimMontage* Montage, int32 Index, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsAttackAllowedToStart(Montage))
	{
		MulticastStartSyncedAttack(Montage, Index, PlayRate, StartYawAngle, TargetYawAngle);
		GetOwner()->ForceNetUpdate();
	}
}

void UALSXTCombatComponent::MulticastStartSyncedAttack_Implementation(UAnimMontage* Montage, int32 Index, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	StartSyncedAttackImplementation(Montage, Index, PlayRate, StartYawAngle, TargetYawAngle);
}

void UALSXTCombatComponent::StartSyncedAttackImplementation(UAnimMontage* Montage, int32 Index, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsAttackAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate))
	{
		if (UKismetSystemLibrary::DoesImplementInterface(GetCombatState().CombatParameters.Target, UALSXTCombatInterface::StaticClass()))
		{
			if (!IALSXTCollisionInterface::Execute_Blocking(GetCombatState().CombatParameters.Target))
			{
				IALSXTCombatInterface::Execute_AnticipateSyncedAttack(GetCombatState().CombatParameters.Target, Index);
			}
		}
		CombatState.CombatParameters.TargetYawAngle = TargetYawAngle;
		Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
		Character->SetLocomotionAction(AlsLocomotionActionTags::PrimaryAction);
		// Crouch(); //Hack
	}
}

void UALSXTCombatComponent::RefreshSyncedAttack(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::PrimaryAction)
	{
		StopAttack();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAttackPhysics(DeltaTime);
	}
}

void UALSXTCombatComponent::RefreshSyncedAttackPhysics(const float DeltaTime)
{
	// float Offset = CombatSettings->Combat.RotationOffset;
	auto ComponentRotation{ IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	// TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	// TargetRotation.Yaw = TargetRotation.Yaw;
	// TargetRotation.Pitch = ComponentRotation.Pitch;
	// TargetRotation.Roll = ComponentRotation.Roll;

	// if (CombatSettings.RotationInterpolationSpeed <= 0.0f)
	// {
	// 	TargetRotation.Yaw = CombatState.TargetYawAngle;
	// 
	// 	Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	// }
	// else
	// {
	// 	TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
	// 		CombatState.TargetYawAngle, DeltaTime,
	// 		CombatSettings->Combat.RotationInterpolationSpeed);
	// 
	// 	Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	// }
}

void UALSXTCombatComponent::StopSyncedAttack()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	if (IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MovementMode == EMovementMode::MOVE_Custom)
	{
		IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), false);
		// Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		OnSyncedAttackEnded();
	}
	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), false);
}

void UALSXTCombatComponent::OnSyncedAttackEnded_Implementation() {}