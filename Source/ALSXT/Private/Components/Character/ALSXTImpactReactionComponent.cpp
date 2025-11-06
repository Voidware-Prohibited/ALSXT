// MIT

#include "Components/Character/AlsxtImpactReactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Utility/AlsxtStructs.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFrameWork/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsRotation.h"
#include "InputActionValue.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtAIInterface.h"
#include "Interfaces/AlsxtCombatInterface.h"
#include "Interfaces/AlsxtCollisionInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "AlsxtCharacter.h"
#include "Utility/AlsxtCollisionStructs.h"
#include "Settings/AlsxtDefensiveModeSettings.h"
#include "Landscape.h"

// Sets default values for this component's properties
UAlsxtImpactReactionComponent::UAlsxtImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UAlsxtImpactReactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
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
void UAlsxtImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// AALSXTCharacter Character = Cast<AALSXTCharacter>(GetOwner());
	// AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	if (GetOwner())
	{
		AnimInstance = IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance();
	}

	if (GetOwner()->GetClass()->ImplementsInterface(UAlsxtCharacterInterface::StaticClass()))
	{
		CharacterCapsule = IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner());
		CharacterCapsule->OnComponentHit.AddDynamic(this, &UAlsxtImpactReactionComponent::OnCapsuleHit);
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

	RefreshBlockingPoses();
	RefreshObstacleNavigationPoses();
	RefreshCrowdNavigationPoses();
}

// Called every frame
void UAlsxtImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ObstacleTrace();
	AnticipationTrace();
}

FGameplayTag UAlsxtImpactReactionComponent::DetermineDefensiveModeFromAttackingCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance)
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

FGameplayTag UAlsxtImpactReactionComponent::DetermineDefensiveModeFromCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance)
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

FGameplayTag UAlsxtImpactReactionComponent::HealthToHealthTag(float Health)
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

TArray<FName> UAlsxtImpactReactionComponent::GetAffectedBones(const FGameplayTag& Side, const FGameplayTag& Height)
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
			AffectedBones.Add("clavicle_l");
			// AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("clavicle_l");
			AffectedBones.Add("neck_01");
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
			AffectedBones.Add("clavicle_r");
			// AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("clavicle_r");
			AffectedBones.Add("neck_01");
			// AffectedBones.Add("spine_03");
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
			AffectedBones.Add("clavicle_l");
			AffectedBones.Add("clavicle_r");
			// AffectedBones.Add("spine_01");
		}
		if (Height == ALSXTImpactHeightTags::High)
		{
			AffectedBones.Add("neck_01");
			// AffectedBones.Add("spine_03");
		}
	}

	return AffectedBones;
}

FGameplayTag UAlsxtImpactReactionComponent::LocationToImpactSide(FVector Location)
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

	float ImpactSideDetectionRange = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ImpactSideDetectionRange;
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

FGameplayTag UAlsxtImpactReactionComponent::LocationToImpactHeight(FVector Location)
{
	float Range = (GetOwner()->GetActorLocation().Z + IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleHalfHeight()) - (GetOwner()->GetActorLocation().Z - IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleHalfHeight());
	float RangeDivisor = Range / 3;

	if (Location.Z <= GetOwner()->GetActorLocation().Z - (RangeDivisor/1.5))
	{
		return ALSXTImpactHeightTags::Low;
	}
	if (Location.Z > GetOwner()->GetActorLocation().Z - (RangeDivisor / 1.5) && Location.Z < GetOwner()->GetActorLocation().Z + (RangeDivisor / 2.5))
	{
		return ALSXTImpactHeightTags::Middle;
	}
	if (Location.Z >= GetOwner()->GetActorLocation().Z + (RangeDivisor / 2.5))
	{
		return ALSXTImpactHeightTags::High;
	}
	else
	{
		return ALSXTImpactHeightTags::Middle;
	}
}

FGameplayTag UAlsxtImpactReactionComponent::LocationToActorImpactSide(AActor* Actor, FVector Location)
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

FGameplayTag UAlsxtImpactReactionComponent::ConvertVelocityToTag(FVector Velocity)
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

FGameplayTag UAlsxtImpactReactionComponent::ConvertVelocityToStrength(FVector Velocity)
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

FGameplayTag UAlsxtImpactReactionComponent::ConvertVelocityTagToStrength(FGameplayTag Velocity)
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

FGameplayTag UAlsxtImpactReactionComponent::ConvertPhysicalSurfaceToFormTag(EPhysicalSurface PhysicalSurface)
{
	FGameplayTag FoundForm;
	UAlsxtImpactReactionSettings* SelectedSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray <FAlsxtFormSurfaces> FormSurfaces = SelectedSettings->FormSurfaces;
	for (FAlsxtFormSurfaces Entry : FormSurfaces)
	{
		if (Entry.Surfaces.Contains(PhysicalSurface))
		{
			FoundForm = Entry.Form;
			return FoundForm;
		}
	}
	return FoundForm;
}

// Grounded Bump and Crowd Navigation
void UAlsxtImpactReactionComponent::ObstacleTrace()
{
	if (!ImpactReactionSettings.EnableBumpReactions && !ImpactReactionSettings.EnableImpactReactions)
	{
		return;
	}
	
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) != AlsLocomotionModeTags::Grounded || IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != FGameplayTag::EmptyTag || (IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Dead || IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Unconscious))
	{
		return;
	}


	if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) == ALSXTCombatStanceTags::Neutral )
	{
		// Crowd Nav
	}
	if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral )
	{
		// IALSXTCharacterInterface::Execute_IsCarryingWeapon
		//		IsHoldingAimableWeapon
		//			ShouldFlee
		//				HitActor->AFlee
		//			Should Avoid
		//				HitActor->Avoid
		//			ShouldAttack
		//				HitActor-EnterCombat



	}

	// Setup Trace
	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ ImpactReactionSettings.BumpDetectionRadius };
	float CharacterVelocity = GetOwner()->GetVelocity().Length();
	float TraceDistance{ 5.0f };
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == AlsLocomotionActionTags::Sliding)
	{
		CapsuleRadius = CapsuleRadius * 2.0;
		TraceDistance = ImpactReactionSettings.MaxSlideToCoverDetectionDistance;
	}
	else if (!IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->IsPlayingNetworkedRootMotionMontage())
	{
		// TraceDistance = ImpactReactionSettings.MaxBumpDetectionDistance;
		if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Walking)
		{
			TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 175.0f), FVector2D(0.0f, 0.125f), CharacterVelocity);
		}
		if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Running)
		{
			TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 375.0f), FVector2D(0.125f, 0.14f), CharacterVelocity);
		}
		if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
		{
			TraceDistance = FMath::GetMappedRangeValueClamped(FVector2D(5.0f, 650.0f), FVector2D(0.14f, 0.18f), CharacterVelocity);
		}
	}
	const FVector StartLocation{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorUpVector() * 20) };
	const FVector EndLocation{ StartLocation + (GetOwner()->GetVelocity() * TraceDistance) };
	TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode;
	BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());	

	// Check Status
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded && (IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal || IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::KnockedDown))
	{
		// Not if performing Action or already in certain Defensive Modes
		if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag && (IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::Anticipation || IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::BraceForImpact || IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::ClutchImpactPoint))
		{
			bool isHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, 46, 70, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode, HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f);
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
						FAlsxtDefensiveModeState DefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
						FAnticipationPose Montage;
						FGameplayTag Health = HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner()));
						FGameplayTag Stance = IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner());
						FGameplayTag Side = LocationToImpactSide(AnticipationPoint);
						FGameplayTag Height = LocationToImpactHeight(AnticipationPoint);
						float LocationDotProduct = FVector::DotProduct(GetOwner()->GetActorForwardVector(), (HitResult.GetActor()->GetActorForwardVector()));
						DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
						DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);

						// If Character
						if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UAlsxtCharacterInterface::StaticClass()))
						{
							// Only if Standing
							if (IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner()) == AlsStanceTags::Standing)
							{
								FGameplayTag HitActorCombatStance = IAlsxtCharacterInterface::Execute_GetCombatStance(HitResult.GetActor());
								FGameplayTag CharacterCombatStance = IAlsxtCharacterInterface::Execute_GetCombatStance(GetOwner());
								IAlsxtCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
								IAlsxtCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);
								FTransform AnticipationTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
								IAlsxtCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationTransform, AnticipationPoint);
								DefensiveModeState.ObstacleTransform = { UKismetMathLibrary::MakeRotFromX(AnticipationPoint), AnticipationPoint, {1.0, 1.0, 1.0} };
								FGameplayTag ImpactStrength = ConvertVelocityToStrength(GetOwner()->GetVelocity() + HitResult.GetActor()->GetVelocity());							

								// Are Characters facing each other?
								if (LocationDotProduct <= 0) // Replase with IALSXTAIInterface::Execute_CanAISeeThreatPoint or similar?
								{
									//Is Current Character AI?
									if (!IAlsxtCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
									{													
										// Determine how AI should respond based on Overlay and Combat Stance

										if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UAlsxtAIInterface::StaticClass()))
										{
											if (IAlsxtAIInterface::Execute_ShouldAIAnticipateImpact(GetOwner()))
											{
												if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UAlsxtCombatInterface::StaticClass()))
												{
													// Is HitActor Hostile?
													if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(HitResult.GetActor()) != ALSXTCombatStanceTags::Neutral)
													{
														// Is this Character already Combat Ready?
														if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
														{

															if (IAlsxtCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
															{
																if (IAlsxtAIInterface::Execute_ShouldAIAvoid(GetOwner()))
																{
																	Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
																	DefensiveMode == ALSXTDefensiveModeTags::Anticipation;

																}
																if (IAlsxtAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
																{
																	DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Blocking;
																	// DefensiveModeState.AnticipationPoseSet = SelectBlockingPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), Form, IALSXTCharacterInterface::Execute_GetCharacterLocomotionVariant(GetOwner()));															
																	IAlsxtCharacterInterface::Execute_SetCharacterCombatStance(GetOwner(), ALSXTCombatStanceTags::Ready);
																	DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
																}

																if (IAlsxtAIInterface::Execute_ShouldAIBlock(GetOwner()))
																{
																	Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
																	DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Blocking;
																	// DefensiveModeState.AnticipationPoseSet = SelectBlockingPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), Form, IALSXTCharacterInterface::Execute_GetCharacterLocomotionVariant(GetOwner()));															
																	IAlsxtCharacterInterface::Execute_SetCharacterCombatStance(GetOwner(), ALSXTCombatStanceTags::Ready);
																	DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
																}
																DefensiveModeState.Mode = DefensiveMode;
																DefensiveModeState.AnticipationMode = DefensiveMode;
																// DefensiveModeState.Montage = Montage.Pose;
																IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
																IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
																return;

															}
															else
															{
																if (IAlsxtAIInterface::Execute_ShouldAIAvoid(GetOwner()))
																{
																	Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
																	DefensiveMode == ALSXTDefensiveModeTags::Avoiding;
																}
																if (IAlsxtAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
																{

																}

																if (IAlsxtAIInterface::Execute_ShouldAIBlock(GetOwner()))
																{
																	Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
																	DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
																}
																DefensiveModeState.Mode = DefensiveMode;
																DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
																// DefensiveModeState.Montage = Montage.Pose;
																IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
																IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
																return;
															}

														}
														else
														{
															if (IAlsxtAIInterface::Execute_ShouldAIAvoid(GetOwner()))
															{
																Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Avoiding;

															}
															if (IAlsxtAIInterface::Execute_ShouldAIEnterCombatReadyMode(GetOwner()))
															{

															}

															if (IAlsxtAIInterface::Execute_ShouldAIBlock(GetOwner()))
															{
																Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Light, IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner()), Side, Form, Health);
																DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
															}
															DefensiveModeState.Mode = DefensiveMode;
															DefensiveModeState.AnticipationMode = DefensiveMode;
															// DefensiveModeState.Montage = Montage.Pose;
															IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
															IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
															return;
														}
													}
													else
													{
														if (IAlsxtCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
														{
															Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
															// DefensiveModeState.Montage = Montage.Pose;
															DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
														}
														else
														{
															// Crowd Nav
															Montage.Pose = SelectCrowdNavigationPose(Side, Form);
															// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
															DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
														}

														DefensiveModeState.Mode = DefensiveMode;
														DefensiveModeState.AnticipationMode = DefensiveMode;
														IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
														IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
														return;
													}
												}
												else
												{
													if (IAlsxtCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
													{
														Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
														DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
													}
													else
													{
														// Crowd Nav
														Montage.Pose = SelectCrowdNavigationPose(Side, Form);
														// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
														// DefensiveModeState.Montage = SelectCrowdNavigationPose(Side, Form);
														DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
													}

													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
													return;
												}
											}
										}
										else
										{
											if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
											{
												if (DefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation)
												{
													const FTransform ConstructedTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
													DefensiveModeState.AnticipationTransform = ConstructedTransform;
													if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
													{
														Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
														DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
														// DefensiveModeState.Montage = Montage.Pose;
													}
													else
													{
														Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
														DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Blocking;
													}
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
													return;
												}
												else
												{
													if (IAlsxtCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
													{
														Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
														DefensiveMode == ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
														IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
														IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
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
													DefensiveModeState.AnticipationTransform = ConstructedTransform;
													if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
													{
														Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
														DefensiveMode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation;
														// DefensiveModeState.Montage = Montage.Pose;
													}
													else
													{
														Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
														DefensiveMode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Blocking;
														// DefensiveModeState.Montage = Montage.Pose;
													}
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
													return;
												}
												else
												{
													if (IAlsxtCharacterInterface::Execute_GetCharacterGait(HitResult.GetActor()) == AlsGaitTags::Sprinting)
													{

														DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
														DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
														DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
													}
													else
													{
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::CrowdNavigation;
														DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
														DefensiveMode = ALSXTDefensiveModeTags::CrowdNavigation;
													}

													DefensiveModeState.Mode = DefensiveMode;
													DefensiveModeState.AnticipationMode = DefensiveMode;
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
													IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
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
										if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
										{
											// If Player, and Player is Blocking Begin Updating Threat Location
											if (DefensiveModeState.AnticipationMode == ALSXTDefensiveModeTags::Blocking)
											{
												DefensiveModeState.AnticipationTransform = { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };

												if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
												{
													return;
												}
												else
												{
													// IALSXTCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner()) == ALSXTDefensiveModeTags::
													
													// update anim if necessary
													// Check if still left/right top/middle/bottom

													// RefreshBlockingPoses();
												}
												IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
											else // Not Blocking
											{
												return;
											}
										}
										else // Combat Neutral
										{
											if (DefensiveModeState.AnticipationMode == ALSXTDefensiveModeTags::Blocking)
											{
												// update anim if necessary
												// Check if still left/right top/middle/bottom
												if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
												{
													// Bump
													// Montage.Pose = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
													Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
													// DefensiveModeState.Montage = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
													// DefensiveModeState.Montage = Montage.Pose;
													DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
													DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
													// DefensiveModeState.AnticipationPoseSet = SelectBlockingPoses()
												}
												else
												{
													// Update Blocking
													const FTransform ConstructedTransform { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };
													DefensiveModeState.AnticipationTransform = ConstructedTransform;

													FAlsxtDefensiveModeState CurrentDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
													FGameplayTagContainer CurrentImpactTagsContainer;
													FGameplayTagContainer NewImpactTagsContainer;

													// Update Animation
													if (!CurrentImpactTagsContainer.HasAllExact(NewImpactTagsContainer))
													{
														Montage = SelectDefensiveMontage(ALSXTActionStrengthTags::Medium, Stance, Side, Form, Health);
														// DefensiveModeState.Montage = Montage.Pose;
													}												
												}
												IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
											else
											{
												DefensiveModeState.AnticipationTransform = { UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
												// update anim if necessary
												// Check if still left/right top/middle/bottom
												if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
												{
													DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
													DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
													DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
												}
												else
												{
													if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(HitResult.GetActor()) != ALSXTCombatStanceTags::Neutral)
													{
														return;
													}
													else
													{
														// Montage.Pose = SelectCrowdNavigationPose(Side, Form);
														// DefensiveModeState.ObstaclePose = Montage.Pose;
														DefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
														DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
														DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
													}												
												}
												IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
												// IALSXTCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
												return;
											}
										}
									}
								}
								else
								{
									if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
									{
										if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
										{
											// DefensiveModeState.Montage = SelectBumpReactionMontage(Velocity, Side, Form).Montage.Montage;
											DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
											DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation; // Create Obstacle Impact Anticipation tag and use it here
											DefensiveModeState.AnticipationSide = LocationToImpactSide(AnticipationPoint);
											DefensiveModeState.AnticipationHeight = LocationToImpactHeight(AnticipationPoint);
											IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
										}
										else
										{
											// Crowd Nav
											Montage.Pose = SelectCrowdNavigationPose(Side, Form);
											DefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
											DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::CrowdNavigation;
											DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
											DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);
											IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
											IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
											return;
										}
									}
									else
									{
										// If not facing each other juet check if CrowdNav or Bump
										if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
										{
											// Bump
											DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
											DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation; // Create Obstacle Impact Anticipation tag and use it here
											DefensiveModeState.AnticipationSide = LocationToImpactSide(AnticipationPoint);
											DefensiveModeState.AnticipationHeight = LocationToImpactHeight(AnticipationPoint);
											Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										}
										else
										{
											// Crowd Nav
											Montage.Pose = SelectCrowdNavigationPose(Side, Form);
											DefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
											DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
											DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
											DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);
										}
										IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
										IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
										return;
									}


								}
							}
							else
							{
								return;
							}
						}
						else // Not Character
						{
							// Collision Interface
							if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UAlsxtCollisionInterface::StaticClass()))
							{

								if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
								{
									return;
								}
								else
								{
									if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										// Mass, Velocity, AnticipationPoint, Side
										IAlsxtCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), ActorMass);
										IAlsxtCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), ActorVelocity);

										FTransform AnticipationTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal), HitResult.ImpactPoint, {1.0f, 1.0f, 1.0f} };

										IAlsxtCollisionInterface::Execute_GetAnticipationInfo(HitResult.GetActor(), Velocity, Form, AnticipationTransform, AnticipationPoint);

										// Bump
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.ImpactTransform = { UKismetMathLibrary::MakeRotFromX(AnticipationPoint), AnticipationPoint, { 1.0f, 1.0f, 1.0f } };
										DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
										DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
										DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation; // Create Obstacle Impact Anticipation tag and use it here
										DefensiveModeState.AnticipationSide = LocationToImpactSide(AnticipationPoint);
										DefensiveModeState.AnticipationHeight = LocationToImpactHeight(AnticipationPoint);
									}
									else
									{
										// Crowd Nav
										Montage.Pose = SelectCrowdNavigationPose(Side, Form);
										DefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
										DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
										DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
										DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);
										DefensiveMode == ALSXTDefensiveModeTags::CrowdNavigation;
									}
									IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
									IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									return;
								}								
							}
							else
							{
								if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) == ALSXTCombatStanceTags::Neutral || IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral && IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
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
									UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
									TArray<FAlsxtFormSurfaces> FormSurfaces = SelectedImpactReactionSettings->FormSurfaces;

									// Get Form from FormSurfaces
									if (FormSurfaces.Num() < 1)
									{
										Form = ALSXTImpactFormTags::Blunt;
										return;
									}
									// Get only Form from FormSurfaces
									if (FormSurfaces.Num() == 1)
									{
										
										if (FormSurfaces[0].Form.IsValid())
										{
											Form = FormSurfaces[0].Form;
											return;
										}
										else
										{
											Form = ALSXTImpactFormTags::Blunt;
											return;
										}
										
									}
									// Get Form from FormSurfaces
									if (FormSurfaces.Num() > 1)
									{
										for (FAlsxtFormSurfaces FormSurface : FormSurfaces)
										{
											if (FormSurface.Surfaces.Contains(HitResult.PhysMaterial->SurfaceType))
											{
												// if (Form.) //  T0DO 12/23
												// {
												// 
												// }

												Form = FormSurface.Form;
												return;
											}
										}
										if (Form.IsValid())
										{
											Form = FormSurfaces[0].Form;
											return;
										}
										else
										{

										}
									}								

									if (IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting)
									{
										// BraceForImpact
										Montage = SelectImpactAnticipationMontage(Velocity, Stance, Side, Form, Health);
										DefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
										DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
										DefensiveModeState.AnticipationSide = Side;
										DefensiveModeState.AnticipationHeight = Height;
										DefensiveMode = ALSXTDefensiveModeTags::BraceForImpact;
										TArray<FName> AffectedBones = GetAffectedBones(Side, Height);
										IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Bump, AffectedBones);
									}
									else
									{
										// ObstacleNav
										Montage.Pose = SelectCrowdNavigationPose(Side, Form);
										// DefensiveModeState.ObstaclePoseSet = SelectObstacleNavigationPoses(IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), CharacterCombatStance, Health);
										DefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
										DefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
										DefensiveModeState.ObstacleSide = LocationToImpactSide(AnticipationPoint);
										DefensiveModeState.ObstacleHeight = LocationToImpactHeight(AnticipationPoint);
										DefensiveMode == ALSXTDefensiveModeTags::ObstacleNavigation;

										FAlsxtDefensiveModeState PreviousDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());

										if (PreviousDefensiveModeState.ObstacleSide != FGameplayTag::EmptyTag && PreviousDefensiveModeState.ObstacleHeight != FGameplayTag::EmptyTag)
										{
											if (PreviousDefensiveModeState.ObstacleSide != Side && PreviousDefensiveModeState.ObstacleHeight != Height)
											{
												IAlsxtCollisionInterface::Execute_ResetCharacterPhysicalAnimationMode(GetOwner());
											}
										}	

										TArray<FName> AffectedBones = GetAffectedBones(Side, Height);
										IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Navigation, AffectedBones);
									}
									IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
									IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), DefensiveMode);
									return;
								}	
							}
						}
					}
				}
			}
			else
			{
				if (IAlsxtCollisionInterface::Execute_GetCharacterPhysicalAnimationMode(GetOwner()) != FGameplayTag::EmptyTag || IAlsxtCollisionInterface::Execute_GetCharacterPhysicalAnimationMode(GetOwner()) != AlsxtPhysicalAnimationModeTags::None)
				{
					// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.25f, FColor::Yellow, TEXT("Reset PhysicalAnimationMode"), true);
					IAlsxtCollisionInterface::Execute_ResetCharacterPhysicalAnimationMode(GetOwner());
				}			
				return;
			}
		}
		else
		{
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Reset PhysicalAnimationMode 2"));
			IAlsxtCollisionInterface::Execute_ResetCharacterPhysicalAnimationMode(GetOwner());
			return;
		}
	}
}

// Dynamically react to colliding objects when moving fast
void UAlsxtImpactReactionComponent::AnticipationTrace()
{
	if (!ImpactReactionSettings.bEnableImpactAnticipationReactions)
	{
		return;
	}
	
	// Check Status
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::InAir && IAlsxtCollisionInterface::Execute_ShouldPerformDefensiveReaction(GetOwner()) && GetOwner()->GetVelocity().Length() > FGenericPlatformMath::Min(ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity, ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity) && (IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal || IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::KnockedDown))
	{
		// Not if performing Action or already in certain Defensive Modes
		if ((IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag || IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BumpInterruptableLocomotionActions.HasTag(IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()))) && (IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::Anticipation || IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::BraceForImpact || IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner()).Mode != ALSXTDefensiveModeTags::ClutchImpactPoint))
		{
			FGameplayTag CharacterCombatStance = IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner());
			const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
			const FVector StartLocation{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorUpVector() * Capsule->GetScaledCapsuleHalfHeight() / 2) };
			TEnumAsByte<EDrawDebugTrace::Type> BumpDebugMode = (ImpactReactionSettings.DebugMode) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
			TArray<FHitResult> HitResults;
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(GetOwner());
			FVector2D VelocityRange{ 199.0, 650.0 };
			FVector2D ConversionRange{ 0.0, 1.0 };
			float VelocityLength = FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
			float TraceDistance = ImpactReactionSettings.MaxBumpDetectionDistance;
			const FVector EndLocation{ StartLocation + (GetOwner()->GetVelocity() * TraceDistance) };

			// Trace
			if (UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ImpactReactionSettings.BumpDetectionRadius, Capsule->GetScaledCapsuleHalfHeight() / 2, ImpactReactionSettings.BumpTraceObjectTypes, false, IgnoreActors, BumpDebugMode,HitResults, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
			{
				for (FHitResult HitResult : HitResults)
				{
					if (ValidateNewAnticipationHit(HitResult.GetActor()))
					{
						//Get All Paramaters
						FGameplayTag FormTag = ConvertPhysicalSurfaceToFormTag(HitResult.PhysMaterial->SurfaceType);
						FGameplayTag VelocityTag = ALSXTImpactVelocityTags::Moderate;
						FGameplayTag HealthTag = HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner()));
						FGameplayTag SideTag = LocationToImpactSide(HitResult.ImpactPoint);
						FGameplayTag ImpactHeight = LocationToImpactHeight(HitResult.ImpactPoint);
						TEnumAsByte<EPhysicalSurface> PhysSurf = HitResult.PhysMaterial->SurfaceType;
						FTransform HitTransform{ UKismetMathLibrary::MakeRotFromX(HitResult.ImpactPoint), HitResult.ImpactPoint, {1.0, 1.0, 1.0} };
						float Mass {0.0};
						FVector Velocity {ForceInit};
						// Mass and Velocity
						// if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UALSXTCollisionInterface::StaticClass()))
						// {
						// 	IALSXTCollisionInterface::Execute_GetActorMass(HitResult.GetActor(), Mass);
						// 	IALSXTCollisionInterface::Execute_GetActorVelocity(HitResult.GetActor(), Velocity);
						// }
						// else
						// {
						// 	DoubleHitResult.HitResult.Mass = 100.00f;
						// 	DoubleHitResult.HitResult.Velocity = HitResult.GetActor()->GetVelocity();
						// }

						// Defensive Mode State
						FAlsxtDefensiveModeState NewDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());


						NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
						NewDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Blocking;
						NewDefensiveModeState.AnticipationForm = ConvertPhysicalSurfaceToFormTag(HitResult.PhysMaterial->SurfaceType);
						NewDefensiveModeState.AnticipationHeight =LocationToImpactHeight(HitResult.ImpactPoint);
						NewDefensiveModeState.AnticipationSide = LocationToImpactSide(HitResult.ImpactPoint);
						NewDefensiveModeState.AnticipationTransform = HitTransform;
						NewDefensiveModeState.ImpactVelocity = VelocityTag;

						IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
						IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), NewDefensiveModeState.Mode);
						

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

void UAlsxtImpactReactionComponent::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	float CurrentMinVelocity = (OtherActor->GetClass()->ImplementsInterface(UAlsxtCharacterInterface::StaticClass())) ? ImpactReactionSettings.CharacterBumpDetectionMinimumVelocity : ImpactReactionSettings.ObstacleBumpDetectionMinimumVelocity;
	
	if (IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()) != ALSXTCombatStanceTags::Neutral)
	{
		// Handle surfaces like lava, spikes etc
		return;
	}

	if (GetOwner()->GetVelocity().Length() < CurrentMinVelocity && NormalImpulse.Length() < 0)
	{
		return;
	}

	if ((IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded && IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()) == AlsGaitTags::Sprinting && GetOwner()->GetVelocity().Length() > CurrentMinVelocity) && (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag || IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BumpInterruptableLocomotionActions.HasTag(IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()))) || (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) != AlsLocomotionModeTags::Grounded && (GetOwner()->GetVelocity() + OtherActor->GetVelocity()).Length() > CurrentMinVelocity))
	{
		if ((IsValid(OtherActor)) && (OtherActor != GetOwner()) && (IsValid(OtherComp)) && ValidateNewHit(Hit.GetActor()))
		{
			FAlsxtImpactReactionState NewImpactReactionState = GetImpactReactionState();
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
			NewDoubleHitResult.HitResult.ImpactGait = IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner());
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
			IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Navigation, AffectedBones);

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
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UAlsxtCharacterInterface::StaticClass()))
				{
					NewDoubleHitResult.OriginHitResult.ImpactGait = IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner());
					// NewImpactReactionState.ImpactReactionParameters.BumpHit.OriginHitResult.ImpactLocation = LocationTo
					NewDoubleHitResult.OriginHitResult.ImpactSide = LocationToActorImpactSide(OriginHitResult.GetActor(), OriginHitResult.Location);
				}
				NewDoubleHitResult.OriginHitResult.ImpactStrength = ConvertVelocityToStrength(OriginHitResult.GetActor()->GetVelocity());
				NewDoubleHitResult.OriginHitResult.Impulse = OriginHitResult.GetActor()->GetVelocity();
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UAlsxtCharacterInterface::StaticClass()))
				{
					NewDoubleHitResult.OriginHitResult.Mass = 62;
				}
				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UAlsxtCollisionInterface::StaticClass()))
				{
					IAlsxtCollisionInterface::Execute_GetActorMass(GetOwner(), NewDoubleHitResult.OriginHitResult.Mass);
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

				if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UAlsxtCharacterInterface::StaticClass()))
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
					CrowdNavigationReaction(IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
					// CrowdNav
				}
				else if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UAlsxtCollisionInterface::StaticClass()))
				{
					if (ImpactReactionSettings.DebugMode)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					}
					IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
					NewImpactReactionState.ImpactReactionParameters.BumpHit = NewDoubleHitResult;
					SetImpactReactionState(NewImpactReactionState);
					BumpReaction(IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
				}
				else
				{
					if (ImpactReactionSettings.DebugMode)
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					}
					IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
					NewImpactReactionState.ImpactReactionParameters.BumpHit = NewDoubleHitResult;
					SetImpactReactionState(NewImpactReactionState);
					BumpReaction(IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()), LocationToImpactSide(Hit.Location), HitForm);
					// BumpReactionImplementation(AlsGaitTags::Running, ALSXTImpactSideTags::Left, ALSXTImpactFormTags::Blunt);
				}
				// GetWorld()->GetTimerManager().SetTimer(OnCapsuleHitTimerHandle, OnCapsuleHitTimerDelegate, 0.33f, false);
			}
		}
	}
}

void UAlsxtImpactReactionComponent::OnRagdollingStarted()
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

void UAlsxtImpactReactionComponent::OnRagdollingEnded()
{

}

bool UAlsxtImpactReactionComponent::GetImpactFallLocation(FVector& Location, FDoubleHitResult Hit)
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
	IAlsxtCollisionInterface::Execute_GetActorMass(Hit.HitResult.HitResult.GetActor(), ActorMass);
	FVector ActorVelocity{ FVector::ZeroVector };
	IAlsxtCollisionInterface::Execute_GetActorVelocity(Hit.HitResult.HitResult.GetActor(), ActorVelocity);
	FVector FallLocation = GetOwner()->GetActorLocation() * ImpactVelocityVector;

	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
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

bool UAlsxtImpactReactionComponent::GetAttackFallLocation(FVector& Location, FAttackDoubleHitResult Hit)
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
		IAlsxtCollisionInterface::Execute_GetActorMass(Hit.DoubleHitResult.HitResult.HitResult.GetActor(), ActorMass);
		FVector ActorVelocity{ FVector::ZeroVector };
		IAlsxtCollisionInterface::Execute_GetActorVelocity(Hit.DoubleHitResult.HitResult.HitResult.GetActor(), ActorVelocity);
		FVector FallLocation = GetOwner()->GetActorLocation() * ImpactVelocityVector;
		const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
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
				if (IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->IsWalkable(DownTraceHitResult))
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
				if (IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->IsWalkable(DownTraceHitResult))
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

float UAlsxtImpactReactionComponent::GetImpactFallenMinimumTime(FDoubleHitResult Hit)
{
	return FGenericPlatformMath::Min(GetDynamicImpactFallenMinimumTime(), IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ImpactFallenMinimumTime);
}

float UAlsxtImpactReactionComponent::GetAttackFallenMinimumTime(FAttackDoubleHitResult Hit)
{
	return FGenericPlatformMath::Min(GetDynamicImpactFallenMinimumTime(), IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->AttackFallenMinimumTime);
}

void UAlsxtImpactReactionComponent::CrowdNavigationVelocityTimer()
{
	FAlsxtBumpPoseState CurrentBumpPoseState = GetBumpPoseState();
	FAlsxtBumpPoseState CurrentCrowdNavigationPoseState = GetCrowdNavigationPoseState();

	// If Same Mode and Actor
	if (CurrentBumpPoseState.Mode == ALSXTBumpPoseModeTags::CrowdNavigation && CurrentBumpPoseState.Actor == CurrentCrowdNavigationPoseState.Actor)
	{
		return;
	}
	else
	{
		FAlsxtBumpPoseState EmptyCrowdNavigationPoseState;
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

void UAlsxtImpactReactionComponent::StartDefensiveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(DefensiveTimerHandle, DefensiveTimerDelegate, 0.1f, false);
}

void UAlsxtImpactReactionComponent::DefensiveTimer()
{
	
	
	
	if (IAlsxtCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner()) != ALSXTDefensiveModeTags::Blocking )
	{
		StopDefensiveTimer();
	}

	//Trace for Angle of closest Attack toward control rotation
	// 
	// 
	// Find and apply correct animation pose
}

void UAlsxtImpactReactionComponent::StopDefensiveTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(DefensiveTimerHandle);
}

void UAlsxtImpactReactionComponent::BumpVelocityTimer()
{
	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
	const auto CapsuleScale{ Capsule->GetComponentScale().Z };
	auto CapsuleRadius{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner())->GetScaledCapsuleRadius() * 1.15 };
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
			
			FAlsxtBumpPoseState NewBumpPoseState = GetBumpPoseState();
			NewBumpPoseState.Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 5.0f), FVector2D(1.0f, 0.0f), HitResult.Distance);
			SetBumpPoseState(NewBumpPoseState);
		}
	}
	else
	{
		FAlsxtBumpPoseState NewBumpPoseState = GetBumpPoseState();
		NewBumpPoseState.Alpha = 0.0f;
		SetBumpPoseState(NewBumpPoseState);
		GetWorld()->GetTimerManager().ClearTimer(BumpVelocityTimerHandle);
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			// AnimInstance->Montage_Resume(AnimInstance->GetActiveMontageInstance());
		}
	}
}

void UAlsxtImpactReactionComponent::StartClutchImpactPointTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ClutchImpactPointTimerHandle, ClutchImpactPointTimerDelegate, 1.25f, false);
}

void UAlsxtImpactReactionComponent::ClutchImpactPointTimer()
{
	IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);

	if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		if (IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
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
			if (IAlsxtCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
			{
				ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
			}
		}
	}
}

void UAlsxtImpactReactionComponent::StartImpactFallingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ImpactFallingTimerHandle, ImpactFallingTimerDelegate, 0.01f, true);
}

void UAlsxtImpactReactionComponent::ImpactFallingTimer()
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded)
	{
		GetWorld()->GetTimerManager().ClearTimer(ImpactFallenTimerHandle);
	}

	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
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

void UAlsxtImpactReactionComponent::StartAttackFallingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AttackFallingTimerHandle, AttackFallingTimerDelegate, 0.01f, true);
	// Character->StartRagdolling();
}

void UAlsxtImpactReactionComponent::AttackFallingTimer()
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackFallenTimerHandle);
	}

	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
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

void UAlsxtImpactReactionComponent::StartStabilizeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StabilizeTimerHandle, StabilizeTimerDelegate, 0.01f, true);
}

void UAlsxtImpactReactionComponent::StabilizeTimer()
{
	//..
}

void UAlsxtImpactReactionComponent::StartBraceForImpactTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BraceForImpactTimerHandle, BraceForImpactTimerDelegate, 0.01f, true);
}

void UAlsxtImpactReactionComponent::BraceForImpactTimer()
{
	//..
	FVector CharVel = GetOwner()->GetVelocity();
	float CharVelLength = CharVel.Length();
	const auto* Capsule{ IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(GetOwner()) };
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

void UAlsxtImpactReactionComponent::StartImpactFallenTimer(FDoubleHitResult Hit)
{
	float FallenMinTime = GetImpactFallenMinimumTime(Hit);
	FAlsxtImpactReactionState Params = GetImpactReactionState();
	Params.ImpactReactionParameters.FallenMinimumTime = FallenMinTime;
	Params.ImpactReactionParameters.CurrentFallenTime = FallenMinTime;
	SetImpactReactionState(Params);

	GetWorld()->GetTimerManager().SetTimer(ImpactFallenTimerHandle, ImpactFallenTimerDelegate, 0.25f, true);
}

void UAlsxtImpactReactionComponent::ImpactFallenTimer()
{
	float CurrentTime = GetImpactReactionState().ImpactReactionParameters.CurrentFallenTime;
	if (CurrentTime > 0.0f )
	{
		FAlsxtImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.CurrentFallenTime = FMath::Clamp(Params.ImpactReactionParameters.CurrentFallenTime - 0.25, 0.0f, 100.f);
		SetImpactReactionState(Params);
	}
	else
	{
		FAlsxtImpactReactionState Params = GetImpactReactionState();
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

void UAlsxtImpactReactionComponent::StartAttackFallenTimer(FAttackDoubleHitResult Hit)
{
	float FallenMinTime = GetAttackFallenMinimumTime(Hit);
	FAlsxtImpactReactionState Params = GetImpactReactionState();
	Params.ImpactReactionParameters.FallenMinimumTime = 8.0f;
	Params.ImpactReactionParameters.CurrentFallenTime = 8.0f;
	SetImpactReactionState(Params);

	GetWorld()->GetTimerManager().SetTimer(AttackFallenTimerHandle, AttackFallenTimerDelegate, 0.25f, true);
}

void UAlsxtImpactReactionComponent::AttackFallenTimer()
{
	float CurrentTime = GetImpactReactionState().ImpactReactionParameters.CurrentFallenTime;
	RefreshAttackFallReaction(GetOwner()->GetWorld()->GetDeltaSeconds());
	if (CurrentTime > 0.0f)
	{
		FAlsxtImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.CurrentFallenTime = FMath::Clamp(Params.ImpactReactionParameters.CurrentFallenTime - 0.25, 0.0f, 100.f);
		SetImpactReactionState(Params);
	}
	else
	{
		FAlsxtImpactReactionState Params = GetImpactReactionState();
		Params.ImpactReactionParameters.FallenMinimumTime = 0.0f;
		Params.ImpactReactionParameters.CurrentFallenTime = 0.0f;
		SetImpactReactionState(Params);
		GetWorld()->GetTimerManager().ClearTimer(AttackFallenTimerHandle);

		if (ImpactReactionSettings.bEnableAutoGetUp && IAlsxtCollisionInterface::Execute_CanGetUp(GetOwner()) && IAlsxtCollisionInterface::Execute_ShouldGetUp(GetOwner()))
		{
			IAlsxtCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::Normal);
			AttackGetUp(Params.ImpactReactionParameters.AttackHit);
			// AttackGetUp(GetLastAttackImpact());
			// ServerGetUp();
		}
	}
}

void UAlsxtImpactReactionComponent::StartAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AnticipationTimerHandle, AnticipationTimerDelegate, 0.1f, true);
}

void UAlsxtImpactReactionComponent::AnticipationTimer()
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

void UAlsxtImpactReactionComponent::StopAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AnticipationTimerHandle);
}

void UAlsxtImpactReactionComponent::StartFallingAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FallingAnticipationTimerHandle, FallingAnticipationTimerDelegate, 0.1f, true);
}

void UAlsxtImpactReactionComponent::FallingAnticipationTimer()
{

}

void UAlsxtImpactReactionComponent::StopFallingAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(FallingAnticipationTimerHandle);
}

void UAlsxtImpactReactionComponent::OnCrowdNavigationReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
	{
		CrowdNavigationFall();
	}
}

void UAlsxtImpactReactionComponent::OnStabilizationBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IAlsxtCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
	{
		// Enable AI Movement
		if (GetOwner()->GetClass()->ImplementsInterface(UAlsxtAIInterface::StaticClass()))
		{
			IAlsxtAIInterface::Execute_SetLockMovement(GetOwner(), false);
		}
	}
	
	if (IAlsxtCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
	{
		ClutchImpactPoint(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
		return;
	}
	else if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.OriginHitResult.HitResult.GetActor()) && IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
	{
		StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
	}
}

void UAlsxtImpactReactionComponent::OnBumpReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	// GetWorld()->GetTimerManager().ClearTimer(BumpVelocityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BumpVelocityTimerHandle, BumpVelocityTimerDelegate, 0.1f, true);
	if (IAlsxtCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
	{
		CrowdNavigationFall();
	}
}

void UAlsxtImpactReactionComponent::OnImpactReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldImpactFall(GetOwner()) && IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
	{
		IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
		ImpactFall(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	}
}

void UAlsxtImpactReactionComponent::OnAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
		if (IAlsxtCollisionInterface::Execute_CanAttackFall(GetOwner()) && IAlsxtCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
		{
			IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
			IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
			AttackFall(GetImpactReactionState().ImpactReactionParameters.AttackHit);
			return;
		}
		else
		{
			if (IAlsxtCollisionInterface::Execute_ShouldStabilize(GetOwner()))
			{
				Stabilize(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
				return;
			}
			if (IAlsxtCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
			{
				ClutchImpactPoint(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult);
				return;
			}
			else if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()) && IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
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

void UAlsxtImpactReactionComponent::OnSyncedAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldSyncedAttackFall(GetOwner()))
	{

	}
}

void UAlsxtImpactReactionComponent::OnClutchImpactPointBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsValid(GetImpactReactionState().ImpactReactionParameters.AttackHit.DoubleHitResult.HitResult.HitResult.GetActor()))
	{
		if (IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
		{
			AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		}
	}
	else if (IsValid(GetImpactReactionState().ImpactReactionParameters.ImpactHit.HitResult.HitResult.GetActor()))
	{
		if (IAlsxtCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
		{
			ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
		}
	}
}

void UAlsxtImpactReactionComponent::OnCrowdNavigationFallBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnImpactFallBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	ImpactFallIdle(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	// StartImpactFallenTimer();
}

void UAlsxtImpactReactionComponent::OnAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	AttackFallIdle(GetImpactReactionState().ImpactReactionParameters.AttackHit);
	StartAttackFallenTimer(GetLastAttackImpact());
}

void UAlsxtImpactReactionComponent::OnSyncedAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnBraceForImpactBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnCrowdNavigationFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldPerformCrowdNavigationResponse(GetOwner()))
	{
		CrowdNavigationResponse();
	}
}

void UAlsxtImpactReactionComponent::OnImpactFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
	{
		ImpactResponse(GetImpactReactionState().ImpactReactionParameters.ImpactHit);
	}
}

void UAlsxtImpactReactionComponent::OnAttackFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
	{
		// AttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
		StartAttackResponse(GetImpactReactionState().ImpactReactionParameters.AttackHit);
	}
}

void UAlsxtImpactReactionComponent::OnSyncedAttackFallGetUpBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnImpactResponseBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnAttackResponseBlendOut(UAnimMontage* Montage, bool bInterrupted){}

void UAlsxtImpactReactionComponent::OnCrowdNavigationReactionEnded(UAnimMontage* Montage, bool bInterrupted) 
{
	if (AnimInstance)
	{
		OnCrowdNavigationReactionBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnBumpReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnBumpReactionBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnImpactReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactReactionBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackReactionBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnSyncedAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackReactionBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnClutchImpactPointEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnClutchImpactPointBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnCrowdNavigationFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnCrowdNavigationFallBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnImpactFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactFallBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnAttackFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackFallBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnSyncedAttackFallEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackFallBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnBraceForImpactEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnBraceForImpactBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnCrowdNavigationFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnCrowdNavigationFallGetupBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnImpactFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactFallGetupBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnAttackFallGetupEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackFallGetupBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnSyncedAttackFallGetUpEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnSyncedAttackFallGetUpBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnImpactResponseEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnImpactResponseBlendOutDelegate.Unbind();
	}
}

void UAlsxtImpactReactionComponent::OnAttackResponseEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance)
	{
		OnAttackResponseBlendOutDelegate.Unbind();
	}
}

FGameplayTag UAlsxtImpactReactionComponent::GetCharacterVelocity()
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::InAir)
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
		FGameplayTag CharacterGait = IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner());
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

bool UAlsxtImpactReactionComponent::ShouldRecieveVelocityDamage()
{
	return GetOwner()->GetVelocity().Length() >= 650;
}

float UAlsxtImpactReactionComponent::GetBaseVelocityDamage()
{
	FVector2D VelocityRange{ 650.0, 2000.0 };
	FVector2D ConversionRange{ 0.0, 100.0 };
	return FMath::GetMappedRangeValueClamped(VelocityRange, ConversionRange, GetOwner()->GetVelocity().Length());
}

void UAlsxtImpactReactionComponent::OnCapsuleHitTimer()
{
	FVector ClosestLocation { ForceInit };

	if (ObstacleImpactHistory.Num() > 0)
	{
		if (ObstacleImpactHistory[ObstacleImpactHistory.Num() - 1].Actor->IsValidLowLevel())
		{
			ObstacleImpactHistory[ObstacleImpactHistory.Num() - 1].Actor->ActorGetDistanceToCollision(GetOwner()->GetActorLocation(), ECollisionChannel::ECC_Camera, ClosestLocation);
			double Distance = FVector::Dist(GetOwner()->GetActorLocation(), ClosestLocation);

			if (Distance > 26)
			{
				ClearObstacleImpactHistory();
				GetWorld()->GetTimerManager().ClearTimer(OnCapsuleHitTimerHandle);
			}
		}
	}
}

void UAlsxtImpactReactionComponent::ClearObstacleImpactHistory()
{
	ObstacleImpactHistory.Empty();
}

bool UAlsxtImpactReactionComponent::ValidateNewHit(AActor* ActorToCheck)
{
	double NewHitTime;
	NewHitTime = GetWorld()->GetTimeSeconds();
	bool RecentlyHit{ false };
	FImpactHistoryEntry EntryToCheck;
	EntryToCheck.Actor = ActorToCheck;

	if (ActorToCheck->IsA(ALandscape::StaticClass()) && (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner())) == AlsLocomotionModeTags::Grounded)
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
		FImpactHistoryEntry NewEntry{ ActorToCheck, static_cast<float>(GetWorld()->GetTimeSeconds()) };
		ObstacleImpactHistory.Add(NewEntry);
		return true;
	}
}

bool UAlsxtImpactReactionComponent::ValidateNewAnticipationHit(AActor* ActorToCheck)
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
		FImpactHistoryEntry NewEntry{ ActorToCheck, static_cast<float>(GetWorld()->GetTimeSeconds()) };
		AnticipationImpactHistory.Add(NewEntry);
		return !RecentlyHit;
	}
	else // Actor not in History. Add New Element
	{
		if (AnticipationImpactHistory.Num() >= 6)
		{
			AnticipationImpactHistory.RemoveAt(0);
		}
		FImpactHistoryEntry NewEntry{ ActorToCheck, static_cast<float>(GetWorld()->GetTimeSeconds()) };
		AnticipationImpactHistory.Add(NewEntry);
		return true;
	}
}

void UAlsxtImpactReactionComponent::RefreshObstacleNavigationPoses()
{
	FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwner());
	NewDefensiveModeAnimations.ObstaclePoseSet = SelectObstacleNavigationPoses(IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner())));
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeAnimations(GetOwner(), NewDefensiveModeAnimations);
}

void UAlsxtImpactReactionComponent::RefreshCrowdNavigationPoses()
{
	FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwner());
	NewDefensiveModeAnimations.CrowdNavigationPoseSet =	SelectCrowdNavigationPoses(IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()));
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeAnimations(GetOwner(), NewDefensiveModeAnimations);
}

void UAlsxtImpactReactionComponent::RefreshBlockingPoses()
{
	FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwner());
	NewDefensiveModeAnimations.AnticipationPoseSet = SelectBlockingPoses(IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), ALSXTImpactFormTags::Blunt, ALSXTLocomotionVariantTags::Default);
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeAnimations(GetOwner(), NewDefensiveModeAnimations);
}

FAlsxtDefensivePoseSet UAlsxtImpactReactionComponent::SelectObstacleNavigationPoses(const FGameplayTag& Overlay, const FGameplayTag& Health)
{
	FAlsxtDefensivePoseSet DefensivePoseStanceSet;
	TArray<FObstaclePose> ObstaclePoses = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ObstacleNavigationPoses;
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

FAlsxtDefensivePoseStanceSet UAlsxtImpactReactionComponent::SelectCrowdNavigationPoses(const FGameplayTag& Overlay)
{
	FAlsxtDefensivePoseStanceSet DefensivePoseStanceSet;
	TArray<FCrowdNavigationPoses> CrowdNavigationPoses = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->CrowdNavigationPosesNew;
	
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

FAlsxtAnticipationPoseSet UAlsxtImpactReactionComponent::SelectBlockingPoses(const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Variant)
{
	FAlsxtAnticipationPoseSet DefensivePoseSet;
	TArray<FAlsxtAnticipationPoses> BlockingPoses = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BlockingPoses;
	FGameplayTagContainer ParameterTagContainer;
	ParameterTagContainer.AddTagFast(Overlay);
	ParameterTagContainer.AddTagFast(Form);
	ParameterTagContainer.AddTagFast(Variant);


	for (FAlsxtAnticipationPoses BlockingPose : BlockingPoses)
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

FAlsxtDefensivePoseSet UAlsxtImpactReactionComponent::SelectBraceForImpactPoses(const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Variant)
{
	FAlsxtDefensivePoseSet DefensivePoseSet;
	TArray<FAlsxtBraceForImpactPoses> BlockingPoses = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->BraceForImpactPosesNew;
	FGameplayTagContainer ParameterTagContainer;
	ParameterTagContainer.AddTagFast(Overlay);
	ParameterTagContainer.AddTagFast(Form);
	ParameterTagContainer.AddTagFast(Variant);


	for (FAlsxtBraceForImpactPoses BlockingPose : BlockingPoses)
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

FAlsxtObstacleCollisionAnticipationPoseSet UAlsxtImpactReactionComponent::SelectObstacleCollisionAnticipationPoses(const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Variant)
{
	FAlsxtObstacleCollisionAnticipationPoseSet ObstacleCollisionAnticipationPoseSet;
	TArray<FAlsxtObstacleCollisionAnticipationPoses> ObstacleCollisionAnticipationPoses = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->ObstacleCollisionAnticipationPoses;
	FGameplayTagContainer ParameterTagContainer;
	ParameterTagContainer.AddTagFast(Overlay);
	ParameterTagContainer.AddTagFast(Form);
	ParameterTagContainer.AddTagFast(Variant);


	for (FAlsxtObstacleCollisionAnticipationPoses ObstacleCollisionAnticipationPose : ObstacleCollisionAnticipationPoses)
	{
		FGameplayTagContainer EntryTagContainer;
		EntryTagContainer.AppendTags(ObstacleCollisionAnticipationPose.Overlay);
		EntryTagContainer.AppendTags(ObstacleCollisionAnticipationPose.Form);
		EntryTagContainer.AppendTags(ObstacleCollisionAnticipationPose.Variant);

		if (EntryTagContainer.HasAll(ParameterTagContainer))
		{
			ObstacleCollisionAnticipationPoseSet = ObstacleCollisionAnticipationPose.Poses;
			return ObstacleCollisionAnticipationPoseSet;
		}
	}

	return ObstacleCollisionAnticipationPoseSet;
}

void UAlsxtImpactReactionComponent::ServerSetDefensiveModeState_Implementation(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), NewDefensiveModeState);
}

// ImpactReaction State
void UAlsxtImpactReactionComponent::SetImpactReactionState(const FAlsxtImpactReactionState& NewImpactReactionState)
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

void UAlsxtImpactReactionComponent::SetImpactReactionStateImplementation(const FAlsxtImpactReactionState& NewImpactReactionState)
{
	const auto PreviousImpactReactionState{ ImpactReactionState };
	ImpactReactionState = NewImpactReactionState;
	OnImpactReactionStateChanged(PreviousImpactReactionState);
}

void UAlsxtImpactReactionComponent::ServerSetImpactReactionState_Implementation(const FAlsxtImpactReactionState& NewImpactReactionState)
{
	SetImpactReactionState(NewImpactReactionState);
}

void UAlsxtImpactReactionComponent::ServerProcessNewImpactReactionState_Implementation(const FAlsxtImpactReactionState& NewImpactReactionState)
{
	ProcessNewImpactReactionState(NewImpactReactionState);
}

void UAlsxtImpactReactionComponent::OnReplicate_ImpactReactionState(const FAlsxtImpactReactionState& PreviousImpactReactionState)
{
	OnImpactReactionStateChanged(PreviousImpactReactionState);
}

FDoubleHitResult UAlsxtImpactReactionComponent::GetLastImpact() const
{
	return PreviousImpacts.Last();
}

FAttackDoubleHitResult UAlsxtImpactReactionComponent::GetLastAttackImpact() const
{
	return PreviousAttackImpacts.Last();
}

void UAlsxtImpactReactionComponent::OnImpactReactionStateChanged_Implementation(const FAlsxtImpactReactionState& PreviousImpactReactionState) {}

// CrowdNavigationPose State
void UAlsxtImpactReactionComponent::SetCrowdNavigationPoseState(const FAlsxtBumpPoseState& NewCrowdNavigationPoseState)
{
	const auto PreviousCrowdNavigationPoseState{ CrowdNavigationPoseState };

	CrowdNavigationPoseState = NewCrowdNavigationPoseState;

	OnCrowdNavigationPoseStateChanged(PreviousCrowdNavigationPoseState);

	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
	{
		ServerSetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
	}
}

void UAlsxtImpactReactionComponent::ServerSetCrowdNavigationPoseState_Implementation(const FAlsxtBumpPoseState& NewCrowdNavigationPoseState)
{
	SetCrowdNavigationPoseState(NewCrowdNavigationPoseState);
}

void UAlsxtImpactReactionComponent::ServerProcessNewCrowdNavigationPoseState_Implementation(const FAlsxtBumpPoseState& NewCrowdNavigationPoseState)
{
	ProcessNewCrowdNavigationPoseState(NewCrowdNavigationPoseState);
}

void UAlsxtImpactReactionComponent::OnReplicate_CrowdNavigationPoseState(const FAlsxtBumpPoseState& PreviousCrowdNavigationPoseState)
{
	OnCrowdNavigationPoseStateChanged(PreviousCrowdNavigationPoseState);
}

void UAlsxtImpactReactionComponent::OnCrowdNavigationPoseStateChanged_Implementation(const FAlsxtBumpPoseState& PreviousCrowdNavigationPoseState) {}

// BumpPose State
void UAlsxtImpactReactionComponent::SetBumpPoseState(const FAlsxtBumpPoseState& NewBumpPoseState)
{
	const auto PreviousBumpPoseState{ BumpPoseState };

	BumpPoseState = NewBumpPoseState;

	OnBumpPoseStateChanged(PreviousBumpPoseState);

	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
	{
		ServerSetBumpPoseState(NewBumpPoseState);
	}
}

void UAlsxtImpactReactionComponent::ServerSetBumpPoseState_Implementation(const FAlsxtBumpPoseState& NewBumpPoseState)
{
	SetBumpPoseState(NewBumpPoseState);
}

void UAlsxtImpactReactionComponent::ServerProcessNewBumpPoseState_Implementation(const FAlsxtBumpPoseState& NewBumpPoseState)
{
	ProcessNewBumpPoseState(NewBumpPoseState);
}

void UAlsxtImpactReactionComponent::OnReplicate_BumpPoseState(const FAlsxtBumpPoseState& PreviousBumpPoseState)
{
	OnBumpPoseStateChanged(PreviousBumpPoseState);
}

void UAlsxtImpactReactionComponent::OnReplicate_ObstacleImpactHistory(const TArray<FImpactHistoryEntry> PreviousObstacleImpactHistory)
{
	// OnBumpPoseStateChanged(PreviousBumpPoseState);
}

void UAlsxtImpactReactionComponent::OnReplicate_AnticipationImpactHistory(const TArray<FImpactHistoryEntry> PreviousAnticipationImpactHistory)
{
	// OnBumpPoseStateChanged(PreviousBumpPoseState);
}

void UAlsxtImpactReactionComponent::OnBumpPoseStateChanged_Implementation(const FAlsxtBumpPoseState& PreviousBumpPoseState) {}

// ENTRY FUNCTIONS

// Defensive Reaction
void UAlsxtImpactReactionComponent::DefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	// ...
}

void UAlsxtImpactReactionComponent::SyncedAnticipationReaction(FVector AnticipationPoint)
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
void UAlsxtImpactReactionComponent::CrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
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
void UAlsxtImpactReactionComponent::BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
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
void UAlsxtImpactReactionComponent::ImpactReaction(FDoubleHitResult Hit)
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

// bool UAlsxtImpactReactionComponent::ServerAttackReaction_Validate(FAttackDoubleHitResult Hit)
// {
// 	return true;
// }

void UAlsxtImpactReactionComponent::OnStaticMeshAttackCollision(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::MulticastOnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	OnStaticMeshAttackCollisionImplementation(Hit);
}

void UAlsxtImpactReactionComponent::ServerOnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastOnStaticMeshAttackCollision(Hit);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::OnStaticMeshAttackCollisionImplementation(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackReaction(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::MulticastAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackReaction(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackReaction_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastAttackReaction(Hit);
	StartAttackReaction(Hit);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::AttackReactionImplementation(FAttackDoubleHitResult Hit)
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

		StartAttackReactionImplementation(Hit, SelectedAttackReaction.Montage, Particle, Audio);
		// StartImpactReactionImplementation(Hit.DoubleHitResult, Montage, Particle, Audio);
	}
}

void UAlsxtImpactReactionComponent::StartAttackReaction(FAttackDoubleHitResult Hit)
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

		// IALSXTCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

		// IALSXTCharacterInterface::Execute_GetCharacterMovement(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
			// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		// FALSXTImpactReactionState NewImpactReactionState = GetImpactReactionState();
		// NewImpactReactionState.ImpactReactionParameters.AttackHit = Hit;
		// NewImpactReactionState.ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
		// NewImpactReactionState.ImpactReactionParameters.PlayRate = SelectedAttackReaction.Montage.PlayRate;
		// // ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// NewImpactReactionState.ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
		// // ImpactReactionParameters.Stance = Stance;
		// NewImpactReactionState.ImpactReactionParameters.ImpactVelocity = Hit.Strength;
		// NewImpactReactionState.ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage;
		// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
		// SetImpactReactionState(NewImpactReactionState);

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
			IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
			// MulticastStartAttackReaction(Hit, SelectedAttackReaction.Montage, Particle, Audio);
			StartAttackReactionImplementation(Hit, SelectedAttackReaction.Montage, Particle, Audio);
			OnAttackReactionStarted(Hit);
		}
	}
}

void UAlsxtImpactReactionComponent::ServerStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	MulticastStartAttackReaction(Hit, Montage, Particle, Audio);
	GetOwner()->ForceNetUpdate();
}

// bool UAlsxtImpactReactionComponent::ServerStartAttackReaction_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
// {
// 	return true;
// }

void UAlsxtImpactReactionComponent::MulticastStartAttackReaction_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartAttackReactionImplementation(Hit, Montage, Particle, Audio);
}

void UAlsxtImpactReactionComponent::StartAttackReactionImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsAttackReactionAllowedToStart(Montage.Montage))
	{
		PreviousAttackImpacts.Add(Hit);
		//Anticipation
		FAlsxtDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.AnticipationTransform = { UKismetMathLibrary::MakeRotFromX(Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint), Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
		// Character->SetFacialExpression();

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance)
		{
			OnAttackReactionBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnAttackReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnAttackReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;


		FAlsxtImpactReactionState NewImpactReactionState = GetImpactReactionState();
		NewImpactReactionState.ImpactReactionParameters.AttackHit = Hit;
		NewImpactReactionState.ImpactReactionParameters.BaseDamage = Hit.BaseDamage;
		NewImpactReactionState.ImpactReactionParameters.PlayRate = Montage.PlayRate;
		NewImpactReactionState.ImpactReactionParameters.ImpactType = Hit.DoubleHitResult.ImpactType;
		// ImpactReactionParameters.Stance = Stance;
		NewImpactReactionState.ImpactReactionParameters.ImpactVelocity = Hit.Strength;
		NewImpactReactionState.ImpactReactionParameters.ImpactReactionAnimation.Montage.Montage = Montage.Montage;
		// NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
		SetImpactReactionState(NewImpactReactionState);

		TArray<FName> HitAffectedBones = GetAffectedBones(LocationToImpactSide(Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint), LocationToImpactHeight(Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint));
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, HitAffectedBones);

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
		// FALSXTPhysicalAnimationState NewPhysicalAnimationState;
		// NewPhysicalAnimationState.AffectedBonesBelow = AffectedBones;
		// NewPhysicalAnimationState.Alpha = 1.0f;
		// NewPhysicalAnimationState.Mode = ALSXTPhysicalAnimationModeTags::Hit;
		// NewPhysicalAnimationState.ProfileName = "Hit";
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, AffectedBones);
		// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationState(GetOwner(), NewPhysicalAnimationState);
		//for (FName AffectBone : AffectedBones)
		//{
		//	IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Hit, AffectBone);
		//}

		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);

		for (FName AffectBone : AffectedBones)
		{
			IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 10, AffectBone, false, true);
		}

		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.BoneName, false, true);
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseAtLocation(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.ImpactPoint, Hit.DoubleHitResult.HitResult.HitResult.BoneName);
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);
		//IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.DoubleHitResult.HitResult.Impulse * 2, Hit.DoubleHitResult.HitResult.HitResult.BoneName, false, true);
		// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::None, "pelvis");

		if (IAlsxtCollisionInterface::Execute_CanAttackFall(GetOwner()) && IAlsxtCollisionInterface::Execute_ShouldAttackFall(GetOwner()))
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

void UAlsxtImpactReactionComponent::SyncedAttackReaction(int Index)
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

void UAlsxtImpactReactionComponent::ServerSyncedAttackReaction_Implementation(int32 Index)
{
	// MulticastSyncedAttackReaction(Index);
	StartSyncedAttackReaction(Index);
	GetOwner()->ForceNetUpdate();
}

// Stabilize

void UAlsxtImpactReactionComponent::Stabilize(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::ClutchImpactPoint(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::ImpactFall(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::ImpactFallLand(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::ImpactFallIdle(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackFall(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackFallLand(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackFallIdle(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::SyncedAttackFall(int32 Index)
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

void UAlsxtImpactReactionComponent::SyncedAttackFallIdle(int32 Index)
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

void UAlsxtImpactReactionComponent::BraceForImpact()
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

void UAlsxtImpactReactionComponent::ImpactGetUp(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackGetUp(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::SyncedAttackGetUp(int32 Index)
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

void UAlsxtImpactReactionComponent::CrowdNavigationResponse()
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

void UAlsxtImpactReactionComponent::ImpactResponse(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::AttackResponse(FAttackDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::BodyFallReaction(FDoubleHitResult Hit)
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

void UAlsxtImpactReactionComponent::ImpactTimelineUpdate(float Value)
{
	//...
}

// Error Checks
bool UAlsxtImpactReactionComponent::IsSyncedAnticipationReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsDefensiveReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsCrowdNavigationReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsBumpReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsAttackReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsSyncedAttackReactionAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsClutchImpactPointAllowedToStart(const UAnimSequenceBase* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsStabilizeAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsBraceForImpactAllowedToStart(const UAnimSequenceBase* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsCrowdNavigationFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsImpactFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsAttackFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsSyncedAttackFallAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsImpactResponseAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::IsAttackResponseAllowedToStart(const UAnimMontage* Montage) const
{
	return (Montage != nullptr);
}

bool UAlsxtImpactReactionComponent::ShouldSpawnImpactParticle(FDoubleHitResult Hit)
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

bool UAlsxtImpactReactionComponent::ShouldSpawnRearImpactParticle(FDoubleHitResult Hit)
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

bool UAlsxtImpactReactionComponent::ShouldSpawnFrontImpactDecal(FDoubleHitResult Hit)
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

bool UAlsxtImpactReactionComponent::ShouldSpawnRearImpactDecal(FDoubleHitResult Hit)
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
void UAlsxtImpactReactionComponent::StartSyncedAnticipationReaction(FVector AnticipationPoint)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	// ...
}

void UAlsxtImpactReactionComponent::BumpReactionImplementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), FGameplayTag::EmptyTag);
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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IAlsxtCollisionInterface::Execute_AddCollisionImpulse(GetOwner(), ((GetOwner()->GetVelocity() * -1) + CurrentBumpHit.HitResult.HitResult.GetActor()->GetVelocity()));
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);
		OnBumpReactionStarted();

		if (AnimInstance)
		{
			OnBumpReactionBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnBumpReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnBumpReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
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
		// IALSXTCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), ALSXTPhysicalAnimationModeTags::Bump, AffectedBones);
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.Impulse * 0.5, CurrentImpactReactionState.ImpactReactionParameters.BumpHit.HitResult.HitResult.BoneName, false, true);
	}
}

// void UAlsxtImpactReactionComponent::StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
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

void UAlsxtImpactReactionComponent::StartImpactReaction(FDoubleHitResult Hit)
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

	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);
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
	FAlsxtImpactReactionState NewImpactReactionState;
	NewImpactReactionState.ImpactReactionParameters = ImpactReactionParameters;
	SetImpactReactionState(NewImpactReactionState);

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		// IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->SetRelativeLocationAndRotation(BaseTranslationOffset, BaseRotationOffset);
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
	}
	else
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();

		StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
		ServerStartImpactReaction(Hit, Montage, Particle, Audio);
		OnImpactReactionStarted(Hit);
	}
}

void UAlsxtImpactReactionComponent::StartSyncedAttackReaction(int32 Index)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartStabilize(FDoubleHitResult Hit)
{
	UAnimMontage* Montage;
	FStabilizationMontage SelectedStabilizationMontage = SelectStabilizationMontage(Hit);
	Montage = SelectedStabilizationMontage.Montage.Montage;
	FAlsxtDefensiveModeState DefensiveModeState;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsStabilizeAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stabilize Montage Invalid"));
		return;
	}

	FGameplayTag Health = HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner()));
	FAlsxtDefensiveModeAnimations DefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwner());
	DefensiveModeAnimations.ImpactMontage = Montage;
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeAnimations(GetOwner(), DefensiveModeAnimations);
	DefensiveModeState.ImpactTransform = { UKismetMathLibrary::MakeRotFromX(Hit.HitResult.HitResult.ImpactPoint), Hit.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
	float MontageLength = AnimInstance->Montage_Play(Montage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f);

	if (!IAlsxtCharacterInterface::Execute_IsCharacterPlayerControlled(GetOwner()))
	{
		// Disable AI Movement
		if (GetOwner()->GetClass()->ImplementsInterface(UAlsxtAIInterface::StaticClass()))
		{
			IAlsxtAIInterface::Execute_SetLockMovement(GetOwner(), true);
		}	
	}

	if (AnimInstance)
	{
		OnStabilizeBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnStabilizationBlendOut);
		AnimInstance->Montage_SetBlendingOutDelegate(OnStabilizeBlendOutDelegate);
	}
	IAlsxtCharacterInterface::Execute_ResetCharacterDefensiveModeState(GetOwner());
	IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::Stabilization);
}

void UAlsxtImpactReactionComponent::StartClutchImpactPoint(FDoubleHitResult Hit)
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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartClutchImpactPoint(SelectedClutchImpactPointPose, Hit.HitResult.HitResult.ImpactPoint);
		OnImpactReactionStarted(Hit);
	}
}

void UAlsxtImpactReactionComponent::StartImpactFall(FDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FFallenAnimation FallenAnimations = SelectImpactFallAnimations(Hit);
	FAlsxtImpactReactionState NewImpactReactionState = GetImpactReactionState();
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

	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartImpactFall(Hit, Montage, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UAlsxtImpactReactionComponent::StartImpactFallLand(FDoubleHitResult Hit)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartImpactFallIdle(FDoubleHitResult Hit)
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

	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartImpactFall(Hit, Montage, Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UAlsxtImpactReactionComponent::StartAttackFall(FAttackDoubleHitResult Hit)
{
	IAlsxtCharacterInterface::Execute_SetCharacterRagdoll(GetOwner(), true);

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

void UAlsxtImpactReactionComponent::StartAttackFallLand(FAttackDoubleHitResult Hit)
{
	StartAttackFallIdle(Hit);
}

void UAlsxtImpactReactionComponent::StartAttackFallIdle(FAttackDoubleHitResult Hit)
{
	// if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	// {
	// 	return;
	// }

	FAlsxtImpactReactionState NewImpactReactionState = GetImpactReactionState();
	FActionMontageInfo SelectedAttackFall = SelectAttackFallMontage(Hit);
	// FActionMontageInfo Montage = SelectedAttackFall;
	UAnimMontage* Montage = NewImpactReactionState.ImpactReactionParameters.AttackFallenAnimations.FallenPose;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsAttackFallAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Fall Idle Montage Invalid"));
		return;
	}

	const auto StartYawAngle{ UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(GetOwner()->GetActorRotation().Yaw)) };

	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);
	IAlsxtCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);

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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartAttackFallIdle(Montage, Hit);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFall(int32 Index)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFallLand(int32 Index)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFallIdle(int32 Index)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartBraceForImpact()
{
	const FGameplayTag Side{ALSXTImpactPositionTags::Front};
	UAnimSequenceBase* Montage = SelectBraceForImpactPose(Side, ALSXTImpactFormTags::Blunt);
	FAlsxtDefensiveModeState DefensiveModeState;

	if ((!ALS_ENSURE(IsValid(Montage)) || !IsBraceForImpactAllowedToStart(Montage)) && ImpactReactionSettings.DebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Brace For Impact Montage Invalid"));
		return;
	}
	
	DefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::BraceForImpact;
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
	IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::BraceForImpact);
}

void UAlsxtImpactReactionComponent::StartImpactGetUp(FDoubleHitResult Hit)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartAttackGetUp(FAttackDoubleHitResult Hit)
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
			IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
			MulticastStartAttackGetUp(Hit, Montage);
			// OnImpactReactionStarted(Hit.DoubleHitResult);
		}
	}
	
}

void UAlsxtImpactReactionComponent::StartSyncedAttackGetUp(int32 Index)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartImpactResponse(FDoubleHitResult Hit)
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

	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		// MulticastStartImpactResponse(Hit, Montage.Montage);
		StartImpactResponseImplementation(Hit, Montage.Montage);
	}

}

void UAlsxtImpactReactionComponent::StartAttackResponse(FAttackDoubleHitResult Hit)
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
	IAlsxtCharacterInterface::Execute_SetCharacterMovementModeLocked(GetOwner(), true);

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
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->FlushServerMoves();
		MulticastStartAttackResponse(Hit, Montage.Montage);
		// StartAttackResponseImplementation(Hit, Montage.Montage);
		// OnImpactReactionStarted(Hit.DoubleHitResult);
	}
}

void UAlsxtImpactReactionComponent::CrowdNavigationFall() {}

//Parameters
UAlsxtImpactReactionSettings* UAlsxtImpactReactionComponent::SelectImpactReactionSettings_Implementation()
{
	return nullptr;
}
 
UMaterialInterface* UAlsxtImpactReactionComponent::GetImpactDecal(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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


UNiagaraSystem* UAlsxtImpactReactionComponent::GetImpactReactionParticle(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

UNiagaraSystem* UAlsxtImpactReactionComponent::GetImpactPointParticle(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FALSXTCharacterSound UAlsxtImpactReactionComponent::GetAttackReactionSound(FDoubleHitResult Hit)
{
	FALSXTCharacterSound FoundAttackReactionSound{ nullptr };
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());

	return FoundAttackReactionSound;
}

FSound UAlsxtImpactReactionComponent::GetImpactReactionSound(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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
	// return FoundImpactSound;
}

// UNiagaraSystem* UAlsxtImpactReactionComponent::GetBodyFallParticle(FDoubleHitResult Hit)
// {
// 
// }

// FALSXTCharacterSound UAlsxtImpactReactionComponent::GetImpactReactionSound(FDoubleHitResult Hit)
// {
// 
// }
// 
// FALSXTCharacterSound UAlsxtImpactReactionComponent::GetAttackReactionSound(FDoubleHitResult Hit)
// {
// 
// }
// 
// FALSXTCharacterSound UAlsxtImpactReactionComponent::GetBodyFallSound(FDoubleHitResult Hit)
// {
// 
// }

UAlsxtElementalInteractionSettings* UAlsxtImpactReactionComponent::GetElementalInteractionSettings_Implementation()
{
	return nullptr;
}

UAnimSequenceBase* UAlsxtImpactReactionComponent::SelectBraceForImpactPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FAnticipationPose UAlsxtImpactReactionComponent::SelectImpactAnticipationMontage_Implementation(const FGameplayTag& Velocity , const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FAnticipationPose UAlsxtImpactReactionComponent::SelectAttackAnticipationMontage_Implementation(const FGameplayTag& CharacterCombatStance, const FGameplayTag& Strength, const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FAnticipationPose UAlsxtImpactReactionComponent::SelectDefensiveMontage_Implementation(const FGameplayTag& Strength, const FGameplayTag& Stance, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FBumpReactionAnimation UAlsxtImpactReactionComponent::SelectBumpReactionMontage_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

UAnimSequenceBase* UAlsxtImpactReactionComponent::SelectCrowdNavigationPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpPose> Reactions = SelectedImpactReactionSettings->CrowdNavigationPoses;
	TArray<FBumpPose> FilteredReactions;
	FBumpPose SelectedCrowdNavigationReactionAnimation;
	TArray<FGameplayTag> TagsArray = { IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()), Side, ALSXTImpactFormTags::Blunt };
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

FBumpReactionAnimation UAlsxtImpactReactionComponent::SelectCrowdNavigationReactionMontage_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

UAnimSequenceBase* UAlsxtImpactReactionComponent::SelectBumpAnticipationPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FBumpPose> Reactions = SelectedImpactReactionSettings->BumpAnticipationPoses;
	TArray<FBumpPose> FilteredReactions;
	FBumpPose SelectedBumpPose;
	TArray<FGameplayTag> TagsArray = { IAlsxtCharacterInterface::Execute_GetCharacterGait(GetOwner()), Side, ALSXTImpactFormTags::Blunt };
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

FAttackReactionAnimation UAlsxtImpactReactionComponent::SelectAttackReactionMontage_Implementation(FAttackDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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
			FilteredMontages.RemoveAt(IndexToRemove, EAllowShrinking::Yes);
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

FImpactReactionAnimation UAlsxtImpactReactionComponent::SelectImpactReactionMontage_Implementation(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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
			FilteredMontages.RemoveAt(IndexToRemove, EAllowShrinking::Yes);
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

FSyncedAttackAnimation UAlsxtImpactReactionComponent::GetSyncedMontage_Implementation(int Index)
{
	UAlsxtCombatSettings* SelectedCombatSettings = IAlsxtCombatInterface::Execute_SelectCombatSettings(this);
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

FStabilizationMontage UAlsxtImpactReactionComponent::SelectStabilizationMontage_Implementation(FDoubleHitResult Hit)
{
	TArray<FStabilizationMontage> StabilizationMontages = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner())->StabilizationMontages;
	TArray<FStabilizationMontage> FilteredStabilizationMontages;
	FStabilizationMontage SelectedMontage;
	const FGameplayTag Health = HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner()));
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

FClutchImpactLocationAnimation UAlsxtImpactReactionComponent::SelectClutchImpactPointMontage_Implementation(FDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
	TArray<FClutchImpactLocationAnimation> Animations = SelectedImpactReactionSettings->ClutchImpactPointAnimations;
	TArray<FClutchImpactLocationAnimation> FilteredAnimations;
	FClutchImpactLocationAnimation SelectedClutchImpactPointAnimation;
	const FGameplayTag Health = HealthToHealthTag(IAlsxtCharacterInterface::Execute_GetHealth(GetOwner()));

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

FAnticipationPose UAlsxtImpactReactionComponent::SelectSteadyMontage_Implementation(const FGameplayTag& Side)
{
	FAnticipationPose SelectedMontage;
	return SelectedMontage;
}

FFallenAnimation UAlsxtImpactReactionComponent::SelectImpactFallAnimations_Implementation(FDoubleHitResult Hit)
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

FFallenAnimation UAlsxtImpactReactionComponent::SelectAttackFallAnimations_Implementation(FAttackDoubleHitResult Hit)
{
	UAlsxtImpactReactionSettings* SelectedImpactReactionSettings = IAlsxtCollisionInterface::Execute_SelectImpactReactionSettings(GetOwner());
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

FActionMontageInfo UAlsxtImpactReactionComponent::SelectImpactFallMontage_Implementation(FDoubleHitResult Hit)
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

FActionMontageInfo UAlsxtImpactReactionComponent::SelectAttackFallMontage_Implementation(FAttackDoubleHitResult Hit)
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

// UAnimSequenceBase* UAlsxtImpactReactionComponent::SelectBraceForImpactPose_Implementation(const FGameplayTag& Side, const FGameplayTag& Form)
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

UAnimMontage* UAlsxtImpactReactionComponent::SelectImpactFallenPose_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage = GetImpactReactionState().ImpactReactionParameters.ImpactFallenAnimations.FallenPose;
	return SelectedMontage;
}

UAnimMontage* UAlsxtImpactReactionComponent::SelectAttackFallenPose_Implementation(FDoubleHitResult Hit)
{
	UAnimMontage* SelectedMontage = GetImpactReactionState().ImpactReactionParameters.AttackFallenAnimations.FallenPose;
	return SelectedMontage;
}

FActionMontageInfo UAlsxtImpactReactionComponent::SelectImpactGetUpMontage_Implementation(FDoubleHitResult Hit)
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

FActionMontageInfo UAlsxtImpactReactionComponent::SelectAttackGetUpMontage_Implementation(FAttackDoubleHitResult Hit)
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

FResponseAnimation UAlsxtImpactReactionComponent::SelectImpactResponseMontage_Implementation(FDoubleHitResult Hit)
{
	FResponseAnimation SelectedMontage;
	return SelectedMontage;
}

FResponseAnimation UAlsxtImpactReactionComponent::SelectAttackResponseMontage_Implementation(FAttackDoubleHitResult Hit)
{
	FResponseAnimation SelectedMontage;
	return SelectedMontage;
}

// RPCs

void UAlsxtImpactReactionComponent::ServerCrouch_Implementation()
{
	IAlsxtCharacterInterface::Execute_SetCharacterStance(GetOwner(), AlsStanceTags::Crouching);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::ServerGetUp_Implementation()
{
	IAlsxtCharacterInterface::Execute_SetCharacterStance(GetOwner(), AlsStanceTags::Standing);
	GetOwner()->ForceNetUpdate();
}


void UAlsxtImpactReactionComponent::ServerBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastBumpReaction(Gait, Side, Form);
	// GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::MulticastBumpReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	BumpReactionImplementation(Gait, Side, Form);
}

void UAlsxtImpactReactionComponent::ServerCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	MulticastCrowdNavigationReaction(Gait, Side, Form);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::MulticastCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	CrowdNavigationReactionImplementation(Gait, Side, Form);
}

void UAlsxtImpactReactionComponent::ServerSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	MulticastSyncedAnticipationReaction(AnticipationPoint);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::MulticastSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	StartSyncedAnticipationReaction(AnticipationPoint);
}

void UAlsxtImpactReactionComponent::ServerDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	MulticastDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::MulticastDefensiveReaction_Implementation(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint)
{
	StartDefensiveReaction(Velocity, Side, Form, AnticipationPoint);
}

void UAlsxtImpactReactionComponent::ServerImpactReaction_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactReaction(Hit);
	GetOwner()->ForceNetUpdate();
}

void UAlsxtImpactReactionComponent::MulticastImpactReaction_Implementation(FDoubleHitResult Hit)
{
	StartImpactReaction(Hit);
}

bool UAlsxtImpactReactionComponent::ServerSyncedAttackReaction_Validate(int32 Index)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastSyncedAttackReaction_Implementation(int32 Index)
{
	StartSyncedAttackReaction(Index);
}

//

void UAlsxtImpactReactionComponent::ServerStabilize_Implementation(FDoubleHitResult Hit)
{
	MulticastStabilize(Hit);
	// StartClutchImpactPoint(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerStabilize_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStabilize_Implementation(FDoubleHitResult Hit)
{
	StartStabilize(Hit);
}

void UAlsxtImpactReactionComponent::ServerClutchImpactPoint_Implementation(FDoubleHitResult Hit)
{
	MulticastClutchImpactPoint(Hit);
	// StartClutchImpactPoint(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerClutchImpactPoint_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastClutchImpactPoint_Implementation(FDoubleHitResult Hit)
{
	StartClutchImpactPoint(Hit);
}

void UAlsxtImpactReactionComponent::ServerImpactFall_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFall(Hit);
	StartImpactFall(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerImpactFall_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastImpactFall_Implementation(FDoubleHitResult Hit)
{
	StartImpactFall(Hit);
}

void UAlsxtImpactReactionComponent::ServerImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFall(Hit);
	StartImpactFallLand(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerImpactFallLand_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastImpactFallLand_Implementation(FDoubleHitResult Hit)
{
	StartImpactFallLand(Hit);
}

void UAlsxtImpactReactionComponent::ServerImpactFallIdle_Implementation(FDoubleHitResult Hit)
{
	// MulticastStartImpactFallIdle(Hit);
	StartImpactFallIdle(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerImpactFallIdle_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastImpactFallIdle_Implementation(FDoubleHitResult Hit)
{
	StartImpactFallIdle(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackFall_Implementation(FAttackDoubleHitResult Hit)
{
	MulticastAttackFall(Hit);
	// StartAttackFall(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerAttackFall_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastAttackFall_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFall(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFall(Hit);
	StartAttackFallLand(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerAttackFallLand_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastAttackFallLand_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFallLand(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackFallIdle_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackFallIdle(Hit);
	StartAttackFallIdle(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerAttackFallIdle_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastAttackFallIdle_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackFallIdle(Hit);
}

void UAlsxtImpactReactionComponent::ServerSyncedAttackFall_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFall(Index);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerSyncedAttackFall_Validate(int32 Index)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastSyncedAttackFall_Implementation(int32 Index)
{
	StartSyncedAttackFall(Index);
}

void UAlsxtImpactReactionComponent::ServerSyncedAttackFallLand_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFallLand(Index);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerSyncedAttackFallLand_Validate(int32 Index)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastSyncedAttackFallLand_Implementation(int32 Index)
{
	StartSyncedAttackFallLand(Index);
}

void UAlsxtImpactReactionComponent::ServerSyncedAttackFallIdle_Implementation(int32 Index)
{
	// MulticastStartClutchImpactPoint(Hit);
	StartSyncedAttackFallIdle(Index);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerSyncedAttackFallIdle_Validate(int32 Index)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastSyncedAttackFallIdle_Implementation(int32 Index)
{
	StartSyncedAttackFallIdle(Index);
}

void UAlsxtImpactReactionComponent::ServerBraceForImpact_Implementation()
{
	// MulticastBraceForImpact();
	StartBraceForImpact();
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerBraceForImpact_Validate()
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastBraceForImpact_Implementation()
{
	StartBraceForImpact();
}

void UAlsxtImpactReactionComponent::ServerImpactGetUp_Implementation(FDoubleHitResult Hit)
{
	// MulticastImpactGetUp(Hit);
	StartImpactGetUp(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerImpactGetUp_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastImpactGetUp_Implementation(FDoubleHitResult Hit)
{
	StartImpactGetUp(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackGetUp_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackGetUp(Hit);
	StartAttackGetUp(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerAttackGetUp_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastAttackGetUp_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackGetUp(Hit);
}

void UAlsxtImpactReactionComponent::ServerSyncedAttackGetUp_Implementation(int32 Index)
{
	// MulticastSyncedAttackGetUp(Index);
	StartSyncedAttackGetUp(Index);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerSyncedAttackGetUp_Validate(int32 Index)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastSyncedAttackGetUp_Implementation(int32 Index)
{
	StartSyncedAttackGetUp(Index);
}

void UAlsxtImpactReactionComponent::ServerImpactResponse_Implementation(FDoubleHitResult Hit)
{
	MulticastImpactResponse(Hit);
	// StartImpactResponse(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerImpactResponse_Validate(FDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastImpactResponse_Implementation(FDoubleHitResult Hit)
{
	StartImpactResponse(Hit);
}

void UAlsxtImpactReactionComponent::ServerAttackResponse_Implementation(FAttackDoubleHitResult Hit)
{
	// MulticastAttackResponse(Hit);
	StartAttackResponse(Hit);
	GetOwner()->ForceNetUpdate();
}

bool UAlsxtImpactReactionComponent::ServerAttackResponse_Validate(FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastAttackResponse_Implementation(FAttackDoubleHitResult Hit)
{
	StartAttackResponse(Hit);
}

// Start RPCs

void UAlsxtImpactReactionComponent::ServerStartSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	// if (IsImpactReactionAllowedToStart(Montage.Montage))
	// {
	// 	MulticastStartSyncedAnticipationReaction(Montage, AnticipationPoint);
	// 	GetOwner()->ForceNetUpdate();
	// }
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAnticipationReaction_Validate(FVector AnticipationPoint)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAnticipationReaction_Implementation(FVector AnticipationPoint)
{
	// StartSyncedAnticipationReactionImplementation(Montage, AnticipationPoint);
}

void UAlsxtImpactReactionComponent::ServerStartDefensiveReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartDefensiveReaction(Montage, Audio, AnticipationPoint);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartDefensiveReaction_Validate(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartDefensiveReaction_Implementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	StartDefensiveReactionImplementation(Montage, Audio, AnticipationPoint);
}

// void UAlsxtImpactReactionComponent::ServerCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	MulticastCrowdNavigationReaction(Gait, Side, Form);
// 	GetOwner()->ForceNetUpdate();
// }

// bool UAlsxtImpactReactionComponent::ServerCrowdNavigationReaction_Validate(FActionMontageInfo Montage, USoundBase* Audio)
// {
// 	return true;
// }

// void UAlsxtImpactReactionComponent::MulticastCrowdNavigationReaction_Implementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
// {
// 	CrowdNavigationReactionImplementation(Gait, Side, Form);
// }

void UAlsxtImpactReactionComponent::ServerStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartImpactReaction(Hit, Montage, Particle, Audio);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartImpactReaction_Validate(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactReaction_Implementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	StartImpactReactionImplementation(Hit, Montage, Particle, Audio);
}

void UAlsxtImpactReactionComponent::ServerStartSyncedAttackReaction_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackReaction(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAttackReaction_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAttackReaction_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackReactionImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartStabilize_Implementation(UAnimMontage* Pose, FVector ImpactPoint)
{
	if (IsStabilizeAllowedToStart(Pose))
	{
		MulticastStartStabilize(Pose, ImpactPoint);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartStabilize_Validate(UAnimMontage* Pose, FVector ImpactPoint)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartStabilize_Implementation(UAnimMontage* Pose, FVector ImpactPoint)
{
	StartStabilizeImplementation(Pose, ImpactPoint);
}

void UAlsxtImpactReactionComponent::ServerStartClutchImpactPoint_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	if (IsClutchImpactPointAllowedToStart(Pose))
	{
		MulticastStartClutchImpactPoint(Pose, ImpactPoint);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartClutchImpactPoint_Validate(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartClutchImpactPoint_Implementation(UAnimSequenceBase* Pose, FVector ImpactPoint)
{
	StartClutchImpactPointImplementation(Pose, ImpactPoint);
}

void UAlsxtImpactReactionComponent::ServerStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactFallAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFall(Hit, Montage, FallMontage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartImpactFall_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactFall_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	StartImpactFallImplementation(Hit, Montage, FallMontage);
}

bool UAlsxtImpactReactionComponent::ServerStartImpactFallIdle_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactFallIdle_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactFallIdleImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartImpactFallIdle_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFallIdle(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

void UAlsxtImpactReactionComponent::ServerStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		// MulticastStartAttackFall(Hit, Montage);
		StartAttackFallImplementation(Hit, Montage, FallMontage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartAttackFall_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartAttackFall_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	StartAttackFallImplementation(Hit, Montage, FallMontage);
}

void UAlsxtImpactReactionComponent::ServerStartAttackFallIdle_Implementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartAttackFallIdle(Montage, Hit);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartAttackFallIdle_Validate(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartAttackFallIdle_Implementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	StartAttackFallIdleImplementation(Montage, Hit);
}

void UAlsxtImpactReactionComponent::ServerStartSyncedAttackFall_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFall(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAttackFall_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAttackFall_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartSyncedAttackFallIdle_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFallIdle(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAttackFallIdle_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAttackFallIdle_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallIdleImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartBraceForImpact_Implementation(UAnimMontage* Montage)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{
		MulticastStartBraceForImpact(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartBraceForImpact_Validate(UAnimMontage* Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartBraceForImpact_Implementation(UAnimMontage* Montage)
{
	StartBraceForImpactImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartImpactFallLand_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactFallLand(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartImpactFallLand_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactFallLand_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactFallLandImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartAttackFallLand_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackFallLand(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartAttackFallLand_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartAttackFallLand_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackFallLandImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartSyncedAttackFallLand_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackFallLand(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAttackFallLand_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAttackFallLand_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackFallLandImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartImpactGetUp_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactGetUp(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartImpactGetUp_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactGetUp_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactGetUpImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartAttackGetUp_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackGetUp(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartAttackGetUp_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartAttackGetUp_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackGetUpImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartSyncedAttackGetUp_Implementation(FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartSyncedAttackGetUp(Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartSyncedAttackGetUp_Validate(FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartSyncedAttackGetUp_Implementation(FActionMontageInfo Montage)
{
	StartSyncedAttackGetUpImplementation(Montage);
}

void UAlsxtImpactReactionComponent::ServerStartImpactResponse_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartImpactResponse(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartImpactResponse_Validate(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartImpactResponse_Implementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartImpactResponseImplementation(Hit, Montage);
}

void UAlsxtImpactReactionComponent::ServerStartAttackResponse_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{
		MulticastStartAttackResponse(Hit, Montage);
		GetOwner()->ForceNetUpdate();
	}
}

bool UAlsxtImpactReactionComponent::ServerStartAttackResponse_Validate(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	return true;
}

void UAlsxtImpactReactionComponent::MulticastStartAttackResponse_Implementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	StartAttackResponseImplementation(Hit, Montage);
}

// Start Implementations

void UAlsxtImpactReactionComponent::StartSyncedAnticipationReactionImplementation(FActionMontageInfo Montage, FVector AnticipationPoint)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartDefensiveReactionImplementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint)
{
	// ...
}

void UAlsxtImpactReactionComponent::CrowdNavigationReactionImplementation(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form)
{
	FAlsxtImpactReactionState IRState = GetImpactReactionState();
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
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(SelectedAnimation, 1.0f);

		// Set Timer to Pause Animation if Velocity Stops
		// GetWorld()->GetTimerManager().SetTimer(CrowdNavigationVelocityTimerHandle, CrowdNavigationVelocityTimerDelegate, 0.1f, true);


		if (AnimInstance)
		{
			OnCrowdNavigationReactionBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnCrowdNavigationReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnCrowdNavigationReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
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
		FAlsxtBumpPoseState NewCrowdNavigationPoseState;
		NewCrowdNavigationPoseState.Actor = GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.GetActor();
		NewCrowdNavigationPoseState.Mode = ALSXTBumpPoseModeTags::CrowdNavigation;
		NewCrowdNavigationPoseState.Pose = SelectedAnimation;
		NewCrowdNavigationPoseState.Form = Form;
		NewCrowdNavigationPoseState.Side = Side;
		NewCrowdNavigationPoseState.Location = GetImpactReactionState().ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.Location;
		FAlsxtDefensiveModeState NewDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		// NewDefensiveModeState.AnticipationPose = SelectedAnimation;
		NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::ObstacleNavigation;
		NewDefensiveModeState.ObstacleMode = ALSXTDefensiveModeTags::CrowdNavigation;
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
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, AffectedBones);
		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.Impulse * -1000, CurrentImpactReactionState.ImpactReactionParameters.CrowdNavigationHit.HitResult.HitResult.BoneName, false, true);

		if (IAlsxtCollisionInterface::Execute_ShouldCrowdNavigationFall(GetOwner()))
		{
			CrowdNavigationFall();
		}
		else
		{
			if (IAlsxtCollisionInterface::Execute_ShouldClutchImpactPoint(GetOwner()))
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

void UAlsxtImpactReactionComponent::StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, UNiagaraSystem* Particle, USoundBase* Audio)
{
	//if (IsImpactReactionAllowedToStart(Montage) && IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f))
	if (IsImpactReactionAllowedToStart(Montage))
	{
		//Anticipation
		FAlsxtDefensiveModeState DefensiveModeState;
		DefensiveModeState.Mode = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.AnticipationMode = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveMode(GetOwner());
		DefensiveModeState.ImpactTransform = { UKismetMathLibrary::MakeRotFromX(Hit.HitResult.HitResult.ImpactPoint), Hit.HitResult.HitResult.ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), DefensiveModeState);
		// Character->SetFacialExpression();

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);

		if (AnimInstance)
		{
			OnImpactReactionBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnImpactReactionBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnImpactReactionBlendOutDelegate);
		}

		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
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
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, AffectedBones);
		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * -1000, Hit.HitResult.HitResult.BoneName, false, true);
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		if (IAlsxtCollisionInterface::Execute_CanImpactFall(GetOwner()) && IAlsxtCollisionInterface::Execute_ShouldImpactFall(GetOwner()))
		{
			ImpactFall(Hit);
			return;
		}
		else
		{
			if (IAlsxtCollisionInterface::Execute_ShouldPerformImpactResponse(GetOwner()))
			{
				ImpactResponse(Hit);
				return;
			}

		}
	}
}

void UAlsxtImpactReactionComponent::StartSyncedAttackReactionImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartStabilizeImplementation(UAnimMontage* Montage, FVector ImpactPoint)
{
	if (IsStabilizeAllowedToStart(Montage))
	{
		// Character->SetFacialExpression();

		FAlsxtDefensiveModeState CurrentDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		CurrentDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		FAlsxtDefensiveModeAnimations DefensiveModeAnimations = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeAnimations(GetOwner());
		DefensiveModeAnimations.ImpactMontage = Montage;
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeAnimations(GetOwner(), DefensiveModeAnimations);
		CurrentDefensiveModeState.ImpactTransform = { UKismetMathLibrary::MakeRotFromX(ImpactPoint), ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), CurrentDefensiveModeState);
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::ClutchImpactPoint);
		// IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartClutchImpactPointImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint)
{
	// UAnimSequenceBase* SelectedClutchImpactPointPose;
	// FClutchImpactLocationAnimation SelectedClutchImpactPointReaction = SelectClutchImpactPointMontage(Hit);
	// SelectedClutchImpactPointPose = SelectedClutchImpactPointReaction.Pose;
	
	
	
	// if (IsClutchImpactPointAllowedToStart(Montage))
	// {
		// Character->SetFacialExpression();

		FAlsxtDefensiveModeState CurrentDefensiveModeState = IAlsxtCharacterInterface::Execute_GetCharacterDefensiveModeState(GetOwner());
		CurrentDefensiveModeState.Mode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		CurrentDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::ClutchImpactPoint;
		// CurrentDefensiveModeState.Montage = Montage;
		CurrentDefensiveModeState.ImpactTransform = { UKismetMathLibrary::MakeRotFromX(ImpactPoint), ImpactPoint, { 1.0f, 1.0f, 1.0f } };
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveModeState(GetOwner(), CurrentDefensiveModeState);
		IAlsxtCharacterInterface::Execute_SetCharacterDefensiveMode(GetOwner(), ALSXTDefensiveModeTags::ClutchImpactPoint);
		// IALSXTCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		StartClutchImpactPointTimer();
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsClutchImpactPointNOTAllowedToStart"));
	// }
}

void UAlsxtImpactReactionComponent::StartImpactFallImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsImpactFallAllowedToStart(Montage.Montage))
	{

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();

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
			OnImpactFallBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnImpactFallBlendOut);
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
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, AffectedBones);
		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactFall);
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);

		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// IALSXTCharacterInterface::Execute_SetDesiredStance(GetOwner(), AlsStanceTags::Crouching);
		Cast<ACharacter>(GetOwner())->Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartImpactFallIdleImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactReactionAllowedToStart(Montage.Montage))
	{

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance)
		{
			OnImpactFallBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnImpactFallBlendOut);
			AnimInstance->Montage_SetBlendingOutDelegate(OnImpactFallBlendOutDelegate);
		}

		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();

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
		IAlsxtCollisionInterface::Execute_SetCharacterPhysicalAnimationMode(GetOwner(), AlsxtPhysicalAnimationModeTags::Hit, AffectedBones);
		IAlsxtCharacterInterface::Execute_SetCharacterLocomotionAction(GetOwner(), AlsLocomotionActionTags::ImpactReaction);
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->AddImpulseToAllBodiesBelow(Hit.HitResult.Impulse * 1000, CurrentImpactReactionState.ImpactReactionParameters.ImpactHit.HitResult.HitResult.BoneName, false, true);
		// Character->ALSXTRefreshRotationInstant(StartYawAngle, ETeleportType::None);

		// Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartAttackFallImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, FActionMontageInfo FallMontage)
{
	if (IsAttackFallAllowedToStart(Montage.Montage))
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();		
		FVector EndLocation{ FVector::ZeroVector };

		if (GetAttackFallLocation(EndLocation, Hit))
		{
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), EndLocation, 90.0f, 30.0f, IAlsxtCharacterInterface::Execute_GetCharacterControlRotation(GetOwner()), FLinearColor::Red, 1.0, 1.0);
			CurrentImpactReactionState.ImpactReactionParameters.FallLandLocation = EndLocation;
			SetImpactReactionState(CurrentImpactReactionState);
		}
		else
		{
			StartAttackFallingTimer();
		}

		if (AnimInstance)
		{
			OnAttackFallBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnAttackFallBlendOut);
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

		IAlsxtCharacterInterface::Execute_SetCharacterStatus(GetOwner(), ALSXTStatusTags::KnockedDown);
		OnFallStarted();
		// ServerCrouch();
		// Character->Crouch(); //Hack
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartAttackFallIdleImplementation(UAnimMontage* Montage, FAttackDoubleHitResult Hit)
{
	if (IsImpactReactionAllowedToStart(Montage))
	{

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage, 1.0f);

		if (AnimInstance)
		{
			OnAttackFallEndedDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnAttackFallEnded);
			// StartAttackGetUp();
			AnimInstance->Montage_SetEndDelegate(OnAttackFallEndedDelegate);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFallImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFallIdleImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartBraceForImpactImplementation(UAnimMontage* Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartImpactFallLandImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartAttackFallLandImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartSyncedAttackFallLandImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartImpactGetUpImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartAttackGetUpImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);

		if (AnimInstance && IAlsxtCollisionInterface::Execute_ShouldPerformAttackResponse(GetOwner()))
		{
			OnAttackFallGetupBlendOutDelegate.BindUObject(this, &UAlsxtImpactReactionComponent::OnAttackFallGetupBlendOut);
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

void UAlsxtImpactReactionComponent::StartSyncedAttackGetUpImplementation(FActionMontageInfo Montage)
{
	// ...
}

void UAlsxtImpactReactionComponent::StartImpactResponseImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsImpactResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsImpactReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::StartAttackResponseImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage)
{
	if (IsAttackResponseAllowedToStart(Montage.Montage))
	{
		// Character->SetFacialExpression();

		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(Montage.Montage, 1.0f);
		// ImpactReactionState.TargetYawAngle = TargetYawAngle;
		FAlsxtImpactReactionState CurrentImpactReactionState = GetImpactReactionState();
		// CurrentImpactReactionState.ImpactReactionParameters.TargetYawAngle = TargetYawAngle;
		// CurrentImpactReactionState.ImpactReactionParameters.Target = PotentialAttackTarget;

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("IsAttackReactionNOTAllowedToStart"));
	}
}

void UAlsxtImpactReactionComponent::SpawnParticleActorImplementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor)
{}

// Refresh

void UAlsxtImpactReactionComponent::RefreshSyncedAnticipationReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopSyncedAnticipationReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAnticipationReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshSyncedAnticipationReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshDefensiveReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::DefensiveReaction)
	{
		StopDefensiveReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshDefensiveReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshDefensiveReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshCrowdNavigationReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::CrowdNavigationReaction)
	{
		StopCrowdNavigationReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshCrowdNavigationReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshCrowdNavigationReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshBumpReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopBumpReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshBumpReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshBumpReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshImpactReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopImpactReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshImpactReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshImpactReactionPhysics(const float DeltaTime)
{
	float Offset = ImpactReactionSettings.RotationOffset;
	auto ComponentRotation{ IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->UpdatedComponent->GetComponentRotation() };
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto TargetRotation{ PlayerController->GetControlRotation() };
	TargetRotation.Yaw = TargetRotation.Yaw + Offset;
	TargetRotation.Pitch = ComponentRotation.Pitch;
	TargetRotation.Roll = ComponentRotation.Roll;

	if (ImpactReactionSettings.RotationInterpolationSpeed <= 0.0f)
	{
		TargetRotation.Yaw = ImpactReactionState.ImpactReactionParameters.TargetYawAngle;

		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		TargetRotation.Yaw = UAlsRotation::DamperExactAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(TargetRotation.Yaw)),
			ImpactReactionState.ImpactReactionParameters.TargetYawAngle, DeltaTime,
			ImpactReactionSettings.RotationInterpolationSpeed);

		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->MoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false);
	}
}

void UAlsxtImpactReactionComponent::RefreshAttackReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactReaction)
	{
		StopAttackReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshAttackReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshAttackReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshSyncedAttackReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::SyncedAttackReaction)
	{
		StopSyncedAttackReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAttackReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshSyncedAttackReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshCrowdNavigationFallReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopCrowdNavigationFallReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshCrowdNavigationFallReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshCrowdNavigationFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshImpactFallReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopImpactFallReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshImpactFallReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshImpactFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshAttackFallReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopAttackFallReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshAttackFallReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshAttackFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::RefreshSyncedAttackFallReaction(const float DeltaTime)
{
	if (IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) != AlsLocomotionActionTags::ImpactFall)
	{
		StopSyncedAttackFallReaction();
		GetOwner()->ForceNetUpdate();
	}
	else
	{
		RefreshSyncedAttackFallReactionPhysics(DeltaTime);
	}
}

void UAlsxtImpactReactionComponent::RefreshSyncedAttackFallReactionPhysics(const float DeltaTime)
{
	// ...
}

void UAlsxtImpactReactionComponent::StopSyncedAnticipationReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAnticipationReactionEnded();
}

void UAlsxtImpactReactionComponent::StopDefensiveReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnDefensiveReactionEnded();
}

void UAlsxtImpactReactionComponent::StopCrowdNavigationReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationReactionEnded();
}

void UAlsxtImpactReactionComponent::StopBumpReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnBumpReactionEnded();
}

void UAlsxtImpactReactionComponent::StopImpactReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactReactionEnded();
}

void UAlsxtImpactReactionComponent::StopAttackReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackReactionEnded();
}

void UAlsxtImpactReactionComponent::StopSyncedAttackReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackReactionEnded();
}

void UAlsxtImpactReactionComponent::StopCrowdNavigationFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnCrowdNavigationFallEnded();
}

void UAlsxtImpactReactionComponent::StopImpactFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnImpactFallEnded();
}

void UAlsxtImpactReactionComponent::StopAttackFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnAttackFallEnded();
}

void UAlsxtImpactReactionComponent::StopSyncedAttackFallReaction()
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMovementComponent(GetOwner())->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	}

	// Character->SetMovementModeLocked(false);
	OnSyncedAttackFallEnded();
}

void UAlsxtImpactReactionComponent::OnImpactReactionEnded_Implementation() {}
