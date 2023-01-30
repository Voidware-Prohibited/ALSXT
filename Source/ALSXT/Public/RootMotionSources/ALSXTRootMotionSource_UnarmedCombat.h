#pragma once

#include "GameFramework/RootMotionSource.h"
#include "ALSXTRootMotionSource_UnarmedCombat.generated.h"

class UALSXTUnarmedCombatSettings;

USTRUCT()
struct ALSXT_API FALSXTRootMotionSource_UnarmedCombat : public FRootMotionSource
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UALSXTUnarmedCombatSettings> UnarmedCombatSettings{nullptr};

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
	float AttackHeight{0.0f};

public:
	FALSXTRootMotionSource_UnarmedCombat();

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
struct TStructOpsTypeTraits<FALSXTRootMotionSource_UnarmedCombat> : public TStructOpsTypeTraitsBase2<FALSXTRootMotionSource_UnarmedCombat>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
