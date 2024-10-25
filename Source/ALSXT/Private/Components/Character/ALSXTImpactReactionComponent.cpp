// MIT

#include "Components/Character/ALSXTImpactReactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Utility/ALSXTStructs.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFrameWork/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsRotation.h"
#include "InputActionValue.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTAIInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTCollisionInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "ALSXTCharacter.h"
#include "Utility/ALSXTCollisionStructs.h"
#include "Settings/ALSXTDefensiveModeSettings.h"
#include "Landscape.h"

// Sets default values for this component's properties
UALSXTImpactReactionComponent::UALSXTImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UALSXTImpactReactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CrowdNavigationPoseState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ImpactReactionState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, BumpPoseState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ObstacleImpactHistory, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AnticipationImpactHistory, Parameters)
}


// Called when the game starts
void UALSXTImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// AALSXTCharacter Character = Cast<AALSXTCharacter>(GetOwner());
	// AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	if (GetOwner())
	{
		AnimInstance = IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance();
	}

	if (GetOwner()->GetClass()->ImplementsInterface(UALSXTCharacterInterface::StaticClass()))
	{
		CharacterCapsule = IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner());
		CharacterCapsule->OnComponentHit.AddDynamic(this, &UALSXTImpactReactionComponent::OnCapsuleHit);
	}

	// Setup Timer Functions
	CrowdNavigationVelocityTimerDelegate.BindUFunction(this, "CrowdNavigationVelocityTimer");
	BumpVelocityTimerDelegate.BindUFunction(this, "BumpVelocityTimer");
	AnticipationTimerDelegate.BindUFunction(this, "AnticipationTimer");
	ImpactFallingTimerDelegate.BindUFunction(this, "ImpactFallingTimer");
	AttackFallingTimerDelegate.BindUFunction(this, "AttackFallingTimer");
	StabilizeTimerDelegate.BindUFunction(this, "StabilizeTimer");
	BraceForImpactTimerDelegate.BindUFunction(this, "BraceForImpactTimer");
	ImpactFallenTimerDelegate.BindUFunction(this, "ImpactFallenTimer");
	AttackFallenTimerDelegate.BindUFunction(this, "AttackFallenTimer");
	ClutchImpactPointTimerDelegate.BindUFunction(this, "ClutchImpactPointTimer");
	OnCapsuleHitTimerDelegate.BindUFunction(this, "OnCapsuleHitTimer");

}


// Called every frame
void UALSXTImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ObstacleTrace();
	AnticipationTrace();
}

FGameplayTag UALSXTImpactReactionComponent::DetermineDefensiveModeFromAttackingCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance)
{
	if (CombatStance == ALSXTCombatStanceTags::Ready || CombatStance == ALSXTCombatStanceTags::Aiming)
	{
		if (Form == ALSXTImpactFormTags::Blunt)
		{
			return ALSXTDefensiveModeTags::Blocking;
		}
		else

		{
			return ALSXTDefensiveModeTags::BraceForImpact;
		}
	}
	else
	{
		return FGameplayTag::EmptyTag;
	}
}

FGameplayTag UALSXTImpactReactionComponent::DetermineDefensiveModeFromCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance)
{
	if (CombatStance == ALSXTCombatStanceTags::Ready || CombatStance == ALSXTCombatStanceTags::Aiming)
	{
		if (Form == ALSXTImpactFormTags::Blunt)
		{
			return ALSXTDefensiveModeTags::Blocking;
		}
		else

		{
			return ALSXTDefensiveModeTags::Avoiding;
		}
	}
	else
	{
		return FGameplayTag::EmptyTag;
	}
}

FGameplayTag UALSXTImpactReactionComponent::HealthToHealthTag(float Health)
{
	FGameplayTag HealthTag;

	if (Health > 99)
	{
		HealthTag = ALSXTHealthTags::All;
	}
	if (Health > 75)
	{
		HealthTag = ALSXTHealthTags::Most;
	}
	if (Health > 50)
	{
		HealthTag = ALSXTHealthTags::Moderate;
	}
	if (Health < 25)
	{
		HealthTag = ALSXTHealthTags::Little;
	}
	if (Health < 05)
	{
		HealthTag = ALSXTHealthTags::None;
	}
	return HealthTag;
}

TArray<FName> UALSXTImpactReactionComponent::GetAffectedBones(const FGameplayTag& Side, const FGameplayTag& Height)
{
	TArray<FName> AffectedBones;
	if (Side == ALSXTImpactSideTags::Left)
	{
		if (Height == ALSXTImpactHeightTags::Low)
		{
			AffectedBones.Add("thigh_l");
		}
		if (Height == ALSXTImpactHeightTags::Middle)
		{
			AffectedBones.Add("upperarm_l");
			AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("clavicle_l");
			AffectedBones.Add("spine_03");
		}
	}
	if (Side == ALSXTImpactSideTags::Right)
	{
		if (Height == ALSXTImpactHeightTags::Low)
		{
			AffectedBones.Add("thigh_r");
		}
		if (Height == ALSXTImpactHeightTags::Middle)
		{
			AffectedBones.Add("upperarm_r");
			AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("clavicle_r");
			AffectedBones.Add("spine_03");
		}
	}
	if (Side == ALSXTImpactSideTags::Front || Side == ALSXTImpactSideTags::Back)
	{
		if (Height == ALSXTImpactHeightTags::Low)
		{
			AffectedBones.Add("thigh_l");
			AffectedBones.Add("thigh_r");
		}
		if (Height == ALSXTImpactHeightTags::Middle)
		{
			AffectedBones.Add("lowerarm_l");
			AffectedBones.Add("lowerarm_r");
			AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("neck_01");
			AffectedBones.Add("spine_03");
		}
	}

	return AffectedBones;
}



FGameplayTag UALSXTImpactReactionComponent::LocationToImpactSide(FVector Location)
{
	// Get the direction vector from the enemy to the player
	FVector PlayerStartPoint{ GetOwner()->GetActorLocation()};
	FVector DirectionToPlayer = Location - PlayerStartPoint;
	DirectionToPlayer.Normalize();

	// Get the enemy's forward and right vectors
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector RightVector = GetOwner()->GetActorRightVector();

	// Calculate dot products
	const float ForwardDot = FVector::DotProduct(DirectionToPlayer, ForwardVector);
	const float RightDot = FVector::DotProduct(DirectionToPlayer, RightVector);

	float ImpactSideDetectionRange = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ImpactSideDetectionRange;
	float FrontDegreeThreshold = ImpactSideDetectionRange / 2.0f;
	const float CosineThreshold = FMath::Cos(FrontDegreeThreshold);
	// Determine the quadrant
	if (ForwardDot > CosineThreshold)
	{
		return ALSXTImpactSideTags::Front;
	}
	if (ForwardDot < -CosineThreshold)
	{
		return ALSXTImpactSideTags::Back;
	}
	if (RightDot > 0)
	{
		return ALSXTImpactSideTags::Right;
	}

	return ALSXTImpactSideTags::Left;
}

FGameplayTag UALSXTImpactReactionComponent::LocationToImpactHeight(FVector Location)
{
	float Range = (GetOwner()->GetActorLocation().Z + IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleHalfHeight()) - (GetOwner()->GetActorLocation().Z - IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleHalfHeight());
	float RangeDivisor = Range / 3;

	if (Location.Z <= GetOwner()->GetActorLocation().Z - (RangeDivisor/2))
	{
		return ALSXTImpactHeightTags::Low;
	}
	if (Location.Z > GetOwner()->GetActorLocation().Z - (RangeDivisor / 2) && Location.Z < GetOwner()->GetActorLocation().Z + (RangeDivisor / 2))
	{
		return ALSXTImpactHeightTags::Middle;
	}
	if (Location.Z >= GetOwner()->GetActorLocation().Z + (RangeDivisor / 2))
	{
		return ALSXTImpactHeightTags::High;
	}
	else
	{
		return ALSXTImpactHeightTags::Middle;
	}
}

FGameplayTag UALSXTImpactReactionComponent::LocationToActorImpactSide(AActor* Actor, FVector Location)
{
	float LocationDotProduct = FVector::DotProduct(Actor->GetActorForwardVector(), (Actor->GetActorLocation() - Location));
	if (LocationDotProduct > 0)
	{
		return ALSXTImpactSideTags::Left;
	}
	else
	{
		return ALSXTImpactSideTags::Right;
	}
}

FGameplayTag UALSXTImpactReactionComponent::ConvertVelocityToTag(FVector Velocity)
{
	if (Velocity.Length() < 100)
	{
		return ALSXTImpactVelocityTags::Slow;
	}
	if (Velocity.Length() > 100)
	{
		return ALSXTImpactVelocityTags::Moderate;
	}
	if (Velocity.Length() > 500)
	{
		return ALSXTImpactVelocityTags::Fast;
	}
	if (Velocity.Length() > 800)
	{
		return ALSXTImpactVelocityTags::Faster;
	}
	if (Velocity.Length() > 1200)
	{
		return ALSXTImpactVelocityTags::TerminalVelocity;
	}
	else
	{
		return ALSXTImpactVelocityTags::Slow;
	}
}

FGameplayTag UALSXTImpactReactionComponent::ConvertVelocityToStrength(FVector Velocity)
{
	if (Velocity.Length() < 100)
	{
		return ALSXTActionStrengthTags::Light;
	}
	if (Velocity.Length() > 100)
	{
		return ALSXTActionStrengthTags::Medium;
	}
	if (Velocity.Length() > 500)
	{
		return ALSXTActionStrengthTags::Heavy;
	}
	else
	{
		return ALSXTActionStrengthTags::Light;
	}
}

FGameplayTag UALSXTImpactReactionComponent::ConvertVelocityTagToStrength(FGameplayTag Velocity)
{
	if (Velocity == ALSXTImpactVelocityTags::Slow)
	{
		return ALSXTActionStrengthTags::Light;
	}
	if (Velocity == ALSXTImpactVelocityTags::Moderate)
	{
		return ALSXTActionStrengthTags::Medium;
	}
	if (Velocity == ALSXTImpactVelocityTags::Fast)
	{
		return ALSXTActionStrengthTags::Medium;
	}
	if (Velocity == ALSXTImpactVelocityTags::Faster)
	{
		return ALSXTActionStrengthTags::Heavy;
	}
	if (Velocity == ALSXTImpactVelocityTags::TerminalVelocity)
	{
		return ALSXTActionStrengthTags::Heavy;
	}
	else
	{
		return ALSXTActionStrengthTags::Light;
	}
}

FGameplayTag UALSXTImpactReactionComponent::ConvertPhysicalSurfaceToFormTag(EPhysicalSurface PhysicalSurface)
{
	FGameplayTag FoundForm;
	UALSXTImpactReactionSettings* SelectedSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray <FALSXTFormSurfaces> FormSurfaces = SelectedSettings->FormSurfaces;
	for (FALSXTFormSurfaces Entry : FormSurfaces)
	{
		if (Entry.Surfaces.Contains(PhysicalSurface))
		{
			FoundForm = Entry.Form;
			return FoundForm;
		}
	}
	return FoundForm;
}

void UALSXTImpactReactionComponent::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	float CurrentMinVelocity = (OtherActor->GetClass()->ImplementsInterface(UALSXTCharacterInterface::StaticClass())) ? ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity : ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity;
	
	if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
	{
		// Handle surfaces like lava, spikes etc
		return;
	}

	if ((IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded && IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting && GetOwner()->GetVelocity().Length() > CurrentMinVelocity) && (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag || IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BumpInterruptableLocomotionActions.HasTag(IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()))) || (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) != AlsLocomotionModeTags::Grounded && (GetOwner()->GetVelocity() + OtherActor->GetVelocity()).Length() > CurrentMinVelocity))
	{
		if ((IsValid(OtherActor)) && (OtherActor != GetOwner()) && (IsValid(OtherComp)) && ValidateNewHit(Hit.GetActor()))
		{
			FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
			// Populate BumpHit
			FGameplayTag HitForm {ALSXTImpactFormTags::Blunt};
			// ConvertPhysicalSurfaceToFormTag()
			FDoubleHitResult NewDoubleHitResult;
			NewDoubleHitResult.CollisionType = ALSXTImpactTypeTags::Bump;
			NewDoubleHitResult.DateTime = GetWorld()->GetTimeSeconds();
			NewDoubleHitResult.HitResult.bHit = true;
			NewDoubleHitResult.HitResult.DamageType = ALSXTDamageTypeTags::Impact;
			NewDoubleHitResult.HitResult.Direction = GetOwner()->GetActorForwardVector();
			NewDoubleHitResult.HitResult.HitResult = Hit;
			NewDoubleHitResult.HitResult.ImpactForm = HitForm;
			NewDoubleHitResult.HitResult.ImpactGait = IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner());
			// NewDoubleHitResult.HitResult.ImpactLocation = ImpactReactionSettings.BoneLocationMap.Find(Hit.BoneName)->Location; // 
			NewDoubleHitResult.HitResult.ImpactSide = LocationToImpactSide(Hit.Location);
			NewDoubleHitResult.HitResult.ImpactHeight = LocationToImpactHeight(Hit.Location);
			NewDoubleHitResult.HitResult.ImpactStrength = ConvertVelocityToStrength(GetOwner()->GetVelocity());
			NewDoubleHitResult.HitResult.Impulse = GetOwner()->GetVelocity();
			NewDoubleHitResult.HitResult.Mass = 62;
			NewDoubleHitResult.HitResult.Velocity = OtherActor->GetVelocity();
			NewDoubleHitResult.ImpactForm = HitForm;
			// NewDoubleHitResult.ImpactLocation = ImpactReactionSettings.BoneLocationMap.Find(Hit.BoneName)->Location; //
			NewDoubleHitResult.HitResult.ImpactSide = LocationToImpactSide(Hit.Location);
			NewDoubleHitResult.ImpactType = ALSXTImpactTypeTags::Bump;
			NewDoubleHitResult.Strength = ConvertVelocityToStrength(GetOwner()->GetVelocity());

			// Setup Origin Trace
			TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes = ImpactReactionSettings.OriginTraceObjectTypes;
			FHitResult OriginHitResult;
			TArray<AActor*> OriginTraceIgnoredActors;
			OriginTraceIgnoredActors.Add(OtherActor);	// Add Hit Actor to Origin Trace Ignored Actors
			FVector MeshHitLocation{ ForceInit };
			FClosestPointOnPhysicsAsset ClosestPointOnPhysicsAsset;

			TArray<FName> AffectedBones = GetAffectedBones(NewDoubleHitResult.HitResult.ImpactSide, NewDoubleHitResult.HitResult.ImpactHeight);
			IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Navigation, AffectedBones);

			// Perform Origin Hit Trace to get PhysMat etc for ImpactLocation
			bool isOriginHit{ false };
			isOriginHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetOwner()->GetWorld(), Hit.ImpactPoint, Hit.TraceStart, 10, AttackTraceObjectTypes, false, OriginTraceIgnoredActors, EDrawDebugTrace::ForOneFrame, OriginHitResult, true, FLinearColor::Green, FLinearColor::Yellow, 4.0f);
			if (isOriginHit)
			{
				FGameplayTag OriginHitForm{ ALSXTImpactFormTags::Blunt };
				NewDoubleHitResult.OriginHitResult.bHit = true;
				NewDoubleHitResult.OriginHitResult.Direction = OriginHitResult.GetActor()->GetActorForwardVector();
				NewDoubleHitResult.OriginHitResult.HitResult = OriginHitResult;
				NewDoubleHitResult.OriginHitResult.ImpactForm = OriginHitForm;
				// NewImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCharacterInterface::StaticClass()))
				{
					NewDoubleHitResult.OriginHitResult.ImpactGait = IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner());
					// NewImpactReactionState.ImpactReactionParameters.BumpHit.OriginHitResult.ImpactLocation = LocationTo
					NewDoubleHitResult.OriginHitResult.ImpactSide = LocationToActorImpactSide(OriginHitResult.GetActor(), OriginHitResult.Location);
				}
				NewDoubleHitResult.OriginHitResult.ImpactStrength = ConvertVelocityToStrength(OriginHitResult.GetActor()->GetVelocity());
				NewDoubleHitResult.OriginHitResult.Impulse = OriginHitResult.GetActor()->GetVelocity();
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCharacterInterface::StaticClass()))
				{
					NewDoubleHitResult.OriginHitResult.Mass = 62;
				}
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCollisionInterface::StaticClass()))
				{
					IALSXTCollisionInterface::Execute_GetActorMass(GetOwner(), NewDoubleHitResult.OriginHitResult.Mass);
				}
				else
				{
					NewDoubleHitResult.OriginHitResult.Mass = 9999999;
				}
				NewDoubleHitResult.OriginHitResult.Velocity = GetOwner()->GetVelocity();

				//NewImpactReactionState.ImpactReactionParameters.CrowdNavigationHit
				// NewImpactReactionState.ImpactReactionParameters.BumpHit

				// NewImpactReactionState.ImpactReactionParameters.BumpHit = NewDoubleHitResult;		
				// SetImpactReactionState(NewImpactReactionState);

				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCharacterInterface::StaticClass()))
				{
					if (ImpactReactionSettings.DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					}
					// BumpReaction(IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
					// BumpReaction(IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), ConvertPhysicalSurfaceToFormTag(Hit.PhysMaterial->SurfaceType));
					// IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
					// FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
					// LocationToImpactHeight(Hit.Location);
					NewImpactReactionState.ImpactReactionParameters.CrowdNavigationHit = NewDoubleHitResult;
					SetImpactReactionState(NewImpactReactionState);
					CrowdNavigationReaction(IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
					// CrowdNav
				}
				else if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UALSXTCollisionInterface::StaticClass()))
				{
					if (ImpactReactionSettings.DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					}
					IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
					NewImpactReactionState.ImpactReactionParameters.BumpHit = NewDoubleHitResult;
					SetImpactReactionState(NewImpactReactionState);
					BumpReaction(IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
				}
				else
				{
					if (ImpactReactionSettings.DebugMode)
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					}
					IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
					NewImpactReactionState.ImpactReactionParameters.BumpHit = NewDoubleHitResult;
					SetImpactReactionState(NewImpactReactionState);
					BumpReaction(IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
					// BumpReactionImplementation(AlsGaitTags::Running, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt);
				}
				// GetWorld()->GetTimerManager().SetTimer(OnCapsuleHitTimerHandle, OnCapsuleHitTimerDelegate, 0.33f, false);
			}
		}
	}
}

void UALSXTImpactReactionComponent::OnRagdollingStarted()
{
	if (PreviousImpacts.IsEmpty() && PreviousAttackImpacts.IsEmpty())
	{
		FDoubleHitResult LastImpact;
		StartImpactFall(LastImpact);
		return;
	}
	else
	{
		FDoubleHitResult LastImpact;
		FAttackDoubleHitResult LastAttackImpact;

		if (!PreviousAttackImpacts.IsEmpty())
		{
			LastAttackImpact = GetLastAttackImpact();

			if (!PreviousImpacts.IsEmpty())
			{
				if (LastAttackImpact.DoubleHitResult.DateTime >= LastImpact.DateTime)
				{
					StartAttackFall(LastAttackImpact);
					return;
				}
				else
				{
					StartImpactFall(LastImpact);
					return;
				}
			}
			else
			{
				StartAttackFall(LastAttackImpact);
				return;
			}
		}

		if (!PreviousAttackImpacts.IsEmpty())
		{
			LastImpact = GetLastImpact();
			StartImpactFall(LastImpact);
			return;

		}
	}	
}

void UALSXTImpactReactionComponent::OnRagdollingEnded()
{

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
	FVector FallLocation = GetOwner()->GetActorLocation() * ImpactVelocityVector;

	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ GetOwner()->GetActorUpVector() };
	const FVector StartLocation{ GetOwner()->GetActorLocation()};
	const FVector EndLocation{ StartLocation - UpVector * 100 };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

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
		FVector FallLocation = GetOwner()->GetActorLocation() * ImpactVelocityVector;
		const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
		const auto CapsuleScale{ Capsule->GetComponentScale().Z };
		auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
		const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
		const FVector UpVector{ GetOwner()->GetActorUpVector() };
		const FVector ForwardVector{ GetOwner()->GetActorForwardVector() };
		const FVector StartLocation{ GetOwner()->GetActorLocation() + UpVector * 2.0f };
		const FVector PotentialLandLocation{ StartLocation - ForwardVector * 100.0f };
		TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
		BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForOneFrame;
		FHitResult HitResult;
		FHitResult DownTraceHitResult;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());

		if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartLocation, PotentialLandLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
		{
			Location = HitResult.ImpactPoint;
			const FVector EndLocation{ StartLocation - UpVector * 100 };

			if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), PotentialLandLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, DownTraceHitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				if (IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->IsWalkable(DownTraceHitResult))
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
				if (IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->IsWalkable(DownTraceHitResult))
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
	FALSXTBumpPoseState CurrentBumpPoseState = GetBumpPoseState();
	FALSXTBumpPoseState CurrentCrowdNavigationPoseState = GetCrowdNavigationPoseState();

	// If Same Mode and Actor
	if (CurrentBumpPoseState.Mode == ALSXTBumpPoseModeTags::CrowdNavigation && CurrentBumpPoseState.Actor == CurrentCrowdNavigationPoseState.Actor)
	{
		return;
	}
	else
	{
		FALSXTBumpPoseState EmptyCrowdNavigationPoseState;
		SetCrowdNavigationPoseState(EmptyCrowdNavigationPoseState);
		GetWorld()->GetTimerManager().ClearTimer(CrowdNavigationVelocityTimerHandle);
	}
	
	// const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	// const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	// auto CapsuleRadius{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleRadius() * 1.15 };
	// const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	// TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	// BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForOneFrame;
	// FHitResult HitResult;
	// TArray<AActor*> IgnoreActors;
	// IgnoreActors.Add(GetOwner());
	// 
	// if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), CapsuleRadius, CapsuleHalfHeight, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
	// {
	// 	if (IsValid(AnimInstance))
	// 	{
	// 		FALSXTBumpPoseState NewCrowdNavigationPoseState = GetCrowdNavigationPoseState();
	// 		NewCrowdNavigationPoseState.Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 5.0f), FVector2D(1.0f, 0.0f), HitResult.Distance);
	// 		SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
	// 	}
	// }
	// else
	// {
	// 	FALSXTBumpPoseState NewCrowdNavigationPoseState = GetCrowdNavigationPoseState();
	// 	NewCrowdNavigationPoseState.Alpha = 0.0f;
	// 	SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
	// 	GetWorld()->GetTimerManager().ClearTimer(CrowdNavigationVelocityTimerHandle);
	// 	if (!AnimInstance->IsAnyMontagePlaying())
	// 	{
	// 		AnimInstance->Montage_Resume(AnimInstance->GetCurrentActiveMontage());
	// 	}
	// }
}

void UALSXTImpactReactionComponent::StartDefensiveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(DefensiveTimerHandle, DefensiveTimerDelegate, 0.1f, false);
}

void UALSXTImpactReactionComponent::DefensiveTimer()
{
	FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
	NewDefensiveModeState.AnticipationPose = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()), NewDefensiveModeState.Side, NewDefensiveModeState.Form, ALSXTHealthTags::All).Pose;
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
	
	if (IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner()) != ALSXTDefensiveModeTags::Blocking )
	{
		StopDefensiveTimer();
	}

	//Trace for Angle of closest Attack toward control rotation
	// 
	// 
	// Find and apply correct animation pose
}

void UALSXTImpactReactionComponent::StopDefensiveTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(DefensiveTimerHandle);
}

void UALSXTImpactReactionComponent::BumpVelocityTimer()
{
	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleRadius() * 1.15 };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), CapsuleRadius, CapsuleHalfHeight, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
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
	IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);

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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded)
	{
		GetWorld()->GetTimerManager().ClearTimer(ImpactFallenTimerHandle);
	}

	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ GetOwner()->GetActorUpVector() };
	const FVector StartLocation{ GetOwner()->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackFallenTimerHandle);
	}

	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ GetOwner()->GetActorUpVector() };
	const FVector StartLocation{ GetOwner()->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
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
	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ GetOwner()->GetActorUpVector() };
	const FVector StartLocation{ GetOwner()->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	float VelocityLength{ 0.0f };
	float TraceDistance{ 0.0f };
	FVector2D VelocityRange{ 199.0, 650.0 };
	FVector2D ConversionRange{ 0.0, 1.0 };
	VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
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
	RefreshAttackFallReaction(GetOwner()->GetWorld()->GetDeltaSeconds());
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
			IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::Normal);
			AttackGetUp(Params.ImpactReactionParameters.AttackHit);
			// AttackGetUp(GetLastAttackImpact());
			// ServerGetUp();
		}
	}
}

void UALSXTImpactReactionComponent::StartAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AnticipationTimerHandle, AnticipationTimerDelegate, 0.1f, true);
}

void UALSXTImpactReactionComponent::AnticipationTimer()
{
	FRotator ControlRotation = Cast<APawn>(GetOwner())->GetControlRotation();
	FVector CharLoc = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	//FVector CameraLocation = Character->Camera->GetFirstPersonCameraLocation();
	FVector StartLocation = ForwardVector * 150 + GetOwner()->GetActorLocation();
	FVector EndLocation = ForwardVector * 200 + StartLocation;
	FVector CenterLocation = (StartLocation - EndLocation) / 8 + StartLocation;
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ImpactReactionSettings.AnticipationAreaHalfSize);
	TArray<FHitResult> OutHits;
	bool Debug = true;
	
	// Display Debug Shape
	if (ImpactReactionSettings.DebugMode)
	{
		DrawDebugBox(GetWorld(), CenterLocation, ImpactReactionSettings.AnticipationAreaHalfSize, ControlRotation.Quaternion(), FColor::Yellow, false, 0.5f, 100, 2);
	}
	
	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto ObjectType : ImpactReactionSettings.ImpactTraceObjectTypes)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(UCollisionProfile::Get()->ConvertToCollisionChannel(false, ObjectType));
	}
	
	bool isHit = GetWorld()->SweepMultiByObjectType(OutHits, StartLocation, EndLocation, ControlRotation.Quaternion(), ObjectQueryParameters, CollisionShape);
	
	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor() != GetOwner())
			{
				FTargetHitResultEntry HitResultEntry;
				HitResultEntry.Valid = true;
				HitResultEntry.DistanceFromPlayer = FVector::Distance(GetOwner()->GetActorLocation(), Hit.Location);
				// HitResultEntry.AngleFromCenter = GetOwner()->GetAngle(Hit.Location);
				HitResultEntry.HitResult = Hit;
				//Targets.Add(HitResultEntry);
			}
		}
	}
}

void UALSXTImpactReactionComponent::StopAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AnticipationTimerHandle);
}

void UALSXTImpactReactionComponent::StartFallingAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FallingAnticipationTimerHandle, FallingAnticipationTimerDelegate, 0.1f, true);
}

void UALSXTImpactReactionComponent::FallingAnticipationTimer()
{

}

void UALSXTImpactReactionComponent::StopFallingAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(FallingAnticipationTimerHandle);
}

void UALSXTImpactReactionComponent::OnCrowdNavigationReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IALSXTCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
	{
		CrowdNavigationFall();
	}
}

void UALSXTImpactReactionComponent::OnStabilizationBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IALSXTCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
	{
		// Enable AI Movement
		if (GetOwner()->GetClass()->ImplementsInterface(UALSXTAIInterface::StaticClass()))
		{
			IALSXTAIInterface::Execute_SetLockMovement(GetOwner(), false);
		}
	}
	
	if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
	{
		ClutchImpactPoint(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
		return;
	}
	else if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.OriginHitResult.HitResult.GetActor()) && IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
	{
		StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
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
		IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
		ImpactFall(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	}
}

void UALSXTImpactReactionComponent::OnAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
		if (IALSXTCollisionInterface::Execute_CanAttackFall(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
		{
			IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
			IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
			AttackFall(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			return;
		}
		else
		{
			if (IALSXTCollisionInterface::Execute_ShouldStabilize(GetOwner()))
			{
				Stabilize(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
				return;
			}
			if (IALSXTCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
			{
				ClutchImpactPoint(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
				return;
			}
			else if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()) && IALSXTCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
			{
				StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
				return;
			}
			else
			{
				return;
			}			
		}
	// }
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
	else if (IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
	{
		if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
		{
			ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::InAir)
	{
		if (GetOwner()->GetVelocity().Length() < 175)
		{
			return ALSXTImpactVelocityTags::Slow;
		}
		else if (GetOwner()->GetVelocity().Length() >= 175 && GetOwner()->GetVelocity().Length() < 350)
		{
			return ALSXTImpactVelocityTags::Moderate;
		}
		else if (GetOwner()->GetVelocity().Length() >= 350 && GetOwner()->GetVelocity().Length() < 650)
		{
			return ALSXTImpactVelocityTags::Fast;
		}
		else if (GetOwner()->GetVelocity().Length() >= 650 && GetOwner()->GetVelocity().Length() < 800)
		{
			return ALSXTImpactVelocityTags::Faster;
		}
		else if (GetOwner()->GetVelocity().Length() >= 800)
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
		FGameplayTag CharacterGait = IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner());
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
	return GetOwner()->GetVelocity().Length() >= 650;
}

float UALSXTImpactReactionComponent::GetBaseVelocityDamage()
{
	FVector2D VelocityRange{ 650.0, 2000.0 };
	FVector2D ConversionRange{ 0.0, 100.0 };
	return FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
}

void UALSXTImpactReactionComponent::OnCapsuleHitTimer()
{
	FVector ClosestLocation { ForceInit };

	ObstacleImpactHistory[ObstacleImpactHistory.Num() - 1].Actor->ActorGetDistanceToCollision(GetOwner()->GetActorLocation(), ECollisionChannel::ECC_Camera, ClosestLocation);
	double Distance = FVector::Dist(GetOwner()->GetActorLocation(), ClosestLocation);

	if (Distance > 26)
	{
		ClearObstacleImpactHistory();
		GetWorld()->GetTimerManager().ClearTimer(OnCapsuleHitTimerHandle);
	}
}

void UALSXTImpactReactionComponent::ClearObstacleImpactHistory()
{
	ObstacleImpactHistory.Empty();
}

bool UALSXTImpactReactionComponent::ValidateNewHit(AActor* ActorToCheck)
{
	double NewHitTime;
	NewHitTime = GetWorld()->GetTimeSeconds();
	bool RecentlyHit{ false };
	FImpactHistoryEntry EntryToCheck;
	EntryToCheck.Actor = ActorToCheck;

	if (ActorToCheck->IsA(ALandscape::StaticClass()) && (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner())) == AlsLocomotionModeTags::Grounded)
	{
		return false;
	}
	
	// if (OnCapsuleHitTimerHandle.IsValid())
	// {
	// 	return false;
	// }

	// GetWorld()->GetTimerManager().ClearTimer(OnCapsuleHitTimerHandle);

	// Start Timer to Clear ObstacleImpactHistory
	GetWorld()->GetTimerManager().SetTimer(OnCapsuleHitTimerHandle, OnCapsuleHitTimerDelegate, 0.1f, true);	

	if (ObstacleImpactHistory.Contains(EntryToCheck)) //Actor Already in History
	{
		return false;
	}
	else // Actor not in History. Add New Element
	{
		if (ObstacleImpactHistory.Num() >= 6)
		{
			ObstacleImpactHistory.RemoveAt(ObstacleImpactHistory.Num() - 1);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, GetWorld()->GetTimeSeconds() };
		ObstacleImpactHistory.Add(NewEntry);
		return true;
	}
}

bool UALSXTImpactReactionComponent::ValidateNewAnticipationHit(AActor* ActorToCheck)
{
	double NewAnticipationHitTime;
	NewAnticipationHitTime = GetWorld()->GetTimeSeconds();
	bool RecentlyHit{ false };
	FImpactHistoryEntry EntryToCheck;
	EntryToCheck.Actor = ActorToCheck;

	if (AnticipationImpactHistory.Contains(EntryToCheck)) //Actor Already in History
	{
		double PreviousTime;
		int FoundIndex = AnticipationImpactHistory.Find(EntryToCheck);
		FImpactHistoryEntry FoundEntry = AnticipationImpactHistory[FoundIndex];
		PreviousTime = FoundEntry.Time;
		double TimeResult;
		TimeResult = NewAnticipationHitTime - PreviousTime;
		RecentlyHit = TimeResult <= 0.33;

		// Prioritize Closer Actors
		// if ((GetOwner()->GetActorLocation() - HitResult.GetActor()->GetActorLocation()).Size() < (GetOwner()->GetActorLocation() - GetCrowdNavigationPoseState().Actor->GetActorLocation()).Size())
		// {
		// 
		// }

		AnticipationImpactHistory.RemoveAt(FoundIndex);
		if (AnticipationImpactHistory.Num() >= 6)
		{
			AnticipationImpactHistory.RemoveAt(0);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, GetWorld()->GetTimeSeconds() };
		AnticipationImpactHistory.Add(NewEntry);
		return !RecentlyHit;
	}
	else // Actor not in History. Add New Element
	{
		if (AnticipationImpactHistory.Num() >= 6)
		{
			AnticipationImpactHistory.RemoveAt(0);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, GetWorld()->GetTimeSeconds() };
		AnticipationImpactHistory.Add(NewEntry);
		return true;
	}
}


// Dynamically react to colliding objects when moving fast
void UALSXTImpactReactionComponent::AnticipationTrace()
{
	// Check Status
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::InAir && IALSXTCollisionInterface::Execute_ShouldPerformDefensiveReaction(GetOwner()) && GetOwner()->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity) && (IALSXTCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal || IALSXTCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::KnockedDown))
	{
		// Not if performing Action or already in certain Defensive Modes
		if ((IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag || IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BumpInterruptableLocomotionActions.HasTag(IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()))) && (IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::Anticipation || IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::BraceForImpact || IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::ClutchImpactPoint))
		{
			FGameplayTag CharacterCombatStance = IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner());
			const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
			const auto CapsuleScale{ Capsule->GetComponentScale().Z };
			auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
			const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
			const FVector UpVector{ GetOwner()->GetActorUpVector() };
			const FVector StartLocation{ GetOwner()->GetActorLocation() + (UpVector * CapsuleHalfHeight / 2) };
			TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
			BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
			TArray<FHitResult> HitResults;
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(GetOwner());
			float VelocityLength{ 0.0f };
			float TraceDistance{ 0.0f };
			FVector2D VelocityRange{ 199.0, 650.0 };
			FVector2D ConversionRange{ 0.0, 1.0 };
			FVector RangedVelocity = GetOwner()->GetVelocity();
			VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
			TraceDistance = ImpactReactionSettings.MaxBumpDetectionDistance;
			const FVector EndLocation{ StartLocation + (GetOwner()->GetVelocity() * TraceDistance) };

			if (GEngine && ImpactReactionSettings.DebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::SanitizeFloat(HitResults.Num()));
			}
			

			// Trace
			if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CapsuleRadius, CapsuleHalfHeight / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode,HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				for (FHitResult HitResult : HitResults)
				{
					if (ValidateNewAnticipationHit(HitResult.GetActor()))
					{
						FGameplayTag FormTag = ConvertPhysicalSurfaceToFormTag(HitResult.PhysMaterial->SurfaceType);
						FGameplayTag VelocityTag = ALSXTImpactVelocityTags::Moderate;
						FGameplayTag HealthTag = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));
						FGameplayTag SideTag = LocationToImpactSide(HitResult.ImpactPoint);
						TEnumAsByte<EPhysicalSurface> PhysSurf = HitResult.PhysMaterial->SurfaceType;
						UAnimSequenceBase* SelectedAnimation{ nullptr };
						SelectedAnimation = SelectBraceForImpactPose(SideTag, FormTag);
						FTransform HitTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactPoint), HitResult.ImpactPoint, {1.0, 1.0, 1.0} };

						// Defensive Mode State
						FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
						// NewDefensiveModeState.Form = FormTag;
						// NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::BraceForImpact;
						NewDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
						// NewDefensiveModeState.ObstaclePose = SelectedAnimation;
						NewDefensiveModeState.AnticipationPose = SelectedAnimation;
						// NewDefensiveModeState.AnticipationSide = SideTag;
						// NewDefensiveModeState.AnticipationHeight = LocationToImpactHeight(HitResult.ImpactPoint);
						NewDefensiveModeState.AnticipationTransform = HitTransform;
						NewDefensiveModeState.Transform = HitTransform;
						NewDefensiveModeState.Velocity = VelocityTag;

						// Mass and Velocity
						// if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
						// {
						// 	IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), DoubleHitResult.HitResult.Mass);
						// 	IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), DoubleHitResult.HitResult.Velocity);
						// }
						// else
						// {
						// 	DoubleHitResult.HitResult.Mass = 100.00f;
						// 	DoubleHitResult.HitResult.Velocity = HitResult.GetActor()->GetVelocity();
						// }

						// Populate NewImpactReactionState
						FALSXTImpactReactionState NewImpactReactionState = IALSXTCollisionInterface::Execute_GetImpactReactionState(GetOwner());
						NewImpactReactionState.ImpactReactionParameters.BraceForImpactPose.Pose = SelectedAnimation;
						// NewImpactReactionState.ImpactReactionParameters.DefensivePose = SelectedAnimation;
						NewImpactReactionState.ImpactReactionParameters.ImpactHeight = 0.5f;
						//NewImpactReactionState.ImpactReactionParameters.ImpactLocation = HitResult.ImpactPoint;
						NewImpactReactionState.ImpactReactionParameters.TargetRelativeLocation = HitResult.ImpactPoint;
						

						// FAnticipationPose NewAnticipationPose;
						// NewAnticipationPose.Pose

						IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
						IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), NewDefensiveModeState.Mode);
						SetImpactReactionState(NewImpactReactionState);
						
						// Set Physical Animation Component Curves/Profile Here
						// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, DoubleHitResult.HitResult.HitResult.BoneName);

						if (ImpactReactionSettings.DebugMode)
						{
							FString BumpHit = "Anticipate: ";
							BumpHit.Append(HitResult.GetActor()->GetName());
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, BumpHit);
						}
					}
				}
			}

		}
	}
}

void UALSXTImpactReactionComponent::RefreshObstacleNavigationPoses()
{
	FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
	NewDefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner())));
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
}

void UALSXTImpactReactionComponent::RefreshCrowdNavigationPoses()
{
	FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
	NewDefensiveModeState.CrowdNavigationPoseSet =	SelectCrowdNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()));
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
}

void UALSXTImpactReactionComponent::RefreshBlockingPoses()
{
	FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
	NewDefensiveModeState.AnticipationPoseSet = SelectBlockingPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), ALSXTImpactFormTags::Blunt, ALSXTImpactVelocityTags::Moderate);
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
}

FALSXTDefensivePoseSet UALSXTImpactReactionComponent::SelectObstacleNavigationPoses(const FGameplayTag& Overlay, const FGameplayTag& Health)
{
	FALSXTDefensivePoseSet DefensivePoseStanceSet;
	TArray<FObstaclePose> ObstaclePoses = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ObstacleNavigationPoses;
	FGameplayTagContainer ParameterTagContainer;
	ParameterTagContainer.AddTagFast(Overlay);
	ParameterTagContainer.AddTagFast(Health);
	
	for (FObstaclePose ObstaclePose : ObstaclePoses)
	{
		FGameplayTagContainer EntryTagContainer;
		EntryTagContainer.AppendTags(ObstaclePose.Overlay);
		EntryTagContainer.AppendTags(ObstaclePose.Health);
	
		if (EntryTagContainer.HasAll(ParameterTagContainer))
		{
			DefensivePoseStanceSet = ObstaclePose.Poses;
			return DefensivePoseStanceSet;
		}
	}
	return DefensivePoseStanceSet;
}

FALSXTDefensivePoseStanceSet UALSXTImpactReactionComponent::SelectCrowdNavigationPoses(const FGameplayTag& Overlay)
{
	FALSXTDefensivePoseStanceSet DefensivePoseStanceSet;
	TArray<FCrowdNavigationPoses> CrowdNavigationPoses = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->CrowdNavigationPosesNew;
	
	for (FCrowdNavigationPoses CrowdNavigationPose : CrowdNavigationPoses)
	{
		FGameplayTagContainer EntryTagContainer;
		EntryTagContainer.AppendTags(CrowdNavigationPose.Overlay);
	
		if (EntryTagContainer.HasTag(Overlay))
		{
			DefensivePoseStanceSet = CrowdNavigationPose.Poses;
			return DefensivePoseStanceSet;
		}
	}

	return DefensivePoseStanceSet;
}

FALSXTDefensivePoseSet UALSXTImpactReactionComponent::SelectBlockingPoses(const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Variant)
{
	FALSXTDefensivePoseSet DefensivePoseSet;
	TArray<FAnticipationPoses> BlockingPoses = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BlockingPoses;
	FGameplayTagContainer ParameterTagContainer;
	ParameterTagContainer.AddTagFast(Overlay);
	ParameterTagContainer.AddTagFast(Form);
	ParameterTagContainer.AddTagFast(Variant);


	for (FAnticipationPoses BlockingPose : BlockingPoses)
	{
		FGameplayTagContainer EntryTagContainer;
		EntryTagContainer.AppendTags(BlockingPose.Overlay);
		EntryTagContainer.AppendTags(BlockingPose.Form);
		EntryTagContainer.AppendTags(BlockingPose.Variant);

		if (EntryTagContainer.HasAll(ParameterTagContainer))
		{
			DefensivePoseSet = BlockingPose.Poses;
			return DefensivePoseSet;
		}
	}

	return DefensivePoseSet;
}

// Grounded Bump and Crowd Navigation
void UALSXTImpactReactionComponent::ObstacleTrace()
{
	if (IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()) != AlsStanceTags::Standing)
	{
		IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
		return;
	}

	if (IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner()) != FGameplayTag::EmptyTag || IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == AlsLocomotionActionTags::Stabilization)
	{
		return;
	}
	
	const auto* Capsule{ IALSXTCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	const auto CapsuleHalfHeight{ Capsule->GetScaledCapsuleHalfHeight() };
	const FVector UpVector{ GetOwner()->GetActorUpVector() };
	float TraceDistance{ 5.0f };
	float CharacterVelocity = GetOwner()->GetVelocity().Length();

	if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Walking)
	{
		TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 175.0f), FVector2D(0.0f, 0.125f), CharacterVelocity);
	}
	if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Running)
	{
		TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 375.0f), FVector2D(0.125f, 0.14f), CharacterVelocity);
	}
	if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
	{
		TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 650.0f), FVector2D(0.14f, 0.18f), CharacterVelocity);
	}

	const FVector StartLocation{ GetOwner()->GetActorLocation() + (UpVector * 20) };
	const FVector EndLocation{ StartLocation + (GetOwner()->GetVelocity() * TraceDistance) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	// BumpDebugMode = EDrawDebugTrace::ForOneFrame;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	// if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == AlsLocomotionActionTags::Sliding)
	// {
	// 	CapsuleRadius = CapsuleRadius * 2.0;
	// 	TraceDistance = ImpactReactionSettings.MaxSlideToCoverDetectionDistance;
	// }
	// else if (!IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->IsPlayingNetworkedRootMotionMontage())
	// {
	// 	TraceDistance = ImpactReactionSettings.MaxBumpDetectionDistance;
	// }

	// Check Status
	if (IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()) == AlsStanceTags::Standing && IALSXTCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded && (IALSXTCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal || IALSXTCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::KnockedDown))
	{
		// Not if performing Action or already in certain Defensive Modes
		if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag && (IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::Anticipation || IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::BraceForImpact || IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::ClutchImpactPoint))
		{
			bool isHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, 44, 70, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f);
			if (isHit)
			{
				for (FHitResult HitResult : HitResults)
				{
					if (ValidateNewAnticipationHit(HitResult.GetActor()))
					{

						FVector MeshHitLocation{ ForceInit };
						FVector ActorVelocity{ FVector::ZeroVector };
						float ActorMass{ 0.0f };
						FGameplayTag Velocity{ FGameplayTag::EmptyTag };
						FGameplayTag Form{ FGameplayTag::EmptyTag };
						FVector AnticipationPoint{ FVector::ZeroVector };
						AnticipationPoint = HitResult.ImpactPoint;
						FALSXTDefensiveModeState DefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
						FAnticipationPose Montage;
						FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));
						FGameplayTag Stance = IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner());
						FGameplayTag Side = LocationToImpactSide(AnticipationPoint);
						FGameplayTag Height = LocationToImpactHeight(AnticipationPoint);
						float LocationDotProduct = FVector::DotProduct(GetOwner()->GetActorForwardVector(), (HitResult.GetActor()->GetActorForwardVector()));
						DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
						DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);

						// If Character
						if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCharacterInterface::StaticClass()))
						{
							FGameplayTag HitActorCombatStance = IALSXTCharacterInterface::Execute_GetCombatStance(HitResult.GetActor());
							FGameplayTag CharacterCombatStance = IALSXTCharacterInterface::Execute_GetCombatStance(GetOwner());
							IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
							IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);
							FTransform AnticipationTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
							IALSXTCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationTransform, AnticipationPoint);
							DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(AnticipationPoint), AnticipationPoint, {1.0, 1.0, 1.0} };
							FGameplayTag ImpactStrength = ConvertVelocityToStrength(GetOwner()->GetVelocity() + HitResult.GetActor()->GetVelocity());							

							// Are Characters facing each other?
							if (LocationDotProduct <= 0) // Replase with IALSXTAIInterface::Execute_CanAISeeThreatPoint or similar?
							{
								//Is Current Character AI?
								if (!IALSXTCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
								{													
									// Determine how AI should respond based on Overlay and Combat Stance

									if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTAIInterface::StaticClass()))
									{
										if (IALSXTAIInterface::Execute_ShouldAIAnticipateImpact(GetOwner()))
										{
											if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCombatInterface::StaticClass()))
											{
												// Is HitActor Hostile?
												if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(HitResult.GetActor()) != ALSXTCombatStanceTags::Neutral)
												{
													// Is this Character already Combat Ready?
													if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
													{

														if (IALSXTCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
														{
															if (IALSXTAIInterface::Execute_ShouldAIAvoid(GetOwner()))
															{
																Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Avoiding;

															}
															if (IALSXTAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
															{

															}

															if (IALSXTAIInterface::Execute_ShouldAIBlock(GetOwner()))
															{
																Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Blocking;
															}
															DefensiveModeState.Mode = DefensiveMode;
															DefensiveModeState.AnticipationMode = DefensiveMode;
															// DefensiveModeState.Montage = Montage.Pose;
															DefensiveModeState.AnticipationPose = Montage.Pose;
															IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
															IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
															return;

														}
														else
														{
															if (IALSXTAIInterface::Execute_ShouldAIAvoid(GetOwner()))
															{
																Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Avoiding;
															}
															if (IALSXTAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
															{

															}

															if (IALSXTAIInterface::Execute_ShouldAIBlock(GetOwner()))
															{
																Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Blocking;
															}
															DefensiveModeState.Mode = DefensiveMode;
															DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
															// DefensiveModeState.Montage = Montage.Pose;
															DefensiveModeState.AnticipationPose = Montage.Pose;
															IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
															IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
															return;
														}

													}
													else
													{
														if (IALSXTAIInterface::Execute_ShouldAIAvoid(GetOwner()))
														{
															Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
															DefensiveMode == ALSXTDefensiveModeTags::Avoiding;

														}
														if (IALSXTAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
														{

														}

														if (IALSXTAIInterface::Execute_ShouldAIBlock(GetOwner()))
														{
															Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IALSXTCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
															DefensiveMode == ALSXTDefensiveModeTags::Blocking;
														}
														DefensiveModeState.Mode = DefensiveMode;
														DefensiveModeState.AnticipationMode = DefensiveMode;
														// DefensiveModeState.Montage = Montage.Pose;
														DefensiveModeState.AnticipationPose = Montage.Pose;
														IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
														IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
														return;
													}
												}
												else
												{
													if (IALSXTCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
													{
														Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
														DefensiveModeState.AnticipationPose = Montage.Pose;
														// DefensiveModeState.Montage = Montage.Pose;
														DefensiveMode == ALSXTDefensiveModeTags::BraceForImpact;
													}
													else
													{
														// Crowd Nav
														Montage.Pose = SelectCrowdNavigationPose(Side, Form);
														// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
														DefensiveModeState.ObstaclePose = Montage.Pose;
														DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
													}

													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
													return;
												}
											}
											else
											{
												if (IALSXTCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
												{
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													DefensiveModeState.Montage = Montage.Pose;
													DefensiveMode == ALSXTDefensiveModeTags::BraceForImpact;
												}
												else
												{
													// Crowd Nav
													Montage.Pose = SelectCrowdNavigationPose(Side, Form);
													DefensiveModeState.ObstaclePose = Montage.Pose;
													// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
													// DefensiveModeState.Montage = SelectCrowdNavigationPose(Side, Form);
													DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
												}

												DefensiveModeState.Mode = DefensiveMode;
												DefensiveModeState.AnticipationMode = DefensiveMode;
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
										}
									}
									else
									{
										if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
										{
											if (DefensiveModeState.Mode == ALSXTDefensiveModeTags::Blocking)
											{
												const FTransform ConstructedTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
												DefensiveModeState.Transform = ConstructedTransform;
												if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
												{
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													DefensiveMode == ALSXTDefensiveModeTags::Blocking;
													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													DefensiveModeState.AnticipationPose = Montage.Pose;
													// DefensiveModeState.Montage = Montage.Pose;
												}
												else
												{
													Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
													DefensiveMode == ALSXTDefensiveModeTags::Blocking;
													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													DefensiveModeState.AnticipationPose = Montage.Pose;
												}
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
											else
											{
												if (IALSXTCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
												{
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													DefensiveModeState.AnticipationPose = Montage.Pose;
													DefensiveMode == ALSXTDefensiveModeTags::BraceForImpact;
													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
													return;
												}
												else
												{
													return;
												}

												
											}
										}
										else
										{
											if (DefensiveModeState.Mode == ALSXTDefensiveModeTags::Blocking)
											{
												const FTransform ConstructedTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
												DefensiveModeState.Transform = ConstructedTransform;
												if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
												{
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													DefensiveMode = ALSXTDefensiveModeTags::Blocking;
													DefensiveModeState.AnticipationPose = Montage.Pose;
													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													// DefensiveModeState.Montage = Montage.Pose;
												}
												else
												{
													Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
													DefensiveMode = ALSXTDefensiveModeTags::Blocking;
													DefensiveModeState.AnticipationPose = Montage.Pose;
													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													// DefensiveModeState.Montage = Montage.Pose;
												}
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
											else
											{
												if (IALSXTCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
												{
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													DefensiveModeState.ObstaclePose = Montage.Pose;
													DefensiveModeState.Mode = ALSXTDefensiveModeTags::BraceForImpact;
													DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
													DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
												}
												else
												{
													Montage.Pose = SelectCrowdNavigationPose(Side, Form);
													DefensiveModeState.ObstaclePose = Montage.Pose;
													DefensiveModeState.Mode = ALSXTDefensiveModeTags::CrowdNavigation;
													DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
													DefensiveMode = ALSXTDefensiveModeTags::CrowdNavigation;
												}

												DefensiveModeState.Mode = DefensiveMode;
												DefensiveModeState.AnticipationMode = DefensiveMode;
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
										}
										
										
									}

								}
								else // Is Player
								{
									// if ((IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) == ALSXTCombatStanceTags::Neutral || (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Ready || IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != // ALSXTCombatStanceTags::Aiming)) && IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner()) == ALSXTDefensiveModeTags::Blocking)
									// {
									// 
									// 
									// }

									// If Combat Ready
									if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
									{
										// If Player, and Player is Blocking Begin Updating Threat Location
										if (DefensiveModeState.Mode == ALSXTDefensiveModeTags::Blocking)
										{
											DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
											// IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
											// update anim if necessary
											// Check if still left/right top/middle/bottom
											if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
											{
												Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
												// DefensiveModeState.Montage = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
												// DefensiveModeState.Montage = Montage.Pose;
												DefensiveModeState.AnticipationPose = Montage.Pose;
											}
											else
											{
												// Crowd Nav
												Montage.Pose = SelectCrowdNavigationPose(Side, Form);
												DefensiveModeState.ObstaclePose = Montage.Pose;
												// DefensiveModeState.Montage = SelectCrowdNavigationPose(Side, Form);
											}
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
											return;
										}
										else // Not Blocking
										{
											return;
											
											// if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
											// {
											// 	// Bump
											// 	DefensiveModeState.Location = HitResult.Location;
											// 	Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
											// 	DefensiveModeState.Montage = Montage.Pose;
											// 	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											// 	IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
											// 	return;
											// }
											// else
											// {
											// 	return;
											// }
										}
									}
									else // Combat Neutral
									{
										if (DefensiveModeState.Mode == ALSXTDefensiveModeTags::Blocking)
										{
											// update anim if necessary
											// Check if still left/right top/middle/bottom
											if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
											{
												// Bump
												// Montage.Pose = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
												Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
												// DefensiveModeState.Montage = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
												// DefensiveModeState.Montage = Montage.Pose;
												DefensiveModeState.AnticipationPose = Montage.Pose;
											}
											else
											{
												// Update Blocking
												const FTransform ConstructedTransform { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
												DefensiveModeState.Transform = ConstructedTransform;
												
												FALSXTDefensiveModeState CurrentDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
												FGameplayTagContainer CurrentImpactTagsContainer;
												FGameplayTagContainer NewImpactTagsContainer;

												// Update Animation
												if (!CurrentImpactTagsContainer.HasAllExact(NewImpactTagsContainer))
												{
													Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
													DefensiveModeState.AnticipationPose = Montage.Pose;
													// DefensiveModeState.Montage = Montage.Pose;
												}												
											}
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
											return;
										}
										else
										{
											DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
											// update anim if necessary
											// Check if still left/right top/middle/bottom
											if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
											{
												Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
												DefensiveModeState.AnticipationPose = Montage.Pose;
												DefensiveModeState.Mode = ALSXTDefensiveModeTags::BraceForImpact;
												DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
												DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
											}
											else
											{
												Montage.Pose = SelectCrowdNavigationPose(Side, Form);
												DefensiveModeState.ObstaclePose = Montage.Pose;
												DefensiveModeState.Mode = ALSXTDefensiveModeTags::CrowdNavigation;
												DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::CrowdNavigation;
												DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
											}
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
											return;
										}
									}
								}
							}
							else
							{
								if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
								{
									if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.AnticipationPose = Montage.Pose;
										// DefensiveModeState.Montage = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
										IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
										IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									}
									else
									{
										return;
									}
								}
								else
								{
									// If not facing each other juet check if CrowdNav or Bump
									if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										// Bump
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.AnticipationPose = Montage.Pose;
									}
									else
									{
										// Crowd Nav
										Montage.Pose = SelectCrowdNavigationPose(Side, Form);
										DefensiveModeState.ObstaclePose = Montage.Pose;
									}
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									return;
								}


							}
						}
						else // Not Character
						{
							// Collision Interface
							if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
							{
								
								if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
								{
									return;
								}
								else
								{
									if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										// Mass, Velocity, AnticipationPoint, Side
										IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
										IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);

										FTransform AnticipationTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };

										IALSXTCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationTransform, AnticipationPoint);

										// Bump
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(AnticipationPoint), AnticipationPoint, { 1.0f, 1.0f, 1.0f } };
										DefensiveModeState.AnticipationPose = Montage.Pose;
										DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
										DefensiveModeState.Mode = DefensiveMode;
										DefensiveModeState.AnticipationMode = DefensiveMode;
									}
									else
									{
										// Crowd Nav
										Montage.Pose = SelectCrowdNavigationPose(Side, Form);
										DefensiveModeState.ObstaclePose = Montage.Pose;
										DefensiveModeState.Mode = ALSXTDefensiveModeTags::CrowdNavigation;
										DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::CrowdNavigation;
										DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
									}
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									return;
								}								
							}
							else
							{
								if (IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) == ALSXTCombatStanceTags::Neutral || IALSXTCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral && IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
								{
									// Mass, Velocity, AnticipationPoint, Side
									if (HitResult.GetComponent()->Mobility == EComponentMobility::Movable && HitResult.GetComponent()->IsSimulatingPhysics())
									{
										ActorMass = HitResult.GetComponent()->GetMass();
									}
									else
									{
										ActorMass = 100000.00;
									}

									ActorVelocity = HitResult.GetActor()->GetVelocity();

									// Get Form
									UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
									TArray<FALSXTFormSurfaces> FormSurfaces = SelectedImpactReactionSettings->FormSurfaces;

									// Get Form from FormSurfaces
									if (FormSurfaces.Num() < 1)
									{
										Form = ALSXTImpactFormTags::Blunt;
									}
									// Get only Form from FormSurfaces
									if (FormSurfaces.Num() == 1)
									{
										Form = FormSurfaces[0].Form;
									}
									// Get Form from FormSurfaces
									if (FormSurfaces.Num() > 1)
									{
										for (FALSXTFormSurfaces FormSurface : FormSurfaces)
										{
											if (FormSurface.Surfaces.Contains(HitResult.PhysMaterial->SurfaceType))
											{
												Form = FormSurface.Form;
												break;
											}
										}
									}								

									if (IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										// BraceForImpact
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.AnticipationPose = Montage.Pose;
										DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
										DefensiveModeState.AnticipationSide = Side;
										DefensiveModeState.AnticipationHeight = Height;
										DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
										TArray<FName> AffectedBones = GetAffectedBones(Side, Height);
										IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Bump, AffectedBones);
									}
									else
									{
										// ObstacleNav
										Montage.Pose = SelectCrowdNavigationPose(Side, Form);
										DefensiveModeState.ObstaclePose = Montage.Pose;
										// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
										DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::ObstacleNavigation;
										DefensiveModeState.ObstacleSide = Side;
										DefensiveModeState.ObstacleHeight = Height;
										DefensiveMode == ALSXTDefensiveModeTags::ObstacleNavigation;

										FALSXTDefensiveModeState PreviousDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());

										if (PreviousDefensiveModeState.ObstacleSide != FGameplayTag::EmptyTag && PreviousDefensiveModeState.ObstacleHeight != FGameplayTag::EmptyTag)
										{
											if (PreviousDefensiveModeState.ObstacleSide != Side && PreviousDefensiveModeState.ObstacleHeight != Height)
											{
												IALSXTCollisionInterface::Execute_ResetCharacterPhysicalAnimationMode(GetOwner());
											}
										}	

										TArray<FName> AffectedBones = GetAffectedBones(Side, Height);
										IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Navigation, AffectedBones);
									}
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
									IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									return;
								}	
							}
						}
					}
				}
			}
			else
			{
				IALSXTCollisionInterface::Execute_ResetCharacterPhysicalAnimationMode(GetOwner());
				if (!IALSXTCharacterInterface::Execute_IsBlocking(GetOwner()) || IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::ClutchImpactPoint)
				{
					IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
				}

			}
		}
	}
}

void UALSXTImpactReactionComponent::ServerSetDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
}

// ImpactReaction State
void UALSXTImpactReactionComponent::SetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState)
{
	const auto PreviousImpactReactionState{ ImpactReactionState };
	ImpactReactionState = NewImpactReactionState;
	OnImpactReactionStateChanged(PreviousImpactReactionState);

	// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerSetImpactReactionState(NewImpactReactionState);
	// }
	// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	// {
	// 	SetImpactReactionStateImplementation(NewImpactReactionState);
	// }


	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
	{
		ServerSetImpactReactionState(NewImpactReactionState);
	}
}

void UALSXTImpactReactionComponent::SetImpactReactionStateImplementation(const FALSXTImpactReactionState& NewImpactReactionState)
{
	const auto PreviousImpactReactionState{ ImpactReactionState };
	ImpactReactionState = NewImpactReactionState;
	OnImpactReactionStateChanged(PreviousImpactReactionState);
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

	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
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

	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
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

void UALSXTImpactReactionComponent::OnReplicate_AnticipationImpactHistory(const TArray<FImpactHistoryEntry> PreviousAnticipationImpactHistory)
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
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAnticipationReaction(AnticipationPoint);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAnticipationReaction(AnticipationPoint);
	}
}

// Crowd Navigation Reaction
void UALSXTImpactReactionComponent::CrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerCrowdNavigationReaction(Gait, Side, Form);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		CrowdNavigationReactionImplementation(Gait, Side, Form);
	}
}

// Bump Reaction
void UALSXTImpactReactionComponent::BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	FString ClientName = GetOwner()->GetDebugName(GetOwner());
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerBumpReaction(Gait, Side, Form);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		BumpReactionImplementation(Gait, Side, Form);
	}
}

// Impact Reaction
void UALSXTImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);

	// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerImpactReaction(Hit);
	// }
	// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	// {
	// 	StartImpactReaction(Hit);
	// }
}

// Attack Reaction

// bool UALSXTImpactReactionComponent::ServerAttackReaction_Validate(FAttackDoubleHitResult Hit)
// {
// 	return true;
// }

void UALSXTImpactReactionComponent::OnStaticMeshAttackCollision(FAttackDoubleHitResult Hit)
{
	FString LocalRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetLocalRole());
	FString RemoteRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetRemoteRole());
	FString DebugString = GetOwner()->GetDebugName(GetOwner());
	DebugString.Append(" AttackReaction : ");
	DebugString.Append(" Local Role : ");
	DebugString.Append(LocalRoleString);
	DebugString.Append(" Remote Role: ");
	DebugString.Append(RemoteRoleString);

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerOnStaticMeshAttackCollision(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		ServerOnStaticMeshAttackCollision(Hit);
		MulticastOnStaticMeshAttackCollision(Hit);
	}
}

void UALSXTImpactReactionComponent::MulticastOnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	OnStaticMeshAttackCollisionImplementation(Hit);
}

void UALSXTImpactReactionComponent::ServerOnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastOnStaticMeshAttackCollision(Hit);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::OnStaticMeshAttackCollisionImplementation(FAttackDoubleHitResult Hit)
{
	if (IsValid(Hit.DoubleHitResult.HitResult.HitResult.GetComponent()))
	{
		PreviousAttackImpacts.Add(Hit);

		UNiagaraSystem* Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
		USoundBase* Audio = GetImpactReactionSound(Hit.DoubleHitResult).Sound;
		UMaterialInterface* FrontDecal = GetImpactDecal(Hit.DoubleHitResult);
		PreviousAttackImpacts.Add(Hit);
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

		if (FrontDecal->IsValidLowLevelFast())
		{
			UDecalComponent* FrontDecalComponent;
			FrontDecalComponent = UGameplayStatics::SpawnDecalAttached(FrontDecal, { 10.0f, 20.0f, 20.0f },
				Hit.DoubleHitResult.HitResult.HitResult.GetComponent(), NAME_None, Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint,
				UKismetMathLibrary::MakeRotFromX(Hit.DoubleHitResult.HitResult.HitResult.ImpactNormal), EAttachLocation::KeepWorldPosition, 5.0);

			if (FrontDecalComponent->IsValidLowLevelFast())
			{
				FrontDecalComponent->SetFadeOut(5.0, 2.0, false);
			}
			
		}

		if (Audio->IsValidLowLevelFast())
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
		
		if (Particle->IsValidLowLevelFast())
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GetImpactReactionParticle(Hit.DoubleHitResult), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, NewRotation, { 1.0f, 1.0f, 1.0f }, true, true, ENCPoolMethod::None, true);
		}

	}
}

void UALSXTImpactReactionComponent::AttackReaction(FAttackDoubleHitResult Hit)
{
	FString LocalRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetLocalRole());
	FString RemoteRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetRemoteRole());
	FString DebugString = GetOwner()->GetDebugName(GetOwner());
	DebugString.Append(" AttackReaction : ");
	DebugString.Append(" Local Role : ");
	DebugString.Append(LocalRoleString);
	DebugString.Append(" Remote Role: ");
	DebugString.Append(RemoteRoleString);

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackReaction(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackReaction(Hit);
		ServerAttackReaction(Hit);
		MulticastAttackReaction(Hit);
	}
}

void UALSXTImpactReactionComponent::MulticastAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackReaction(Hit);
}

void UALSXTImpactReactionComponent::ServerAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastAttackReaction(Hit);
	StartAttackReaction(Hit);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::AttackReactionImplementation(FAttackDoubleHitResult Hit)
{
	UAnimMontage* Montage{ nullptr };

	if (IsValid(Hit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		
		FAttackReactionAnimation SelectedAttackReaction = SelectAttackReactionMontage(Hit);
		Montage = SelectedAttackReaction.Montage.Montage;
		PreviousAttackImpacts.Add(Hit);

		if ((!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reaction Montage Invalid"));
			return;
		}

		UNiagaraSystem* Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
		USoundBase* Audio = GetImpactReactionSound(Hit.DoubleHitResult).Sound;
		const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

		IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

		// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

		StartAttackReactionImplementation(Hit, SelectedAttackReaction.Montage, Particle, Audio);
		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);
	}
}

void UALSXTImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
{
	if (IsValid(Hit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		UAnimMontage* Montage{ nullptr };
		FAttackReactionAnimation SelectedAttackReaction = SelectAttackReactionMontage(Hit);
		Montage = SelectedAttackReaction.Montage.Montage;
		PreviousAttackImpacts.Add(Hit);

		if ((!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reaction Montage Invalid"));
			return;
		}

		UNiagaraSystem* Particle = GetImpactReactionParticle(Hit.DoubleHitResult);
		USoundBase* Audio = GetImpactReactionSound(Hit.DoubleHitResult).Sound;
		const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

		IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

		// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
			// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

		if (GetOwner()->GetLocalRole() >= ROLE_Authority)
		{
			FString LocalRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetLocalRole());
			FString RemoteRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetRemoteRole());
			FString DebugString = GetOwner()->GetDebugName(GetOwner());
			DebugString.Append(" ServerStartAttackReaction : ");
			DebugString.Append(" Local Role : ");
			DebugString.Append(LocalRoleString);
			DebugString.Append(" Remote Role: ");
			DebugString.Append(RemoteRoleString);
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DebugString);
			ServerStartAttackReaction(Hit, SelectedAttackReaction.Montage, Particle, Audio);
		}
		else
		{
			FString LocalRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetLocalRole());
			FString RemoteRoleString = StaticEnum<ENetRole>()->GetValueAsString(GetOwner()->GetRemoteRole());
			FString DebugString = GetOwner()->GetDebugName(GetOwner());
			DebugString.Append(" MulticastStartAttackReaction : ");
			DebugString.Append(" Local Role : ");
			DebugString.Append(LocalRoleString);
			DebugString.Append(" Remote Role: ");
			DebugString.Append(RemoteRoleString);
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DebugString);
			IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
			// MulticastStartAttackReaction(Hit, SelectedAttackReaction.Montage, Particle, Audio);
			StartAttackReactionImplementation(Hit, SelectedAttackReaction.Montage, Particle, Audio);
			OnAttackReactionStarted(Hit);
		}
	}
}

void UALSXTImpactReactionComponent::ServerStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	MulticastStartAttackReaction(Hit, Montage, Particle, Audio);
	GetOwner()->ForceNetUpdate();
}

// bool UALSXTImpactReactionComponent::ServerStartAttackReaction_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
// {
// 	return true;
// }

void UALSXTImpactReactionComponent::MulticastStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartAttackReactionImplementation(Hit, Montage, Particle, Audio);
}

void UALSXTImpactReactionComponent::StartAttackReactionImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsAttackReactionAllowedToStart(Montage.Montage))
	{
		PreviousAttackImpacts.Add(Hit);
		//Anticipation
		FALSXTDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
		// Character->SetFacialExpression();

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

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
			// Ignore HitActor, Find Forward Vector of Atttack
			// Do one line trace from back + attack strength for distance/angle
			// Do one line trace opposite of attack angle (toward source of attack), with less strength influence
			// Determine Spatter based on Form, Velocity
			// Draw to meshes
			TArray<AActor*> BloodSpatterIgnoreActors;
			TArray<FHitResult> FrontBloodSpatterHitResults;
			TArray<FHitResult> RearBloodSpatterHitResults;
			FRotator ImpactRotation {ForceInit};
			TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
			BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
			FVector RearVector{ ForceInit };
			FVector FrontVector{ ForceInit };
			ImpactRotation = UKismetMathLibrary::MakeRotFromX(Hit.DoubleHitResult.HitResult.HitResult.ImpactNormal);

			// Hit.DoubleHitResult.HitResult.HitResult.ImpactNormal
			float FrontTraceDistance { 40.0f};
			float RearTraceDistance{ 80.0f };
			FVector CollectiveVelocity = Hit.DoubleHitResult.HitResult.Velocity + GetOwner()->GetVelocity();
			FGameplayTag SpatterVelocityTag = ConvertVelocityToTag(CollectiveVelocity);

			// Check if Hit should spawn Front Spatter
			if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, FrontVector * FrontTraceDistance, 15, 15, ImpactReactionSettings.BumpTraceObjectTypes, false, BloodSpatterIgnoreActors, BumpDebugMode, FrontBloodSpatterHitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				for (FHitResult HitResult : FrontBloodSpatterHitResults)
				{
					// DrawDebugCapsule(GetWorld(), HitResult.ImpactPoint, 15, 15, ImpactRotation.Quaternion(), FColor::Red, false, 1.5, 2, 1);
				}
				
			}

			// Check if Hit should spawn Rear Spatter
			// if (Hit.BaseDamage > 0.25 && Hit.Strength == ALSXTActionStrengthTags::)

			if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, RearVector * RearTraceDistance , 15, 15, ImpactReactionSettings.BumpTraceObjectTypes, false, BloodSpatterIgnoreActors, BumpDebugMode, RearBloodSpatterHitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				for (FHitResult HitResult : RearBloodSpatterHitResults)
				{
					// DrawDebugCapsule(GetWorld(), HitResult.ImpactPoint, 15, 15, ImpactRotation.Quaternion(), FColor::Red, false, 1.5, 2, 1);
				}

			}	
			
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GetImpactReactionParticle(Hit.DoubleHitResult), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, NewRotation, { 1.0f, 1.0f, 1.0f }, true, true, ENCPoolMethod::None, true);
		}

		// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, Hit.DoubleHitResult.HitResult.HitResult.BoneName);

		TArray<FName> AffectedBones = GetAffectedBones(Hit.DoubleHitResult.HitResult.ImpactSide, Hit.DoubleHitResult.HitResult.ImpactHeight);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectedBones);
		//for (FName AffectBone : AffectedBones)
		//{
		//	IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectBone);
		//}

		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.BoneName, false, true);
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseAtLocation(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, Hit.DoubleHitResult.HitResult.HitResult.BoneName);
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);
		//IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.BoneName, false, true);
		// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::None, "pelvis");

		if (IALSXTCollisionInterface::Execute_CanAttackFall(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
		{
			AttackFall(Hit);
			return;
		}

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		//
	}
}

// Synced Attack Reaction

void UALSXTImpactReactionComponent::SyncedAttackReaction(int Index)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackReaction(Index);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackReaction(Index);
	}
}

void UALSXTImpactReactionComponent::ServerSyncedAttackReaction_Implementation(int32 Index)
{
	// MulticastSyncedAttackReaction(Index);
	StartSyncedAttackReaction(Index);
	GetOwner()->ForceNetUpdate();
}

// Stabilize

void UALSXTImpactReactionComponent::Stabilize(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerStabilize(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartStabilize(Hit);
	}
}

void UALSXTImpactReactionComponent::ClutchImpactPoint(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerClutchImpactPoint(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartClutchImpactPoint(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFall(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFall(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFall(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFallLand(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFallLand(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFallLand(Hit);
	}
}

void UALSXTImpactReactionComponent::ImpactFallIdle(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactFallIdle(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactFallIdle(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFall(FAttackDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFall(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		//MulticastAttackFall(Hit);
		StartAttackFall(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFallLand(FAttackDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFallLand(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackFallLand(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackFallIdle(FAttackDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackFallIdle(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackFallIdle(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackFall(int32 Index)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackFall(Index);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackFall(Index);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackFallIdle(int32 Index)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackFallIdle(Index);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackFallIdle(Index);
	}
}

void UALSXTImpactReactionComponent::BraceForImpact()
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerBraceForImpact();
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartBraceForImpact();
	}
}

void UALSXTImpactReactionComponent::ImpactGetUp(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactGetUp(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactGetUp(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackGetUp(FAttackDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackGetUp(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackGetUp(Hit);
	}
}

void UALSXTImpactReactionComponent::SyncedAttackGetUp(int32 Index)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncedAttackGetUp(Index);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartSyncedAttackGetUp(Index);
	}
}

void UALSXTImpactReactionComponent::CrowdNavigationResponse()
{
	// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerCrowdNavigationResponse();
	// }
	// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	// {
	// 	CrowdNavigationImplementation();
	// }
}

void UALSXTImpactReactionComponent::ImpactResponse(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactResponse(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartImpactResponse(Hit);
	}
}

void UALSXTImpactReactionComponent::AttackResponse(FAttackDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerAttackResponse(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		StartAttackResponse(Hit);
		// MulticastAttackResponse(Hit);
		// ServerAttackResponse(Hit);
	}
}

void UALSXTImpactReactionComponent::BodyFallReaction(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerImpactReaction(Hit);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
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

bool UALSXTImpactReactionComponent::IsStabilizeAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UALSXTImpactReactionComponent::IsBraceForImpactAllowedToStart(const UAnimSequenceBase* Montage) const
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

bool UALSXTImpactReactionComponent::ShouldSpawnImpactParticle(FDoubleHitResult Hit)
{
	float CombinedVelocity = (Hit.HitResult.Velocity + Hit.OriginHitResult.Velocity).Length();
	
	if (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt)
	{
		return CombinedVelocity > 1000;
	}
	if ((Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blade && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt) || (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blade))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UALSXTImpactReactionComponent::ShouldSpawnRearImpactParticle(FDoubleHitResult Hit)
{
	float CombinedVelocity = (Hit.HitResult.Velocity + Hit.OriginHitResult.Velocity).Length();

	if (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt)
	{
		return CombinedVelocity > 1000;
	}
	if ((Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blade && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt) || (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blade))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UALSXTImpactReactionComponent::ShouldSpawnFrontImpactDecal(FDoubleHitResult Hit)
{
	float CombinedVelocity = (Hit.HitResult.Velocity + Hit.OriginHitResult.Velocity).Length();

	if (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt)
	{
		return CombinedVelocity > 1000;
	}
	if ((Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blade && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt) || (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blade))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UALSXTImpactReactionComponent::ShouldSpawnRearImpactDecal(FDoubleHitResult Hit)
{
	float CombinedVelocity = (Hit.HitResult.Velocity + Hit.OriginHitResult.Velocity).Length();

	if (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt)
	{
		return CombinedVelocity > 1000;
	}
	if ((Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blade && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blunt) || (Hit.HitResult.ImpactForm == ALSXTImpactFormTags::Blunt && Hit.OriginHitResult.ImpactForm == ALSXTImpactFormTags::Blade))
	{
		return true;
	}
	else
	{
		return false;
	}
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

void UALSXTImpactReactionComponent::BumpReactionImplementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
	FDoubleHitResult CurrentBumpHit = GetImpactReactionState().ImpactReactionParameters.BumpHit;
	FBumpReactionAnimation SelectedBumpReaction = SelectBumpReactionMontage(Gait, Side, Form);
	UAnimMontage* Montage = SelectedBumpReaction.Montage.Montage;
	
	// Error Messages
	if ((!IsValid(Montage) || !IsBumpReactionAllowedToStart(Montage) || !IsValid(CurrentBumpHit.HitResult.HitResult.GetActor())))
	{
		if (ImpactReactionSettings.DebugMode)
		{
			if (!IsValid(CurrentBumpHit.HitResult.HitResult.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("BumpHit Actor Invalid"));
			}
			if (!IsBumpReactionAllowedToStart(Montage))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("IsBumpReactionNOTAllowedToStart"));
			}
			if (!IsValid(CurrentBumpHit.HitResult.HitResult.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Bump GetActor Invalid"));
			}
		}		
		return;
	}

	if (IsBumpReactionAllowedToStart(Montage) && IsValid(CurrentBumpHit.HitResult.HitResult.GetActor()))
	{
		float CombinedVelocity = (GetOwner()->GetVelocity() + CurrentBumpHit.HitResult.HitResult.GetActor()->GetVelocity()).Length();
		bool VelocityShouldAllowParticle {(CombinedVelocity > 100) ? VelocityShouldAllowParticle = true : VelocityShouldAllowParticle = false};
		UNiagaraSystem* Particle = {nullptr};
		if (ShouldSpawnImpactParticle(CurrentBumpHit))
		{
			Particle = GetImpactReactionParticle(GetImpactReactionState().ImpactReactionParameters.BumpHit);
		}

		// UNiagaraSystem* Particle = { (VelocityShouldAllowParticle) ? GetImpactReactionParticle(GetImpactReactionState().ImpactReactionParameters.BumpHit) : nullptr };
		USoundBase* Audio = nullptr;
		
		UMaterialInterface* FrontDecal = nullptr;
		// UMaterialInterface* FrontDecal = GetImpactDecal(GetImpactReactionState().ImpactReactionParameters.BumpHit);
		Audio = GetImpactReactionSound(CurrentBumpHit).Sound;
		// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IALSXTCollisionInterface::Execute_AddCollisionImpulse(GetOwner(), ((GetOwner()->GetVelocity() * -1) + CurrentBumpHit.HitResult.HitResult.GetActor()->GetVelocity()));
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);
		OnBumpReactionStarted();

		if (AnimInstance)
		{
			OnBumpReactionBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnBumpReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnBumpReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

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

		if (Audio->IsValidLowLevelFast())
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				UAudioComponent* AudioComponent{ nullptr };
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Audio, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		if (FrontDecal->IsValidLowLevelFast() && ShouldSpawnFrontImpactDecal(CurrentBumpHit))
		{
			UDecalComponent* FrontDecalComponent;
			FrontDecalComponent = UGameplayStatics::SpawnDecalAttached(FrontDecal, { 10.0f, 20.0f, 20.0f },
				CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.GetComponent(), NAME_None, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint,
				UKismetMathLibrary::MakeRotFromX(CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactNormal), EAttachLocation::KeepWorldPosition, 5.0);
		
			if (FrontDecalComponent->IsValidLowLevelFast())
			{
				FrontDecalComponent->SetFadeOut(5.0, 2.0, false);
			}
		
		}


		if (Particle->IsValidLowLevelFast() && ShouldSpawnImpactParticle(CurrentBumpHit))
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.ImpactPoint, NewRotation, { 1.0f, 1.0f, 1.0f }, true, true, ENCPoolMethod::None, true);
		}
		TArray<FName> AffectedBones;
		AffectedBones.Add(CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.BoneName);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Bump, AffectedBones);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.Impulse * 10, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.BoneName, false, true);
	}
}

// void UALSXTImpactReactionComponent::StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
// 	// {
// 	// 	return;
// 	// }
// 
// 	FALSXTImpactReactionState IRState = GetImpactReactionState();
// 	FBumpReactionAnimation SelectedCrowdNavigationReaction = SelectCrowdNavigationReactionMontage(IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactGait, IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactSide, IRState.ImpactReactionParameters.CrowdNavigationHit.HitResult.ImpactForm);
// 	UAnimMontage* Montage = SelectedCrowdNavigationReaction.Montage.Montage;
// 
// 	if ((!IsValid(Montage) || !IsImpactReactionAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
// 	{
// 		return;
// 	}
// 
// 	USoundBase* Audio = GetImpactReactionSound(IRState.ImpactReactionParameters.CrowdNavigationHit).Sound.Sound;
// 	// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };
// 
// 	CrowdNavigationReactionImplementation(Gait, Side, Form);
// 
// 	// if (GetOwner()->GetLocalRole() >= ROLE_Authority)
// 	// {
// 	// 	ServerStartCrowdNavigationReaction(SelectedCrowdNavigationReaction.Montage, Audio);
// 	// }
// 	// else
// 	// {
// 	// 	IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
// 	// 	MulticastStartCrowdNavigationReaction(SelectedCrowdNavigationReaction.Montage, Audio);
// 	// 	OnCrowdNavigationReactionStarted();
// 	// }
// }

void UALSXTImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
{
	if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	UAnimMontage* Montage{ nullptr };
	UNiagaraSystem* Particle{ nullptr };
	USoundBase* Audio{ nullptr };

	FImpactReactionAnimation ImpactReactionAnimation = SelectImpactReactionMontage(Hit);
	Montage = ImpactReactionAnimation.Montage.Montage;
	Particle = GetImpactReactionParticle(Hit);
	Audio = GetImpactReactionSound(Hit).Sound;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsImpactReactionAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Impact Reaction Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	// Clear the character movement mode and set the locomotion action to mantling.

	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);
	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();

		StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
		ServerStartImpactReaction(Hit, Montage, Particle, Audio);
		OnImpactReactionStarted(Hit);
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackReaction(int32 Index)
{
	// ...
}

void UALSXTImpactReactionComponent::StartStabilize(FDoubleHitResult Hit)
{
	UAnimMontage* Montage;
	FStabilizationMontage SelectedStabilizationMontage = SelectStabilizationMontage(Hit);
	Montage = SelectedStabilizationMontage.Montage.Montage;
	FALSXTDefensiveModeState DefensiveModeState;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsStabilizeAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stabilize Montage Invalid"));
		return;
	}

	FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));	
	DefensiveModeState.Montage = Montage;
	DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(Hit.HitResult.HitResult.ImpactPoint), Hit.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
	float MontageLength = AnimInstance->Montage_Play(Montage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f);

	if (!IALSXTCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
	{
		// Disable AI Movement
		if (GetOwner()->GetClass()->ImplementsInterface(UALSXTAIInterface::StaticClass()))
		{
			IALSXTAIInterface::Execute_SetLockMovement(GetOwner(), true);
		}	
	}

	if (AnimInstance)
	{
		OnStabilizeBlendOutDelegate.BindUObject(this, &UALSXTImpactReactionComponent::OnStabilizationBlendOut);
		AnimInstance->Montage_SetBlendingOutDelegate(OnStabilizeBlendOutDelegate);
	}
	IALSXTCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::Stabilization);
}

void UALSXTImpactReactionComponent::StartClutchImpactPoint(FDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	UAnimSequenceBase* SelectedClutchImpactPointPose;
	FClutchImpactLocationAnimation SelectedClutchImpactPointReaction = SelectClutchImpactPointMontage(Hit);
	SelectedClutchImpactPointPose = SelectedClutchImpactPointReaction.Pose;
	// FALSXTDefensiveModeState DefensiveModeState;

	// if ((!ALS_ENSURE(IsValid(SelectedClutchImpactPointPose)) || !IsClutchImpactPointAllowedToStart(SelectedClutchImpactPointPose)) && // ImpactReactionSettings.DebugMode)
	// {
	// 	return;
	// }
	// FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));
	// DefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
	// DefensiveModeState.Montage = SelectedClutchImpactPointPose;
	// DefensiveModeState.Location = Hit.HitResult.HitResult.ImpactPoint;
	// IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
	// IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::ClutchImpactPoint);
	// StartClutchImpactPointTimer();

	// const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };
	
	StartClutchImpactPointImplementation(SelectedClutchImpactPointPose, Hit.HitResult.HitResult.ImpactPoint);
	
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartClutchImpactPoint(SelectedClutchImpactPointPose, Hit.HitResult.HitResult.ImpactPoint);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartClutchImpactPoint(SelectedClutchImpactPointPose, Hit.HitResult.HitResult.ImpactPoint);
		OnImpactReactionStarted(Hit);
	}
}

void UALSXTImpactReactionComponent::StartImpactFall(FDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
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

	if ((!ALS_ENSURE(IsValid(Montage.Montage)) || !IsImpactFallAllowedToStart(Montage.Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Impact Fall Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactFall(Hit, Montage, Montage);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
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
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FActionMontageInfo Montage{ nullptr };
	FActionMontageInfo SelectedAttackFall = SelectImpactFallMontage(Hit);
	Montage = SelectedAttackFall;

	if ((!ALS_ENSURE(IsValid(Montage.Montage)) || !IsAttackFallAllowedToStart(Montage.Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Impact Fall Idle Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactFall(Hit, Montage, Montage);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartImpactFall(Hit, Montage, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UALSXTImpactReactionComponent::StartAttackFall(FAttackDoubleHitResult Hit)
{
	IALSXTCharacterInterface::Execute_SetCharacterRagdoll(GetOwner(), true);

	// FFallenAnimation FallenAnimations = SelectAttackFallAnimations(Hit);
	// FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
	// NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations = FallenAnimations;
	// NewImpactReactionState.ImpactReactionParameters.FallType = ALSXTFallTypeTags::Attack;
	// SetImpactReactionState(NewImpactReactionState);
	// FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
	// FActionMontageInfo Montage = SelectedAttackFall;
	// 
	// if ((!ALS_ENSURE(IsValid(Montage.Montage)) || !IsAttackFallAllowedToStart(Montage.Montage)) && ImpactReactionSettings.DebugMode)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Fall Montage Invalid"));
	// 	return;
	// }
	// 
	// // const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };
	// 
	// // IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	// 	// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// // ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
	// // ImpactReactionParameters.PlayRate = SelectedAttackFall.Montage.PlayRate;
	// // ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
	// // ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
	// // ImpactReactionParameters.Stance = Stance;
	// // ImpactReactionParameters.ImpactVelocity = Hit.Strength;
	// // ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// // FALSXTImpactReactionState NewImpactReactionState;
	// // NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// // SetImpactReactionState(NewImpactReactionState);
	// 
	// IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);
	// 
	// if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	// {
	// 	IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);
	// 	ServerStartAttackFall(Hit, Montage, Montage);
	// 	// StartAttackFallImplementation(Hit, Montage);
	// }
	// else
	// {
	// 	IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
	// 	IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);
	// 	MulticastStartAttackFall(Hit, Montage, Montage);
	// 	// StartAttackFallImplementation(Hit, Montage);
	// 	// ServerStartAttackFall(Hit, Montage);
	// }
}

void UALSXTImpactReactionComponent::StartAttackFallLand(FAttackDoubleHitResult Hit)
{
	StartAttackFallIdle(Hit);
}

void UALSXTImpactReactionComponent::StartAttackFallIdle(FAttackDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
	FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
	// FActionMontageInfo Montage = SelectedAttackFall;
	UAnimMontage* Montage = NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations.FallenPose;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsAttackFallAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Fall Idle Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);
	IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);

	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartAttackFallIdle(Montage, Hit);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
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
	const FGameplayTag Side{ALSXTImpactPositionTags::Front};
	UAnimSequenceBase* Montage = SelectBraceForImpactPose(Side, ALSXTImpactFormTags::Blunt);
	FALSXTDefensiveModeState DefensiveModeState;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsBraceForImpactAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Brace For Impact Montage Invalid"));
		return;
	}
	
	DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
	DefensiveModeState.AnticipationPose = Montage;
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
	IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::BraceForImpact);
}

void UALSXTImpactReactionComponent::StartImpactGetUp(FDoubleHitResult Hit)
{
	// ...
}

void UALSXTImpactReactionComponent::StartAttackGetUp(FAttackDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }
	if (GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.GetUpMontages.IsValidIndex(0))
	{
		FActionMontageInfo SelectedAttackGetUp = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.GetUpMontages[0];
		FActionMontageInfo Montage = SelectedAttackGetUp;

		if (!ALS_ENSURE(IsValid(Montage.Montage)) && ImpactReactionSettings.DebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Get Up Montage Invalid"));
			return;
		}
		const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

		// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

		if (GetOwner()->GetLocalRole() >= ROLE_Authority)
		{
			ServerStartAttackGetUp(Hit, Montage);
		}
		else
		{
			IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
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
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FResponseAnimation Montage;
	FResponseAnimation SelectedImpactResponse = SelectImpactResponseMontage(Hit);
	Montage = SelectedImpactResponse;

	if ((!ALS_ENSURE(IsValid(Montage.Montage.Montage)) || !IsImpactResponseAllowedToStart(Montage.Montage.Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Impact Resonse Montage Invalid"));
		return;
	}
	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
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

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartImpactResponse(Hit, Montage.Montage);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		// MulticastStartImpactResponse(Hit, Montage.Montage);
		StartImpactResponseImplementation(Hit, Montage.Montage);
	}

}

void UALSXTImpactReactionComponent::StartAttackResponse(FAttackDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FResponseAnimation SelectedAttackResponse = SelectAttackResponseMontage(Hit);
	FResponseAnimation Montage = SelectedAttackResponse;

	if ((!ALS_ENSURE(IsValid(Montage.Montage.Montage)) || !IsAttackResponseAllowedToStart(Montage.Montage.Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Response Montage Invalid"));
		return;
	}
	
	if (!Hit.DoubleHitResult.OriginHitResult.HitResult.GetActor() && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Actor Invalid"));
		return;
	}
	
	FVector OtherActorLocation = Hit.DoubleHitResult.OriginHitResult.HitResult.GetActor()->GetActorLocation();
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), OtherActorLocation);
	const float StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(PlayerRot.Yaw)) };
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = PlayerRot.Yaw;
	GetOwner()->SetActorRotation(CurrentRotation);
	IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

	// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
	// ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
	// FALSXTImpactReactionState NewImpactReactionState;
	// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	// SetImpactReactionState(NewImpactReactionState);

	// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		ServerStartAttackResponse(Hit, Montage.Montage);
	}
	else
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
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
 
UMaterialInterface* UALSXTImpactReactionComponent::GetImpactDecal(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	FALSXTImpactDecalMap ImpactParticleMap = SelectedImpactReactionSettings->ImpactDecals;
	TEnumAsByte<EPhysicalSurface> HitSurface = (Hit.HitResult.HitResult.PhysMaterial.IsValid()) ? Hit.HitResult.HitResult.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType_Default;
	TEnumAsByte<EPhysicalSurface> OriginHitSurface = (Hit.OriginHitResult.HitResult.PhysMaterial.IsValid()) ? Hit.HitResult.HitResult.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType_Default;
	UMaterialInterface* FoundImpactDecal{ nullptr };
	FGameplayTagContainer HitTags;
	HitTags.AddTagFast(ALSXTImpactVelocityTags::Moderate);
	HitTags.AddTagFast(Hit.ImpactForm);

	TArray<FALSXTImpactDecal> FilteredImpactParticles = ImpactParticleMap.Decals.Find(HitSurface)->Decals;

	if (FilteredImpactParticles.Num() > 0)
	{
		for (FALSXTImpactDecal Entry : FilteredImpactParticles)
		{
			FGameplayTagContainer EntryTags;
			EntryTags.AppendTags(Entry.Velocity);
			EntryTags.AppendTags(Entry.Form);
			if (EntryTags.HasAll(HitTags))
			{
				if (Entry.PhysicalMaterials.Contains(OriginHitSurface))
				{
					FoundImpactDecal = Entry.ImpactDecal;
					return FoundImpactDecal;
				}
			}
		}
	}

	return FoundImpactDecal;
}


UNiagaraSystem* UALSXTImpactReactionComponent::GetImpactReactionParticle(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	FALSXTImpactParticleMap ImpactParticleMap = SelectedImpactReactionSettings->ImpactParticles;
	UNiagaraSystem* FoundImpactParticle{ nullptr };
	FGameplayTagContainer HitTags;
	HitTags.AddTagFast(ALSXTImpactVelocityTags::Moderate);
	HitTags.AddTagFast(Hit.ImpactForm);
	TEnumAsByte<EPhysicalSurface> HitSurface = (Hit.HitResult.HitResult.PhysMaterial.IsValid()) ? Hit.HitResult.HitResult.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType_Default;

	if (HitSurface)
	{
		TArray<FALSXTImpactParticle> FilteredImpactParticles = ImpactParticleMap.Particles.Find(HitSurface)->Particles;

		TEnumAsByte<EPhysicalSurface> OriginHitSurface = (Hit.OriginHitResult.HitResult.PhysMaterial.IsValid()) ? Hit.OriginHitResult.HitResult.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType_Default;

		if (FilteredImpactParticles.Num() > 0)
		{
			for (FALSXTImpactParticle Entry : FilteredImpactParticles)
			{
				FGameplayTagContainer EntryTags;
				EntryTags.AppendTags(Entry.Form);
				EntryTags.AppendTags(Entry.Velocity);

				if (EntryTags.HasAll(HitTags))
				{
					if (Entry.PhysicalMaterials.Contains(OriginHitSurface))
					{
						FoundImpactParticle = Entry.ImpactParticle;
						return FoundImpactParticle;
					}
				}
			}
		}
	}
	return FoundImpactParticle;
}

UNiagaraSystem* UALSXTImpactReactionComponent::GetImpactPointParticle(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FALSXTImpactPointParticle> ImpactPointParticles = SelectedImpactReactionSettings->ImpactPointParticles;
	UNiagaraSystem* FoundImpactPointParticle {nullptr};
	FGameplayTagContainer HitTags;
	HitTags.AddTagFast(ALSXTImpactVelocityTags::Moderate);
	HitTags.AddTagFast(Hit.ImpactForm);
	for (FALSXTImpactPointParticle Entry : ImpactPointParticles)
	{
		FGameplayTagContainer EntryTags;
		EntryTags.AppendTags(Entry.Form);
		EntryTags.AppendTags(Entry.Velocity);
		
		if (EntryTags.HasAll(HitTags))
		{
				FoundImpactPointParticle = Entry.Particle;
		}
	}
	return FoundImpactPointParticle;
}

FALSXTCharacterSound UALSXTImpactReactionComponent::GetAttackReactionSound(FDoubleHitResult Hit)
{
	FALSXTCharacterSound FoundAttackReactionSound{ nullptr };
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());

	return FoundAttackReactionSound;
}

FSound UALSXTImpactReactionComponent::GetImpactReactionSound(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	FALSXTImpactSoundMap ImpactSoundMap = SelectedImpactReactionSettings->ImpactSounds;
	FSound FoundImpactSound{ nullptr };
	FGameplayTagContainer HitTags;
	HitTags.AddTagFast(ConvertVelocityToTag(Hit.HitResult.Velocity + Hit.OriginHitResult.Velocity));
	HitTags.AddTagFast(Hit.HitResult.ImpactForm);
	TEnumAsByte<EPhysicalSurface> HitSurface = (Hit.HitResult.HitResult.PhysMaterial.IsValid()) ? Hit.HitResult.HitResult.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType_Default;
	TArray<FALSXTImpactSound> FilteredImpactSounds = ImpactSoundMap.Sounds.Find(HitSurface)->Sounds;
	TArray<FSound> FoundImpactSounds;

	if (FilteredImpactSounds.Num() > 0)
	{
		for (FALSXTImpactSound Entry : FilteredImpactSounds)
		{
			FGameplayTagContainer EntryTags;
			EntryTags.AppendTags(Entry.Form);
			EntryTags.AppendTags(Entry.Velocity);

			if (EntryTags.HasAll(HitTags))
			{
				FoundImpactSounds.Append(Entry.Sounds);
			}
		}
	}



	// Return if there are no filtered Montages
	if (FoundImpactSounds.Num() < 1 || !FoundImpactSounds[0].Sound)
	{
		return FoundImpactSound;
	}

	// If more than one result, avoid duplicates
	if (FoundImpactSounds.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FoundImpactSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FoundImpactSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FoundImpactSounds.Num() - 1));
		FoundImpactSound = FoundImpactSounds[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return FoundImpactSound;
	}
	else
	{
		FoundImpactSound = FoundImpactSounds[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return FoundImpactSound;
	}
	return FoundImpactSound;
}

// UNiagaraSystem* UALSXTImpactReactionComponent::GetBodyFallParticle(FDoubleHitResult Hit)
// {
// 
// }

// FALSXTCharacterSound UALSXTImpactReactionComponent::GetImpactReactionSound(FDoubleHitResult Hit)
// {
// 
// }
// 
// FALSXTCharacterSound UALSXTImpactReactionComponent::GetAttackReactionSound(FDoubleHitResult Hit)
// {
// 
// }
// 
// FALSXTCharacterSound UALSXTImpactReactionComponent::GetBodyFallSound(FDoubleHitResult Hit)
// {
// 
// }

UALSXTElementalInteractionSettings* UALSXTImpactReactionComponent::GetElementalInteractionSettings_Implementation()
{
	return nullptr;
}

UAnimSequenceBase* UALSXTImpactReactionComponent::SelectBraceForImpactPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAnticipationPose> Poses = SelectedImpactReactionSettings->BraceForImpactPoses;
	TArray<FAnticipationPose> FilteredPoses;
	FAnticipationPose SelectedBraceForImpactPose;
	TArray<FGameplayTag> TagsArray = { Side, Form };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Poses.Num() < 1 || !Poses[0].Pose)
	{
		return SelectedBraceForImpactPose.Pose;
	}

	// Filter Montages based on Tag parameters
	for (auto Pose : Poses)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Pose.Side);
		CurrentTagsContainer.AppendTags(Pose.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredPoses.Add(Pose);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredPoses.Num() < 1 || !FilteredPoses[0].Pose)
	{
		return SelectedBraceForImpactPose.Pose;
	}

	// If more than one result, avoid duplicates
	if (FilteredPoses.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredPoses.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredPoses.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredPoses.Num() - 1));
		SelectedBraceForImpactPose = FilteredPoses[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBraceForImpactPose.Pose;
	}
	else
	{
		SelectedBraceForImpactPose = FilteredPoses[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBraceForImpactPose.Pose;
	}
	return SelectedBraceForImpactPose.Pose;
}

FAnticipationPose UALSXTImpactReactionComponent::SelectImpactAnticipationMontage_Implementation(const FGameplayTag& Velocity , const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FAnticipationPose> Montages = SelectedImpactReactionSettings->ImpactAnticipationPoses;
	TArray<FAnticipationPose> FilteredMontages;
	FAnticipationPose SelectedImpactAnticipationPose;
	TArray<FGameplayTag> TagsArray = { Velocity, Stance, Side, Form, Health };
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
	TArray<FGameplayTag> TagsArray = { Strength, Stance, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt, ALSXTHealthTags::All };
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
	TArray<FGameplayTag> TagsArray = { Velocity, Side, Form};
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
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpPose> Reactions = SelectedImpactReactionSettings->CrowdNavigationPoses;
	TArray<FBumpPose> FilteredReactions;
	FBumpPose SelectedCrowdNavigationReactionAnimation;
	TArray<FGameplayTag> TagsArray = { IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), Side, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
	
	// Return is there are no Montages
	if (Reactions.Num() < 1 || !Reactions[0].Pose)
	{
		return SelectedCrowdNavigationReactionAnimation.Pose;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Reactions)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Gait);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredReactions.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredReactions.Num() < 1 || !FilteredReactions[0].Pose)
	{
		return SelectedCrowdNavigationReactionAnimation.Pose;
	}

	// If more than one result, avoid duplicates
	if (FilteredReactions.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredReactions.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredReactions.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredReactions.Num() - 1));
		SelectedCrowdNavigationReactionAnimation = FilteredReactions[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedCrowdNavigationReactionAnimation.Pose;
	}
	else
	{
		SelectedCrowdNavigationReactionAnimation = FilteredReactions[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedCrowdNavigationReactionAnimation.Pose;
	}
	return SelectedCrowdNavigationReactionAnimation.Pose;
	
	//UAnimSequenceBase* Pose { nullptr };
	//return Pose;
}

FBumpReactionAnimation UALSXTImpactReactionComponent::SelectCrowdNavigationReactionMontage_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpReactionAnimation> Montages = SelectedImpactReactionSettings->CrowdNavigationBumpReactions;
	TArray<FBumpReactionAnimation> FilteredMontages;
	FBumpReactionAnimation SelectedCrowdNavigationReactionAnimation;
	TArray<FGameplayTag> TagsArray = { Gait, Side, ALSXTImpactFormTags::Blunt };
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

	if (FilteredMontages.Num() == 1)
	{
		SelectedCrowdNavigationReactionAnimation = FilteredMontages[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
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

UAnimSequenceBase* UALSXTImpactReactionComponent::SelectBumpAnticipationPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpPose> Reactions = SelectedImpactReactionSettings->BumpAnticipationPoses;
	TArray<FBumpPose> FilteredReactions;
	FBumpPose SelectedBumpPose;
	TArray<FGameplayTag> TagsArray = { IALSXTCharacterInterface::Execute_GetCharacterGait(GetOwner()), Side, ALSXTImpactFormTags::Blunt };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Reactions.Num() < 1 || !Reactions[0].Pose)
	{
		return SelectedBumpPose.Pose;
	}

	// Filter Montages based on Tag parameters
	for (auto Montage : Reactions)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Montage.Gait);
		CurrentTagsContainer.AppendTags(Montage.Side);
		CurrentTagsContainer.AppendTags(Montage.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredReactions.Add(Montage);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredReactions.Num() < 1 || !FilteredReactions[0].Pose)
	{
		return SelectedBumpPose.Pose;
	}

	// If more than one result, avoid duplicates
	if (FilteredReactions.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredMontages.Contains(LastAttackReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastAttackReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredReactions.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredReactions.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredReactions.Num() - 1));
		SelectedBumpPose = FilteredReactions[RandIndex];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBumpPose.Pose;
	}
	else
	{
		SelectedBumpPose = FilteredReactions[0];
		// LastAttackReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedBumpPose.Pose;
	}
	return SelectedBumpPose.Pose;
	
	
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
	UALSXTCombatSettings* SelectedCombatSettings = IALSXTCombatInterface::Execute_SelectCombatSettings(this);
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

FStabilizationMontage UALSXTImpactReactionComponent::SelectStabilizationMontage_Implementation(FDoubleHitResult Hit)
{
	TArray<FStabilizationMontage> StabilizationMontages = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->StabilizationMontages;
	TArray<FStabilizationMontage> FilteredStabilizationMontages;
	FStabilizationMontage SelectedMontage;
	const FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));
	TArray<FGameplayTag> TagsArray = { ALSXTImpactSideTags::Left, Health };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (StabilizationMontages.Num() < 1 || !StabilizationMontages[0].Montage.Montage)
	{
		return SelectedMontage;
	}

	// Filter Montages based on Tag parameters
	for (auto Pose : StabilizationMontages)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Pose.Side);
		CurrentTagsContainer.AppendTags(Pose.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredStabilizationMontages.Add(Pose);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredStabilizationMontages.Num() < 1 || !FilteredStabilizationMontages[0].Montage.Montage)
	{
		return SelectedMontage;
	}

	// If more than one result, avoid duplicates
	if (FilteredStabilizationMontages.Num() > 1)
	{
		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
		// if (FilteredStabilizationMontages.Contains(LastImpactReactionAnimation))
		// {
		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
		// }

		//Shuffle Array
		for (int m = FilteredStabilizationMontages.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredStabilizationMontages.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredStabilizationMontages.Num() - 1));
		SelectedMontage = FilteredStabilizationMontages[RandIndex];
		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedMontage;
	}
	else
	{
		SelectedMontage = FilteredStabilizationMontages[0];
		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
		return SelectedMontage;
	}
	return SelectedMontage;
}

FClutchImpactLocationAnimation UALSXTImpactReactionComponent::SelectClutchImpactPointMontage_Implementation(FDoubleHitResult Hit)
{
	UALSXTImpactReactionSettings* SelectedImpactReactionSettings = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FClutchImpactLocationAnimation> Animations = SelectedImpactReactionSettings->ClutchImpactPointAnimations;
	TArray<FClutchImpactLocationAnimation> FilteredAnimations;
	FClutchImpactLocationAnimation SelectedClutchImpactPointAnimation;
	const FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));

	TArray<FGameplayTag> TagsArray = { Hit.Strength, Hit.HitResult.ImpactSide, Hit.ImpactForm, Health };
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);

	// Return is there are no Montages
	if (Animations.Num() < 1 || !Animations[0].Pose)
	{
		return SelectedClutchImpactPointAnimation;
	}

	// Filter Montages based on Tag parameters
	for (auto Animation : Animations)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Animation.ImpactStrength);
		CurrentTagsContainer.AppendTags(Animation.ImpactSide);
		CurrentTagsContainer.AppendTags(Animation.ImpactForm);
		CurrentTagsContainer.AppendTags(Animation.Health);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredAnimations.Add(Animation);
		}
	}

	// Return if there are no filtered Montages
	if (FilteredAnimations.Num() < 1 || !FilteredAnimations[0].Pose)
	{
		return SelectedClutchImpactPointAnimation;
	}
	else
	{
		SelectedClutchImpactPointAnimation = FilteredAnimations[0];
	}

	// If more than one result, avoid duplicates
	// if (FilteredAnimations.Num() > 1)
	// {
	// 	// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
	// 	if (FilteredAnimations.Contains(LastImpactReactionAnimation))
	// 	{
	// 		int IndexToRemove = FilteredAnimations.Find(LastImpactReactionAnimation);
	// 		FilteredAnimations.RemoveAt(IndexToRemove, 1, true);
	// 	}
	// 
	// 	//Shuffle Array
	// 	for (int m = FilteredAnimations.Num() - 1; m >= 0; --m)
	// 	{
	// 		int n = FMath::Rand() % (m + 1);
	// 		if (m != n) FilteredAnimations.Swap(m, n);
	// 	}
	// 
	// 	// Select Random Array Entry
	// 	int RandIndex = FMath::RandRange(0, (FilteredAnimations.Num() - 1));
	// 	SelectedClutchImpactPointAnimation = FilteredAnimations[RandIndex];
	// 	LastImpactReactionAnimation = SelectedClutchImpactPointAnimation;
	// 	return SelectedClutchImpactPointAnimation;
	// }
	// else
	// {
	// 	SelectedClutchImpactPointAnimation = FilteredAnimations[0];
	// 	LastImpactReactionAnimation = SelectedClutchImpactPointAnimation;
	// 	return SelectedClutchImpactPointAnimation;
	// }

	return SelectedClutchImpactPointAnimation;
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

// UAnimSequenceBase* UALSXTImpactReactionComponent::SelectBraceForImpactPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	TArray<FAnticipationPose> BracePoses = IALSXTCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BraceForImpactPoses;
// 	TArray<FAnticipationPose> FilteredBracePoses;
// 	UAnimSequenceBase* SelectedPose{ nullptr };
// 	const FGameplayTag Health = HealthToHealthTag(IALSXTCharacterInterface::Execute_GetHealth(GetOwner()));
// 	TArray<FGameplayTag> TagsArray = { Side, Health };
// 	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
// 
// 	// Return is there are no Montages
// 	if (BracePoses.Num() < 1 || !BracePoses[0].Pose)
// 	{
// 		return SelectedPose;
// 	}
// 
// 	// Filter Montages based on Tag parameters
// 	for (auto Pose : BracePoses)
// 	{
// 		FGameplayTagContainer CurrentTagsContainer;
// 		CurrentTagsContainer.AppendTags(Pose.Side);
// 		CurrentTagsContainer.AppendTags(Pose.Health);
// 
// 		if (CurrentTagsContainer.HasAll(TagsContainer))
// 		{
// 			FilteredBracePoses.Add(Pose);
// 		}
// 	}
// 
// 	// Return if there are no filtered Montages
// 	if (FilteredBracePoses.Num() < 1 || !FilteredBracePoses[0].Pose)
// 	{
// 		return SelectedPose;
// 	}
// 
// 	// If more than one result, avoid duplicates
// 	if (FilteredBracePoses.Num() > 1)
// 	{
// 		// If FilteredMontages contains LastAttackReactionAnimation, remove it from FilteredMontages array to avoid duplicates
// 		// if (FilteredBracePoses.Contains(LastImpactReactionAnimation))
// 		// {
// 		// 	int IndexToRemove = FilteredMontages.Find(LastImpactReactionAnimation);
// 		// 	FilteredMontages.RemoveAt(IndexToRemove, 1, true);
// 		// }
// 
// 		//Shuffle Array
// 		for (int m = FilteredBracePoses.Num() - 1; m >= 0; --m)
// 		{
// 			int n = FMath::Rand() % (m + 1);
// 			if (m != n) FilteredBracePoses.Swap(m, n);
// 		}
// 
// 		// Select Random Array Entry
// 		int RandIndex = FMath::RandRange(0, (FilteredBracePoses.Num() - 1));
// 		SelectedPose = FilteredBracePoses[RandIndex].Pose;
// 		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
// 		return SelectedPose;
// 	}
// 	else
// 	{
// 		SelectedPose = FilteredBracePoses[0].Pose;
// 		// LastImpactReactionAnimation = SelectedAttackReactionAnimation;
// 		return SelectedPose;
// 	}
// 	return SelectedPose;
// }

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
	IALSXTCharacterInterface::Execute_SetCharacterStance(GetOwner(), AlsStanceTags::Crouching);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::ServerGetUp_Implementation()
{
	IALSXTCharacterInterface::Execute_SetCharacterStance(GetOwner(), AlsStanceTags::Standing);
	GetOwner()->ForceNetUpdate();
}


void UALSXTImpactReactionComponent::ServerBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastBumpReaction(Gait, Side, Form);
	// GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	BumpReactionImplementation(Gait, Side, Form);
}

void UALSXTImpactReactionComponent::ServerCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastCrowdNavigationReaction(Gait, Side, Form);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	CrowdNavigationReactionImplementation(Gait, Side, Form);
}

void UALSXTImpactReactionComponent::ServerSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	MulticastSyncedAnticipationReaction(AnticipationPoint);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	StartSyncedAnticipationReaction(AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	MulticastDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	StartDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
}

void UALSXTImpactReactionComponent::ServerImpactReaction_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactReaction(Hit);
	GetOwner()->ForceNetUpdate();
}

void UALSXTImpactReactionComponent::MulticastImpactReaction_Implementation(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	GetOwner()->ForceNetUpdate();
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
	// 	GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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

// void UALSXTImpactReactionComponent::ServerCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	MulticastCrowdNavigationReaction(Gait, Side, Form);
// 	GetOwner()->ForceNetUpdate();
// }

// bool UALSXTImpactReactionComponent::ServerCrowdNavigationReaction_Validate(FActionMontageInfo Montage, USoundBase* Audio)
// {
// 	return true;
// }

// void UALSXTImpactReactionComponent::MulticastCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	CrowdNavigationReactionImplementation(Gait, Side, Form);
// }

void UALSXTImpactReactionComponent::ServerStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
		GetOwner()->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartImpactReaction_Validate(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
}

void UALSXTImpactReactionComponent::ServerStartSyncedAttackReaction_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackReaction(Montage);
		GetOwner()->ForceNetUpdate();
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

void UALSXTImpactReactionComponent::ServerStartStabilize_Implementation(UAnimMontage* Pose, FVector ImpactPoint)
{
	if (IsStabilizeAllowedToStart(Pose))
	{
		MulticastStartStabilize(Pose, ImpactPoint);
		GetOwner()->ForceNetUpdate();
	}
}

bool UALSXTImpactReactionComponent::ServerStartStabilize_Validate(UAnimMontage* Pose, FVector ImpactPoint)
{
	return true;
}

void UALSXTImpactReactionComponent::MulticastStartStabilize_Implementation(UAnimMontage* Pose, FVector ImpactPoint)
{
	StartStabilizeImplementation(Pose, ImpactPoint);
}

void UALSXTImpactReactionComponent::ServerStartClutchImpactPoint_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Pose))
	{
		MulticastStartClutchImpactPoint(Pose, ImpactPoint);
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
	}
}

void UALSXTImpactReactionComponent::ServerStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		// MulticastStartAttackFall(Hit, Montage);
		StartAttackFallImplementation(Hit, Montage, FallMontage);
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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
		GetOwner()->ForceNetUpdate();
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

void UALSXTImpactReactionComponent::CrowdNavigationReactionImplementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	FALSXTImpactReactionState IRState = GetImpactReactionState();
	FBumpReactionAnimation SelectedCrowdNavigationReaction = SelectCrowdNavigationReactionMontage(Gait, Side, Form);
	UAnimMontage* SelectedAnimation = SelectedCrowdNavigationReaction.Montage.Montage;
	// USoundBase* Audio = GetImpactReactionSound(IRState.ImpactReactionParameters.CrowdNavigationHit).Sound.Sound;

	if ((!IsValid(SelectedAnimation) || !IsCrowdNavigationReactionAllowedToStart(SelectedAnimation)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Crowd Navigation Montage Invalid"));
		return;
	}

	USoundBase* SelectedAudio = GetImpactReactionSound(IRState.ImpactReactionParameters.CrowdNavigationHit).Sound;

	if (IsCrowdNavigationReactionAllowedToStart(SelectedAnimation) && IsValid(GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor()))
	{
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(SelectedAnimation, 1.0f);

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
		FALSXTBumpPoseState NewCrowdNavigationPoseState;
		NewCrowdNavigationPoseState.Actor = GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor();
		NewCrowdNavigationPoseState.Mode = ALSXTBumpPoseModeTags::CrowdNavigation;
		NewCrowdNavigationPoseState.Pose = SelectedAnimation;
		NewCrowdNavigationPoseState.Form = Form;
		NewCrowdNavigationPoseState.Side = Side;
		NewCrowdNavigationPoseState.Location = GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.Location;
		FALSXTDefensiveModeState NewDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		// NewDefensiveModeState.AnticipationPose = SelectedAnimation;
		NewDefensiveModeState.ObstacleMode = ALSXTBumpPoseModeTags::CrowdNavigation;
		NewDefensiveModeState.ObstaclePose = SelectedAnimation;
		NewDefensiveModeState.ObstacleSide = Side;

		
		SetBumpPoseState(NewCrowdNavigationPoseState);
		SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);

		if (SelectedAudio)
		{
			if (GetWorld()->WorldType == EWorldType::EditorPreview)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SelectedAudio, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.ImpactPoint,
					1.0f, 1.0f);
			}
			else
			{
				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SelectedAudio, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.ImpactPoint,
					NewRotation,
					1.0f, 1.0f);
			}
		}

		TArray<FName> AffectedBones;
		AffectedBones.Add(CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.BoneName);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectedBones);
		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.Impulse * -1000, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.BoneName, false, true);

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

void UALSXTImpactReactionComponent::StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	//if (IsImpactReactionAllowedToStart(Montage) && IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f))
	if (IsImpactReactionAllowedToStart(Montage))
	{
		//Anticipation
		FALSXTDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.AnticipationMode = IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(Hit.HitResult.HitResult.ImpactPoint), Hit.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
		// Character->SetFacialExpression();

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);

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
		TArray<FName> AffectedBones;
		AffectedBones.Add(Hit.HitResult.HitResult.BoneName);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectedBones);
		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * -1000, Hit.HitResult.HitResult.BoneName, false, true);
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		if (IALSXTCollisionInterface::Execute_CanImpactFall(GetOwner()) && IALSXTCollisionInterface::Execute_ShouldImpactFall(GetOwner()))
		{
			ImpactFall(Hit);
			return;
		}
		else
		{
			if (IALSXTCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
			{
				ImpactResponse(Hit);
				return;
			}

		}
	}
}

void UALSXTImpactReactionComponent::StartSyncedAttackReactionImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UALSXTImpactReactionComponent::StartStabilizeImplementation(UAnimMontage* Montage, FVector ImpactPoint)
{
	if (IsStabilizeAllowedToStart(Montage))
	{
		// Character->SetFacialExpression();

		FALSXTDefensiveModeState CurrentDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		CurrentDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(ImpactPoint), ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), CurrentDefensiveModeState);
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::ClutchImpactPoint);
		// IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UALSXTImpactReactionComponent::StartClutchImpactPointImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint)
{
	// UAnimSequenceBase* SelectedClutchImpactPointPose;
	// FClutchImpactLocationAnimation SelectedClutchImpactPointReaction = SelectClutchImpactPointMontage(Hit);
	// SelectedClutchImpactPointPose = SelectedClutchImpactPointReaction.Pose;
	
	
	
	// if (IsClutchImpactPointAllowedToStart(Montage))
	// {
		// Character->SetFacialExpression();

		FALSXTDefensiveModeState CurrentDefensiveModeState = IALSXTCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		CurrentDefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.AnticipationPose = Montage;
		// CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.Transform = { UKismetMathLibrary::MakeRotFromX(ImpactPoint), ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), CurrentDefensiveModeState);
		IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::ClutchImpactPoint);
		// IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsClutchImpactPointNOTAllowedToStart"));
	// }
}

void UALSXTImpactReactionComponent::StartImpactFallImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactFallAllowedToStart(Montage.Montage))
	{

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
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

		TArray<FName> AffectedBones;
		AffectedBones.Add(CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectedBones);
		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactFall);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// IALSXTCharacterInterface::Execute_SetDesiredStance(GetOwner(), AlsStanceTags::Crouching);
		Cast<ACharacter>(GetOwner())->Crouch(); //Hack
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

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

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

		TArray<FName> AffectedBones;
		AffectedBones.Add(CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName);
		IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectedBones);
		IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);
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
		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		FALSXTImpactReactionState CurrentImpactReactionState = GetImpactReactionState();		
		FVector EndLocation{ FVector::ZeroVector };

		if (GetAttackFallLocation(EndLocation, Hit))
		{
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), EndLocation, 90.0f, 30.0f, IALSXTCharacterInterface::Execute_GetCharacterControlRotation(GetOwner()), FLinearColor::Red, 1.0, 1.0);
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

			// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.BoneName);
			// IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactFall);
			// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.BoneName, false,  true);
			// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::None, "pelvis");
		}

		IALSXTCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);
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

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);

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

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

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

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
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

		IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
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

// Refresh

void UALSXTImpactReactionComponent::RefreshSyncedAnticipationReaction(const float DeltaTime)
{
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopSyncedAnticipationReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopDefensiveReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::CrowdNavigationReaction)
	{
		StopCrowdNavigationReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopBumpReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopImpactReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshImpactReactionPhysics(DeltaTime);
	}
}

void UALSXTImpactReactionComponent::RefreshImpactReactionPhysics(const float DeltaTime)
{
	float Offset = ImpactReactionSettings.RotationOffset;
	auto ComponentRotation{ IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	TargetRotation.Pitch = ComponentRotation.Pitch;
	TargetRotation.Roll = ComponentRotation.Roll;

	if (ImpactReactionSettings.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = ImpactReactionState.ImpactReactionParameters.TargetYawAngle;

		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsRotation::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			ImpactReactionState.ImpactReactionParameters.TargetYawAngle, DeltaTime,
			ImpactReactionSettings.RotationInterpolationSpeed);

		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

void UALSXTImpactReactionComponent::RefreshAttackReaction(const float DeltaTime)
{
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopAttackReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::SyncedAttackReaction)
	{
		StopSyncedAttackReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopCrowdNavigationFallReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopImpactFallReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopAttackFallReaction();
		GetOwner()->ForceNetUpdate();
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
	if (IALSXTCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopSyncedAttackFallReaction();
		GetOwner()->ForceNetUpdate();
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
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAnticipationReactionEnded();
}

void UALSXTImpactReactionComponent::StopDefensiveReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnDefensiveReactionEnded();
}

void UALSXTImpactReactionComponent::StopCrowdNavigationReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationReactionEnded();
}

void UALSXTImpactReactionComponent::StopBumpReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnBumpReactionEnded();
}

void UALSXTImpactReactionComponent::StopImpactReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactReactionEnded();
}

void UALSXTImpactReactionComponent::StopAttackReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackReactionEnded();
}

void UALSXTImpactReactionComponent::StopSyncedAttackReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackReactionEnded();
}

void UALSXTImpactReactionComponent::StopCrowdNavigationFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationFallEnded();
}

void UALSXTImpactReactionComponent::StopImpactFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactFallEnded();
}

void UALSXTImpactReactionComponent::StopAttackFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackFallEnded();
}

void UALSXTImpactReactionComponent::StopSyncedAttackFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IALSXTCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackFallEnded();
}

void UALSXTImpactReactionComponent::OnImpactReactionEnded_Implementation() {}
