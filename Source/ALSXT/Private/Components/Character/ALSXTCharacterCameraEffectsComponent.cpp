// MIT

#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Engine/Scene.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UALSXTCharacterCameraEffectsComponent::UALSXTCharacterCameraEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTCharacterCameraEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GeneralCameraEffectsSettings.bEnableEffects && GeneralCameraEffectsSettings.bEnableMovementCameraShake && IsValid(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake))
	{
		if (Character->GetViewMode() == AlsViewModeTags::FirstPerson)
		{
			UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
		}
		if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson && GeneralCameraEffectsSettings.bEnableMovementCameraShake)
		{
			GetWorld()->GetTimerManager().SetTimer(CameraShakeTimer, this, &UALSXTCharacterCameraEffectsComponent::SetRadialBlur, 0.01f, true);
			UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.ThirdPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
		}
	}

	if (GetOwner()->GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}

	if (GeneralCameraEffectsSettings.bEnableEffects)
	{
		Character = Cast<AALSXTCharacter>(GetOwner());
		// PostProcessComponent = NewObject<UPostProcessComponent>(this);
		// PostProcessComponent = Character->AddComponentByClass(UPostProcessComponent::StaticClass(), false, Character->GetTransform(), false);
		FString PostProcessComponentName = "PostProcess Component";
		PostProcessComponent = NewObject<UPostProcessComponent>(this, UPostProcessComponent::StaticClass(), *PostProcessComponentName);

		if (GeneralCameraEffectsSettings.bEnableMovementCameraShake && IsValid(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake))
		{
			if (Character->GetViewMode() == AlsViewModeTags::FirstPerson)
			{
				UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
			}
			if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson && GeneralCameraEffectsSettings.bEnableMovementCameraShake)
			{
				GetWorld()->GetTimerManager().SetTimer(CameraShakeTimer, this, &UALSXTCharacterCameraEffectsComponent::SetRadialBlur, 0.01f, true);
				UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.ThirdPersonDefaultCameraShake, 1.0f,ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
			}
		}

		if (PostProcessComponent != nullptr)
		{
			PostProcessComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PostProcessComponent->RegisterComponent();

			if (GeneralCameraEffectsSettings.bEnableDepthOfFieldEffect)
			{
				GetWorld()->GetTimerManager().SetTimer(CameraEffectsTraceTimer, this, &UALSXTCharacterCameraEffectsComponent::CameraEffectsTrace, 0.01f, true);

				//Enable Parameters
				PostProcessComponent->Settings.bOverride_DepthOfFieldFocalDistance = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldFocalRegion = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldFarTransitionRegion = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldNearTransitionRegion = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldFstop = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldDepthBlurAmount = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldDepthBlurRadius = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldNearBlurSize = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldFarBlurSize = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldSkyFocusDistance = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldSqueezeFactor = true;
				PostProcessComponent->Settings.bOverride_DepthOfFieldVignetteSize = true;

				// Set some defaults
				PostProcessComponent->Settings.DepthOfFieldNearBlurSize = GeneralCameraEffectsSettings.DepthOfFieldNearBlurSize;
				PostProcessComponent->Settings.DepthOfFieldFarBlurSize = GeneralCameraEffectsSettings.DepthOfFieldFarBlurSize;
				PostProcessComponent->Settings.DepthOfFieldSkyFocusDistance = GeneralCameraEffectsSettings.MaxDOFTraceDistance + .001;
			}

			if (GeneralCameraEffectsSettings.bEnableRadialBlurEffect)
			{
				const FWeightedBlendable RadialBlurBlend {0.0f, GeneralCameraEffectsSettings.RadialBlurMaterial};
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(RadialBlurBlend);
				RadialBlurBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
				GetWorld()->GetTimerManager().SetTimer(RadialBlurTimer, this, &UALSXTCharacterCameraEffectsComponent::SetRadialBlur, 0.01f, true);
			}

			if (GeneralCameraEffectsSettings.bEnableDrunkEffect)
			{
				const FWeightedBlendable DrunkBlend{ 0.0f, GeneralCameraEffectsSettings.DrunkEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(DrunkBlend);
				DrunkEffectBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}

			if (GeneralCameraEffectsSettings.bEnableHighEffect)
			{
				const FWeightedBlendable HighEffectBlend{ 0.0f, GeneralCameraEffectsSettings.HighEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(HighEffectBlend);
				HighEffectBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}

			if (GeneralCameraEffectsSettings.bEnableSuppressionEffect)
			{
				const FWeightedBlendable SuppressionBlend{ 0.0f, GeneralCameraEffectsSettings.SuppressionEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(SuppressionBlend);
				SuppressionBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}

			if (GeneralCameraEffectsSettings.bEnableBlindnessEffect)
			{
				const FWeightedBlendable BlindnessBlend{ 0.0f, GeneralCameraEffectsSettings.BlindnessEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(BlindnessBlend);
				BlindnessEffectBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}

			if (GeneralCameraEffectsSettings.bEnableConcussionEffect)
			{
				const FWeightedBlendable ConcussionBlend{ 0.0f, GeneralCameraEffectsSettings.ConcussionEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(ConcussionBlend);
				ConcussionEffectBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}

			if (GeneralCameraEffectsSettings.bEnableDamageEffect)
			{
				const FWeightedBlendable DamageBlend{ 0.0f, GeneralCameraEffectsSettings.DamageEffectMaterial };
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(DamageBlend);
				DamageEffectBlendableIndex = PostProcessComponent->Settings.WeightedBlendables.Array.Num() - 1;
			}
		}
		
	}
	
}


// Called every frame
void UALSXTCharacterCameraEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UALSXTCharacterCameraEffectsComponent::Initialize()
{
	FVector TraceStartPoint;
	TObjectPtr<UAlsCameraComponent> Camera = Character->Camera;
	if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson)
	{
		TraceStartPoint = Character->GetMesh()->GetSocketLocation("head");
	}
	else
	{
		TraceStartPoint = IALSXTCharacterInterface::Execute_GetCameraLocationOld(GetOwner());
	}
}

void UALSXTCharacterCameraEffectsComponent::UpdateCameraShake()
{
	if (CurrentRotationMode == Character->GetRotationMode() && CurrentViewMode == Character->GetViewMode() && CurrentGait == Character->GetDesiredGait() && CurrentStance == Character->GetDesiredStance() && IsPlayerCurrentlyMoving == (Character->GetVelocity().Size() > 0.0))
	{
		return;
	}

	if (Character->GetDesiredStance() == AlsStanceTags::Standing)
	{
		FGameplayTag NewGait = Character->GetDesiredGait();
		float NewVelocity = Character->GetVelocity().Size();
		bool NewIsPlayerCurrentlyMoving = Character->GetVelocity().Size() > 0.0;

		if (CurrentGait == AlsGaitTags::Walking && !NewIsPlayerCurrentlyMoving)
		{
			if (Character->GetViewMode() == AlsViewModeTags::FirstPerson && GeneralCameraEffectsSettings.bEnableFirstPersonDefaultCameraShake && IsValid(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake))
			{
				UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.FirstPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
			}
			if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson && GeneralCameraEffectsSettings.bEnableThirdPersonDefaultCameraShake && IsValid(GeneralCameraEffectsSettings.ThirdPersonDefaultCameraShake))
			{
				UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.ThirdPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
			}
		}
		if (CurrentGait == AlsGaitTags::Walking && NewIsPlayerCurrentlyMoving)
		{

		}
		if (CurrentGait == AlsGaitTags::Running)
		{

		}
		if (CurrentGait == AlsGaitTags::Sprinting)
		{

		}
	}
	if (Character->GetDesiredStance() == AlsStanceTags::Crouching)
	{

	}
	
	
	if (CurrentViewMode != Character->GetViewMode())
	{
		CurrentViewMode = Character->GetViewMode();
	}
	if (CurrentStance != Character->GetDesiredStance())
	{
		CurrentStance = Character->GetDesiredStance();
	}
	
	if (CurrentGait != Character->GetDesiredGait())
	{
		CurrentGait = Character->GetDesiredGait();
		float Velocity = Character->GetVelocity().Size();

		if (CurrentGait == AlsGaitTags::Walking && Velocity < 1.0f)
		{
			if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson && GeneralCameraEffectsSettings.bEnableThirdPersonDefaultCameraShake)
			{

			}
			
			UCameraShakeBase* CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(GeneralCameraEffectsSettings.ThirdPersonDefaultCameraShake, 1.0f, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
		}
		if (CurrentGait == AlsGaitTags::Walking && Velocity > 1.0f)
		{

		}
		if (CurrentGait == AlsGaitTags::Running)
		{

		}
		if (CurrentGait == AlsGaitTags::Sprinting)
		{

		}
	}
	
	float Velocity = Character->GetVelocity().Size();
	float BlurAmount = FMath::GetMappedRangeValueClamped(FVector2D{ 0.0, GeneralCameraEffectsSettings.RadialBlurMaxVelocity }, FVector2D{ 0.0f, GeneralCameraEffectsSettings.RadialBlurMaxWeight }, Velocity);
	PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = BlurAmount;
}

void UALSXTCharacterCameraEffectsComponent::CameraEffectsTrace()
{
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		return;
	}
	
	FVector CameraLocation = IALSXTCharacterInterface::Execute_GetCameraLocationOld(GetOwner());
	FRotator CameraRotation = IALSXTCharacterInterface::Execute_GetCameraRotationOld(GetOwner());
	FGameplayTag ViewMode = Character->GetViewMode();
	FGameplayTag CombatStance = Character->GetDesiredCombatStance();
	FVector ThirdPersonTraceStartPoint = Character->GetMesh()->GetSocketLocation("head");
	FVector FirstPersonTraceStartPoint = CameraLocation;
	FVector TraceStartPoint = Character->GetViewMode() == AlsViewModeTags::FirstPerson ? FirstPersonTraceStartPoint : ThirdPersonTraceStartPoint;
	FVector TraceEndPoint = TraceStartPoint + (CameraRotation.Vector() * GeneralCameraEffectsSettings.MaxDOFTraceDistance);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	FHitResult OutHit;

	float HitDistance;

	if (GeneralCameraEffectsSettings.bFocusOnCombatTarget && IsValid(IALSXTCombatInterface::Execute_GetCurrentTarget(GetOwner())))
	{
		AActor* CurrentTarget = IALSXTCombatInterface::Execute_GetCurrentTarget(GetOwner());
		HitDistance = FVector::Dist(Character->GetActorLocation(), CurrentTarget->GetActorLocation());
	}
	else
	{
		bool Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStartPoint, TraceEndPoint, 10.0f, GeneralCameraEffectsSettings.TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 0.01f);
		HitDistance = Hit ? OutHit.Distance : 0.0f;
	}

	float UnaimedFirstPersonFStop = GeneralCameraEffectsSettings.FirstPersonFocalDistanceToFStopCurve->GetVectorValue(HitDistance).X;
	float UnaimedThirdPersonFStop = GeneralCameraEffectsSettings.ThirdPersonFocalDistanceToFStopCurve->GetVectorValue(HitDistance).X;
	float AimedFirstPersonFStop = GeneralCameraEffectsSettings.FirstPersonFocalDistanceToFStopCurve->GetVectorValue(HitDistance).Y;
	float AimedThirdPersonFStop = GeneralCameraEffectsSettings.ThirdPersonFocalDistanceToFStopCurve->GetVectorValue(HitDistance).Y;
	FGameplayTag CurrentCombatStance = Character->GetDesiredCombatStance();
	float FirstPersonFStop = CurrentCombatStance == ALSXTCombatStanceTags::Aiming ? AimedFirstPersonFStop : UnaimedFirstPersonFStop;
	float ThirdPersonFStop = CurrentCombatStance == ALSXTCombatStanceTags::Aiming ? AimedThirdPersonFStop : UnaimedThirdPersonFStop;

	PostProcessComponent->Settings.DepthOfFieldFstop = Character->GetViewMode() == AlsViewModeTags::FirstPerson ? FirstPersonFStop : ThirdPersonFStop;
	PostProcessComponent->Settings.DepthOfFieldFocalDistance = HitDistance;
	PostProcessComponent->Settings.DepthOfFieldFocalRegion = HitDistance;
	// PostProcessComponent->Settings.DepthOfFieldFarTransitionRegion = HitDistance + 100.0f;
	// PostProcessComponent->Settings.DepthOfFieldNearTransitionRegion = FMath::Min(0.0, HitDistance);
}

void UALSXTCharacterCameraEffectsComponent::AddSuppression(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableSuppressionEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(SuppressionBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.RadialBlurMaxWeight);
			BeginFadeOutSuppression(1.0f, RecoveryDelay);
		}		
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetSuppression()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = 0.0f;
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutSuppression(float NewRecoveryScale, float NewRecoveryDelay)
{
	RecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(SuppressionFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutSuppression, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutSuppression()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight - 0.001 * RecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(SuppressionFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::AddBlindnessEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableBlindnessEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(BlindnessEffectBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.BlindnessEffectMaxWeight);
			BeginFadeOutBlindnessEffect(1.0f, RecoveryDelay);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetBlindnessEffect()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = 0.0f;
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutBlindnessEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	BlindnessEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(BlindnessEffectFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutBlindnessEffect, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutBlindnessEffect()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight - 0.001 * BlindnessEffectRecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(BlindnessEffectFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::AddDamageEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableDamageEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DamageEffectBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.DamageEffectMaxWeight);
			BeginFadeOutDamageEffect(1.0f, RecoveryDelay);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetDamageEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DamageEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = 0.0f;
	}
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutDamageEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	DamageEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(DamageEffectFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutDamageEffect, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutDamageEffect()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight - 0.001 * DamageEffectRecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(DamageEffectFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::AddConcussionEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableConcussionEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(ConcussionEffectBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.ConcussionEffectMaxWeight);
			BeginFadeOutConcussionEffect(1.0f, RecoveryDelay);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetConcussionEffect()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = 0.0f;
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutConcussionEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	ConcussionEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(ConcussionEffectFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutConcussionEffect, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutConcussionEffect()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight - 0.001 * ConcussionEffectRecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(ConcussionEffectFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::AddDrunkEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableDrunkEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DrunkEffectBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.DrunkEffectMaxWeight);
			BeginFadeOutDrunkEffect(1.0f, RecoveryDelay);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetDrunkEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DrunkEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = 0.0f;
	}
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutDrunkEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	DrunkEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(DrunkEffectFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutDrunkEffect, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutDrunkEffect()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight - 0.001 * DrunkEffectRecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(DrunkEffectFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::AddHighEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableHighEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(HighEffectBlendableIndex))
		{
			PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.HighEffectMaxWeight);
			BeginFadeOutHighEffect(1.0f, RecoveryDelay);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::ResetHighEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(HighEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = 0.0f;
	}
}

void UALSXTCharacterCameraEffectsComponent::BeginFadeOutHighEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	HighEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(HighEffectFadeOutTimer, this, &UALSXTCharacterCameraEffectsComponent::FadeOutHighEffect, 0.01f, true, NewRecoveryDelay);
}

void UALSXTCharacterCameraEffectsComponent::FadeOutHighEffect()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight - 0.001 * HighEffectRecoveryScale), 0.0f, 1.0);

		if (PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(HighEffectFadeOutTimer);
		}
	}
}

void UALSXTCharacterCameraEffectsComponent::SetRadialBlur()
{
	float Velocity = Character->GetVelocity().Size();
	float BlurAmount = FMath::GetMappedRangeValueClamped(FVector2D{ 0.0, GeneralCameraEffectsSettings.RadialBlurMaxVelocity }, FVector2D{ 0.0f, GeneralCameraEffectsSettings.RadialBlurMaxWeight }, Velocity);
	PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = BlurAmount;
}

void UALSXTCharacterCameraEffectsComponent::SetDepthOfField()
{

}