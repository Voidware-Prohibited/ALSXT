#pragma once

#include "GameplayTagContainer.h"
#include "State/AlsViewState.h"
#include "ALSXTHeadLookAtState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCameraLookAt{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCameraOffset{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CameraOffset{ EForceInit::ForceInit };

	bool operator==(const FALSXTHeadLookAtSettings& other) const
	{
		return (other.bCameraLookAt == bCameraLookAt) && (other.bEnableCameraOffset == bEnableCameraOffset) && (other.CameraOffset == CameraOffset);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> HeadLookAtActor {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime DateTime;

	bool operator==(const FALSXTHeadLookAtActor& other) const
	{
		return (other.HeadLookAtActor == HeadLookAtActor) && (other.Score == Score) && (other.Distance == Distance) && (other.DateTime == DateTime);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeadLookAtActor HeadLookAtActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HeadLookAtTransform{ EForceInit::ForceInit };

	bool operator==(const FALSXTHeadLookAtEntry& other) const
	{
		return (other.HeadLookAtActor == HeadLookAtActor) && (other.HeadLookAtTransform == HeadLookAtTransform);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableHeadLookAt{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Head Look At Mode"))
	FGameplayTag HeadLookAtMode {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeadLookAtEntry HeadLookAtEntry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeadLookAtSettings LookAtSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FALSXTHeadLookAtEntry> HeadLookAtEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* HeadLookAtActor{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HeadLookAtLocation{ EForceInit::ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator HeadLookAtRotation{ EForceInit::ForceInit };

	bool operator==(const FALSXTHeadLookAtState& other) const
	{
		return (other.bEnableHeadLookAt == bEnableHeadLookAt) && (other.HeadLookAtEntry == HeadLookAtEntry) && (other.LookAtSettings == LookAtSettings) && (other.HeadLookAtEntries == HeadLookAtEntries);
	}
};

