// MIT

#include "Components/Character/AlsxtCharacterCameraEffectsComponent.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtCharacterCameraEffectsComponentInterface.h"
#include "Engine/Scene.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UAlsxtCharacterCameraEffectsComponent::UAlsxtCharacterCameraEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAlsxtCharacterCameraEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}

	if (GeneralCameraEffectsSettings.bEnableEffects)
	{
		Character = Cast<AAlsxtCharacter>(GetOwner());
		// PostProcessComponent = NewObject<UPostProcessComponent>(this);
		// PostProcessComponent = Character->AddComponentByClass(UPostProcessComponent::StaticClass(), false, Character->GetTransform(), false);
		FString PostProcessComponentName = "PostProcess Component";
		PostProcessComponent = NewObject<UPostProcessComponent>(this, UPostProcessComponent::StaticClass(), *PostProcessComponentName);

		if (GeneralCameraEffectsSettings.bEnableMovementCameraShake && Character->Implements<UAlsxtCharacterInterface>() && IAlsxtCharacterInterface::Execute_SelectCameraShakeSettings(GetOwner()))
		{
			CurrentRotationMode = AlsRotationModeTags::ViewDirection;
			CurrentViewMode = Character->GetViewMode();
			CurrentGait = AlsGaitTags::Walking;
			CurrentStance = AlsStanceTags::Standing;
			IsPlayerCurrentlyMoving = false;
			GetWorld()->GetTimerManager().SetTimer(CameraShakeTimer, this, &UAlsxtCharacterCameraEffectsComponent::UpdateCameraShake, 0.01f, true);
		}

		if (PostProcessComponent != nullptr)
		{
			PostProcessComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PostProcessComponent->RegisterComponent();

			if (GeneralCameraEffectsSettings.bEnableDepthOfFieldEffect)
			{
				GetWorld()->GetTimerManager().SetTimer(CameraEffectsTraceTimer, this, &UAlsxtCharacterCameraEffectsComponent::CameraEffectsTrace, 0.01f, true);

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
				GetWorld()->GetTimerManager().SetTimer(RadialBlurTimer, this, &UAlsxtCharacterCameraEffectsComponent::UpdateRadialBlur, 0.01f, true);
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
void UAlsxtCharacterCameraEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAlsxtCharacterCameraEffectsComponent::Initialize()
{
	FVector TraceStartPoint;
	TObjectPtr<UAlsCameraComponent> Camera = Character->Camera;
	if (Character->GetViewMode() == AlsViewModeTags::ThirdPerson)
	{
		TraceStartPoint = Character->GetMesh()->GetSocketLocation("head");
	}
	else
	{
		// TraceStartPoint = IALSXTCharacterInterface::Execute_GetCameraLocationOld(GetOwner());

		TraceStartPoint = IAlsxtCharacterCameraEffectsComponentInterface::Execute_GetPlayerCameraManager(GetOwner())->GetCameraLocation();
	}
}

void UAlsxtCharacterCameraEffectsComponent::UpdateCameraShake()
{
	if (!GetWorld() || !Character->Implements<UAlsxtCharacterInterface>() || !IAlsxtCharacterInterface::Execute_SelectCameraShakeSettings(GetOwner()))
	{
		return;
	}
	
	CurrentRotationMode = Character->GetRotationMode();
	CurrentViewMode = Character->GetViewMode();
	CurrentGait = Character->GetDesiredGait();
	CurrentStance = Character->GetDesiredStance();
	IsPlayerCurrentlyMoving = (Character->GetVelocity().Size() > 0.0);

	if (CurrentRotationMode == Character->GetRotationMode() && CurrentViewMode == Character->GetViewMode() && CurrentGait == Character->GetDesiredGait() && CurrentStance == Character->GetDesiredStance() && IsPlayerCurrentlyMoving == (Character->GetVelocity().Size() > 0.0))
	{
		return;
	}

	UAlsxtCameraShakeSettings* CameraShakeSettings = IAlsxtCharacterInterface::Execute_SelectCameraShakeSettings(GetOwner());

	if (CurrentViewMode == AlsViewModeTags::FirstPerson)
	{
		if (FAlsxtStanceMovementCameraViewShakeSettings* CameraViewShakeSettings = CameraShakeSettings->CameraShakeSettings.FirstPerson.Grounded.Find(CurrentStance))
		{
			if (CameraViewShakeSettings->Gaits.Find(CurrentGait))
			{
				CurrentCameraShakeSettings = *CameraViewShakeSettings->Gaits.Find(CurrentGait);
				UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StopAllInstancesOfCameraShake(CurrentCameraShakeSettings.CameraShake, false);
				if (GeneralCameraEffectsSettings.bEnableFirstPersonDefaultCameraShake && IsValid(CurrentCameraShakeSettings.CameraShake)  && IsPlayerCurrentlyMoving)
				{
					CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(CurrentCameraShakeSettings.CameraShake, CurrentCameraShakeSettings.BaseMagnitude, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
				}
				
				OnCameraShakeChanged();
				return;
			}
		}
		
	}
	if (CurrentViewMode == AlsViewModeTags::ThirdPerson)
	{
		if (FAlsxtStanceMovementCameraViewShakeSettings* CameraViewShakeSettings = CameraShakeSettings->CameraShakeSettings.ThirdPerson.Grounded.Find(CurrentStance))
		{
			if (CameraViewShakeSettings->Gaits.Find(CurrentGait))
			{
				CurrentCameraShakeSettings = *CameraViewShakeSettings->Gaits.Find(CurrentGait);
				UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StopAllInstancesOfCameraShake(CurrentCameraShakeSettings.CameraShake, false);
				if (GeneralCameraEffectsSettings.bEnableFirstPersonDefaultCameraShake && IsValid(CurrentCameraShakeSettings.CameraShake)  && IsPlayerCurrentlyMoving)
				{
					CurrentCameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(CurrentCameraShakeSettings.CameraShake, CurrentCameraShakeSettings.BaseMagnitude, ECameraShakePlaySpace::CameraLocal, UE::Math::TRotator<double>::ZeroRotator);
				}
				
				OnCameraShakeChanged();
				return;
			}
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::SetRadialBlur(float Amount)
{
	// SetRadialBlur(0.0f);

	float BlurAmount = FMath::GetMappedRangeValueClamped(FVector2D{ 0.0, GeneralCameraEffectsSettings.RadialBlurMaxVelocity }, FVector2D{ 0.0f, GeneralCameraEffectsSettings.RadialBlurMaxWeight }, Amount);
	PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = Amount;
	CurrentRadialBlurAmount = Amount;
}

void UAlsxtCharacterCameraEffectsComponent::ResetRadialBlur()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = 0.0f;
	CurrentRadialBlurAmount = 0.0f;
}

void UAlsxtCharacterCameraEffectsComponent::SetFocusEffect(bool NewFocus)
{
	if (NewFocus)
	{
		PostProcessComponent->Settings.VignetteIntensity = 0.5f;
		PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = 0.25f;
	}
	else
	{
		PostProcessComponent->Settings.VignetteIntensity = 0.0f;
		PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = 0.0f;
	}
}

void UAlsxtCharacterCameraEffectsComponent::CameraEffectsTrace()
{
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		return;
	}
	FMinimalViewInfo ViewInfo;
	IAlsxtCharacterInterface::Execute_GetCharacterCamera(GetOwner())->GetViewInfo(ViewInfo);
	FVector CameraLocation = ViewInfo.Location;
	FRotator CameraRotation = ViewInfo.Rotation;
	FGameplayTag CombatStance = Character->GetDesiredCombatStance();
	FVector ThirdPersonTraceStartPoint = Character->GetMesh()->GetSocketLocation("head");
	FVector FirstPersonTraceStartPoint = CameraLocation;
	FVector TraceStartPoint = Character->GetViewMode() == AlsViewModeTags::FirstPerson ? FirstPersonTraceStartPoint : ThirdPersonTraceStartPoint;
	FVector TraceEndPoint = TraceStartPoint + (CameraRotation.Vector() * GeneralCameraEffectsSettings.MaxDOFTraceDistance);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	FHitResult OutHit;

	float HitDistance;

	if (GeneralCameraEffectsSettings.bFocusOnCombatTarget && IsValid(IAlsxtCombatInterface::Execute_GetCurrentTarget(GetOwner())))
	{
		AActor* CurrentTarget = IAlsxtCombatInterface::Execute_GetCurrentTarget(GetOwner());
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

void UAlsxtCharacterCameraEffectsComponent::AddSuppression(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableSuppressionEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(SuppressionBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.RadialBlurMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = EffectAmount;
			CurrentSuppressionAmount = EffectAmount;

			BeginFadeOutSuppression(1.0f, RecoveryDelay);
		}		
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetSuppression()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = 0.0f;
	CurrentSuppressionAmount = 0.0f;
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutSuppression(float NewRecoveryScale, float NewRecoveryDelay)
{
	RecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(SuppressionFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutSuppression, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutSuppression()
{
	if (IsValid(PostProcessComponent))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight - 0.001 * RecoveryScale), 0.0f, 1.0);

		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight - 0.001 * RecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentSuppressionAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[SuppressionBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(SuppressionFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::AddBlindnessEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableBlindnessEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(BlindnessEffectBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.BlindnessEffectMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = EffectAmount;
			CurrentBlindnessEffectAmount = EffectAmount;

			BeginFadeOutBlindnessEffect(1.0f, RecoveryDelay);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetBlindnessEffect()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = 0.0f;
	CurrentDamageEffectAmount = 0.0f;
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutBlindnessEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	BlindnessEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(BlindnessEffectFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutBlindnessEffect, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutBlindnessEffect()
{
	if (IsValid(PostProcessComponent))
	{
		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight - 0.001 * BlindnessEffectRecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentBlindnessEffectAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[BlindnessEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(BlindnessEffectFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::AddDamageEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableDamageEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DamageEffectBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.DamageEffectMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = EffectAmount;
			CurrentDamageEffectAmount = EffectAmount;

			BeginFadeOutDamageEffect(1.0f, RecoveryDelay);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetDamageEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DamageEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = 0.0f;
		CurrentDamageEffectAmount = 0.0f;
	}
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutDamageEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	DamageEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(DamageEffectFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutDamageEffect, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutDamageEffect()
{
	if (IsValid(PostProcessComponent))
	{
		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight - 0.001 * DamageEffectRecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentDamageEffectAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[DamageEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(DamageEffectFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::AddConcussionEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableConcussionEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(ConcussionEffectBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.ConcussionEffectMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = EffectAmount;
			CurrentConcussionEffectAmount = EffectAmount;

			BeginFadeOutConcussionEffect(1.0f, RecoveryDelay);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetConcussionEffect()
{
	PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = 0.0f;
	CurrentConcussionEffectAmount = 0.0f;
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutConcussionEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	ConcussionEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(ConcussionEffectFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutConcussionEffect, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutConcussionEffect()
{
	if (IsValid(PostProcessComponent))
	{
		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight - 0.001 * ConcussionEffectRecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentConcussionEffectAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[ConcussionEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(ConcussionEffectFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::AddDrunkEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableDrunkEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DrunkEffectBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.DrunkEffectMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = EffectAmount;
			CurrentDrunkEffectAmount = EffectAmount;

			BeginFadeOutDrunkEffect(1.0f, RecoveryDelay);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetDrunkEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(DrunkEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = 0.0f;
		CurrentDrunkEffectAmount = 0.0f;
	}
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutDrunkEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	DrunkEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(DrunkEffectFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutDrunkEffect, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutDrunkEffect()
{
	if (IsValid(PostProcessComponent))
	{
		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight - 0.001 * DrunkEffectRecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentDrunkEffectAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[DrunkEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(DrunkEffectFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::AddHighEffect(float NewMagnitude, float RecoveryDelay)
{
	if (GeneralCameraEffectsSettings.bEnableHighEffect)
	{
		if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(HighEffectBlendableIndex))
		{
			float EffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight + NewMagnitude), 0.0, GeneralCameraEffectsSettings.HighEffectMaxWeight);
			PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = EffectAmount;
			CurrentHighEffectAmount = EffectAmount;
			BeginFadeOutHighEffect(1.0f, RecoveryDelay);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::ResetHighEffect()
{
	if (IsValid(PostProcessComponent) && PostProcessComponent->Settings.WeightedBlendables.Array.IsValidIndex(HighEffectBlendableIndex))
	{
		PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = 0.0f;
		CurrentHighEffectAmount = 0.0f;
	}
}

void UAlsxtCharacterCameraEffectsComponent::BeginFadeOutHighEffect(float NewRecoveryScale, float NewRecoveryDelay)
{
	HighEffectRecoveryScale = NewRecoveryScale;
	GetWorld()->GetTimerManager().SetTimer(HighEffectFadeOutTimer, this, &UAlsxtCharacterCameraEffectsComponent::FadeOutHighEffect, 0.01f, true, NewRecoveryDelay);
}

void UAlsxtCharacterCameraEffectsComponent::FadeOutHighEffect()
{
	if (IsValid(PostProcessComponent))
	{
		float FadeOutEffectAmount = FMath::Clamp((PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight - 0.001 * HighEffectRecoveryScale), 0.0f, 1.0);
		PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight = FadeOutEffectAmount;
		CurrentHighEffectAmount = FadeOutEffectAmount;

		if (PostProcessComponent->Settings.WeightedBlendables.Array[HighEffectBlendableIndex].Weight <= 0.0)
		{
			Character->GetWorld()->GetTimerManager().ClearTimer(HighEffectFadeOutTimer);
		}
	}
}

void UAlsxtCharacterCameraEffectsComponent::UpdateRadialBlur()
{
	float Velocity = Character->GetVelocity().Size();
	float BlurAmount = FMath::GetMappedRangeValueClamped(FVector2D{ 0.0, GeneralCameraEffectsSettings.RadialBlurMaxVelocity }, FVector2D{ 0.0f, GeneralCameraEffectsSettings.RadialBlurMaxWeight }, Velocity);
	PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = BlurAmount;
	CurrentRadialBlurAmount = BlurAmount;
}

void UAlsxtCharacterCameraEffectsComponent::SetDepthOfField()
{

}