#pragma once

#include "GameFramework/RootMotionSource.h"
#include "Settings/ALSXTImpactReactionSettings.h"
#include "State/ALSXTImpactReactionState.h"
#include "ALSXTRootMotionSource_ImpactReaction.generated.h"

class UALSXTImpactReactionSettings;

USTRUCT()
struct ALSXT_API FALSXTRootMotionSource_ImpactReaction : public FRootMotionSource
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UALSXTImpactReactionSettings> ImpactReactionSettings{nullptr};

	UPROPERTY()
	FALSXTImpactReactionState ImpactReactionState;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY()
	FVector TargetRelativeLocation{ForceInit};

	UPROPERTY()
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY()
	FVector ActorFeetLocationOffset{ForceInit};

	UPROPERTY()
	FRotator ActorRotationOffset{ForceInit};

	UPROPERTY(Meta = (ClampMin = 0, ForceUnits = "cm"))
	float ImpactHeight{0.0f};

public:
	FALSXTRootMotionSource_ImpactReaction();

	virtual FRootMotionSource* Clone() const override;

	virtual bool Matches(const FRootMotionSource* Other) const override;

	virtual void PrepareRootMotion(float SimulationDeltaTime, float DeltaTime, const ACharacter& Character,
	                               const UCharacterMovementComponent& Movement) override;

	virtual bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess) override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToSimpleString() const override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
};

template <>
struct TStructOpsTypeTraits<FALSXTRootMotionSource_ImpactReaction> : public TStructOpsTypeTraitsBase2<FALSXTRootMotionSource_ImpactReaction>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
