#include "RootMotionSources/ALSXTRootMotionSource_CombatAttack.h"

#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/ALSXTCombatSettings.h"
#include "Utility/AlsMacros.h"

FALSXTRootMotionSource_CombatAttack::FALSXTRootMotionSource_CombatAttack()
{
	Priority = 1000;
}

FRootMotionSource* FALSXTRootMotionSource_CombatAttack::Clone() const
{
	return new FALSXTRootMotionSource_CombatAttack{*this};
}

bool FALSXTRootMotionSource_CombatAttack::Matches(const FRootMotionSource* Other) const
{
	if (!Super::Matches(Other))
	{
		return false;
	}

	const auto* OtherCasted{static_cast<const FALSXTRootMotionSource_CombatAttack*>(Other)};

	return CombatSettings == OtherCasted->CombatSettings &&
	       TargetPrimitive == OtherCasted->TargetPrimitive;
}

void FALSXTRootMotionSource_CombatAttack::PrepareRootMotion(const float SimulationDeltaTime, const float DeltaTime,
                                                      const ACharacter& Character, const UCharacterMovementComponent& Movement)
{
	SetTime(GetTime() + SimulationDeltaTime);

	if (!ALS_ENSURE(Duration > SMALL_NUMBER) || DeltaTime <= SMALL_NUMBER)
	{
		RootMotionParams.Clear();
		return;
	}

	const auto AttackTime{GetTime() * CombatSettings->CalculatePlayRate(CombatSettings->CurrentActionMontage.ReferenceHeight, CombatSettings->CurrentActionMontage.PlayRate, AttackHeight)};

	// Calculate target transform from the stored relative transform to follow along with moving objects.

	auto TargetTransform{
		TargetPrimitive.IsValid()
			? FTransform{TargetRelativeRotation, TargetRelativeLocation, TargetPrimitive->GetComponentScale()}
			.GetRelativeTransformReverse(TargetPrimitive->GetComponentTransform())
			: FTransform{TargetRelativeRotation, TargetRelativeLocation}
	};

	FVector LocationOffset;
	FRotator RotationOffset;

	const auto BlendInAmount{CombatSettings->CurrentActionMontage.BlendInCurve->GetFloatValue(AttackTime)};

	if (!FAnimWeight::IsRelevant(BlendInAmount))
	{
		LocationOffset = ActorFeetLocationOffset;
		RotationOffset = ActorRotationOffset;
	}
	else
	{
		const FVector3f InterpolationAndCorrectionAmounts{
			CombatSettings->CurrentActionMontage.InterpolationAndCorrectionAmountsCurve->GetVectorValue(
				AttackTime + CombatSettings->CalculateStartTime(CombatSettings->CurrentActionMontage.ReferenceHeight, CombatSettings->CurrentActionMontage.StartTime, AttackHeight))
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

			auto AnimationLocationOffset{TargetTransform.GetUnitAxis(EAxis::X) * CombatSettings->CurrentActionMontage.StartRelativeLocation.X};
			AnimationLocationOffset.Z = CombatSettings->CurrentActionMontage.StartRelativeLocation.Z;
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
		// the midpoint. This prevents pops when mantling an object lower than the animated mantling.

		if (!FAnimWeight::IsFullWeight(BlendInAmount))
		{
			LocationOffset = FMath::Lerp(ActorFeetLocationOffset, LocationOffset, BlendInAmount);
			RotationOffset = FMath::Lerp(ActorRotationOffset, RotationOffset, BlendInAmount);
		}
	}

	// Apply final offsets.

	TargetTransform.AddToTranslation(LocationOffset);
	TargetTransform.ConcatenateRotation(RotationOffset.Quaternion());

	// Find the delta transform between the character and the target transform and divide by the delta time to get the velocity.

	TargetTransform.AddToTranslation(-Movement.GetActorFeetLocation());
	TargetTransform.ConcatenateRotation(Movement.UpdatedComponent->GetComponentQuat().Inverse());

	RootMotionParams.Set(TargetTransform * ScalarRegister{1.0f / DeltaTime});
	bSimulatedNeedsSmoothing = true;
}

bool FALSXTRootMotionSource_CombatAttack::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	if (!Super::NetSerialize(Archive, Map, bSuccess))
	{
		bSuccess = false;
		return false;
	}

	bSuccess = true;
	auto bSuccessLocal{true};

	Archive << CombatSettings;
	Archive << TargetPrimitive;

	bSuccess &= SerializePackedVector<100, 30>(TargetRelativeLocation, Archive);

	TargetRelativeRotation.NetSerialize(Archive, Map, bSuccessLocal);
	TargetRelativeRotation.Normalize();
	bSuccess &= bSuccessLocal;

	bSuccess &= SerializePackedVector<100, 30>(ActorFeetLocationOffset, Archive);

	ActorRotationOffset.NetSerialize(Archive, Map, bSuccessLocal);
	ActorRotationOffset.Normalize();
	bSuccess &= bSuccessLocal;

	Archive << AttackHeight;

	return bSuccess;
}

UScriptStruct* FALSXTRootMotionSource_CombatAttack::GetScriptStruct() const
{
	return StaticStruct();
}

FString FALSXTRootMotionSource_CombatAttack::ToSimpleString() const
{
	return FString::Format(TEXT("{0} ({1}, {2})"), {ALS_GET_TYPE_STRING(FALSXTRootMotionSource_CombatAttack), *InstanceName.ToString(), LocalID});
}

void FALSXTRootMotionSource_CombatAttack::AddReferencedObjects(FReferenceCollector& Collector)
{
	Super::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(CombatSettings);
}
