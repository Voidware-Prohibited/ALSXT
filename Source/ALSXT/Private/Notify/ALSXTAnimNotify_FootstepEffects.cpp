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


				// Set ALSXT Footstep Values
		
				LeftFootstepSoleDetail = ALSXTCharacter->GetFootstepValues().LeftFootSoleDetail;
				RightFootstepSoleDetail = ALSXTCharacter->GetFootstepValues().RightFootSoleDetail;
				LeftFootstepDecalNormalAmount = ALSXTCharacter->GetFootstepValues().LeftFootDecalNormalAmount;
				RightFootstepDecalNormalAmount = ALSXTCharacter->GetFootstepValues().RightFootDecalNormalAmount;
				LeftFootstepDurationModifierMin = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMin;
				RightFootstepDurationModifierMin = ALSXTCharacter->GetFootstepValues().RightFootDurationModifierMin;
				LeftFootstepDurationModifierMax = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMax;
				RightFootstepDurationModifierMax = ALSXTCharacter->GetFootstepValues().RightFootDurationModifierMax;


				// Declare local variables
				UMaterialInstanceDynamic* MI;
				float DurationAverage{ 0.0f };
				FVector2D InputRange{ 0, 1 };
				FVector2D OutputRange{ 0, 1 };
				float DurationModifier{ 0.0f };
				FALSXTFootstepState NewFootstepState;

				// Set New Left or Right foot Values based on current FootBone
				if (FootBone == EALSXTFootBone::Left) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Left.Current.FootPhysicalMaterial));

					if (NewSurface) {

						//Set Current as Previous
						CurrentFootprintsState.Left.Previous = ALSXTCharacter->GetFootprintsState().Left.Current;

						//Set New Current
						CurrentFootprintsState.Left.Current.FootPhysicalMaterial = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Left.Current.FootTransferDetailTexture = EffectSettings->FootstepSoleDetail;
						CurrentFootprintsState.Left.Current.FootTransferDetailScale = EffectSettings->FootstepSoleDetailScale;
						CurrentFootprintsState.Left.Current.FootMaterialPrimaryColor = EffectSettings->MaterialPrimaryColor;
						CurrentFootprintsState.Left.Current.FootMaterialSecondaryColor = EffectSettings->MaterialSecondaryColor;
						CurrentFootprintsState.Left.Current.FootMaterialGrainSize = EffectSettings->MaterialGrainSize;
						CurrentFootprintsState.Left.Current.FootMaterialWetness = EffectSettings->MaterialWetness;
						CurrentFootprintsState.Left.Current.FootMaterialTransferAmount = EffectSettings->MaterialTransferAmount;
						CurrentFootprintsState.Left.Current.FootMaterialSaturationRate = EffectSettings->MaterialSaturationRate;
						CurrentFootprintsState.Left.Current.FootMaterialDesaturationRate = EffectSettings->MaterialDesaturationRate;
						CurrentFootprintsState.Left.Current.FootDecalNormalAmount = EffectSettings->TransferNormalAmount;
						CurrentFootprintsState.Left.Current.FootDecalEmissiveAmount = EffectSettings->MaterialEmissive;
						CurrentFootprintsState.Left.Current.FootDecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Left.Current.FootDecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Left.Current.FootDurationModifierMin = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMin;
						CurrentFootprintsState.Left.Current.FootDurationModifierMax = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMax;
					}
					else {
						//Set New Current
						CurrentFootprintsState.Left.Current.FootPhysicalMaterial = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Left.Current.FootTransferDetailTexture = EffectSettings->FootstepSoleDetail;
						CurrentFootprintsState.Left.Current.FootTransferDetailScale = EffectSettings->FootstepSoleDetailScale;
						CurrentFootprintsState.Left.Current.FootMaterialPrimaryColor = EffectSettings->MaterialPrimaryColor;
						CurrentFootprintsState.Left.Current.FootMaterialSecondaryColor = EffectSettings->MaterialSecondaryColor;
						CurrentFootprintsState.Left.Current.FootMaterialGrainSize = EffectSettings->MaterialGrainSize;
						CurrentFootprintsState.Left.Current.FootMaterialWetness = EffectSettings->MaterialWetness;
						CurrentFootprintsState.Left.Current.FootMaterialTransferAmount = EffectSettings->MaterialTransferAmount;
						CurrentFootprintsState.Left.Current.FootMaterialSaturationRate = EffectSettings->MaterialSaturationRate;
						CurrentFootprintsState.Left.Current.FootMaterialDesaturationRate = EffectSettings->MaterialDesaturationRate;
						CurrentFootprintsState.Left.Current.FootDecalNormalAmount = EffectSettings->TransferNormalAmount;
						CurrentFootprintsState.Left.Current.FootDecalEmissiveAmount = EffectSettings->MaterialEmissive;
						CurrentFootprintsState.Left.Current.FootDecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Left.Current.FootDecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Left.Current.FootDurationModifierMin = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMin;
						CurrentFootprintsState.Left.Current.FootDurationModifierMax = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMax;
					}

					ALSXTCharacter->ProcessNewFootprintsState(EALSXTFootBone::Left, CurrentFootprintsState);

					CurrentFootprintsState = ALSXTCharacter->GetFootprintsState();

					
					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);
					MI->SetTextureParameterValue(FName("SoleTexture"), ALSXTCharacter->GetFootwearDetails().FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), ALSXTCharacter->GetFootwearDetails().FootwearSoleNormal);
					MI->SetTextureParameterValue(FName("SoleDetail"), LeftFootstepSoleDetail);
					MI->SetScalarParameterValue(FName("SoleNormalAmount"), LeftFootstepDecalNormalAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexture"), ALSXTCharacter->GetFootprintsState().Left.Current.FootTransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), ALSXTCharacter->GetFootprintsState().Left.Current.FootTransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalAmount"), ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialTransferDetailNormalAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), ALSXTCharacter->GetFootprintsState().Left.Current.FootTransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColor"), ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), ALSXTCharacter->GetFootprintsState().Left.Current.FootDecalEmissiveAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootTransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootTransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootMaterialTransferDetailNormalAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootTransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootMaterialPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootMaterialSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootMaterialGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootMaterialWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootDecalEmissiveAmount);

					MI->SetScalarParameterValue(FName("PhaseAlpha"), ALSXTCharacter->GetFootprintsState().Left.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), EffectSettings->MaterialTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountCurrent"), ALSXTCharacter->GetFootprintsState().Left.Current.FootTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Left.Previous.FootTransferAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = ALSXTCharacter->GetFootprintsState().Left.Current.FootMaterialWetness + EffectSettings->MaterialTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { LeftFootstepDurationModifierMin, LeftFootstepDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration* DurationModifier, false);
				}
				if (FootBone == EALSXTFootBone::Right) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Right.Current.FootPhysicalMaterial));

					if (NewSurface) {

						//Set Current as Previous

						CurrentFootprintsState.Right.Previous = ALSXTCharacter->GetFootprintsState().Right.Current;

						// CurrentFootprintsState.Right.Previous.FootMaterialSaturationRate = ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialSaturationRate;

						//Set New Current
						CurrentFootprintsState.Right.Current.FootPhysicalMaterial = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Right.Current.FootTransferDetailTexture = EffectSettings->FootstepSoleDetail;
						CurrentFootprintsState.Right.Current.FootTransferDetailScale = EffectSettings->FootstepSoleDetailScale;
						CurrentFootprintsState.Right.Current.FootMaterialPrimaryColor = EffectSettings->MaterialPrimaryColor;
						CurrentFootprintsState.Right.Current.FootMaterialSecondaryColor = EffectSettings->MaterialSecondaryColor;
						CurrentFootprintsState.Right.Current.FootMaterialGrainSize = EffectSettings->MaterialGrainSize;
						CurrentFootprintsState.Right.Current.FootMaterialWetness = EffectSettings->MaterialWetness;
						CurrentFootprintsState.Right.Current.FootMaterialTransferAmount = EffectSettings->MaterialTransferAmount;
						CurrentFootprintsState.Right.Current.FootMaterialSaturationRate = EffectSettings->MaterialSaturationRate;
						CurrentFootprintsState.Right.Current.FootMaterialDesaturationRate = EffectSettings->MaterialDesaturationRate;
						CurrentFootprintsState.Right.Current.FootDecalNormalAmount = EffectSettings->TransferNormalAmount;
						CurrentFootprintsState.Right.Current.FootDecalEmissiveAmount = EffectSettings->MaterialEmissive;
						CurrentFootprintsState.Right.Current.FootDecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Right.Current.FootDecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Right.Current.FootDurationModifierMin = CurrentFootprintsState.Right.Current.FootDurationModifierMin;
						CurrentFootprintsState.Right.Current.FootDurationModifierMax = CurrentFootprintsState.Right.Current.FootDurationModifierMax;
					}
					else {
						//Set New Current
						CurrentFootprintsState.Right.Current.FootPhysicalMaterial = UGameplayStatics::GetSurfaceType(Hit);
						CurrentFootprintsState.Right.Current.FootTransferDetailTexture = EffectSettings->FootstepSoleDetail;
						CurrentFootprintsState.Right.Current.FootTransferDetailScale = EffectSettings->FootstepSoleDetailScale;
						CurrentFootprintsState.Right.Current.FootMaterialPrimaryColor = EffectSettings->MaterialPrimaryColor;
						CurrentFootprintsState.Right.Current.FootMaterialSecondaryColor = EffectSettings->MaterialSecondaryColor;
						CurrentFootprintsState.Right.Current.FootMaterialGrainSize = EffectSettings->MaterialGrainSize;
						CurrentFootprintsState.Right.Current.FootMaterialWetness = EffectSettings->MaterialWetness;
						CurrentFootprintsState.Right.Current.FootMaterialTransferAmount = EffectSettings->MaterialTransferAmount;
						CurrentFootprintsState.Right.Current.FootMaterialSaturationRate = EffectSettings->MaterialSaturationRate;
						CurrentFootprintsState.Right.Current.FootMaterialDesaturationRate = EffectSettings->MaterialDesaturationRate;
						CurrentFootprintsState.Right.Current.FootDecalNormalAmount = EffectSettings->TransferNormalAmount;
						CurrentFootprintsState.Right.Current.FootDecalEmissiveAmount = EffectSettings->MaterialEmissive;
						CurrentFootprintsState.Right.Current.FootDecalDuration = EffectSettings->DecalDuration;
						CurrentFootprintsState.Right.Current.FootDecalFadeOutDuration = EffectSettings->DecalFadeOutDuration;
						CurrentFootprintsState.Right.Current.FootDurationModifierMin = CurrentFootprintsState.Right.Current.FootDurationModifierMin;
						CurrentFootprintsState.Right.Current.FootDurationModifierMax = CurrentFootprintsState.Right.Current.FootDurationModifierMax;
					}

					ALSXTCharacter->ProcessNewFootprintsState(EALSXTFootBone::Right, CurrentFootprintsState);

					CurrentFootprintsState = ALSXTCharacter->GetFootprintsState();

					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);
					MI->SetTextureParameterValue(FName("SoleTexture"), ALSXTCharacter->GetFootwearDetails().FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), ALSXTCharacter->GetFootwearDetails().FootwearSoleNormal);
					MI->SetTextureParameterValue(FName("SoleDetail"), RightFootstepSoleDetail);
					MI->SetScalarParameterValue(FName("SoleNormalAmount"), RightFootstepDecalNormalAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexture"), ALSXTCharacter->GetFootprintsState().Right.Current.FootTransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), ALSXTCharacter->GetFootprintsState().Right.Current.FootTransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalAmount"), ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialTransferDetailNormalAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), ALSXTCharacter->GetFootprintsState().Right.Current.FootTransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColor"), ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), ALSXTCharacter->GetFootprintsState().Right.Current.FootDecalEmissiveAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootTransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootTransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootMaterialTransferDetailNormalAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootTransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootMaterialPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootMaterialSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootMaterialGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootMaterialWetness);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootDecalEmissiveAmount);

					MI->SetScalarParameterValue(FName("PhaseAlpha"), ALSXTCharacter->GetFootprintsState().Right.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), EffectSettings->MaterialTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountCurrent"), ALSXTCharacter->GetFootprintsState().Right.Current.FootTransferAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), ALSXTCharacter->GetFootprintsState().Right.Previous.FootTransferAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = ALSXTCharacter->GetFootprintsState().Right.Current.FootMaterialWetness + EffectSettings->MaterialTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { RightFootstepDurationModifierMin, RightFootstepDurationModifierMax };
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

	if (bSpawnParticleSystem && IsValid(EffectSettings->ParticleSystem.LoadSynchronous()))
	{
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
