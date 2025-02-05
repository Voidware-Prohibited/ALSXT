#include "RootMotionSources/ALSXTRootMotionSource_Vaulting.h"

#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "ALSXTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Utility/AlsMacros.h"

FALSXTRootMotionSource_Vaulting::FALSXTRootMotionSource_Vaulting()
{
	Priority = 1000;
}

FRootMotionSource* FALSXTRootMotionSource_Vaulting::Clone() const
{
	return new FALSXTRootMotionSource_Vaulting{*this};
}

bool FALSXTRootMotionSource_Vaulting::Matches(const FRootMotionSource* Other) const
{
	if (!Super::Matches(Other))
	{
		return false;
	}

	const auto* OtherCasted{static_cast<const FALSXTRootMotionSource_Vaulting*>(Other)};

	return VaultingSettings == OtherCasted->VaultingSettings &&
	       TargetPrimitive == OtherCasted->TargetPrimitive;
}

void FALSXTRootMotionSource_Vaulting::PrepareRootMotion(const float SimulationDeltaTime, const float DeltaTime,
                                                      const ACharacter& Character, const UCharacterMovementComponent& Movement)
{
	const AALSXTCharacter& ALSXTCharacter = dynamic_cast<const AALSXTCharacter&>(Character);
	VaultingState = ALSXTCharacter.GetVaultingState();

	SetTime(GetTime() + SimulationDeltaTime);

	if (!ALS_ENSURE(Duration > SMALL_NUMBER) || DeltaTime <= SMALL_NUMBER)
	{
		RootMotionParams.Clear();
		return;
	}

	const auto VaultingTime{GetTime() * VaultingSettings->GetPlayRateForHeight(VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight, VaultingState.VaultingParameters.VaultAnimation.Montage.PlayRate, VaultingHeight)};

	// Calculate target transform from the stored relative transform to follow along with moving objects.

	auto PlantingTargetTransform{
		TargetPrimitive.IsValid()
			? FTransform{TargetRelativeRotation, PlantingRelativeLocation, TargetPrimitive->GetComponentScale()}
			.GetRelativeTransformReverse(TargetPrimitive->GetComponentTransform())
			: FTransform{TargetRelativeRotation, PlantingRelativeLocation}
	};

	// Calculate target transform from the stored relative transform to follow along with moving objects.

	auto TargetTransform{
		TargetPrimitive.IsValid()
			? FTransform{TargetRelativeRotation, TargetRelativeLocation, TargetPrimitive->GetComponentScale()}
			.GetRelativeTransformReverse(TargetPrimitive->GetComponentTransform())
			: FTransform{TargetRelativeRotation, TargetRelativeLocation}
	};

	FVector LocationOffset;
	FRotator RotationOffset;

	const auto BlendInAmount{ VaultingState.VaultingParameters.VaultAnimation.Montage.BlendInCurve->GetFloatValue(VaultingTime)};

	if (!FAnimWeight::IsRelevant(BlendInAmount))
	{
		LocationOffset = ActorFeetLocationOffset;
		RotationOffset = ActorRotationOffset;
	}
	else
	{
		const FVector3f InterpolationAndCorrectionAmounts{
			VaultingState.VaultingParameters.VaultAnimation.Montage.InterpolationAndCorrectionAmountsCurve->GetVectorValue(
				VaultingTime + VaultingSettings->GetStartTimeForHeight(VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight, VaultingState.VaultingParameters.VaultAnimation.Montage.StartTime, VaultingHeight))
		};

		const auto InterpolationAmount{InterpolationAndCorrectionAmounts.X};
		const auto HorizontalCorrectionAmount{InterpolationAndCorrectionAmounts.Y};
		const auto VerticalCorrectionAmount{InterpolationAndCorrectionAmounts.Z};

		if (!FAnimWeight::IsRelevant(InterpolationAmount))
		{
			LocationOffset = FVector::ZeroVector;
			RotationOffset = FRotator::ZeroRotator;
		}
		else
		{
			// Calculate the animation offset. This would be the location the actual animation starts at relative to the target transform.

			auto AnimationLocationOffset{PlantingTargetTransform.GetUnitAxis(EAxis::X) * VaultingState.VaultingParameters.TargetHandPlantRelativeLocation.X};
			AnimationLocationOffset.Z = VaultingState.VaultingParameters.TargetHandPlantRelativeLocation.Z;
			AnimationLocationOffset *= Character.GetMesh()->GetComponentScale().Z;

			// Blend into the animation offset and final offset at the same time.
			// Horizontal and vertical blends use different correction amounts.

			LocationOffset.X = FMath::Lerp(ActorFeetLocationOffset.X, AnimationLocationOffset.X, HorizontalCorrectionAmount) *
			                   InterpolationAmount;
			LocationOffset.Y = FMath::Lerp(ActorFeetLocationOffset.Y, AnimationLocationOffset.Y, HorizontalCorrectionAmount) *
			                   InterpolationAmount;
			LocationOffset.Z = FMath::Lerp(ActorFeetLocationOffset.Z, AnimationLocationOffset.Z, VerticalCorrectionAmount) *
			                   InterpolationAmount;

			// Actor rotation offset must be normalized for this block of code to work properly.

			RotationOffset = ActorRotationOffset * (1.0f - HorizontalCorrectionAmount) * InterpolationAmount;
		}

		// Initial blend in allows the actor to blend into the interpolation and correction curves at
		// the midpoint. This prevents pops when Vaulting an object lower than the animated Vaulting.

		if (!FAnimWeight::IsFullWeight(BlendInAmount))
		{
			LocationOffset = FMath::Lerp(ActorFeetLocationOffset, LocationOffset, BlendInAmount);
			RotationOffset = FMath::Lerp(ActorRotationOffset, RotationOffset, BlendInAmount);
		}
	}

	// Apply final offsets.

	PlantingTargetTransform.AddToTranslation(LocationOffset);
	PlantingTargetTransform.ConcatenateRotation(RotationOffset.Quaternion());

	// Find the delta transform between the character and the target transform and divide by the delta time to get the velocity.

	TargetTransform.AddToTranslation(-Movement.GetActorFeetLocation());
	TargetTransform.ConcatenateRotation(Movement.UpdatedComponent->GetComponentQuat().Inverse());

	RootMotionParams.Set(TargetTransform * ScalarRegister{1.0f / DeltaTime});
	bSimulatedNeedsSmoothing = true;
}

bool FALSXTRootMotionSource_Vaulting::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	if (!Super::NetSerialize(Archive, Map, bSuccess))
	{
		bSuccess = false;
		return false;
	}

	bSuccess = true;
	auto bSuccessLocal{true};

	Archive << VaultingSettings;
	Archive << TargetPrimitive;
	Archive << VaultingState.VaultingParameters.VaultAnimation.Montage.Montage;
	Archive << VaultingState.VaultingParameters.VaultAnimation.Montage.BlendInCurve;
	Archive << VaultingState.VaultingParameters.VaultAnimation.Montage.InterpolationAndCorrectionAmountsCurve;

	bSuccess &= SerializePackedVector<100, 30>(VaultingState.VaultingParameters.VaultAnimation.Montage.StartRelativeLocation, Archive);

	VaultingState.VaultingParameters.VaultAnimation.Montage.StartRelativeLocation.NetSerialize(Archive, Map, bSuccessLocal);
	VaultingState.VaultingParameters.VaultAnimation.Montage.StartRelativeLocation.Normalize();
	bSuccess &= bSuccessLocal;

	VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight.NetSerialize(Archive, Map, bSuccessLocal);
	VaultingState.VaultingParameters.VaultAnimation.Montage.ReferenceHeight.Normalize();
	bSuccess &= bSuccessLocal;

	VaultingState.VaultingParameters.VaultAnimation.Montage.StartTime.NetSerialize(Archive, Map, bSuccessLocal);
	VaultingState.VaultingParameters.VaultAnimation.Montage.StartTime.Normalize();
	bSuccess &= bSuccessLocal;

	VaultingState.VaultingParameters.VaultAnimation.Montage.PlayRate.NetSerialize(Archive, Map, bSuccessLocal);
	VaultingState.VaultingParameters.VaultAnimation.Montage.PlayRate.Normalize();
	bSuccess &= bSuccessLocal;

	bSuccess &= SerializePackedVector<100, 30>(TargetRelativeLocation, Archive);

	TargetRelativeRotation.NetSerialize(Archive, Map, bSuccessLocal);
	TargetRelativeRotation.Normalize();
	bSuccess &= bSuccessLocal;

	bSuccess &= SerializePackedVector<100, 30>(ActorFeetLocationOffset, Archive);

	ActorRotationOffset.NetSerialize(Archive, Map, bSuccessLocal);
	ActorRotationOffset.Normalize();
	bSuccess &= bSuccessLocal;

	Archive << VaultingHeight;

	return bSuccess;
}

UScriptStruct* FALSXTRootMotionSource_Vaulting::GetScriptStruct() const
{
	return StaticStruct();
}

FString FALSXTRootMotionSource_Vaulting::ToSimpleString() const
{
	return FString::Format(TEXT("{0} ({1}, {2})"), {ALS_GET_TYPE_STRING(FALSXTRootMotionSource_Vaulting), *InstanceName.ToString(), LocalID});
}

void FALSXTRootMotionSource_Vaulting::AddReferencedObjects(FReferenceCollector& Collector)
{
	Super::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(VaultingSettings);
}
