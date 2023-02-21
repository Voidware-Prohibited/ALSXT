// MIT

#include "Components/Character/ImpactReactionComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Settings/ALSXTAttackReactionSettings.h"
#include "Utility/AlsMacros.h"

// Sets default values for this component's properties
UImpactReactionComponent::UImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// UnarmedAttack

void UImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
}

void UImpactReactionComponent::AttackReaction(FAttackDoubleHitResult Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("1 - AttackReaction"));
	StartAttackReaction(Hit);
	// MulticastAttackReaction(Hit);
	// ServerAttackReaction(Hit);

	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("1.5 - Authority"));
	// 	MulticastAttackReaction(Hit);
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("1.5 - Not Authority"));
	// 	// ServerAttackReaction(Hit);
	// 	StartAttackReaction(Hit);
	// }
}

bool UImpactReactionComponent::IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

void UImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("2 - StartAttackReaction"));
	// if (GetOwnerRole() <= ROLE_SimulatedProxy)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("3 - return false: Role: Simulated Proxy"));
	// 	return;
	// }
	UAnimMontage* Montage{ nullptr };
	UNiagaraSystem* Particle{ nullptr };
	USoundBase* Audio{ nullptr };

	Montage = SelectAttackReactionMontage(Hit);
	Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
	Audio = GetImpactReactionSound(Hit.DoubleHitResult);

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	Character->SetMovementModeLocked(true);
	// APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// Character->DisableInput(PlayerController);

	if (GetOwnerRole() >= ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("3 - Authority"));
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		MulticastStartImpactReaction(Hit.DoubleHitResult.HitResult.HitResult, Montage, Particle, Audio);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("3 - Not Authority"));
		Character->GetCharacterMovement()->FlushServerMoves();
		StartImpactReactionImplementation(Hit.DoubleHitResult.HitResult.HitResult, Montage, Particle, Audio);
		// ServerStartImpactReaction(Hit.DoubleHitResult.HitResult.HitResult, Montage, Particle, Audio);
		OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage { nullptr};
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
	// APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// Character->DisableInput(PlayerController);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		MulticastStartImpactReaction(Hit.HitResult.HitResult, Montage, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartImpactReactionImplementation(Hit.HitResult.HitResult, Montage, Particle, Audio);
		ServerStartImpactReaction(Hit.HitResult.HitResult, Montage, Particle, Audio);
		OnImpactReactionStarted(Hit);
	}
}

UALSXTImpactReactionSettings* UImpactReactionComponent::SelectImpactReactionSettings_Implementation(const FGameplayTag& Location)
{
	return nullptr;
}

UALSXTAttackReactionSettings* UImpactReactionComponent::SelectAttackReactionSettings_Implementation(const FGameplayTag& Location)
{
	return nullptr;
}

UAnimMontage* UImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit)
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

UAnimMontage* UImpactReactionComponent::SelectImpactReactionMontage_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage { nullptr };
	FActionMontageInfo LastAnimation { nullptr };
	FGameplayTag ImpactLoc = Hit.ImpactLocation;

	UALSXTImpactReactionSettings* ImpactReactionSettings = SelectImpactReactionSettings(ImpactLoc);

	for (int i = 0; i < ImpactReactionSettings->ImpactReactionLocations.Num(); ++i)
	{
		if (ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionLocation == Hit.ImpactLocation)
		{
			for (int j = 0; j < ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths.Num(); ++j)
			{
				if (ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionStrength == Hit.Strength)
				{
					for (int k = 0; k < ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides.Num(); ++k)
					{
						if (ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionSide == Hit.ImpactSide)
						{
							for (int l = 0; l < ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms.Num(); ++l)
							{
								if (ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].ImpactReactionForm == Hit.ImpactForm)
								{
									
									// Determine if Blocking and Declare Local Copy of Montages Array
									TArray<FActionMontageInfo> MontageArray;
									if (Character->IsBlocking())
									{
										MontageArray = ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].BlockingMontages;
									}
									else
									{
										MontageArray = ImpactReactionSettings->ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].RegularMontages;
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

void UImpactReactionComponent::ServerAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastAttackReaction(Hit);
	Character->ForceNetUpdate();
}

void UImpactReactionComponent::MulticastAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackReaction(Hit);
}

void UImpactReactionComponent::ServerImpactReaction_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactReaction(Hit);
	Character->ForceNetUpdate();
}

void UImpactReactionComponent::MulticastImpactReaction_Implementation(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
}

void UImpactReactionComponent::ServerStartImpactReaction_Implementation(FHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

void UImpactReactionComponent::MulticastStartImpactReaction_Implementation(FHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
}

void UImpactReactionComponent::StartImpactReactionImplementation(FHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{	
	//if (IsImpactReactionAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f))
	if (IsImpactReactionAllowedToStart(Montage))
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f);
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;

		UAudioComponent* AudioComponent{ nullptr };

		//Calculate Rotation from Normal Vector
		FVector UpVector = Hit.GetActor()->GetRootComponent()->GetUpVector();
		FVector NormalVector = Hit.ImpactNormal;
		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();
		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);
		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = Hit.GetActor()->GetRootComponent()->GetComponentQuat();
		FQuat NewQuat = Quat * RootQuat;
		FRotator NewRotation = NewQuat.Rotator();

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("4 - Reaction"));

		if (Particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, Hit.ImpactPoint, NewRotation, FVector(1.0f, 1.0f, 1.0f), true, true, ENCPoolMethod::None, true);
		}

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, Hit.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, Hit.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);
		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::HitReaction);
		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Cannot Start"));
	}
}

void UImpactReactionComponent::RefreshImpactReaction(const float DeltaTime)
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

void UImpactReactionComponent::RefreshImpactReactionPhysics(const float DeltaTime)
{
	float Offset = Character->ALSXTSettings->ImpactReaction.RotationOffset;
	auto ComponentRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	TargetRotation.Pitch = ComponentRotation.Pitch;
	TargetRotation.Roll = ComponentRotation.Roll;

	if (Character->ALSXTSettings->ImpactReaction.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = ImpactReactionState.TargetYawAngle;

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			ImpactReactionState.TargetYawAngle, DeltaTime,
			Character->ALSXTSettings->ImpactReaction.RotationInterpolationSpeed);

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

void UImpactReactionComponent::StopImpactReaction()
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

void UImpactReactionComponent::OnImpactReactionEnded_Implementation() {}
