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
#include "Utility/AlsMacro.h"
#include "Utility/AlsMath.h"
#include "Utility/AlsUtility.h"

FString UALSXTAnimNotify_FootstepEffects::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("ALSXT Footstep Effects: {0}"), { AlsEnumUtility::GetNameStringByValue(FootBone) });
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
	const auto* ALSXTCharacter{ Cast<AALSXTCharacter>(Mesh->GetOwner()) };

	if (bSkipEffectsWhenInAir && IsValid(Character) && Character->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}

	const auto CapsuleScale{IsValid(Character) ? Character->GetCapsuleComponent()->GetComponentScale().Z : 1.0f};

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

	FVector HitLocation;
	FVector HitNormal;
	TWeakObjectPtr<UPrimitiveComponent> HitComponent;
	TWeakObjectPtr<UPhysicalMaterial> HitPhysicalMaterial;

	// const auto* FeetState{
	// 	!IsValid(ALSXTAnimationInstance)
	// 		? nullptr
	// 		: FootBone == EALSXTFootBone::Left
	// 		? &ALSXTAnimationInstance->GetFeetState().Left
	// 		: &ALSXTAnimationInstance->GetFeetState().Right
	// };

	const bool LeftFoot = FootBone == EALSXTFootBone::Left ? true : false;

#if ENABLE_DRAW_DEBUG
	const auto bDisplayDebug{ UAlsUtility::ShouldDisplayDebug(Mesh->GetOwner(), UAlsConstants::TracesDisplayName()) };
#endif

	FCollisionQueryParams QueryParameters{ ANSI_TO_TCHAR(__FUNCTION__), true, Mesh->GetOwner() };
	QueryParameters.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, FootTransform.GetLocation(),
		FootTransform.GetLocation() - FootZAxis *
		(FootstepEffectsSettings->SurfaceTraceDistance * CapsuleScale),
		UEngineTypes::ConvertToCollisionChannel(FootstepEffectsSettings->SurfaceTraceChannel),
		QueryParameters))
	{
		HitResult = Hit;
		HitLocation = Hit.ImpactPoint;
		HitNormal = Hit.ImpactNormal;
		HitComponent = Hit.Component;
		HitPhysicalMaterial = Hit.PhysMaterial;

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
		Hit.ImpactPoint = FootTransform.GetLocation();
		Hit.ImpactNormal = FVector::UpVector;
		HitLocation = FootTransform.GetLocation();
		HitNormal = FVector::UpVector;
		HitComponent = nullptr;
		HitPhysicalMaterial = nullptr;
	}

	const auto SurfaceType{ Hit.PhysMaterial.IsValid() ? Hit.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default };
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

	const auto FootstepLocation{ Hit.ImpactPoint };

	const auto FootstepRotation{
		FRotationMatrix::MakeFromZY(Hit.ImpactNormal,
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

		if (EffectSettings->DecalSpawnType == EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent && Hit.Component.IsValid())
		{
			Decal = UGameplayStatics::SpawnDecalAttached(EffectSettings->DecalMaterial.Get(), EffectSettings->DecalSize * CapsuleScale,
				Hit.Component.Get(), NAME_None, DecalLocation,
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

				// Set ALSXT Footstep Values
				LeftFootstepPhysicalMaterial = ALSXTCharacter->GetFootstepValues().LeftFootPhysicalMaterial;
				PrevLeftFootstepPhysicalMaterial = ALSXTCharacter->GetFootstepValues().PrevLeftFootPhysicalMaterial;
				RightFootstepPhysicalMaterial = ALSXTCharacter->GetFootstepValues().RightFootPhysicalMaterial;
				PrevRightFootstepPhysicalMaterial = ALSXTCharacter->GetFootstepValues().PrevRightFootPhysicalMaterial;
				LeftFootstepWetness = ALSXTCharacter->GetFootstepValues().LeftFootWetness;
				RightFootstepWetness = ALSXTCharacter->GetFootstepValues().RightFootWetness;
				LeftFootstepBlood = ALSXTCharacter->GetFootstepValues().LeftFootBlood;
				RightFootstepBlood = ALSXTCharacter->GetFootstepValues().RightFootBlood;
				LeftFootstepDirt1 = ALSXTCharacter->GetFootstepValues().LeftFootDirt1;
				RightFootstepDirt1 = ALSXTCharacter->GetFootstepValues().RightFootDirt1;
				LeftFootstepDirt2 = ALSXTCharacter->GetFootstepValues().LeftFootDirt2;
				RightFootstepDirt2 = ALSXTCharacter->GetFootstepValues().RightFootDirt2;
				LeftFootstepSand = ALSXTCharacter->GetFootstepValues().LeftFootSand;
				RightFootstepSand = ALSXTCharacter->GetFootstepValues().RightFootSand;
				LeftFootstepVegetation = ALSXTCharacter->GetFootstepValues().LeftFootVegetation;
				RightFootstepVegetation = ALSXTCharacter->GetFootstepValues().RightFootVegetation;
				LeftFootstepSnow = ALSXTCharacter->GetFootstepValues().LeftFootSnow;
				RightFootstepSnow = ALSXTCharacter->GetFootstepValues().RightFootSnow;
				LeftFootstepOil = ALSXTCharacter->GetFootstepValues().LeftFootOil;
				RightFootstepOil = ALSXTCharacter->GetFootstepValues().RightFootOil;
				LeftFootstepDurationModifierMin = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMin;
				RightFootstepDurationModifierMin = ALSXTCharacter->GetFootstepValues().RightFootDurationModifierMin;
				LeftFootstepDurationModifierMax = ALSXTCharacter->GetFootstepValues().LeftFootDurationModifierMax;
				RightFootstepDurationModifierMax = ALSXTCharacter->GetFootstepValues().RightFootDurationModifierMax;
				FootstepSoleTexture = ALSXTCharacter->GetFootwearDetails().FootwearSoleTexture;
				FootstepSoleNormal = ALSXTCharacter->GetFootwearDetails().FootwearSoleNormal;
				FootstepSoleSurfaceType = ALSXTCharacter->GetFootwearDetails().FootwearSoleSurfaceType;

				// Declare local variables
				UMaterialInstanceDynamic* MI;
				float DurationAverage{ 0.0f };
				FVector2D InputRange{ 0, 1 };
				FVector2D OutputRange{ 0, 1 };
				float DurationModifier{ 0.0f };
				FALSXTFootstepState NewFootstepState;

				// Set New Left or Right foot Values based on current FootBone
				if (FootBone == EALSXTFootBone::Left) {
					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);
					MI->SetTextureParameterValue(FName("SoleTexture"), FootstepSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), FootstepSoleNormal);
					MI->SetScalarParameterValue(TEXT("Wetness"), LeftFootstepWetness);
					MI->SetScalarParameterValue(TEXT("Blood"), LeftFootstepBlood);
					MI->SetScalarParameterValue(TEXT("Dirt1"), LeftFootstepDirt1);
					MI->SetScalarParameterValue(TEXT("Dirt2"), LeftFootstepDirt2);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = LeftFootstepWetness + LeftFootstepBlood + LeftFootstepDirt1 + LeftFootstepDirt2 / 4;
					InputRange = { 0, 1 };
					OutputRange = { LeftFootstepDurationModifierMin, LeftFootstepDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration* DurationModifier, false);

					// Set New Footstep State
					NewFootstepState = { HitPhysicalMaterial, PrevLeftFootstepPhysicalMaterial, RightFootstepPhysicalMaterial, HitPhysicalMaterial, LeftFootstepWetness, RightFootstepWetness, LeftFootstepBlood, RightFootstepBlood, LeftFootstepDirt1, RightFootstepDirt1, LeftFootstepDirt2, RightFootstepDirt2, LeftFootstepSand, RightFootstepSand, LeftFootstepVegetation, RightFootstepVegetation, LeftFootstepSnow, RightFootstepSnow, LeftFootstepOil, RightFootstepOil, LeftFootstepDurationModifierMin, RightFootstepDurationModifierMin, LeftFootstepDurationModifierMax, RightFootstepDurationModifierMax };

					if (IsValid(ALSXTCharacter)) {
						Cast<AALSXTCharacter>(Mesh->GetOwner())->SetFootstepState(NewFootstepState);
					}		
				}
				if (FootBone == EALSXTFootBone::Right) {
					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);
					MI->SetTextureParameterValue(FName("SoleTexture"), FootstepSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), FootstepSoleNormal);
					MI->SetScalarParameterValue(TEXT("Wetness"), RightFootstepWetness);
					MI->SetScalarParameterValue(TEXT("Blood"), RightFootstepBlood);
					MI->SetScalarParameterValue(TEXT("Dirt1"), RightFootstepDirt1);
					MI->SetScalarParameterValue(TEXT("Dirt2"), RightFootstepDirt2);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = RightFootstepWetness + RightFootstepBlood + RightFootstepDirt1 + RightFootstepDirt2 / 4;
					InputRange = { 0, 1 };
					OutputRange = { RightFootstepDurationModifierMin, RightFootstepDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration * DurationModifier, false);

					// Set New Footstep State
					NewFootstepState = { LeftFootstepPhysicalMaterial, HitPhysicalMaterial, HitPhysicalMaterial, PrevRightFootstepPhysicalMaterial, LeftFootstepWetness, RightFootstepWetness, LeftFootstepBlood, RightFootstepBlood, LeftFootstepDirt1, RightFootstepDirt1, LeftFootstepDirt2, RightFootstepDirt2, LeftFootstepSand, RightFootstepSand, LeftFootstepVegetation, RightFootstepVegetation, LeftFootstepSnow, RightFootstepSnow, LeftFootstepOil, RightFootstepOil, LeftFootstepDurationModifierMin, RightFootstepDurationModifierMin, LeftFootstepDurationModifierMax, RightFootstepDurationModifierMax };

					if (IsValid(ALSXTCharacter)) {
						Cast<AALSXTCharacter>(Mesh->GetOwner())->SetFootstepState(NewFootstepState);
					}				
				}
			}
			else
			{
#if WITH_EDITOR
				Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration, false);
#endif		
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
