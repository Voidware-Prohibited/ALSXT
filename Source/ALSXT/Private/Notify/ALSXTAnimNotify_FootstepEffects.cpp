#include "Notify/ALSXTAnimNotify_FootstepEffects.h"

#include "AlsCharacter.h"
#include "ALSXTCharacter.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsEnumUtility.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsMath.h"
#include "Utility/AlsUtility.h"
#include "State/ALSXTFootstepState.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathUtility.h"

FString UALSXTAnimNotify_FootstepEffects::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("ALSXT Footstep Effects: {0}"), { AlsEnumUtility::GetNameStringByValue(FootBone) });
}

void UALSXTAnimNotify_FootstepEffects::SetFootstepEffectsSettings(UALSXTFootstepEffectsSettings* NewALSXTFootstepEffectsSettings, float NewSoundVolumeMultiplier, float NewSoundPitchMultiplier, EALSXTFootBone NewFootBone, bool bNewSkipEffectsWhenInAir, bool bNewSpawnSound, EALSXTFootstepSoundType NewFootstepSoundType, bool bNewIgnoreFootstepSoundBlockCurve, bool bNewSpawnDecal, bool bNewSpawnParticleSystem)
{
	FootstepEffectsSettings = NewALSXTFootstepEffectsSettings;
	SoundVolumeMultiplier = NewSoundVolumeMultiplier;
	SoundPitchMultiplier = NewSoundPitchMultiplier;
	FootBone = NewFootBone;
	bSkipEffectsWhenInAir = bNewSkipEffectsWhenInAir;
	bSpawnSound = bNewSpawnSound;
	SoundType = NewFootstepSoundType;
	bIgnoreFootstepSoundBlockCurve = bNewIgnoreFootstepSoundBlockCurve;
	bSpawnDecal = bNewSpawnDecal;
	bSpawnParticleSystem = bNewSpawnParticleSystem;
}

void UALSXTAnimNotify_FootstepEffects::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);

	if (!IsValid(Mesh) || !ALS_ENSURE(IsValid(FootstepEffectsSettings)))
	{
		return;
	}

	const auto* Character{Cast<AAlsCharacter>(Mesh->GetOwner())};
	AALSXTCharacter* ALSXTCharacter{Cast<AALSXTCharacter>(Mesh->GetOwner())};

	if (bSkipEffectsWhenInAir && IsValid(ALSXTCharacter) && ALSXTCharacter->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}

	const auto CapsuleScale{IsValid(ALSXTCharacter) ? ALSXTCharacter->GetCapsuleComponent()->GetComponentScale().Z : 1.0f};

	const auto* World{Mesh->GetWorld()};
	const auto* AnimationInstance{Mesh->GetAnimInstance()};
	const auto* ALSXTAnimationInstance{ Cast<UALSXTAnimationInstance>(Mesh->GetAnimInstance()) };

	const auto FootBoneName{FootBone == EALSXTFootBone::Left ? UAlsConstants::FootLeftBone() : UAlsConstants::FootRightBone()};
	const auto FootTransform{Mesh->GetSocketTransform(FootBoneName)};

	const auto FootZAxis{
		FootTransform.TransformVectorNoScale(FootBone == EALSXTFootBone::Left
												 ? FootstepEffectsSettings->FootLeftZAxis
												 : FootstepEffectsSettings->FootRightZAxis)
	};

	FHitResult Hit;
	bool NewSurface;
	FVector HitLocation;
	FVector HitNormal;
	TWeakObjectPtr<UPrimitiveComponent> HitComponent;
	TWeakObjectPtr<UPhysicalMaterial> HitPhysicalMaterial;

#if ENABLE_DRAW_DEBUG
	const auto bDisplayDebug{ UAlsUtility::ShouldDisplayDebug(Mesh->GetOwner(), UAlsConstants::TracesDisplayName()) };
#endif

	FCollisionQueryParams QueryParameters{ ANSI_TO_TCHAR(__FUNCTION__), true, Mesh->GetOwner() };
	QueryParameters.bReturnPhysicalMaterial = true;

	if (World->LineTraceSingleByChannel(Hit, FootTransform.GetLocation(),
		FootTransform.GetLocation() - FootZAxis *
		(FootstepEffectsSettings->SurfaceTraceDistance * CapsuleScale),
		UEngineTypes::ConvertToCollisionChannel(FootstepEffectsSettings->SurfaceTraceChannel),
		QueryParameters))
	{
		HitResult = Hit;
		HitLocation = HitResult.ImpactPoint;
		HitNormal = HitResult.ImpactNormal;
		HitComponent = HitResult.Component;
		HitPhysicalMaterial = HitResult.PhysMaterial;

#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit,
				Hit, { 0.333333f, 0.0f, 0.0f }, FLinearColor::Red, 10.0f);
		}
#endif
	}
	else
	{
		HitResult.ImpactPoint = FootTransform.GetLocation();
		Hit.ImpactPoint = FootTransform.GetLocation();
		HitResult.ImpactNormal = FVector::UpVector;
		Hit.ImpactNormal = FVector::UpVector;
		HitLocation = FootTransform.GetLocation();
		HitNormal = FVector::UpVector;
		HitComponent = nullptr;
		HitResult.Component = nullptr;
		HitPhysicalMaterial = nullptr;
		HitResult.PhysMaterial = nullptr;
	}

	const auto SurfaceType{ HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default };
	const auto* EffectSettings{ FootstepEffectsSettings->Effects.Find(SurfaceType) };

	if (EffectSettings == nullptr)
	{
		for (const auto& Pair : FootstepEffectsSettings->Effects)
		{
			EffectSettings = &Pair.Value;
			break;
		}

		if (EffectSettings == nullptr)
		{
			return;
		}
	}

	const auto FootstepLocation{ HitResult.ImpactPoint };

	const auto FootstepRotation{
		FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
									FootTransform.TransformVectorNoScale(FootBone == EALSXTFootBone::Left
																			 ? FootstepEffectsSettings->FootLeftYAxis
																			 : FootstepEffectsSettings->FootRightYAxis)).ToQuat()
	};

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		DrawDebugCoordinateSystem(World, FootstepLocation, FootstepRotation.Rotator(),
			25.0f, false, 10.0f, 0, UAlsUtility::DrawLineThickness);
	}
#endif

	if (bSpawnSound)
	{
		auto VolumeMultiplier{ SoundVolumeMultiplier };

		if (!bIgnoreFootstepSoundBlockCurve && IsValid(AnimationInstance))
		{
			VolumeMultiplier *= 1.0f - UAlsMath::Clamp01(AnimationInstance->GetCurveValue(UAlsConstants::FootstepSoundBlockCurve()));
		}

		if (FAnimWeight::IsRelevant(VolumeMultiplier) && IsValid(EffectSettings->Sound.LoadSynchronous()))
		{
			UAudioComponent* Audio{ nullptr };

			switch (EffectSettings->SoundSpawnType)
			{
			case EALSXTFootstepSoundSpawnType::SpawnAtTraceHitLocation:
				if (World->WorldType == EWorldType::EditorPreview)
				{
					UGameplayStatics::PlaySoundAtLocation(World, EffectSettings->Sound.Get(), FootstepLocation,
						VolumeMultiplier, SoundPitchMultiplier);
				}
				else
				{
					Audio = UGameplayStatics::SpawnSoundAtLocation(World, EffectSettings->Sound.Get(), FootstepLocation,
						FootstepRotation.Rotator(),
						VolumeMultiplier, SoundPitchMultiplier);
				}
				break;

			case EALSXTFootstepSoundSpawnType::SpawnAttachedToFootBone:
				Audio = UGameplayStatics::SpawnSoundAttached(EffectSettings->Sound.Get(), Mesh, FootBoneName, FVector::ZeroVector,
					FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
					true, VolumeMultiplier, SoundPitchMultiplier);
				break;
			}

			if (IsValid(Audio))
			{
				Audio->SetIntParameter(TEXT("FootstepType"), static_cast<int32>(SoundType));
			}
		}
	}

	if (bSpawnDecal && IsValid(EffectSettings->DecalMaterial.LoadSynchronous()))
	{
		const auto DecalRotation{
			FootstepRotation * (FootBone == EALSXTFootBone::Left
									? EffectSettings->DecalFootLeftRotationOffset
									: EffectSettings->DecalFootRightRotationOffset).Quaternion()
		};

		const auto DecalLocation{
			FootstepLocation + DecalRotation.RotateVector(EffectSettings->DecalLocationOffset * CapsuleScale)
		};

		UDecalComponent* Decal;

		if (EffectSettings->DecalSpawnType == EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent && HitResult.Component.IsValid())
		{
			Decal = UGameplayStatics::SpawnDecalAttached(EffectSettings->DecalMaterial.Get(), EffectSettings->DecalSize * CapsuleScale,
				HitResult.Component.Get(), NAME_None, DecalLocation,
				DecalRotation.Rotator(), EAttachLocation::KeepWorldPosition);
		}
		else
		{
			Decal = UGameplayStatics::SpawnDecalAtLocation(World, EffectSettings->DecalMaterial.Get(),
				EffectSettings->DecalSize * CapsuleScale,
				DecalLocation, DecalRotation.Rotator());
		}

		if (IsValid(Decal))
		{
			if (IsValid(ALSXTCharacter)) {

				CurrentFootprintsState = ALSXTCharacter->GetFootprintsState();

				// Declare local variables
				UMaterialInstanceDynamic* MI;
				float CalculatedSoleNormalScale{ 0.0f };
				float CalculatedTransferNormalScale{ 0.0f };
				float DurationAverage{ 0.0f };
				FVector2D InputRange{ 0, 1 };
				FVector2D OutputRange{ 0, 1 };
				float DurationModifier{ 0.0f };

				// Set New Left or Right foot Values based on current FootBone
				if (FootBone == EALSXTFootBone::Left) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Left.Current.SurfaceType));

					if (NewSurface) {

						//Set Current as Previous
						CurrentFootprintsState.Left.Previous = ALSXTCharacter->GetFootprintsState().Left.Current;

						//Set New Current
						CurrentFootprintsState.Left.Current.SurfaceType = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Left.Current.TransferDetailTexture = EffectSettings->TransferDetailTexture;
						CurrentFootprintsState.Left.Current.TransferDetailScale = EffectSettings->TransferDetailTextureScale;
						CurrentFootprintsState.Left.Current.TransferPrimaryColor = EffectSettings->TransferPrimaryColor;
						CurrentFootprintsState.Left.Current.TransferSecondaryColor = EffectSettings->TransferSecondaryColor;
						CurrentFootprintsState.Left.Current.TransferWetness = EffectSettings->TransferWetness;
						CurrentFootprintsState.Left.Current.TransferSaturationRate = EffectSettings->TransferSaturationRate;
						CurrentFootprintsState.Left.Current.TransferDesaturationRate = EffectSettings->TransferDesaturationRate;
						CurrentFootprintsState.Left.Current.TransferEmissiveAmount = EffectSettings->TransferEmissive;
						CurrentFootprintsState.Left.Current.DecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Left.Current.DecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Left.Current.DecalDurationModifierMin = EffectSettings->DecalDurationModifierMin;
						CurrentFootprintsState.Left.Current.DecalDurationModifierMax = EffectSettings->DecalDurationModifierMax;
						CurrentFootprintsState.Left.Current.SurfaceTransferAcceptanceAmount = EffectSettings->SurfaceTransferAcceptanceAmount;

						CurrentFootprintsState.Left.Current.TransferDetailScale = EffectSettings->TransferDetailScale;
						CurrentFootprintsState.Left.Current.TransferAmount = EffectSettings->TransferAmount;
						CurrentFootprintsState.Left.Current.SurfaceTransferAmount = EffectSettings->SurfaceTransferAmount;
						CurrentFootprintsState.Left.Current.TransferNormalScale = EffectSettings->TransferNormalScale;
						CurrentFootprintsState.Left.Current.TransferGrainSize = EffectSettings->TransferGrainSize;
						CurrentFootprintsState.Left.Current.SurfaceTransferAcceptanceNormalScale = EffectSettings->SurfaceTransferAcceptanceNormalScale;
						CurrentFootprintsState.Left.Current.TransferDetailNormalAmount = EffectSettings->TransferDetailNormalAmount;
					}
					else {
						//Set New Current
						CurrentFootprintsState.Left.Current.SurfaceType = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Left.Current.TransferDetailTexture = EffectSettings->TransferDetailTexture;
						CurrentFootprintsState.Left.Current.TransferDetailScale = EffectSettings->TransferDetailTextureScale;
						CurrentFootprintsState.Left.Current.TransferPrimaryColor = EffectSettings->TransferPrimaryColor;
						CurrentFootprintsState.Left.Current.TransferSecondaryColor = EffectSettings->TransferSecondaryColor;
						CurrentFootprintsState.Left.Current.TransferWetness = EffectSettings->TransferWetness;
						CurrentFootprintsState.Left.Current.TransferSaturationRate = EffectSettings->TransferSaturationRate;
						CurrentFootprintsState.Left.Current.TransferDesaturationRate = EffectSettings->TransferDesaturationRate;
						CurrentFootprintsState.Left.Current.TransferEmissiveAmount = EffectSettings->TransferEmissive;
						CurrentFootprintsState.Left.Current.DecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Left.Current.DecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Left.Current.DecalDurationModifierMin = EffectSettings->DecalDurationModifierMin;
						CurrentFootprintsState.Left.Current.DecalDurationModifierMax = EffectSettings->DecalDurationModifierMax;
						CurrentFootprintsState.Left.Current.SurfaceTransferAcceptanceAmount = EffectSettings->SurfaceTransferAcceptanceAmount;

						CurrentFootprintsState.Left.Current.TransferDetailScale = EffectSettings->TransferDetailScale;
						CurrentFootprintsState.Left.Current.TransferAmount = EffectSettings->TransferAmount;
						CurrentFootprintsState.Left.Current.SurfaceTransferAmount = EffectSettings->SurfaceTransferAmount;
						CurrentFootprintsState.Left.Current.TransferNormalScale = EffectSettings->TransferNormalScale;
						CurrentFootprintsState.Left.Current.TransferGrainSize = EffectSettings->TransferGrainSize;
						CurrentFootprintsState.Left.Current.SurfaceTransferAcceptanceNormalScale = EffectSettings->SurfaceTransferAcceptanceNormalScale;
						CurrentFootprintsState.Left.Current.TransferDetailNormalAmount = EffectSettings->TransferDetailNormalAmount;
					}
					CalculatedSoleNormalScale = (EffectSettings->TransferNormalScale * EffectSettings->SurfaceTransferAmount) * (EffectSettings->SurfaceTransferAcceptanceNormalScale * EffectSettings->SurfaceTransferAcceptanceAmount);
					ALSXTCharacter->ProcessNewFootprintsState(EALSXTFootBone::Left, CurrentFootprintsState);

					CurrentFootprintsState = ALSXTCharacter->GetFootprintsState();

					
					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);

					MI->SetTextureParameterValue(FName("SoleTexture"), ALSXTCharacter->GetFootwearDetails().FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), ALSXTCharacter->GetFootwearDetails().FootwearSoleNormalTexture);
					MI->SetTextureParameterValue(FName("SoleDetail"), ALSXTCharacter->GetFootwearDetails().FootwearSoleDetailTexture);
					MI->SetScalarParameterValue(FName("SoleNormalScale"), CalculatedSoleNormalScale);

					MI->SetTextureParameterValue(FName("TransferDetailTexture"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScale"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferNormalScale);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferDetailScale);

					MI->SetVectorParameterValue(FName("PrimaryColor"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferEmissiveAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScalePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferNormalScale);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferEmissiveAmount);

					MI->SetScalarParameterValue(FName("PhaseAlpha"), ALSXTCharacter->GetFootprintsState().Left.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), ALSXTCharacter->GetFootprintsState().Left.Current.TransferAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmount"), ALSXTCharacter->GetFootprintsState().Left.Current.SurfaceTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.TransferAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.SurfaceTransferAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = ALSXTCharacter->GetFootprintsState().Left.Current.TransferWetness + EffectSettings->SurfaceTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { EffectSettings->DecalDurationModifierMin, EffectSettings->DecalDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration* DurationModifier, false);
				}
				if (FootBone == EALSXTFootBone::Right) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Right.Current.SurfaceType));

					if (NewSurface) {

						//Set Current as Previous

						CurrentFootprintsState.Right.Previous = ALSXTCharacter->GetFootprintsState().Right.Current;

						// CurrentFootprintsState.Right.Previous.TransferSaturationRate = ALSXTCharacter->GetFootprintsState().Right.Current.TransferSaturationRate;

						//Set New Current
						CurrentFootprintsState.Right.Current.SurfaceType = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Right.Current.TransferDetailTexture = EffectSettings->TransferDetailTexture;
						CurrentFootprintsState.Right.Current.TransferDetailScale = EffectSettings->TransferDetailTextureScale;
						CurrentFootprintsState.Right.Current.TransferPrimaryColor = EffectSettings->TransferPrimaryColor;
						CurrentFootprintsState.Right.Current.TransferSecondaryColor = EffectSettings->TransferSecondaryColor;
						CurrentFootprintsState.Right.Current.TransferWetness = EffectSettings->TransferWetness;
						CurrentFootprintsState.Right.Current.TransferSaturationRate = EffectSettings->TransferSaturationRate;
						CurrentFootprintsState.Right.Current.TransferDesaturationRate = EffectSettings->TransferDesaturationRate;
						CurrentFootprintsState.Right.Current.TransferEmissiveAmount = EffectSettings->TransferEmissive;
						CurrentFootprintsState.Right.Current.DecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Right.Current.DecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Right.Current.DecalDurationModifierMin = EffectSettings->DecalDurationModifierMin;
						CurrentFootprintsState.Right.Current.DecalDurationModifierMax = EffectSettings->DecalDurationModifierMax;
						CurrentFootprintsState.Right.Current.SurfaceTransferAcceptanceAmount = EffectSettings->SurfaceTransferAcceptanceAmount;

						CurrentFootprintsState.Right.Current.TransferDetailScale = EffectSettings->TransferDetailScale;
						CurrentFootprintsState.Right.Current.TransferAmount = EffectSettings->TransferAmount;
						CurrentFootprintsState.Right.Current.SurfaceTransferAmount = EffectSettings->SurfaceTransferAmount;
						CurrentFootprintsState.Right.Current.TransferNormalScale = EffectSettings->TransferNormalScale;
						CurrentFootprintsState.Right.Current.TransferGrainSize = EffectSettings->TransferGrainSize;
						CurrentFootprintsState.Right.Current.SurfaceTransferAcceptanceNormalScale = EffectSettings->SurfaceTransferAcceptanceNormalScale;
						CurrentFootprintsState.Right.Current.TransferDetailNormalAmount = EffectSettings->TransferDetailNormalAmount;
					}
					else {
						//Set New Current
						CurrentFootprintsState.Right.Current.SurfaceType = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Right.Current.TransferDetailTexture = EffectSettings->TransferDetailTexture;
						CurrentFootprintsState.Right.Current.TransferDetailScale = EffectSettings->TransferDetailTextureScale;
						CurrentFootprintsState.Right.Current.TransferPrimaryColor = EffectSettings->TransferPrimaryColor;
						CurrentFootprintsState.Right.Current.TransferSecondaryColor = EffectSettings->TransferSecondaryColor;
						CurrentFootprintsState.Right.Current.TransferWetness = EffectSettings->TransferWetness;
						CurrentFootprintsState.Right.Current.TransferSaturationRate = EffectSettings->TransferSaturationRate;
						CurrentFootprintsState.Right.Current.TransferDesaturationRate = EffectSettings->TransferDesaturationRate;
						CurrentFootprintsState.Right.Current.TransferEmissiveAmount = EffectSettings->TransferEmissive;
						CurrentFootprintsState.Right.Current.DecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Right.Current.DecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Right.Current.DecalDurationModifierMin = EffectSettings->DecalDurationModifierMin;
						CurrentFootprintsState.Right.Current.DecalDurationModifierMax = EffectSettings->DecalDurationModifierMax;
						CurrentFootprintsState.Right.Current.SurfaceTransferAcceptanceAmount = EffectSettings->SurfaceTransferAcceptanceAmount;
						CurrentFootprintsState.Right.Current.TransferDetailScale = EffectSettings->SurfaceTransferAcceptanceNormalScale;

						CurrentFootprintsState.Right.Current.TransferDetailScale = EffectSettings->TransferDetailScale;
						CurrentFootprintsState.Right.Current.TransferAmount = EffectSettings->TransferAmount;
						CurrentFootprintsState.Right.Current.SurfaceTransferAmount = EffectSettings->SurfaceTransferAmount;
						CurrentFootprintsState.Right.Current.TransferNormalScale = EffectSettings->TransferNormalScale;
						CurrentFootprintsState.Right.Current.TransferGrainSize = EffectSettings->TransferGrainSize;
						CurrentFootprintsState.Right.Current.SurfaceTransferAcceptanceNormalScale = EffectSettings->SurfaceTransferAcceptanceNormalScale;
						CurrentFootprintsState.Right.Current.TransferDetailNormalAmount = EffectSettings->TransferDetailNormalAmount;
					}
					CalculatedSoleNormalScale = (EffectSettings->TransferNormalScale * EffectSettings->SurfaceTransferAmount) * (EffectSettings->SurfaceTransferAcceptanceNormalScale * EffectSettings->SurfaceTransferAcceptanceAmount);
					ALSXTCharacter->ProcessNewFootprintsState(EALSXTFootBone::Right, CurrentFootprintsState);

					CurrentFootprintsState = ALSXTCharacter->GetFootprintsState();

					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);

					MI->SetTextureParameterValue(FName("SoleTexture"), ALSXTCharacter->GetFootwearDetails().FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), ALSXTCharacter->GetFootwearDetails().FootwearSoleNormalTexture);
					MI->SetTextureParameterValue(FName("SoleDetail"), ALSXTCharacter->GetFootwearDetails().FootwearSoleDetailTexture);
					MI->SetScalarParameterValue(FName("SoleNormalScale"), CalculatedSoleNormalScale);
					MI->SetTextureParameterValue(FName("TransferDetailTexture"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScale"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferNormalScale);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColor"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferEmissiveAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScalePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferNormalScale);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferEmissiveAmount);
					MI->SetScalarParameterValue(FName("PhaseAlpha"), ALSXTCharacter->GetFootprintsState().Right.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), ALSXTCharacter->GetFootprintsState().Right.Current.TransferAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmount"), ALSXTCharacter->GetFootprintsState().Right.Current.SurfaceTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.TransferAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.SurfaceTransferAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = ALSXTCharacter->GetFootprintsState().Right.Current.TransferWetness + EffectSettings->SurfaceTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { EffectSettings->DecalDurationModifierMin, EffectSettings->DecalDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration * DurationModifier, false);	
				}
			}
			else
			{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));	
			}
		}
	}

	if (bSpawnParticleSystem && IsValid(EffectSettings->ParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.WalkParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.RunParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.LandParticleSystem.LoadSynchronous()))
	{
		UNiagaraSystem* GaitParticleSystem;
		if (IsValid(ALSXTCharacter)) {
			if (ALSXTCharacter->GetDesiredGait() == AlsGaitTags::Walking)
			{
				GaitParticleSystem = EffectSettings->FootstepParticles.WalkParticleSystem.Get();
			}
			else if (ALSXTCharacter->GetDesiredGait() == AlsGaitTags::Running || ALSXTCharacter->GetDesiredGait() == AlsGaitTags::Sprinting)
			{
				GaitParticleSystem = EffectSettings->FootstepParticles.RunParticleSystem.Get();
			}
			else
			{
				GaitParticleSystem = EffectSettings->FootstepParticles.LandParticleSystem.Get();
			}
		}
		else
		{
			GaitParticleSystem = EffectSettings->FootstepParticles.WalkParticleSystem.Get();
		}
		
		switch (EffectSettings->ParticleSystemSpawnType)
		{
		case EALSXTFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation:
		{
			const auto ParticleSystemRotation{
				FootstepRotation * (FootBone == EALSXTFootBone::Left
										? EffectSettings->ParticleSystemFootLeftRotationOffset
										: EffectSettings->ParticleSystemFootLeftRotationOffset).Quaternion()
			};

			const auto ParticleSystemLocation{
				FootstepLocation +
				ParticleSystemRotation.RotateVector(EffectSettings->ParticleSystemLocationOffset * CapsuleScale)
			};

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, EffectSettings->ParticleSystem.Get(),
				ParticleSystemLocation, ParticleSystemRotation.Rotator(),
				FVector::OneVector * CapsuleScale, true, true, ENCPoolMethod::AutoRelease);
		}
		break;

		case EALSXTFootstepParticleEffectSpawnType::SpawnAttachedToFootBone:
			UNiagaraFunctionLibrary::SpawnSystemAttached(EffectSettings->ParticleSystem.Get(), Mesh, FootBoneName,
				EffectSettings->ParticleSystemLocationOffset * CapsuleScale,
				EffectSettings->ParticleSystemFootLeftRotationOffset,
				FVector::OneVector * CapsuleScale, EAttachLocation::KeepRelativeOffset,
				true, ENCPoolMethod::AutoRelease);
			break;
		}
	}
}
