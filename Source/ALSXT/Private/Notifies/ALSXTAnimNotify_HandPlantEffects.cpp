#include "Notifies/ALSXTAnimNotify_HandPlantEffects.h"

#include "AlsCharacter.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsDebugUtility.h"
#include "Utility/AlsEnumUtility.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsMath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimNotify_HandPlantEffects)

#if WITH_EDITOR
void FALSXTHandPlantEffectSettings::PostEditChangeProperty(const FPropertyChangedEvent& ChangedEvent)
{
	// ...
}

void UALSXTHandPlantEffectsSettings::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, Effects))
	{
		for (auto& Tuple : Effects)
		{
			Tuple.Value.PostEditChangeProperty(ChangedEvent);
		}
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif

FString UALSXTAnimNotify_HandPlantEffects::GetNotifyName_Implementation() const
{
	TStringBuilder<64> NotifyNameBuilder{InPlace, TEXTVIEW("Als Footstep Effects: "), AlsEnumUtility::GetNameStringByValue(HandBone)};

	return FString{NotifyNameBuilder};
}

#if WITH_EDITOR
void UALSXTAnimNotify_HandPlantEffects::OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent)
{
	Super::OnAnimNotifyCreatedInEditor(NotifyEvent);

	NotifyEvent.bTriggerOnDedicatedServer = false;
}
#endif

void UALSXTAnimNotify_HandPlantEffects::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                            const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::Notify(Mesh, Sequence, NotifyEventReference);

	if (!IsValid(Mesh) || !ALS_ENSURE(IsValid(HandPlantEffectsSettings)))
	{
		return;
	}

	const auto* Character{Cast<AAlsCharacter>(Mesh->GetOwner())};

	if (bSkipEffectsWhenInAir && IsValid(Character) && Character->GetLocomotionMode() == AlsLocomotionModeTags::InAir)
	{
		return;
	}

#if ENABLE_DRAW_DEBUG
	const auto bDisplayDebug{UAlsDebugUtility::ShouldDisplayDebugForActor(Mesh->GetOwner(), UAlsConstants::TracesDebugDisplayName())};
#endif

	const auto* World{Mesh->GetWorld()};
	const auto MeshScale{Mesh->GetComponentScale().Z};

	const auto& HandBoneName{HandBone == EAlsFootBone::Left ? UAlsConstants::FootLeftBoneName() : UAlsConstants::FootRightBoneName()};
	const auto FootTransform{Mesh->GetSocketTransform(HandBoneName)};

	const auto FootZAxis{
		FootTransform.TransformVectorNoScale(HandBone == EAlsFootBone::Left
			                                     ? FVector{HandPlantEffectsSettings->HandLeftZAxis}
			                                     : FVector{HandPlantEffectsSettings->HandRightZAxis})
	};

	FCollisionQueryParams QueryParameters{__FUNCTION__, true, Mesh->GetOwner()};
	QueryParameters.bReturnPhysicalMaterial = true;

	FHitResult HandPlantHit;
	if (!World->LineTraceSingleByChannel(HandPlantHit, FootTransform.GetLocation(),
	                                     FootTransform.GetLocation() - FootZAxis *
	                                     (HandPlantEffectsSettings->SurfaceTraceDistance * MeshScale),
	                                     HandPlantEffectsSettings->SurfaceTraceChannel, QueryParameters))
	{
		// As a fallback, trace down the world Z axis if the first trace didn't hit anything.

		World->LineTraceSingleByChannel(HandPlantHit, FootTransform.GetLocation(),
		                                FootTransform.GetLocation() - FVector{
			                                0.0f, 0.0f, HandPlantEffectsSettings->SurfaceTraceDistance * MeshScale
		                                }, HandPlantEffectsSettings->SurfaceTraceChannel, QueryParameters);
	}

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		UAlsDebugUtility::DrawLineTraceSingle(World, HandPlantHit.TraceStart, HandPlantHit.TraceEnd, HandPlantHit.bBlockingHit,
		                                      HandPlantHit, {0.333333f, 0.0f, 0.0f}, FLinearColor::Red, 10.0f);
	}
#endif

	if (!HandPlantHit.bBlockingHit)
	{
		return;
	}

	const auto SurfaceType{HandPlantHit.PhysMaterial.IsValid() ? HandPlantHit.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default};
	const auto* EffectSettings{ HandPlantEffectsSettings->Effects.Find(SurfaceType)};

	if (EffectSettings == nullptr)
	{
		for (const auto& Tuple : HandPlantEffectsSettings->Effects)
		{
			EffectSettings = &Tuple.Value;
			break;
		}

		if (EffectSettings == nullptr)
		{
			return;
		}
	}

	const auto FootstepLocation{HandPlantHit.ImpactPoint};

	const auto FootstepRotation{
		FRotationMatrix::MakeFromZY(HandPlantHit.ImpactNormal,
		                            FootTransform.TransformVectorNoScale(HandBone == EAlsFootBone::Left
			                                                                 ? FVector{ HandPlantEffectsSettings->HandLeftYAxis}
			                                                                 : FVector{ HandPlantEffectsSettings->HandRightYAxis})).ToQuat()
	};

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		DrawDebugCoordinateSystem(World, FootstepLocation, FootstepRotation.Rotator(),
		                          25.0f, false, 10.0f, 0, UAlsDebugUtility::DrawLineThickness);
	}
#endif

	if (bSpawnSound)
	{
		SpawnSound(Mesh, EffectSettings->Sound, FootstepLocation, FootstepRotation);
	}
}

void UALSXTAnimNotify_HandPlantEffects::SpawnSound(USkeletalMeshComponent* Mesh, const FALSXTHandPlantSoundSettings& SoundSettings,
                                                const FVector& FootstepLocation, const FQuat& FootstepRotation) const
{
	auto VolumeMultiplier{SoundVolumeMultiplier};

	if (!bIgnoreHandPlantSoundBlockCurve && IsValid(Mesh->GetAnimInstance()))
	{
		VolumeMultiplier *= 1.0f - UAlsMath::Clamp01(Mesh->GetAnimInstance()->GetCurveValue(UAlsConstants::FootstepSoundBlockCurveName()));
	}

	if (!FAnimWeight::IsRelevant(VolumeMultiplier) || !IsValid(SoundSettings.Sound.LoadSynchronous()))
	{
		return;
	}

	UAudioComponent* Audio{nullptr};

	if (SoundSettings.SpawnMode == EAlsFootstepSoundSpawnMode::SpawnAtTraceHitLocation)
	{
		const auto* World{Mesh->GetWorld()};

		if (World->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySoundAtLocation(World, SoundSettings.Sound.Get(), FootstepLocation,
			                                      VolumeMultiplier, SoundPitchMultiplier);
		}
		else
		{
			Audio = UGameplayStatics::SpawnSoundAtLocation(World, SoundSettings.Sound.Get(), FootstepLocation,
			                                               FootstepRotation.Rotator(),
			                                               VolumeMultiplier, SoundPitchMultiplier);
		}
	}
	else if (SoundSettings.SpawnMode == EAlsFootstepSoundSpawnMode::SpawnAttachedToFootBone)
	{
		const auto& HandBoneName{
			HandBone == EAlsFootBone::Left ? UAlsConstants::FootLeftBoneName() : UAlsConstants::FootRightBoneName()
		};

		Audio = UGameplayStatics::SpawnSoundAttached(SoundSettings.Sound.Get(), Mesh, HandBoneName, FVector::ZeroVector,
		                                             FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
		                                             true, VolumeMultiplier, SoundPitchMultiplier);
	}

	// if (IsValid(Audio)) // Send Audio Parameters
	// {
	// 	// Audio->SetIntParameter(FName{TEXTVIEW("FootstepType")}, static_cast<int32>(SoundType));
	// }
}
