#pragma once

#include "GameFramework/RootMotionSource.h"
#include "State/ALSXTVaultingState.h"
#include "ALSXTRootMotionSource_Vaulting.generated.h"

class UALSXTVaultingSettings;

USTRUCT()
struct ALSXT_API FALSXTRootMotionSource_Vaulting : public FRootMotionSource
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UALSXTVaultingSettings> VaultingSettings{nullptr};

	UPROPERTY()
	FALSXTVaultingState VaultingState;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY()
	FVector PlantingRelativeLocation{ForceInit};

	UPROPERTY()
	FVector TargetRelativeLocation{ForceInit};

	UPROPERTY()
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY()
	FVector ActorFeetLocationOffset{ForceInit};

	UPROPERTY()
	FRotator ActorRotationOffset{ForceInit};

	UPROPERTY(Meta = (ClampMin = 0, ForceUnits = "cm"))
	float VaultingHeight{0.0f};

public:
	FALSXTRootMotionSource_Vaulting();

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
struct TStructOpsTypeTraits<FALSXTRootMotionSource_Vaulting> : public TStructOpsTypeTraitsBase2<FALSXTRootMotionSource_Vaulting>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
