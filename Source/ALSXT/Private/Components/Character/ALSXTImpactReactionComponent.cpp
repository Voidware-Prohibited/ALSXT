// MIT

#include "Components/Character/ALSXTImpactReactionComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Settings/ALSXTAttackReactionSettings.h"
#include "Utility/AlsMacros.h"

// Sets default values for this component's properties
UALSXTImpactReactionComponent::UALSXTImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UALSXTImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UALSXTImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Character->GetVelocity().Length() > 0)
	{
		ObstacleTrace();
	}
	// ...
}

void UALSXTImpactReactionComponent::ObstacleTrace()
{
	const auto* Capsule{ Character->GetCapsuleComponent() };

	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	// const auto CapsuleRadius{ Capsule->GetScaledCapsuleRadius() };
	const auto CapsuleRadius{ 30.0f };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };

	// Set Local Variables
	//const FVector LandingStartLocation{	TargetCapsuleLocation + (DepthTraceHit.Normal * 60) + (DownwardTraceHit.Normal * -(CapsuleHalfHeight * 1)) };
	const FVector UpVector{ Character->GetActorUpVector() };
	// const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight/2) + (Character->GetActorForwardVector() * 5)};
	const FVector StartLocation{ Character->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2)};
	
	//LocomotionState.InputYawAngle
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	float TraceDistance {0.0f};

	if (Character->GetVelocity().Length() > 0)
	{
		if (Character->GetGait() == AlsGaitTags::Walking)
		{
			TraceDistance = ImpactReactionSettings.WalkingBumpDetectionDistance;
		}
		if (Character->GetGait() == AlsGaitTags::Running)
		{
			TraceDistance = ImpactReactionSettings.RunningBumpDetectionDistance;
		}
		if (Character->GetGait() == AlsGaitTags::Sprinting)
		{
			TraceDistance = ImpactReactionSettings.SprintingBumpDetectionDistance;
		}
	}
	else
	{
		TraceDistance = 0.0f;
	}

	// const FVector EndLocation{ StartLocation + (Character->GetActorForwardVector() * TraceDistance) };
	const FVector EndLocation{ StartLocation + (Character->GetControlRotation().Yaw * TraceDistance) };
	// const FVector EndLocation{ StartLocation + (Character->GetControlRotation().Yaw * TraceDistance) };
	
	// Trace for room for Vaulting action
	if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight/2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	{
		FString BumpHit = HitResults[0].GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BumpHit);
	}
}


// UnarmedAttack

void UALSXTImpactReactionComponent::BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	// ...
}

void UALSXTImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
}

void UALSXTImpactReactionComponent::AttackReaction(FAttackDoubleHitResult Hit)
{
	// if (GetOwnerRole() == ROLE_AutonomousProxy)
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackReaction(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackReaction(Hit);
		// MulticastAttackReaction(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackReaction(FAttackDoubleHitResult Hit, const FGameplayTag& Overlay, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, int Index)
{
	// if (GetOwnerRole() == ROLE_AutonomousProxy)
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackReaction(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackReaction(Hit);
		// MulticastAttackReaction(Hit);
	}
}

void UALSXTImpactReactionComponent::BodyFallReaction(FAttackDoubleHitResult Hit)
{
	// ...
}

void UALSXTImpactReactionComponent::ImpactTimelineUpdate(float Value)
{
	//...
}

bool UALSXTImpactReactionComponent::IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

void UALSXTImpactReactionComponent::StartBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{

}

void UALSXTImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
{
	UAnimMontage* Montage{ nullptr };
	UNiagaraSystem* Particle{ nullptr };
	TSubclassOf<AActor> ParticleActor{ nullptr };
	USoundBase* Audio{ nullptr };

	Montage = SelectAttackReactionMontage(Hit);
	Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
	ParticleActor = GetImpactReactionParticleActor(Hit.DoubleHitResult);
	Audio = GetImpactReactionSound(Hit.DoubleHitResult);

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	Character->SetMovementModeLocked(true);

	StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);
}

void UALSXTImpactReactionComponent::StartSyncedAttackReaction(FAttackDoubleHitResult Hit)
{
	
}

void UALSXTImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage { nullptr};
	TSubclassOf<AActor> ParticleActor{ nullptr };
	UNiagaraSystem* Particle {nullptr};
	USoundBase* Audio {nullptr};

	Montage = SelectImpactReactionMontage(Hit);
	Particle = GetImpactReactionParticle(Hit);
	Audio = GetImpactReactionSound(Hit);

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	Character->SetMovementModeLocked(true);

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

void UALSXTImpactReactionComponent::StartFall(FDoubleHitResult Hit)
{

}

void UALSXTImpactReactionComponent::StartGetUp(FDoubleHitResult Hit)
{

}

void UALSXTImpactReactionComponent::StartResponse(FAttackDoubleHitResult Hit)
{

}

UALSXTImpactReactionSettings* UALSXTImpactReactionComponent::SelectImpactReactionSettings_Implementation(const FGameplayTag& Location)
{
	return nullptr;
}

UALSXTAttackReactionSettings* UALSXTImpactReactionComponent::SelectAttackReactionSettings_Implementation(const FGameplayTag& Location)
{
	return nullptr;
}

void UALSXTImpactReactionComponent::SyncedReaction() {}

void UALSXTImpactReactionComponent::Fall() {}

void UALSXTImpactReactionComponent::FallLand() {}

void UALSXTImpactReactionComponent::GeUp() {}

void UALSXTImpactReactionComponent::AttackResponse() {}

UAnimMontage* UALSXTImpactReactionComponent::SelectBumpReactionMontage_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAnimMontage* SelectedMontage{ nullptr };
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage{ nullptr };
	FActionMontageInfo LastAnimation{ nullptr };
	FGameplayTag ImpactLoc = Hit.DoubleHitResult.ImpactLocation;

	UALSXTAttackReactionSettings* AttackReactionSettings = SelectAttackReactionSettings(ImpactLoc);

	for (int i = 0; i < AttackReactionSettings->ImpactReactionLocations.Num(); ++i)
	{
		if (AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionLocation == Hit.DoubleHitResult.ImpactLocation)
		{
			for (int j = 0; j < AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths.Num(); ++j)
			{
				if (AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionStrength == Hit.Strength)
				{
					for (int k = 0; k < AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides.Num(); ++k)
					{
						if (AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionSide == Hit.DoubleHitResult.ImpactSide)
						{
							for (int l = 0; l < AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms.Num(); ++l)
							{
								if (AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].ImpactReactionForm == Hit.DoubleHitResult.ImpactForm)
								{
									FImpactForm SelectedEntry { AttackReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l] };

									// Determine if Blocking and Declare Local Copy of Montages Array
									TArray<FActionMontageInfo> MontageArray;
									if (Character->IsBlocking()) 
									{
										MontageArray = SelectedEntry.BlockingMontages;
									}
									else
									{
										MontageArray = SelectedEntry.RegularMontages;
									}
	
									if (MontageArray.Num() > 1)
									{
										// If LastAnimation exists, remove it from Local Montages array to avoid duplicates
										// if (LastAnimation.Montage)
										// {
										// 	MontageArray.Remove(LastAnimation);
										// }
	
										//Shuffle Array
										for (int m = MontageArray.Max(); m >= 0; --m)
										{
											int n = FMath::Rand() % (m + 1);
											if (m != n) MontageArray.Swap(m, n);
										}
	
										// Select Random Array Entry
										int RandIndex = rand() % MontageArray.Max();
										SelectedMontage = MontageArray[RandIndex].Montage;
										return SelectedMontage;
									}
									else
									{
										if (MontageArray[0].Montage) 
										{
											SelectedMontage = MontageArray[0].Montage;
										}
										else
										{
											SelectedMontage = SelectedEntry.DefaultFallbackMontage.Montage;
										}
										return SelectedMontage;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectImpactReactionMontage_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage { nullptr };
	FActionMontageInfo LastAnimation { nullptr };
	FGameplayTag ImpactLoc = Hit.ImpactLocation;

	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings(ImpactLoc);

	for (int i = 0; i < SelectedImpactReactionSettings->ImpactReactionLocations.Num(); ++i)
	{
		if (SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionLocation == Hit.ImpactLocation)
		{
			for (int j = 0; j < SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths.Num(); ++j)
			{
				if (SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionStrength == Hit.Strength)
				{
					for (int k = 0; k < SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides.Num(); ++k)
					{
						if (SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionSide == Hit.ImpactSide)
						{
							for (int l = 0; l < SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms.Num(); ++l)
							{
								if (SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].ImpactReactionForm == Hit.ImpactForm)
								{
									
									// Determine if Blocking and Declare Local Copy of Montages Array
									TArray<FActionMontageInfo> MontageArray;
									if (Character->IsBlocking())
									{
										MontageArray = SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].BlockingMontages;
									}
									else
									{
										MontageArray = SelectedImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].RegularMontages;
									}
									if (MontageArray.Num() > 1)
									{
										// If LastAnimation exists, remove it from Local Montages array to avoid duplicates
										// if (LastAnimation.Montage)
										// {
										// 	MontageArray.Remove(LastAnimation);
										// }

										//Shuffle Array
										for (int m = MontageArray.Max(); m >= 0; --m)
										{
											int n = FMath::Rand() % (m + 1);
											if (m != n) MontageArray.Swap(m, n);
										}

										// Select Random Array Entry
										int RandIndex = rand() % MontageArray.Max();
										SelectedMontage = MontageArray[RandIndex].Montage;
									}
									else
									{
										SelectedMontage = MontageArray[0].Montage;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return SelectedMontage;
}

FSyncedAttackAnimation UALSXTImpactReactionComponent::GetSyncedMontage_Implementation(const FGameplayTag& Overlay, FAttackDoubleHitResult Hit, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, int Index)
{
	FSyncedAttackAnimation SyncedAttackAnimation;
	return SyncedAttackAnimation;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectFallMontage_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage{ nullptr };
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectFallenPose_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage{ nullptr };
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectGetUpMontage_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage{ nullptr };
	return SelectedMontage;
}

UAnimMontage* UALSXTImpactReactionComponent::SelectResponseMontage_Implementation(FAttackDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage{ nullptr };
	return SelectedMontage;
}

bool UALSXTImpactReactionComponent::ServerAttackReaction_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::ServerAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackReaction(Hit);
	StartAttackReaction(Hit);
	Character->ForceNetUpdate();
}


void UALSXTImpactReactionComponent::MulticastAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackReaction(Hit);
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

void UALSXTImpactReactionComponent::ServerStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, ParticleActor, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

void UALSXTImpactReactionComponent::MulticastStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
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
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;

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
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::HitReaction);
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

void UALSXTImpactReactionComponent::RefreshImpactReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::HitReaction)
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
		TargetRotation.Yaw = ImpactReactionState.TargetYawAngle;

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			ImpactReactionState.TargetYawAngle, DeltaTime,
			ImpactReactionSettings.RotationInterpolationSpeed);

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

void UALSXTImpactReactionComponent::StopImpactReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	Character->SetMovementModeLocked(false);
	// Character->Cast<UAlsCharacterMovementComponent>(GetCharacterMovement())->SetMovementModeLocked(false);
	// Character->GetCharacterMovement<UAlsCharacterMovementComponent>()->SetMovementModeLocked(false);

	// ULocalPlayer* LocalPlayer = Character->GetWorld()->GetFirstLocalPlayerFromController();
	// 
	// APlayerController* PlayerController = Character->GetWorld()->GetFirstPlayerController();
	// 
	// if (Character->IsPlayerControlled())
	// {
	// 	Character->EnableInput(PlayerController);
	// }

	OnImpactReactionEnded();
}

void UALSXTImpactReactionComponent::OnImpactReactionEnded_Implementation() {}
