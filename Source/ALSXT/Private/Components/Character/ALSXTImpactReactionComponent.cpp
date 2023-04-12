// MIT

#include "Components/Character/ALSXTImpactReactionComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Utility/AlsMacros.h"
#include "InputActionValue.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTCollisionInterface.h"

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
	if (Character->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity))
	{
		ObstacleTrace();
	}
	AnticipationTrace();
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

	if (ImpactReactionSettings.DebugMode)
	{
		FString VelMsg = "Vel: ";
		VelMsg.Append(FString::SanitizeFloat(Character->GetVelocity().Length()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, VelMsg);
	}

	if (Character->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity))
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
				
				if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
				{
					FHitResult OriginHitResult;
					TArray<AActor*> IgnoreActorsOrigin;
					IgnoreActorsOrigin.Add(HitResult.GetActor());

					if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), HitResult.ImpactPoint, StartLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActorsOrigin, EDrawDebugTrace::None, OriginHitResult, false, FLinearColor::Green, FLinearColor::Red, 5.0f))
					{
						FDoubleHitResult DoubleHitResult;
						DoubleHitResult.HitResult.HitResult = HitResult;
						IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), DoubleHitResult.HitResult.Mass);
						IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), DoubleHitResult.HitResult.Velocity);
						DoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
						IALSXTCollisionInterface::Execute_GetActorMass(Character, DoubleHitResult.OriginHitResult.Mass);
						IALSXTCollisionInterface::Execute_GetActorVelocity(Character, DoubleHitResult.OriginHitResult.Velocity);

						if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCharacterInterface::StaticClass()))
						{
							IALSXTCharacterInterface::Execute_BumpReaction(HitResult.GetActor(), DoubleHitResult, Character->GetDesiredGait(), ALSXTImpactSideTags::Right, ALSXTImpactFormTags::Blunt);
						}
						else if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
						{
							IALSXTCollisionInterface::Execute_OnActorImpactCollision(HitResult.GetActor(), DoubleHitResult);
						}

					}

					AALSXTCharacter* HitIsCharacter = Cast<AALSXTCharacter>(HitResult.GetActor());
					if (HitIsCharacter) 
					{
						// ...
					}
					else
					{

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
	else
	{
		TraceDistance = 0.0f;
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

	bool isHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ImpactReactionSettings.AnticipationAreaHalfSize, Character->GetControlRotation(), ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f);
	if (isHit)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCharacterInterface::StaticClass()))
			{
				float ActorVelocity{ 0.0f };
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

				if (DefensiveMode == ALSXTDefensiveModeTags::Avoiding)
				{
					Montage = SelectAttackAnticipationMontage(CharacterCombatStance, Velocity, Stance, Side, Form, Health);
				}
				else
				{
					if (DefensiveMode == ALSXTDefensiveModeTags::Blocking)
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
					float ActorVelocity{ 0.0f };
					float ActorMass{ 0.0f };
					FGameplayTag Velocity{ FGameplayTag::EmptyTag };
					FGameplayTag Form{ FGameplayTag::EmptyTag };
					FVector AnticipationPoint{ FVector::ZeroVector };
					FALSXTDefensiveModeState DefensiveModeState;
					FAnticipationPose Montage;
					IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
					IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);
					IALSXTCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationPoint);
					FGameplayTag DefensiveMode = DetermineDefensiveMode(Form);
					FGameplayTag Stance = Character->GetDesiredStance();
					FGameplayTag Side = LocationToImpactSide(AnticipationPoint);
					FGameplayTag Health = HealthToHealthTag(GetHealth());

					if (DefensiveMode == ALSXTDefensiveModeTags::Avoiding)
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
		if (!Character->IsBlocking())
		{
			Character->ResetDefensiveModeState();
			Character->SetDesiredDefensiveMode(ALSXTDefensiveModeTags::None);
		}
		
	}
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

void UALSXTImpactReactionComponent::OnImpactReactionStateChanged_Implementation(const FALSXTImpactReactionState& PreviousImpactReactionState) {}

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

void UALSXTImpactReactionComponent::AttackFall(FAttackDoubleHitResult Hit)
{
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFall(Hit);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackFall(Hit);
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

bool UALSXTImpactReactionComponent::IsBumpFallAllowedToStart(const UAnimMontage* Montage) const
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
	// ...
}

void UALSXTImpactReactionComponent::StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage{ nullptr };
	TSubclassOf<AActor> ParticleActor{ nullptr };
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
	
	UAnimMontage* Montage{ nullptr };
	FAttackReactionAnimation SelectedAttackReaction = SelectAttackReactionMontage(Hit);
	Montage = SelectedAttackReaction.Montage.Montage;

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
	ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	ImpactReactionParameters.PlayRate = SelectedAttackReaction.Montage.PlayRate;
	// ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// ImpactReactionParameters.Stance = Stance;
	ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	FALSXTImpactReactionState NewImpactReactionState;
	NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactReaction(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartImpactReaction(Hit.DoubleHitResult, Montage, ParticleActor, Particle, Audio);
		OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackReaction(int32 Index)
{
	// ...
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

	if (!ALS_ENSURE(IsValid(Montage)) || !IsClutchImpactPointAllowedToStart(Montage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(Character->GetActorRotation().Yaw)) };

	// StartClutchImpactPointImplementation(Montage, Hit.HitResult.HitResult.ImpactPoint);

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartClutchImpactPoint(Montage, Hit.HitResult.HitResult.ImpactPoint);
		OnImpactReactionStarted(Hit);
	}
}

void UALSXTImpactReactionComponent::StartImpactFall(FDoubleHitResult Hit)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackFall(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FActionMontageInfo Montage{ nullptr };
	FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
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
		ServerStartAttackFall(Hit, Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartAttackFall(Hit, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackFall(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartBraceForImpact()
{
	// ...
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

	FActionMontageInfo Montage{ nullptr };
	FActionMontageInfo SelectedAttackGetUp = SelectAttackFallMontage(Hit);
	Montage = SelectedAttackGetUp;

	if (!ALS_ENSURE(IsValid(Montage.Montage)))
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
		ServerStartAttackGetUp(Hit, Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartAttackGetUp(Hit, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
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
	FResponseAnimation SelectedAttackResponse = SelectImpactResponseMontage(Hit);
	Montage = SelectedAttackResponse;

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
		MulticastStartImpactResponse(Hit, Montage.Montage);
	}

}

void UALSXTImpactReactionComponent::StartAttackResponse(FAttackDoubleHitResult Hit)
{
	// if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FResponseAnimation Montage;
	FResponseAnimation SelectedAttackResponse = SelectAttackResponseMontage(Hit);
	Montage = SelectedAttackResponse;

	if (!ALS_ENSURE(IsValid(Montage.Montage.Montage)) || !IsAttackResponseAllowedToStart(Montage.Montage.Montage))
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
		ServerStartAttackResponse(Hit, Montage.Montage);
	}
	else
	{
		Character->GetCharacterMovement()->FlushServerMoves();
		MulticastStartAttackResponse(Hit, Montage.Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::BumpFall() {}

//Parameters
UALSXTImpactReactionSettings* UALSXTImpactReactionComponent::SelectImpactReactionSettings_Implementation()
{
	return nullptr;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectImpactAnticipationMontage_Implementation(const FGameplayTag& Velocity , const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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

FBumpReactionAnimation UALSXTImpactReactionComponent::SelectCrowdNavigationReactionMontage_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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

FAttackReactionAnimation UALSXTImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = SelectImpactReactionSettings();
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
	return SelectedMontage;
}

FFallenAnimation UALSXTImpactReactionComponent::SelectAttackFallAnimations_Implementation(FAttackDoubleHitResult Hit)
{
	FFallenAnimation SelectedMontage;
	return SelectedMontage;
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
	TArray<FActionMontageInfo> FilteredMontages = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallingMontages;
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

void UALSXTImpactReactionComponent::ServerClutchImpactPoint_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartClutchImpactPoint(Hit);
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

void UALSXTImpactReactionComponent::ServerAttackFall_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFall(Hit);
	StartAttackFall(Hit);
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

void UALSXTImpactReactionComponent::ServerImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	// MulticastImpactFallLand(Hit);
	// StartImpactFallLand(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerImpactFallLand_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	// StartImpactFallLand(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFallLand(Hit);
	// StartAttackFallLand(Hit);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerAttackFallLand_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	// StartAttackFallLand(Hit);
}

void UALSXTImpactReactionComponent::ServerSyncedAttackFallLand_Implementation(int32 Index)
{
	// MulticastSyncedAttackFallLand(Index);
	// StartSyncedAttackFallLand(Index);
	Character->ForceNetUpdate();
}

bool UALSXTImpactReactionComponent::ServerSyncedAttackFallLand_Validate(int32 Index)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastSyncedAttackFallLand_Implementation(int32 Index)
{
	// StartSyncedAttackFallLand(Index);
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
	// MulticastImpactResponse(Hit);
	StartImpactResponse(Hit);
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

void UALSXTImpactReactionComponent::ServerStartBumpReaction_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
	MulticastStartBumpReaction(Hit, Montage, ParticleActor, Particle, Audio);
	Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartBumpReaction_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartBumpReaction_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartBumpReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
}

void UALSXTImpactReactionComponent::ServerStartCrowdNavigationReaction_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartCrowdNavigationReaction(Hit, Montage, ParticleActor, Particle, Audio);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartCrowdNavigationReaction_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartCrowdNavigationReaction_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartCrowdNavigationReactionImplementation(Hit, Montage, ParticleActor, Particle, Audio);
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

void UALSXTImpactReactionComponent::ServerStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFall(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactFall_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactFallImplementation(Hit, Montage);
}

void UALSXTImpactReactionComponent::ServerStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackFall(Hit, Montage);
		Character->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartAttackFall_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackFallImplementation(Hit, Montage);
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

void UALSXTImpactReactionComponent::StartBumpReactionImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	// ...
}

void UALSXTImpactReactionComponent::StartCrowdNavigationReactionImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	// ...
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

		if (ShouldImpactFall())
		{
			ImpactFall(Hit);
		}
		else
		{
			if (ShouldClutchImpactPoint())
			{
				ClutchImpactPoint(Hit);
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

void UALSXTImpactReactionComponent::StartAttackReactionImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsAttackReactionAllowedToStart(Montage.Montage))
	{
		//Anticipation
		FALSXTDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = Character->GetDefensiveMode();
		DefensiveModeState.Location = Hit.DoubleHitResult.HitResult.HitResult.Location;
		Character->SetDefensiveModeState(DefensiveModeState);
		// Character->SetFacialExpression();

		Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
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

		if (ShouldAttackFall())
		{
			AttackFall(Hit);
		}
		else
		{
			if (ShouldClutchImpactPoint())
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

void UALSXTImpactReactionComponent::StartClutchImpactPointImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Montage))
	{
		// Character->SetFacialExpression();

		FALSXTDefensiveModeState CurrentDefensiveModeState = Character->GetDefensiveModeState();
		CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.Location = ImpactPoint;
		Character->SetDefensiveModeState(CurrentDefensiveModeState);

		AlsCharacter->SetLocomotionAction(AlsLocomotionActionTags::ImpactReaction);

		if (ShouldPerformImpactResponse())
		{
			ImpactResponse(GetImpactReactionState().ImpactReactionParameters.Hit);
		}
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartImpactFallImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackFallImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackFallImplementation(FActionMontageInfo Montage)
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
	// ...
}

void UALSXTImpactReactionComponent::StartSyncedAttackGetUpImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartImpactResponseImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
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

void UALSXTImpactReactionComponent::RefreshBumpFallReaction(const float DeltaTime)
{
	if (Character->GetLocomotionAction() != AlsLocomotionActionTags::ImpactFall)
	{
		StopBumpFallReaction();
		Character->ForceNetUpdate();
	}
	else
	{
		RefreshBumpFallReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshBumpFallReactionPhysics(const float DeltaTime)
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

void UALSXTImpactReactionComponent::StopBumpFallReaction()
{
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		Character->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnBumpFallEnded();
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
