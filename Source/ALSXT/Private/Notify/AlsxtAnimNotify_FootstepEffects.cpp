#include "Notify/AlsxtAnimNotify_FootstepEffects.h"

#include "AlsCharacter.h"
#include "AlsxtAnimationInstance.h"
#include "AlsxtCharacter.h"
#include "AlsxtVertexFunctionLibrary.h"
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
#include "State/AlsxtFootstepState.h"
#include "Engine/GameEngine.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Math/UnrealMathUtility.h"
#include "Utility/AlsDebugUtility.h"

FString UAlsxtAnimNotify_FootstepEffects::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("ALSXT Footstep Effects: {0}"), { "AlsEnumUtility::GetNameStringByValue(FootBone)" });
}

void UAlsxtAnimNotify_FootstepEffects::SetFootstepEffectsSettings(UALSXTFootstepEffectsSettings* NewALSXTFootstepEffectsSettings, float NewSoundVolumeMultiplier, float NewSoundPitchMultiplier, EAlsFootBone NewFootBone, bool bNewSkipEffectsWhenInAir, bool bNewSpawnSound, EALSXTFootstepSoundType NewFootstepSoundType, bool bNewIgnoreFootstepSoundBlockCurve, bool bNewSpawnDecal, bool bNewSpawnParticleSystem)
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

void UAlsxtAnimNotify_FootstepEffects::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);
	const auto* World{ Mesh->GetWorld() };
	if (World->WorldType == EWorldType::EditorPreview)
	{
		return;
	}

	if (!IsValid(Mesh) || !ALS_ENSURE(IsValid(FootstepEffectsSettings)))
	{
		return;
	}

	if (!Mesh->GetOwner()->Implements<UAlsxtCharacterInterface>())
	{
		return;
	}

	const auto* Character{Cast<AAlsCharacter>(Mesh->GetOwner())};
	AAlsxtCharacter* ALSXTCharacter{ IAlsxtCharacterInterface::Execute_GetCharacter(Mesh->GetOwner()) };

	if (bSkipEffectsWhenInAir && IsValid(ALSXTCharacter) && IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(Mesh->GetOwner()) == AlsLocomotionModeTags::InAir)
	{
		return;
	}

	const auto CapsuleScale{IsValid(ALSXTCharacter) ? IAlsxtCharacterInterface::Execute_GetCharacterCapsuleComponent(Mesh->GetOwner())->GetComponentScale().Z : 1.0f};	
	const auto* AnimationInstance{Mesh->GetAnimInstance()};
	const auto* ALSXTAnimationInstance{ IAlsxtCharacterInterface::Execute_GetCharacterAnimInstance(Mesh->GetOwner()) };
	// IALSXTCharacterInterface::Execute_GetCharacterAnimationInstance(Mesh->GetOwner());

	const auto FootBoneName{FootBone == EAlsFootBone::Left ? UAlsConstants::FootLeftBoneName() : UAlsConstants::FootRightBoneName()};
	const auto FootTransform{Mesh->GetSocketTransform(FootBoneName)};

	const auto FootZAxis{
		FootTransform.TransformVectorNoScale(FootBone == EAlsFootBone::Left
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
	const auto bDisplayDebug{ false };
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
			UAlsDebugUtility::DrawLineTraceSingle(World, Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit,
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

	if (HitComponent != nullptr)
	{
		UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(HitComponent);

		if (StaticMeshComp)
		{
			if (FootstepEffectsSettings->EnableVertexPaintTrace && FootstepEffectsSettings->VertexColorPhysicalMaterialMap.IsValid())
			{
				int32 HitVertexID = UAlsxtVertexFunctionLibrary::GetClosestVertexIDFromStaticMesh(StaticMeshComp, Hit.ImpactPoint);
				EProminentRGBAChannel ProminentRGBAChannel = UAlsxtVertexFunctionLibrary::GetProminentVertexColorChannel(StaticMeshComp, HitVertexID);
				if (ProminentRGBAChannel != EProminentRGBAChannel::None)
				{
					// Convert ProminentRGBAChannel to EVertexColorChannel
					uint8 EnumAsUint8 = static_cast<uint8>(ProminentRGBAChannel);
					EVertexColorChannel VertexColorChannel = static_cast<EVertexColorChannel>(EnumAsUint8);

					// Find Matching Physical Material
					UAlsxtVertexColorPhysicalMaterialMap* VertexColorPhysicalMaterialMap = FootstepEffectsSettings->VertexColorPhysicalMaterialMap.Get();
					UPhysicalMaterial** VertexColorChannelPhysicalMaterial = VertexColorPhysicalMaterialMap->VertexColorPhysicalMaterialMap.Find(VertexColorChannel);
				}
			}
		}
	}

	FGameplayTag CharacterGait = IAlsxtCharacterInterface::Execute_GetCharacterGait(Mesh->GetOwner());
	FGameplayTag CharacterStance = IAlsxtCharacterInterface::Execute_GetCharacterStance(Mesh->GetOwner());
	const auto SurfaceType{ HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default };
	const auto* EffectSettings{ FootstepEffectsSettings->Effects.Find(SurfaceType) };
	FAlsxtFootwearDetails FootwearDetails = IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(Mesh->GetOwner());
	FGameplayTag CharacterFootwearType = FootwearDetails.FootwearType;
	UTexture2D* SoleTexture = FootwearDetails.FootwearSoleTexture;
	UTexture2D* SoleNormalTexture = FootwearDetails.FootwearSoleNormalTexture;
	UTexture2D* SoleDetailTexture = FootwearDetails.FootwearSoleDetailTexture;
	FALSXTFootwearTypeEffectsSettings CharacterFootwearTypeEffectsSettings;

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

	for (FALSXTFootwearTypeEffectsSettings FootwearTypeEffectsSettings : EffectSettings->FootwearTypeEffectsSettings)
	{
		if (FootwearTypeEffectsSettings.FootwearType.HasTag(CharacterFootwearType))
		{
			CharacterFootwearTypeEffectsSettings = FootwearTypeEffectsSettings;
		}
	}

	const auto FootstepLocation{ HitResult.ImpactPoint };

	const auto FootstepRotation{
		FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
									FootTransform.TransformVectorNoScale(FootBone == EAlsFootBone::Left
																			 ? FootstepEffectsSettings->FootLeftYAxis
																			 : FootstepEffectsSettings->FootRightYAxis)).ToQuat()
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
		auto VolumeMultiplier{ SoundVolumeMultiplier };

		if (!bIgnoreFootstepSoundBlockCurve && IsValid(AnimationInstance))
		{
			VolumeMultiplier *= 1.0f - UAlsMath::Clamp01(AnimationInstance->GetCurveValue(UAlsConstants::FootstepSoundBlockCurveName()));
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

					if (FootstepEffectsSettings->EnableMakeNoise)
					{
						float FootstepMakeNoiseLoudness { 0.0f };

						if (SoundType == EALSXTFootstepSoundType::Land)
						{
							FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.LandingSoundLevel;
						}
						else
						{
							if (CharacterStance == AlsStanceTags::Standing)
							{
								if (CharacterGait == AlsGaitTags::Walking)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.StandingWalkSoundLevel;
								}
								if (CharacterGait == AlsGaitTags::Running)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.StandingRunSoundLevel;
								}
								if (CharacterGait == AlsGaitTags::Sprinting)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.StandingSprintSoundLevel;
								}
							}
							if (CharacterStance == AlsStanceTags::Crouching)
							{
								if (CharacterGait == AlsGaitTags::Walking)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.CrouchedWalkSoundLevel;
								}
								if (CharacterGait == AlsGaitTags::Running)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.CrouchedRunSoundLevel;
								}
								if (CharacterGait == AlsGaitTags::Sprinting)
								{
									FootstepMakeNoiseLoudness = CharacterFootwearTypeEffectsSettings.FootwearTypeSoundLevels.CrouchedSprintSoundLevel;
								}
							}
						}

						Mesh->GetOwner()->MakeNoise(FootstepMakeNoiseLoudness, Cast<APawn>(Mesh->GetOwner()), FootstepLocation, 10, "Footstep");
					}
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

	// Prevent Spawning Decal if not in TransferrableSurfaces, but continue with saturation calculations
	// if EffectSettings->TransferrableSurfaces.Contains(HitResult.PhysMaterial->SurfaceType.GetValue())

	if (bSpawnDecal && IsValid(EffectSettings->DecalMaterial.LoadSynchronous()))
	{
		const auto DecalRotation{
			FootstepRotation * (FootBone == EAlsFootBone::Left
									? EffectSettings->DecalFootLeftRotationOffset
									: EffectSettings->DecalFootRightRotationOffset).Quaternion()
		};

		const auto DecalLocation{
			FootstepLocation + DecalRotation.RotateVector(EffectSettings->DecalLocationOffset * CapsuleScale)
		};

		// Calculate New Values
		// Then
		if (EffectSettings->TransferrableSurfaces.Contains(HitResult.PhysMaterial->SurfaceType.GetValue()))
		{
			// Spawn Decal
		}

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

				CurrentFootprintsState = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter);

				// Declare local variables
				UMaterialInstanceDynamic* MI;
				float CalculatedSoleNormalScale{ 0.0f };
				float CalculatedTransferNormalScale{ 0.0f };
				float DurationAverage{ 0.0f };
				FVector2D InputRange{ 0, 1 };
				FVector2D OutputRange{ 0, 1 };
				float DurationModifier{ 0.0f };

				FAlsxtFootprintState ActiveFootState;
				FAlsxtFootprintState CharacterFootState;

				if (FootBone == EAlsFootBone::Left)
				{
					ActiveFootState = CurrentFootprintsState.Left;
					CharacterFootState = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left;
					ActiveFootState.Previous = CharacterFootState.Current;
				}
				
				if (FootBone == EAlsFootBone::Right)
				{
					ActiveFootState = CurrentFootprintsState.Right;
					CharacterFootState = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right;
					ActiveFootState.Previous = CharacterFootState.Current;
				}

				//Example using above
				//Set New Current
				// ActiveFootState.Current.SurfaceType = UGameplayStatics::GetSurfaceType(Hit);
				// ActiveFootState.Current.TransferDetailTexture = EffectSettings->TransferDetailTexture;
				// ActiveFootState.Current.TransferDetailScale = EffectSettings->TransferDetailTextureScale;
				
				// ALSXTCharacter->ProcessNewFootprintsState(FootBone, CurrentFootprintsState);

				// Set New Left or Right foot Values based on current FootBone
				if (FootBone == EAlsFootBone::Left) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Left.Current.SurfaceType));

					if (NewSurface) {

						//Set Current as Previous
						CurrentFootprintsState.Left.Previous = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current;

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

						ALSXTCharacter->ProcessNewFootprintsState(EAlsFootBone::Left, CurrentFootprintsState);
					}
					
					CalculatedSoleNormalScale = (EffectSettings->TransferNormalScale + EffectSettings->SurfaceTransferAcceptanceNormalScale) * EffectSettings->SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount;	
					// CurrentFootprintsState = IALSXTCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter);
					
					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);

					MI->SetTextureParameterValue(FName("SoleTexture"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleNormalTexture);
					MI->SetTextureParameterValue(FName("SoleDetail"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleDetailTexture);
					MI->SetScalarParameterValue(FName("SoleNormalScale"), CalculatedSoleNormalScale);
					MI->SetTextureParameterValue(FName("TransferDetailTexture"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScale"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferNormalScale * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferDetailScale);
					MI->SetScalarParameterValue(FName("Opacity"), EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetVectorParameterValue(FName("PrimaryColor"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferWetness * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferEmissiveAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScalePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferNormalScale* EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferWetness * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferEmissiveAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("PhaseAlpha"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.TransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Previous.SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Left.Current.TransferWetness + EffectSettings->SurfaceTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { EffectSettings->DecalDurationModifierMin, EffectSettings->DecalDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration* DurationModifier, false);
				}
				if (FootBone == EAlsFootBone::Right) {

					NewSurface = ((UGameplayStatics::GetSurfaceType(Hit) != CurrentFootprintsState.Right.Current.SurfaceType));

					if (NewSurface) {

						//Set Current as Previous

						CurrentFootprintsState.Right.Previous = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current;

						// CurrentFootprintsState.Right.Previous.TransferSaturationRate = IALSXTCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferSaturationRate;

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

						ALSXTCharacter->ProcessNewFootprintsState(EAlsFootBone::Right, CurrentFootprintsState);
					}
					
					CalculatedSoleNormalScale = (EffectSettings->TransferNormalScale + EffectSettings->SurfaceTransferAcceptanceNormalScale) * EffectSettings->SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount;
					// CurrentFootprintsState = IALSXTCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter);

					//Create Dynamic Material Instance and Set Parameters
					MI = UMaterialInstanceDynamic::Create(Decal->GetMaterial(0), this);
					Decal->SetMaterial(0, MI);

					MI->SetTextureParameterValue(FName("SoleTexture"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleTexture);
					MI->SetTextureParameterValue(FName("SoleNormal"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleNormalTexture);
					MI->SetTextureParameterValue(FName("SoleDetail"), IAlsxtCharacterInterface::Execute_GetCharacterFootwearDetails(ALSXTCharacter).FootwearSoleDetailTexture);
					MI->SetScalarParameterValue(FName("SoleNormalScale"), CalculatedSoleNormalScale);
					MI->SetTextureParameterValue(FName("TransferDetailTexture"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormal"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScale"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferNormalScale * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("TransferDetailScale"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferDetailScale);
					MI->SetScalarParameterValue(FName("Opacity"), EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetVectorParameterValue(FName("PrimaryColor"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColor"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSize"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferGrainSize);
					MI->SetScalarParameterValue(FName("Wetness"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferWetness * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("EmissiveAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferEmissiveAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetTextureParameterValue(FName("TransferDetailTexturePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferDetailTexture);
					MI->SetTextureParameterValue(FName("TransferDetailNormalPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferDetailNormal);
					MI->SetScalarParameterValue(FName("TransferNormalScalePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferNormalScale);
					MI->SetScalarParameterValue(FName("TransferDetailScalePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferDetailScale);
					MI->SetVectorParameterValue(FName("PrimaryColorPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferPrimaryColor);
					MI->SetVectorParameterValue(FName("SecondaryColorPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferSecondaryColor);
					MI->SetScalarParameterValue(FName("GrainSizePrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferGrainSize);
					MI->SetScalarParameterValue(FName("WetnessPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferWetness * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("EmissiveAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferEmissiveAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("PhaseAlpha"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.FootSurfaceAlpha);
					MI->SetScalarParameterValue(FName("TransferAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmount"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("TransferAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.TransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);
					MI->SetScalarParameterValue(FName("SurfaceTransferAmountPrevious"), IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Previous.SurfaceTransferAmount * EffectSettings->SurfaceTransferAcceptanceAmount);

					//Calculate Duration based on Materials. Wetter materials stay longer
					DurationAverage = IAlsxtCharacterInterface::Execute_GetCharacterFootprintsState(ALSXTCharacter).Right.Current.TransferWetness + EffectSettings->SurfaceTransferAmount / 2;
					InputRange = { 0, 1 };
					OutputRange = { EffectSettings->DecalDurationModifierMin, EffectSettings->DecalDurationModifierMax };
					DurationModifier = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DurationAverage);
					Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration * DurationModifier, false);	
				}
			}
			else
			{
			if (GEngine)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));	
			}
			}
		}
	}

	if (bSpawnParticleSystem && IsValid(EffectSettings->ParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.WalkParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.RunParticleSystem.LoadSynchronous()) && IsValid(EffectSettings->FootstepParticles.LandParticleSystem.LoadSynchronous()))
	{
		UNiagaraSystem* GaitParticleSystem;
		if (IsValid(ALSXTCharacter)) {
			if (IAlsxtCharacterInterface::Execute_GetCharacterGait(ALSXTCharacter) == AlsGaitTags::Walking)
			{
				GaitParticleSystem = EffectSettings->FootstepParticles.WalkParticleSystem.Get();
			}
			else if (IAlsxtCharacterInterface::Execute_GetCharacterGait(ALSXTCharacter) == AlsGaitTags::Running || IAlsxtCharacterInterface::Execute_GetCharacterGait(ALSXTCharacter) == AlsGaitTags::Sprinting)
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
				FootstepRotation * (FootBone == EAlsFootBone::Left
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
