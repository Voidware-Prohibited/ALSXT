// MIT

#include "Components/Character/ALSXTCombatComponent.h"
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
#include "Interfaces/ALSXTTargetLockInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"

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

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());

	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(Character) };
	if (IsValid(EnhancedInput))
	{
		//FSetupPlayerInputComponentDelegate Del = Character->OnSetupPlayerInputComponentUpdated;
		//Del.AddUniqueDynamic(this, &UALSXTCombatComponent::SetupInputComponent(EnhancedInput));
	}
	TargetTraceTimerDelegate.BindUFunction(this, "TryTraceForTargets");
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

bool UALSXTCombatComponent::IsTartgetObstructed()
{
	FVector CharLoc = Character->GetActorLocation();
	TArray<FHitResult> OutHits;

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : CombatSettings.ObstructionTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}
	FCollisionQueryParams CollisionQueryParameters;
	CollisionQueryParameters.AddIgnoredActor(GetOwner());
	CollisionQueryParameters.AddIgnoredActor(CurrentTarget.HitResult.GetActor());

	if (GetWorld()->LineTraceMultiByObjectType(OutHits, CharLoc, CurrentTarget.HitResult.GetActor()->GetActorLocation(), ObjectQueryParameters, CollisionQueryParameters))
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
	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (Character && TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming() && IsValid(CurrentTarget.HitResult.GetActor()))
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
	FRotator ControlRotation = Character->GetControlRotation();
	FVector CharLoc = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector CameraLocation = Character->Camera->GetFirstPersonCameraLocation();
	FVector StartLocation = ForwardVector * 150 + CameraLocation;
	FVector EndLocation = ForwardVector * 200 + StartLocation;
	FVector CenterLocation = (StartLocation - EndLocation) / 8 + StartLocation;
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(CombatSettings.TraceAreaHalfSize);
	TArray<FHitResult> OutHits;

	// Display Debug Shape
	if (CombatSettings.DebugMode)
	{
		DrawDebugBox(GetWorld(), CenterLocation, CombatSettings.TraceAreaHalfSize, ControlRotation.Quaternion(), FColor::Yellow, false, CombatSettings.DebugDuration, 100, 2);
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : CombatSettings.TargetTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}

	bool isHit = GetWorld()->SweepMultiByObjectType(OutHits, StartLocation, EndLocation, ControlRotation.Quaternion(), ObjectQueryParameters, CollisionShape);

	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UALSXTTargetLockInterface::StaticClass()) && Hit.GetActor() != Character && Character->GetDistanceTo(Hit.GetActor()) < CombatSettings.MaxLockDistance)
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

void UALSXTCombatComponent::GetClosestTarget()
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
	OnNewTarget(NewTarget);
	USkeletalMeshComponent* HitMesh;
	AALSXTCharacter* ALSXTChar = Cast<AALSXTCharacter>(CurrentTarget.HitResult.GetActor());

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
	TArray<FGameplayTag> TargetableOverlayModes;
	GetTargetableOverlayModes(TargetableOverlayModes);

	if (TargetableOverlayModes.Contains(Character->GetOverlayMode()) && Character->IsDesiredAiming())
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && (Hit.AngleFromCenter < CurrentTarget.AngleFromCenter))
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
		SetCurrentTarget(FoundHit);
	}
}

void UALSXTCombatComponent::GetTargetRight()
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
			if (Hit.HitResult.GetActor() != CurrentTarget.HitResult.GetActor() && (Hit.AngleFromCenter > CurrentTarget.AngleFromCenter))
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
	FVector SweepStart = Character->GetActorLocation();
	FVector SweepEnd = Character->GetActorLocation() + (Character->GetActorForwardVector() * Distance);
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(50.0f);
	TArray<AActor*> InitialIgnoredActors;
	TArray<AActor*> OriginTraceIgnoredActors;
	InitialIgnoredActors.Add(Character);	// Add Self to Initial Trace Ignored Actors
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;
	AttackTraceObjectTypes = CombatSettings.AttackTraceObjectTypes;
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SweepStart, SweepEnd, 50, CombatSettings.AttackTraceObjectTypes, false, InitialIgnoredActors, EDrawDebugTrace::None, OutHits, true, FLinearColor::Green, FLinearColor::Red, 0.0f);

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

void UALSXTCombatComponent::Attack(const FGameplayTag& ActionType, const FGameplayTag& AttackType, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate)
{
	if (Character->GetLocomotionAction() == AlsLocomotionActionTags::PrimaryAction)
	{
		return;
	}
	
	FGameplayTag NewStance = ALSXTActionStanceTags::Standing;

	if (Character->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
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
	SetCombatState(CurrentCombatState);
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
	if ((AlsCharacter->GetOverlayMode() == AlsOverlayModeTags::Default) && ((Character->GetCombatStance() == ALSXTCombatStanceTags::Ready) || (Character->GetCombatStance() == ALSXTCombatStanceTags::Aiming)) && CanAttack())
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
		Character->GetCharacterMovement()->FlushServerMoves();

		ServerStartAttack(Montage.Montage.Montage, PlayRate, StartYawAngle, TargetYawAngle);
		OnAttackStarted(AttackType, Stance, Strength, BaseDamage);
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
		Character->GetCharacterMovement()->FlushServerMoves();

		StartSyncedAttackImplementation(Montage.SyncedMontage.InstigatorSyncedMontage.Montage, SelectSyncedAttackMontageIndex, PlayRate, StartYawAngle, TargetYawAngle);
		ServerStartSyncedAttack(Montage.SyncedMontage.InstigatorSyncedMontage.Montage, SelectSyncedAttackMontageIndex, PlayRate, StartYawAngle, TargetYawAngle);
		OnSyncedAttackStarted(AttackType, Stance, Strength, BaseDamage);
	}
}

UALSXTCombatSettings* UALSXTCombatComponent::SelectAttackSettings_Implementation()
{
	return nullptr;
}

void UALSXTCombatComponent::DetermineAttackMethod_Implementation(FGameplayTag& AttackMethod, const FGameplayTag& ActionType, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const AActor* Target)
{
	if (UKismetSystemLibrary::DoesImplementInterface(GetCombatState().CombatParameters.Target, UALSXTCombatInterface::StaticClass()))
	{
		if (ActionType == ALSXTActionTypeTags::Secondary)
		{
			// if (IALSXTCombatInterface::Execute_CanBeTakenDown(GetCombatState().CombatParameters.Target) && CanPerformTakedown())
			if (CanPerformTakedown())
			{
				AttackMethod = ALSXTAttackMethodTags::TakeDown;
				return;
			}
			else if (CanGrapple())
			{
				if (CanThrow())
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
						else if (LastTarget.ConsecutiveHits >= SelectAttackSettings()->ConsecutiveHitsForSpecialAttack)
						{
							if (CanPerformUniqueAttack())
							{
								AttackMethod = ALSXTAttackMethodTags::Unique;
								return;
							}
							else if (CanPerformTakedown())
							{
								AttackMethod = ALSXTAttackMethodTags::TakeDown;
								return;
							}
							else if (CanGrapple())
							{
								if (CanThrow())
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
	UALSXTCombatSettings* Settings = SelectAttackSettings();
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
	UALSXTCombatSettings* Settings = SelectAttackSettings();
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
	UALSXTCombatSettings* Settings = SelectAttackSettings();
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

		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::PrimaryAction);
		// Crouch(); //Hack
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
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	if (Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Custom)
	{
		Character->SetMovementModeLocked(false);
		// Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		OnAttackEnded();
	}
	Character->SetMovementModeLocked(false);
}

void UALSXTCombatComponent::OnAttackEnded_Implementation() {}

void UALSXTCombatComponent::ServerStartSyncedAttack_Implementation(UAnimMontage* Montage, int32 Index, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsAttackAllowedToStart(Montage))
	{
		MulticastStartSyncedAttack(Montage, Index, PlayRate, StartYawAngle, TargetYawAngle);
		Character->ForceNetUpdate();
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
			if (!IALSXTCombatInterface::Execute_Blocking(GetCombatState().CombatParameters.Target))
			{
				IALSXTCombatInterface::Execute_AnticipateSyncedAttack(GetCombatState().CombatParameters.Target, Index);
			}
		}
		CombatState.CombatParameters.TargetYawAngle = TargetYawAngle;
		Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::PrimaryAction);
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

void UALSXTCombatComponent::StopSyncedAttack()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		// Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	if (Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Custom)
	{
		Character->SetMovementModeLocked(false);
		// Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		OnSyncedAttackEnded();
	}
	Character->SetMovementModeLocked(false);
}

void UALSXTCombatComponent::OnSyncedAttackEnded_Implementation() {}