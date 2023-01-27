// MIT

#include "Components/Character/UnarmedCombatComponent.h"
#include "Utility/AlsUtility.h"

// Sets default values for this component's properties
UUnarmedCombatComponent::UUnarmedCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnarmedCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());

	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(Character) };
	if (IsValid(EnhancedInput))
	{
		//FSetupPlayerInputComponentDelegate Del = Character->OnSetupPlayerInputComponentUpdated;
		//Del.AddUniqueDynamic(this, &UUnarmedCombatComponent::SetupInputComponent(EnhancedInput));
	}
}


// Called every frame
void UUnarmedCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// UnarmedAttack

void UUnarmedCombatComponent::UnarmedAttack(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate)
{
	if (Character->GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
	{
		 StartUnarmedAttack(UnarmedAttackType, Stance, Strength, BaseDamage, PlayRate, Character->ALSXTSettings->UnarmedCombat.bRotateToInputOnStart && Character->GetLocomotionState().bHasInput
		 	? Character->GetLocomotionState().InputYawAngle
		 	: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)));
	}
}

void UUnarmedCombatComponent::SetupInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	if (PrimaryAction)
	{
		PlayerInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &UUnarmedCombatComponent::InputPrimaryAction);
	}
}

void UUnarmedCombatComponent::InputPrimaryAction()
{
	if ((AlsCharacter->GetOverlayMode() == AlsOverlayModeTags::Default) && (Character->GetCombatStance() == ALSXTCombatStanceTags::Ready) && CanUnarmedAttack())
	{
		// StartSlideInternal();
		UnarmedAttack(ALSXTUnarmedAttackTypeTags::RightFist, Character->GetStance(), ALSXTActionStrengthTags::Medium, 0.10f, 1.3f);
	}
}

bool UUnarmedCombatComponent::IsUnarmedAttackAllowedToStart(const UAnimMontage* Montage) const
{
	return !Character->GetLocomotionAction().IsValid() ||
		// ReSharper disable once CppRedundantParentheses
		(Character->GetLocomotionAction() == AlsLocomotionActionTags::PrimaryAction &&
			!Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage));
}


void UUnarmedCombatComponent::StartUnarmedAttack(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate, const float TargetYawAngle)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	auto* Montage{ SelectUnarmedAttackMontage(UnarmedAttackType, Stance, Strength, BaseDamage) };

	if (!ALS_ENSURE(IsValid(Montage)) || !IsUnarmedAttackAllowedToStart(Montage))
	{
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastStartUnarmedAttack(Montage, PlayRate, StartYawAngle, TargetYawAngle);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();

		StartUnarmedAttackImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		ServerStartUnarmedAttack(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		OnUnarmedAttackStarted(UnarmedAttackType, Stance, Strength, BaseDamage);
	}
}

UAnimMontage* UUnarmedCombatComponent::SelectUnarmedAttackMontage_Implementation(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage)
{
	UAnimMontage& SelectedMontage = nullptr;
	
	if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes.IsEmpty())
	{
		return nullptr;
	}
	else if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[0].UnarmedAttackStrengths.IsEmpty())
	{
		return nullptr;
	}
	else if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[0].UnarmedAttackStrengths[0].UnarmedAttackStances.IsEmpty())
	{
		return nullptr;
	}
	else if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[0].UnarmedAttackStrengths[0].UnarmedAttackStances[0].Montage = nullptr)
	{
		return nullptr;
	}
	else
	{
		for (int i = 0; i < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes.Num(); ++i)
		{
			if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackType == UnarmedAttackType)
			{
				for (int j = 0; j < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths.Num(); ++j)
				{
					if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStrength == Strength)
					{
						for (int k = 0; k < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances.Num(); ++k)
						{
							if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances[k].UnarmedAttackStance == Stance)
							{
								SelectedMontage = Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances[k].Montage;								
								// return SelectedMontage;
							}
						}
					}
				}	
			}
		}
	}
	return SelectedMontage;
}

void UUnarmedCombatComponent::ServerStartUnarmedAttack_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsUnarmedAttackAllowedToStart(Montage))
	{
		MulticastStartUnarmedAttack(Montage, PlayRate, StartYawAngle, TargetYawAngle);
		Character->ForceNetUpdate();
	}
}

void UUnarmedCombatComponent::MulticastStartUnarmedAttack_Implementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	StartUnarmedAttackImplementation(Montage, PlayRate, StartYawAngle, TargetYawAngle);
}

void UUnarmedCombatComponent::StartUnarmedAttackImplementation(UAnimMontage* Montage, const float PlayRate,
	const float StartYawAngle, const float TargetYawAngle)
{
	if (IsUnarmedAttackAllowedToStart(Montage) && Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate))
	{
		UnarmedCombatState.TargetYawAngle = TargetYawAngle;

		Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::PrimaryAction);
		// Crouch(); //Hack
	}
}

void UUnarmedCombatComponent::RefreshUnarmedAttack(const float DeltaTime)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy ||
		Character->GetMesh()->GetAnimInstance()->RootMotionMode <= ERootMotionMode::IgnoreRootMotion)
	{
		// Refresh UnarmedAttack physics here because UUnarmedCombatComponent::PhysicsRotation()
		// won't be called on simulated proxies or with ignored root motion.

		RefreshUnarmedAttackPhysics(DeltaTime);
	}
}

void UUnarmedCombatComponent::RefreshUnarmedAttackPhysics(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::PrimaryAction)
	{
		return;
	}

	auto TargetRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };

	if (Character->ALSXTSettings->UnarmedCombat.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = UnarmedCombatState.TargetYawAngle;

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			UnarmedCombatState.TargetYawAngle, DeltaTime,
			Character->ALSXTSettings->UnarmedCombat.RotationInterpolationSpeed);

		Character->GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}