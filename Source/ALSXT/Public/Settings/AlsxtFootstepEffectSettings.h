#pragma once

#include "AlsxtVertexSettings.h"
#include "Chooser.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimMontage.h"
#include "AlsxtFootstepEffectSettings.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralFootstepEffectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFootstepEffects { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bEnableFootstepEffects"))
	bool bEnableFootstepSounds { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bEnableFootstepEffects"))
	bool bEnableFootstepParticles { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bEnableFootstepEffects"))
	bool bEnableFootstepDecals { true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bEnableFootstepEffects"))
	bool EnableCharacterMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bEnableFootstepEffects"))
	bool EnableCharacterMovementAccentSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bEnableFootstepEffects"))
	bool bEnableOverlayObjectMovementSounds{ true };

	// EXPERIMENTAL: Perform a Trace for Vertex Paint color (ie Vertex-painted puddles) NOTE: More Resource-intensive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bEnableFootstepEffects"))
	bool bEnableVertexPaintTrace { true };

	// Allow AI with Listening components to hear the footprint sounds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bEnableFootstepEffects"))
	bool EnableMakeNoiseForAI{ true };
	
	// Allow AI with Viewing components to find footprint decals
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bEnableFootstepEffects"))
	bool EnableVisualAlertForAI{ true };

};

UENUM(BlueprintType)
enum class EALSXTFootstepSoundType : uint8
{
	Step,
	WalkRun,
	Land
};

UENUM(BlueprintType)
enum class EALSXTFootstepSoundSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToFootBone
};

UENUM(BlueprintType)
enum class EALSXTFootstepDecalSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToTraceHitComponent
};

UENUM(BlueprintType)
enum class EALSXTFootstepParticleEffectSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToFootBone
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootstepParticles
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> WalkParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> RunParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> LandParticleSystem;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearTypeSoundLevels
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float LandingSoundLevel{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float StandingWalkSoundLevel{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float StandingRunSoundLevel{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float StandingSprintSoundLevel{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float CrouchedWalkSoundLevel{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float CrouchedRunSoundLevel{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Make Sound")
	float CrouchedSprintSoundLevel{ 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFootwearTypeTextureSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> Surface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> Shallow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> Deep;

	bool operator==(const FAlsxtFootwearTypeTextureSet& other) const
	{
		return (other.Surface == Surface) && (other.Shallow == Shallow) && (other.Deep == Deep);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearTypeEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Footwear Type", AllowPrivateAccess))
	FGameplayTagContainer FootwearType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	FALSXTFootwearTypeSoundLevels FootwearTypeSoundLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FALSXTFootstepParticles FootstepParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System|Offset")
	FRotator ParticleSystemFootLeftRotationOffset{ ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System|Offset")
	FRotator ParticleSystemFootRightRotationOffset{ ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	EALSXTFootstepDecalSpawnType DecalSpawnType{ EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FVector DecalSize{ 10.0f, 20.0f, 20.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Offset")
	FRotator DecalFootLeftRotationOffset{ 90.0f, -90.0f, 180.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Offset")
	FRotator DecalFootRightRotationOffset{ -90.0f, 90.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Transfer")
	UTexture2D* TransferTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Transfer")
	UTexture2D* TransferNormalTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Transfer")
	UTexture2D* TransferDetailTexture = nullptr;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFootstepEffectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FGameplayTag SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<FALSXTFootwearTypeEffectsSettings> FootwearTypeEffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<enum EPhysicalSurface>> TransferrableSurfaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	EALSXTFootstepSoundSpawnType SoundSpawnType{EALSXTFootstepSoundSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferNormalTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferDetailTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDetailTextureScale{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float SurfaceTransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float SurfaceTransferAcceptanceAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = -2))
	float SurfaceTransferAcceptanceNormalScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferSaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDesaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0.0001))
	float TransferDetailScale{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDetailNormalAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = -2))
	float TransferNormalScale{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor TransferPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor TransferSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferGrainSize{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferEmissive{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FVector DecalSize{10.0f, 20.0f, 20.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalDuration{4.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float DecalDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float DecalDurationModifierMax{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalFadeOutDuration{2.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	EALSXTFootstepDecalSpawnType DecalSpawnType{EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FVector DecalLocationOffset{0.0f, -10.0f, -1.75f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootLeftRotationOffset{90.0f, -90.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootRightRotationOffset{-90.0f, 90.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FALSXTFootstepParticles FootstepParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	EALSXTFootstepParticleEffectSpawnType ParticleSystemSpawnType{EALSXTFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FVector ParticleSystemLocationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootLeftRotationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootRightRotationOffset{ForceInit};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFootstepEffectsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> SurfaceTraceChannel;

	// Allow AI with Listening components to hear the footprint sounds
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableMakeNoise{ true };

	// EXPERIMENTAL: Perform a Trace for Vertex Paint color (ie Vertex-painted puddles) NOTE: Requires being enabled in General Footstep Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableVertexPaintTrace{ false };

	// Map that defines wh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="EnableVertexPaintTrace"))
	TSoftObjectPtr<UAlsxtVertexColorPhysicalMaterialMap> VertexColorPhysicalMaterialMap;

	// Enable movement sound based on the Characters clothing/equipment Physical Material
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableCharacterMovementAccentSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableWeaponMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool EnableOverlayObjectMovementSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SurfaceTraceDistance{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Y Axis")
	FVector FootLeftYAxis{0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Z Axis")
	FVector FootLeftZAxis{1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Y Axis")
	FVector FootRightYAxis{0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Z Axis")
	FVector FootRightZAxis{-1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TSoftObjectPtr<UChooserTable> FootstepSoundChooserTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow, EditCondition="EnableCharacterMovementSound"))
	TSoftObjectPtr<UChooserTable> CharacterMovementSoundChooserTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow, EditCondition="EnableCharacterMovementAccentSound"))
	TSoftObjectPtr<UChooserTable> CharacterMovementAccentSoundChooserTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow, EditCondition="EnableOverlayObjectMovementSound"))
	TSoftObjectPtr<UChooserTable> CharacterOverlayObjectMovementSoundChooserTable {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TSoftObjectPtr<UChooserTable> ParticleChooserTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TSoftObjectPtr<UChooserTable> DecalTextureSetChooserTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TSoftObjectPtr<UALSXTFootstepEffectsSettings> EffectsNew {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FAlsxtFootstepEffectSettings> Effects;
};
