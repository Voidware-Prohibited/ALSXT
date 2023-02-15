// MIT

#include "Components/Character/ImpactReactionComponent.h"
#include "Utility/ALSXTStructs.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"

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

void UImpactReactionComponent::GetLocationFromBoneName_Implementation(FName Hit, FGameplayTag& Location) {}

void UImpactReactionComponent::GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side) 
{
	float DotProduct { Character->GetDotProductTo(Hit.OriginHitResult.HitResult.GetActor()) };

	// 1 is face to face, 0 is side,, -1 is behind

	FVector CrossProduct{ FVector::CrossProduct(Hit.HitResult.Impulse, Hit.HitResult.Impulse) };
}

void UImpactReactionComponent::GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength) 
{
	float HitMass = Hit.HitResult.Mass;
	float HitVelocity = Hit.HitResult.Velocity;
	float HitMomentum = HitMass * HitVelocity;

	float SelfMass = Hit.OriginHitResult.Mass;
	float SelfVelocity = Hit.OriginHitResult.Velocity;
	float SelfMomentum = SelfMass * SelfVelocity;

	float MomemtumSum = HitMomentum + SelfMomentum;
}

void UImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit, UAnimMontage* Montage, const UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReaction(Hit);
}

bool UImpactReactionComponent::IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

void UImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage{ nullptr };
	UNiagaraSystem* Particle{ nullptr };
	USoundBase* Audio{ nullptr };

	SelectAttackReactionMontage(Hit, Montage);
	GetImpactReactionParticle(Hit.DoubleHitResult, Particle);
	GetImpactReactionSound(Hit.DoubleHitResult, Audio);	

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
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
		MulticastStartImpactReaction(Hit.DoubleHitResult, Montage, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);
		ServerStartImpactReaction(Hit.DoubleHitResult, Montage, Particle, Audio);
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

	SelectImpactReactionMontage(Hit, Montage);
	GetImpactReactionParticle(Hit, Particle);
	GetImpactReactionSound(Hit, Audio);

	if (!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage))
	{
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
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
		ServerStartImpactReaction(Hit, Montage, Particle, Audio);
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

UAnimMontage* UImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit, const UAnimMontage* Montage)
{
	UAnimMontage* SelectedMontage{ nullptr };
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	FGameplayTag HitLocation { FGameplayTag::EmptyTag };
	GetLocationFromBoneName(Hit.DoubleHitResult.HitResult.HitResult.BoneName, HitLocation);
	FGameplayTag Side { FGameplayTag::EmptyTag };
	GetSideFromHit(Hit.DoubleHitResult, Side);
	FActionMontageInfo LastAnimation{ nullptr };

	 for (i = 0; i < Character->ALSXTSettings->AttackReaction.AttackReactionLocations.Num(); ++i)
	 {
	 	if (Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionLocation == HitLocation)
	 	{
	 		for (j = 0; j < Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths.Num(); ++j)
	 		{
	 			if (Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionStrength == Hit.Strength)
	 			{
	 				for (k = 0; k < Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides.Num(); ++k)
	 				{
	 					if (Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionSide == Side)
	 					{
							for (l = 0; l < Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms.Num(); ++l)
							{
								if (Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].ImpactReactionForm == Hit.DoubleHitResult.ImpactForm)
								{
									// Declare Local Copy of Montages Array
									TArray<FActionMontageInfo> MontageArray{ Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].MontageInfo };
									
									if (MontageArray.Num() > 1)
									{
										// If LastAnimation exists, remove it from Local Montages array to avoid duplicates
										if (LastAnimation.Montage)
										{
											MontageArray.Remove(LastAnimation);
										}

										//Shuffle Array
										for (int m = MontageArray.Max(); m >= 0; --m)
										{
											int n = FMath::Rand() % (m + 1);
											if (m != n) MontageArray.Swap(m, n);
										}

										// Select Random Array Entry
										int RandIndex = rand() % MontageArray.Max();
										SelectedMontage = Character->ALSXTSettings->AttackReaction.AttackReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].MontageInfo[RandIndex].Montage;
									}
									else
									{
										SelectedMontage = Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances[k].MontageInfo[0].Montage;
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

UAnimMontage* UImpactReactionComponent::SelectImpactReactionMontage_Implementation(FDoubleHitResult Hit, const UAnimMontage* Montage)
{
	UAnimMontage* SelectedMontage { nullptr };
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	FGameplayTag HitLocation { FGameplayTag::EmptyTag };
	GetLocationFromBoneName(Hit.HitResult.HitResult.BoneName, HitLocation);
	FGameplayTag Strength { FGameplayTag::EmptyTag };
	GetStrengthFromHit(Hit, Strength);
	FGameplayTag Side { FGameplayTag::EmptyTag };
	GetSideFromHit(Hit, Side);
	FActionMontageInfo LastAnimation { nullptr };

	for (i = 0; i < Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations.Num(); ++i)
	{
		if (Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionLocation == HitLocation)
		{
			for (j = 0; j < Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths.Num(); ++j)
			{
				if (Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionStrength == Strength)
				{
					for (k = 0; k < Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides.Num(); ++k)
					{
						if (Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionSide == Side)
						{
							for (l = 0; l < Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms.Num(); ++l)
							{
								if (Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].ImpactReactionForm == Hit.ImpactForm)
								{
									// Declare Local Copy of Montages Array
									TArray<FActionMontageInfo> MontageArray { Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].MontageInfo };
									
									if (MontageArray.Num() > 1)
									{
										// If LastAnimation exists, remove it from Local Montages array to avoid duplicates
										if (LastAnimation.Montage)
										{
											MontageArray.Remove(LastAnimation);
										}

										//Shuffle Array
										for (int m = MontageArray.Max(); m >= 0; --m)
										{
											int n = FMath::Rand() % (m + 1);
											if (m != n) MontageArray.Swap(m, n);
										}

										// Select Random Array Entry
										int RandIndex = rand() % MontageArray.Max();
										SelectedMontage = Character->ALSXTSettings->ImpactReaction.ImpactReactionLocations[i].ImpactReactionStrengths[j].ImpactReactionSides[k].ImpactReactionForms[l].MontageInfo[RandIndex].Montage;
									}
									else
									{
										SelectedMontage = Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances[k].MontageInfo[0].Montage;
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

void UImpactReactionComponent::ServerStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

void UImpactReactionComponent::MulticastStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
}

void UImpactReactionComponent::StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{

	if (IsImpactReactionAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f))
	{
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

		if (Particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, Hit.HitResult.HitResult.Location, NewRotation, FVector(1.0f, 1.0f, 1.0f), true, true, ENCPoolMethod::None, true);
		}

		if (Audio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, Hit.HitResult.HitResult.Location,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, Hit.HitResult.HitResult.Location,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::HitReaction);
		// Crouch(); //Hack
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
