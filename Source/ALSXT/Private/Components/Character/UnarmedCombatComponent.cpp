// MIT

#include "Components/Character/UnarmedCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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
	RefreshUnarmedAttack(DeltaTime);
}

// UnarmedAttack

void UUnarmedCombatComponent::UnarmedAttack(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Strength, const float BaseDamage, const float PlayRate)
{
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
	
	StartUnarmedAttack(UnarmedAttackType, NewStance, Strength, BaseDamage, PlayRate, Character->ALSXTSettings->UnarmedCombat.bRotateToInputOnStart && Character->GetLocomotionState().bHasInput
		 	? Character->GetLocomotionState().InputYawAngle
		 	: UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)));
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
	if ((AlsCharacter->GetOverlayMode() == AlsOverlayModeTags::Default) && ((Character->GetCombatStance() == ALSXTCombatStanceTags::Ready) || (Character->GetCombatStance() == ALSXTCombatStanceTags::Aiming)) && CanUnarmedAttack())
	{
		UnarmedAttack(ALSXTUnarmedAttackTypeTags::RightFist, ALSXTActionStrengthTags::Medium, 0.10f, 1.3f);
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

	// Clear the character movement mode and set the locomotion action to mantling.

	// Character->SetMovementModeLocked(true);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	Character->DisableInput(PlayerController);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// Character->GetMesh()->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

UALSXTUnarmedCombatSettings* UUnarmedCombatComponent::SelectUnarmedAttackSettings_Implementation()
{
	return nullptr;
}

UAnimMontage* UUnarmedCombatComponent::SelectUnarmedAttackMontage_Implementation(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage)
{
	TObjectPtr<UAnimMontage> SelectedMontage = nullptr;
	int i = 0;
	int j = 0;
	int k = 0;

	for (i = 0; i < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes.Num(); ++i)
		{
			if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackType == UnarmedAttackType)
			{
				for (j = 0; j < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths.Num(); ++j)
				{
					if (Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStrength == Strength)
					{
						for (k = 0; k < Character->ALSXTSettings->UnarmedCombat.UnarmedAttackTypes[i].UnarmedAttackStrengths[j].UnarmedAttackStances.Num(); ++k)
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
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::PrimaryAction)
	{
		StopUnarmedAttack();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshUnarmedAttackPhysics(DeltaTime);
	}
}

void UUnarmedCombatComponent::RefreshUnarmedAttackPhysics(const float DeltaTime)
{
	float Offset = Character->ALSXTSettings->UnarmedCombat.RotationOffset;
	auto ComponentRotation{ Character->GetCharacterMovement()->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	TargetRotation.Pitch = ComponentRotation.Pitch;
	TargetRotation.Roll = ComponentRotation.Roll;

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

void UUnarmedCombatComponent::StopUnarmedAttack()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);

	ULocalPlayer* LocalPlayer = Character->GetWorld()->GetFirstLocalPlayerFromController();

	APlayerController* PlayerController = Character->GetWorld()->GetFirstPlayerController();

	if (Character->IsPlayerControlled())
	{
		Character->EnableInput(PlayerController);
	}

	OnUnarmedAttackEnded();
}

void UUnarmedCombatComponent::OnUnarmedAttackEnded_Implementation() {}