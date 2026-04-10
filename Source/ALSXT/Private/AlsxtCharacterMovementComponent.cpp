// MIT

#include "AlsxtCharacterMovementComponent.h"

#include "AlsxtCharacter.h"
#include "AbilitySystem/AttributeSets/AlsxtMovementAttributeSet.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UAlsxtCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UAlsxtCharacterMovementComponent::ConsumeInputVector()
{
	// return Super::ConsumeInputVector();

	if (IAlsxtCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal)
	{
		return Super::ConsumeInputVector();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

float UAlsxtCharacterMovementComponent::GetMaxSpeed() const
{
	// if (!SpeedDataAsset) return Super::GetMaxSpeed();
// 
	// // 1. Get Tags from Character Owner
	// AMyCharacter* CharOwner = Cast<AMyCharacter>(GetOwner());
	// if (!CharOwner) return Super::GetMaxSpeed();
    // 
	// FGameplayTag CurrentStance = CharOwner->GetStanceTag();
	// FGameplayTag CurrentGait = CharOwner->GetGaitTag();
// 
	// // 2. Lookup Base Speed
	// float BaseSpeed = MovementDataAsset->GetSpeedForTags(CurrentStance, CurrentGait);
// 
	// // 3. Apply GAS Modifiers (Additive/Multiplicative)
	// if (CharOwner->GetAbilitySystemComponent()) {
	// 	// AttributeSet::GetMoveSpeed() returns the final value calculated by GE
	// 	return CharOwner->GetAttributeSet()->GetMoveSpeed(); 
	// }
    // 
	// return BaseSpeed;
	// 
	// 
	// // 1. Determine Base Speed from Tags (Stance/Gait set on Actor)
	// float BaseSpeed = SpeedDataAsset->DefaultConfig.WalkSpeed;
    // 
	// // Example: Check Actor Tags for current Gait (requires implementing tag setting)
	// if (GetOwner()->Tags.Contains(FName("Gait.Sprint"))) BaseSpeed = SpeedDataAsset->DefaultConfig.SprintSpeed;
	// else if (IsCrouching()) BaseSpeed = SpeedDataAsset->DefaultConfig.CrouchWalkSpeed;
// 
	// // 2. Apply GAS Movement Speed Modifier Attribute
	// if (const auto* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	// {
	// 	// AttributeSet::GetMoveSpeedAttribute() returns the FGameplayAttribute
	// 	float SpeedModifier = ASC->GetNumericAttribute(UAlsxtMovementAttributeSet::GetMovementSpeedAttribute());
	// 	return BaseSpeed * SpeedModifier; // e.g., 0.5 for 50% speed
	// }
// 
	// return BaseSpeed;
	
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetStandingWalkSpeedMultiplier() const
{
	return StandingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetStandingAimingSpeedMultiplier() const
{
	return StandingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetStandingCombatSpeedMultiplier() const
{
	return StandingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetStandingRunSpeedMultiplier() const
{
	return StandingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetStandingSprintSpeedMultiplier() const
{
	return StandingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetCrouchedWalkSpeedMultiplier() const
{
	return CrouchedWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetCrouchedAimingSpeedMultiplier() const
{
	return CrouchedAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetCrouchedCombatSpeedMultiplier() const
{
	return CrouchedCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetCrouchedRunSpeedMultiplier() const
{
	return CrouchedRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetCrouchedSprintSpeedMultiplier() const
{
	return CrouchedSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetProneWalkSpeedMultiplier() const
{
	return ProneWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetProneAimingSpeedMultiplier() const
{
	return ProneAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetProneCombatSpeedMultiplier() const
{
	return ProneCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetProneRunSpeedMultiplier() const
{
	return ProneRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetProneSprintSpeedMultiplier() const
{
	return ProneSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetClimbingWalkSpeedMultiplier() const
{
	return ClimbingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetClimbingAimingSpeedMultiplier() const
{
	return ClimbingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetClimbingCombatSpeedMultiplier() const
{
	return ClimbingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetClimbingRunSpeedMultiplier() const
{
	return ClimbingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetClimbingSprintSpeedMultiplier() const
{
	return ClimbingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetSwimmingWalkSpeedMultiplier() const
{
	return SwimmingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetSwimmingAimingSpeedMultiplier() const
{
	return SwimmingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetSwimmingCombatSpeedMultiplier() const
{
	return SwimmingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetSwimmingRunSpeedMultiplier() const
{
	return SwimmingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetSwimmingSprintSpeedMultiplier() const
{
	return SwimmingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxStandingWalkSpeed() const
{
	return Super::GetMaxSpeed() * StandingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxStandingAimingSpeed() const
{
	return Super::GetMaxSpeed() * StandingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxStandingCombatSpeed() const
{
	return Super::GetMaxSpeed() * StandingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxStandingRunSpeed() const
{
	return Super::GetMaxSpeed() * StandingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxStandingSprintSpeed() const
{
	return Super::GetMaxSpeed() * StandingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxCrouchedWalkSpeed() const
{
	return Super::GetMaxSpeed() * CrouchedWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxCrouchedAimingSpeed() const
{
	return Super::GetMaxSpeed() * CrouchedAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxCrouchedCombatSpeed() const
{
	return Super::GetMaxSpeed() * CrouchedCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxCrouchedRunSpeed() const
{
	return Super::GetMaxSpeed() * CrouchedRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxCrouchedSprintSpeed() const
{
	return Super::GetMaxSpeed() * CrouchedSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxProneWalkSpeed() const
{
	return Super::GetMaxSpeed() * ProneWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxProneAimingSpeed() const
{
	return Super::GetMaxSpeed() * ProneAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxProneCombatSpeed() const
{
	return Super::GetMaxSpeed() * ProneCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxProneRunSpeed() const
{
	return Super::GetMaxSpeed() * ProneRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxProneSprintSpeed() const
{
	return Super::GetMaxSpeed() * ProneSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxClimbingWalkSpeed() const
{
	return Super::GetMaxSpeed() * ClimbingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxClimbingAimingSpeed() const
{
	return Super::GetMaxSpeed() * ClimbingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxClimbingCombatSpeed() const
{
	return Super::GetMaxSpeed() * ClimbingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxClimbingRunSpeed() const
{
	return Super::GetMaxSpeed() * ClimbingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxClimbingSprintSpeed() const
{
	return Super::GetMaxSpeed() * ClimbingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxSwimmingWalkSpeed() const
{
	return Super::GetMaxSpeed() * SwimmingWalkSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxSwimmingAimingSpeed() const
{
	return Super::GetMaxSpeed() * SwimmingAimingSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxSwimmingCombatSpeed() const
{
	return Super::GetMaxSpeed() * SwimmingCombatSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxSwimmingRunSpeed() const
{
	return Super::GetMaxSpeed() * SwimmingRunSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxSwimmingSprintSpeed() const
{
	return Super::GetMaxSpeed() * SwimmingSprintSpeedMultiplier;
}

float UAlsxtCharacterMovementComponent::GetMaxAcceleration() const
{
	return Super::GetMaxAcceleration();
}

void UAlsxtCharacterMovementComponent::SetMovementSpeedMultiplier(float Multiplier)
{
	MovementSpeedMultiplier = Multiplier;
}

float UAlsxtCharacterMovementComponent::GetCharacterWeight() const
{
	return 90.62f;
}

void UAlsxtCharacterMovementComponent::SetStandingWalkSpeedMultiplier(float Multiplier)
{
	StandingWalkSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetStandingAimingSpeedMultiplier(float Multiplier)
{
	StandingAimingSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetStandingCombatSpeedMultiplier(float Multiplier)
{
	StandingCombatSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetStandingRunSpeedMultiplier(float Multiplier)
{
	StandingRunSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetStandingSprintSpeedMultiplier(float Multiplier)
{
	StandingSprintSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetCrouchedWalkSpeedMultiplier(float Multiplier)
{
	CrouchedWalkSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetCrouchedAimingSpeedMultiplier(float Multiplier)
{
	CrouchedAimingSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetCrouchedCombatSpeedMultiplier(float Multiplier)
{
	CrouchedCombatSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetCrouchedRunSpeedMultiplier(float Multiplier)
{
	CrouchedRunSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetCrouchedSprintSpeedMultiplier(float Multiplier)
{
	CrouchedSprintSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetProneWalkSpeedMultiplier(float Multiplier)
{
	ProneWalkSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetProneAimingSpeedMultiplier(float Multiplier)
{
	ProneAimingSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetProneCombatSpeedMultiplier(float Multiplier)
{
	ProneCombatSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetProneRunSpeedMultiplier(float Multiplier)
{
	ProneRunSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetProneSprintSpeedMultiplier(float Multiplier)
{
	ProneSprintSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetClimbingWalkSpeedMultiplier(float Multiplier)
{
	ClimbingWalkSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetClimbingAimingSpeedMultiplier(float Multiplier)
{
	ClimbingAimingSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetClimbingCombatSpeedMultiplier(float Multiplier)
{
	ClimbingCombatSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetClimbingRunSpeedMultiplier(float Multiplier)
{
	ClimbingRunSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetClimbingSprintSpeedMultiplier(float Multiplier)
{
	ClimbingSprintSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetSwimmingWalkSpeedMultiplier(float Multiplier)
{
	SwimmingWalkSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetSwimmingAimingSpeedMultiplier(float Multiplier)
{
	SwimmingAimingSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetSwimmingCombatSpeedMultiplier(float Multiplier)
{
	SwimmingCombatSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetSwimmingRunSpeedMultiplier(float Multiplier)
{
	SwimmingRunSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::SetSwimmingSprintSpeedMultiplier(float Multiplier)
{
	SwimmingSprintSpeedMultiplier = Multiplier;
}

void UAlsxtCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (AlsxtMovementSettings.Get())
	{
		if (CurrentFloor.IsWalkableFloor() && MovementMode == MOVE_Walking && AlsxtMovementSettings.Get()->bEnableSlopeSliding)
		{
			CheckSlopeAngle();
		}
	}
	
}

void UAlsxtCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (AlsxtMovementSettings.Get())
	{
		if (CurrentFloor.IsWalkableFloor() && MovementMode == MOVE_Walking && AlsxtMovementSettings.Get()->bEnableSlopeSliding)
		{
			CheckSlopeAngle();
		}
	}
}

void UAlsxtCharacterMovementComponent::CheckSlopeAngle()
{
	if (!CurrentFloor.IsWalkableFloor())
	{
		// Not on a walkable floor (e.g., in the air), so return.
		return;
	}

	// Get the normal of the surface the character is standing on
	FVector FloorNormal = CurrentFloor.HitResult.Normal;

	// The Up vector for the world is FVector::UpVector or FVector(0, 0, 1)
	FVector UpVector = FVector::UpVector;

	// Calculate the dot product between the surface normal and the world's Up vector
	float DotProduct = FVector::DotProduct(FloorNormal, UpVector);

	// Calculate the angle in radians using acos, then convert to degrees
	float AngleInRadians = acosf(DotProduct);
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

	

	if (AlsxtMovementSettings->bEnableSlopeSliding && (AngleInDegrees > AlsxtMovementSettings->BaseSlopeSlideAngle))
	{
		// Calculate sliding direction based on floor normal
		FVector SlideDirection = FVector::CrossProduct(FloorNormal, FVector::CrossProduct(FVector::UpVector, FloorNormal));
		// FVector SlideDirection = FVector(CurrentFloor.HitResult.ImpactNormal.X, CurrentFloor.HitResult.ImpactNormal.Y, 0.0f).GetSafeNormal();
		SlideDirection.Normalize();
		
		float SurfaceFriction {CurrentFloor.HitResult.PhysMaterial.Get()->Friction};
		float CharacterWeight {GetCharacterWeight()};
		float SlideForceAmount{0.0f};
		float SlideSpeed = FVector::DotProduct(FloorNormal, SlideDirection); // Calculate from Surface Friction, Character Weight
		float SlideForwardVectorBias{0.5f};

		// Optionally, use character's forward vector for direction bias
		FVector CharacterForward = GetOwner()->GetActorForwardVector();
		SlideDirection = FMath::Lerp(SlideDirection, CharacterForward, SlideForwardVectorBias); // SlideForwardVectorBias is a float from 0 to 1

		// Apply a force or set a desired velocity for sliding
		AddForce(SlideDirection * SlideSpeed);
		AddInputVector(SlideDirection * SlideForceAmount, true); // Adjust SlideForceAmount as needed
		// Cast<AAlsxtCharacter>(GetCharacterOwner())->SetLocomotionMode(AlsLocomotionModeTags::SlopeSliding);
		// OnEnterSlideSlopeAngle.Broadcast();
		// SetMovementMode(MOVE_Falling); // Optional: transition to falling to bypass walking restrictions
	}
}

bool UAlsxtCharacterMovementComponent::CanProneInCurrentState() const
{
	if (!CanEverCrouch())
	{
		return false;
	}

	return (IsFalling() || IsMovingOnGround()) && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}

void UAlsxtCharacterMovementComponent::SetProneHalfHeight(const float NewValue)
{
	ProneHalfHeight = NewValue;

	if (CharacterOwner != nullptr)
	{
		// CharacterOwner->RecalculateProneEyeHeight();
	}
}

float UAlsxtCharacterMovementComponent::GetProneHalfHeight() const
{ 
	return ProneHalfHeight; 
}

void UAlsxtCharacterMovementComponent::Prone(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation && !CanCrouchInCurrentState())
	{
		return;
	}

	// See if collision is already at desired size.
	if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == GetProneHalfHeight())
	{
		if (!bClientSimulation)
		{
			// CharacterOwner->SetIsProne(true);
		}
		CharacterOwner->OnStartCrouch( 0.f, 0.f );
		return;
	}

	if (bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		// restore collision size before crouching
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		bShrinkProxyCapsule = true;
	}

	// Change collision size to prone dimensions
	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float OldUnscaledRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	// Height is not allowed to be smaller than radius.
	const float ClampedProneHalfHeight = FMath::Max3(0.f, OldUnscaledRadius, GetProneHalfHeight());
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, ClampedProneHalfHeight);
	float HalfHeightAdjust = (OldUnscaledHalfHeight - ClampedProneHalfHeight);
	float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	if( !bClientSimulation )
	{
		// Crouching to a larger height? (this is rare)
		if (ClampedProneHalfHeight > OldUnscaledHalfHeight)
		{
			FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, CharacterOwner);
			FCollisionResponseParams ResponseParam;
			InitCollisionParams(CapsuleParams, ResponseParam);
			const bool bEncroached = GetWorld()->OverlapBlockingTestByChannel(UpdatedComponent->GetComponentLocation() + ScaledHalfHeightAdjust * GetGravityDirection(), GetWorldToGravityTransform(),
				UpdatedComponent->GetCollisionObjectType(), GetPawnCapsuleCollisionShape(SHRINK_None), CapsuleParams, ResponseParam);

			// If encroached, cancel
			if( bEncroached )
			{
				CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, OldUnscaledHalfHeight);
				return;
			}
		}

		if (bCrouchMaintainsBaseLocation)
		{
			// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
			UpdatedComponent->MoveComponent(ScaledHalfHeightAdjust * GetGravityDirection(), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
		}

		// CharacterOwner->SetIsProne(true);
	}

	bForceNextFloorCheck = true;

	// OnStartCrouch takes the change from the Default size, not the current one (though they are usually the same).
	const float MeshAdjust = ScaledHalfHeightAdjust;
	ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
	HalfHeightAdjust = (DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - ClampedProneHalfHeight);
	ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	AdjustProxyCapsuleSize();
	// CharacterOwner->OnStartProne( HalfHeightAdjust, ScaledHalfHeightAdjust );

	// Don't smooth this change in mesh position
	if ((bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy) || (IsNetMode(NM_ListenServer) && CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy))
	{
		FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
		if (ClientData)
		{
			ClientData->MeshTranslationOffset -= MeshAdjust * -GetGravityDirection();
			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;
		}
	}
}


void UAlsxtCharacterMovementComponent::UnProne(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	AAlsxtCharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<AAlsxtCharacter>();

	// See if collision is already at desired size.
	if( CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() )
	{
		if (!bClientSimulation)
		{
			// CharacterOwner->SetIsProne(false);
		}
		CharacterOwner->OnEndCrouch( 0.f, 0.f );
		return;
	}

	const float CurrentProneHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float HalfHeightAdjust = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - OldUnscaledHalfHeight;
	const float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
	const FVector PawnLocation = UpdatedComponent->GetComponentLocation();

	// Grow to unProne size.
	check(CharacterOwner->GetCapsuleComponent());

	if( !bClientSimulation )
	{
		// Try to stay in place and see if the larger capsule fits. We use a slightly taller capsule to avoid penetration.
		const UWorld* MyWorld = GetWorld();
		const float SweepInflation = UE_KINDA_SMALL_NUMBER * 10.f;
		FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(CapsuleParams, ResponseParam);

		// Compensate for the difference between current capsule size and standing size
		const FCollisionShape StandingCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, -SweepInflation - ScaledHalfHeightAdjust); // Shrink by negative amount, so actually grow it.
		const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
		bool bEncroached = true;

		if (!bCrouchMaintainsBaseLocation)
		{
			// Expand in place
			bEncroached = MyWorld->OverlapBlockingTestByChannel(PawnLocation, GetWorldToGravityTransform(), CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
		
			if (bEncroached)
			{
				// Try adjusting capsule position to see if we can avoid encroachment.
				if (ScaledHalfHeightAdjust > 0.f)
				{
					// Shrink to a short capsule, sweep down to base to find where that would hit something, and then try to stand up from there.
					float PawnRadius, PawnHalfHeight;
					CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);
					const float ShrinkHalfHeight = PawnHalfHeight - PawnRadius;
					const float TraceDist = PawnHalfHeight - ShrinkHalfHeight;
					const FVector Down = TraceDist * GetGravityDirection();

					FHitResult Hit(1.f);
					const FCollisionShape ShortCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, ShrinkHalfHeight);
					const bool bBlockingHit = MyWorld->SweepSingleByChannel(Hit, PawnLocation, PawnLocation + Down, GetWorldToGravityTransform(), CollisionChannel, ShortCapsuleShape, CapsuleParams);
					if (Hit.bStartPenetrating)
					{
						bEncroached = true;
					}
					else
					{
						// Compute where the base of the sweep ended up, and see if we can stand there
						const float DistanceToBase = (Hit.Time * TraceDist) + ShortCapsuleShape.Capsule.HalfHeight;
						const FVector Adjustment = (-DistanceToBase + StandingCapsuleShape.Capsule.HalfHeight + SweepInflation + MIN_FLOOR_DIST / 2.f) * -GetGravityDirection();
						const FVector NewLoc = PawnLocation + Adjustment;
						bEncroached = MyWorld->OverlapBlockingTestByChannel(NewLoc, GetWorldToGravityTransform(), CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
						if (!bEncroached)
						{
							// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
							UpdatedComponent->MoveComponent(NewLoc - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
						}
					}
				}
			}
		}
		else
		{
			// Expand while keeping base location the same.
			FVector StandingLocation = PawnLocation + (StandingCapsuleShape.GetCapsuleHalfHeight() - CurrentProneHalfHeight) * -GetGravityDirection();
			bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, GetWorldToGravityTransform(), CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				if (IsMovingOnGround())
				{
					// Something might be just barely overhead, try moving down closer to the floor to avoid it.
					const float MinFloorDist = UE_KINDA_SMALL_NUMBER * 10.f;
					if (CurrentFloor.bBlockingHit && CurrentFloor.FloorDist > MinFloorDist)
					{
						StandingLocation -= (CurrentFloor.FloorDist - MinFloorDist) * -GetGravityDirection();
						bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, GetWorldToGravityTransform(), CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
					}
				}				
			}

			if (!bEncroached)
			{
				// Commit the change in location.
				UpdatedComponent->MoveComponent(StandingLocation - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
				bForceNextFloorCheck = true;
			}
		}

		// If still encroached then abort.
		if (bEncroached)
		{
			return;
		}

		// CharacterOwner->SetIsProne(false);
	}	
	else
	{
		bShrinkProxyCapsule = true;
	}

	// Now call SetCapsuleSize() to cause touch/untouch events and actually grow the capsule
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);

	const float MeshAdjust = ScaledHalfHeightAdjust;
	AdjustProxyCapsuleSize();
	// CharacterOwner->OnEndProne( HalfHeightAdjust, ScaledHalfHeightAdjust );

	// Don't smooth this change in mesh position
	if ((bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy) || (IsNetMode(NM_ListenServer) && CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy))
	{
		FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
		if (ClientData)
		{
			ClientData->MeshTranslationOffset += MeshAdjust * -GetGravityDirection();
			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;
		}
	}
}
