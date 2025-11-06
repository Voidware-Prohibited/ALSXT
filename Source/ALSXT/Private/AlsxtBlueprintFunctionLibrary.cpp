// MIT


#include "AlsxtBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"
#include "Interfaces/AlsxtControllerRenderInterface.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Utility/AlsxtGameplayTags.h"

FQuat UAlsxtBlueprintFunctionLibrary::ConvertEulerToQuaternion(FRotator CurrentRotation)
{
	FQuat q;
	float yaw = CurrentRotation.Yaw * PI / 180;
	float roll = CurrentRotation.Roll * PI / 180;
	float pitch = CurrentRotation.Pitch * PI / 180;

	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);

	q.W = cy * cr * cp + sy * sr * sp;
	q.X = cy * sr * cp - sy * cr * sp;
	q.Y = cy * cr * sp + sy * sr * cp;
	q.Z = sy * cr * cp - cy * sr * sp;

	return q;
}

bool UAlsxtBlueprintFunctionLibrary::GetAdjustedRenderMatrix(const UMeshComponent* Component,
	APlayerController* PlayerController, float DesiredFOV, FMatrix& AdjustedRenderMatrix)
{
	if (Component && PlayerController && PlayerController->PlayerCameraManager)
	{
		if (PlayerController->GetClass()->ImplementsInterface(UAlsxtControllerRenderInterface::StaticClass()))
		{
			if (IAlsxtControllerRenderInterface::Execute_IsSeparateFirstPersonFOVEnabled(PlayerController))
			{
				const float WorldFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
				const float ViewmodelFOV = DesiredFOV;

				FVector ViewOrigin;
				FRotator ViewRotation;
				PlayerController->GetPlayerViewPoint(ViewOrigin, ViewRotation);

				const float WorldHalfFOVRadians = FMath::DegreesToRadians(FMath::Max(0.001f, WorldFOV)) / 2.0f;
				const float DesiredHalfFOVRadians = FMath::DegreesToRadians(FMath::Max(0.001f, ViewmodelFOV)) / 2.0f;
				const float FOVRatio = WorldHalfFOVRadians / DesiredHalfFOVRadians;

				const FMatrix PerspectiveAdjustmentMatrix = FMatrix(
					FPlane(FOVRatio, 0, 0, 0),
					FPlane(0, FOVRatio, 0, 0),
					FPlane(0, 0, 1, 0),
					FPlane(0, 0, 0, 1));

				FMatrix ViewRotationMatrix = FInverseRotationMatrix(ViewRotation) * FMatrix(
					FPlane(0, 0, 1, 0),
					FPlane(1, 0, 0, 0),
					FPlane(0, 1, 0, 0),
					FPlane(0, 0, 0, 1));

				if (!ViewRotationMatrix.GetOrigin().IsNearlyZero(0.0f))
				{
					ViewOrigin += ViewRotationMatrix.InverseTransformPosition(FVector::ZeroVector);
					ViewRotationMatrix = ViewRotationMatrix.RemoveTranslation();
				}

				const FMatrix ViewMatrix = FTranslationMatrix(-ViewOrigin) * ViewRotationMatrix;
				const FMatrix InverseViewMatrix = FTranslationMatrix(-ViewMatrix.GetOrigin()) * ViewMatrix.RemoveTranslation().GetTransposed();
				//Compensate for FOV Difference
				const float OffsetRange = FMath::GetMappedRangeValueUnclamped(FVector2D(45.f, 71.f), FVector2D(4.5f, -4.5f), DesiredFOV);

				const FVector CompensationOffset = ViewRotation.Vector() * OffsetRange;
				const FMatrix FOVCompensation = FTransform(FRotator::ZeroRotator, CompensationOffset, FVector::OneVector).ToMatrixNoScale();

				AdjustedRenderMatrix = Component->GetComponentToWorld().ToMatrixWithScale() * ViewMatrix * PerspectiveAdjustmentMatrix * InverseViewMatrix * FOVCompensation;

				return true;
			}
		}
	}
	return false;
}

TArray<FGameplayTag> UAlsxtBlueprintFunctionLibrary::GetDirectChildTags(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return TArray<FGameplayTag>();
	}

	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	FGameplayTagContainer AllDescendantTags;
	// TagsManager.RequestGameplayTagChildrenInDictionary(Tag);
	AllDescendantTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(Tag);
	TArray<FGameplayTag> FirstLevelChildTags;

	// Iterate through all descendant tags and check if they are a direct child
	for (const FGameplayTag& DescendantTag : AllDescendantTags)
	{
		// Check if the current tag's direct parent is the tag we're looking for
		if (TagsManager.RequestGameplayTagDirectParent(DescendantTag) == Tag)
		{
			FirstLevelChildTags.Add(DescendantTag);
		}
	}

	return FirstLevelChildTags;
}

bool UAlsxtBlueprintFunctionLibrary::DoesGameplayTagEndWithString(FGameplayTag Tag, FString String)
{
	if (!Tag.IsValid())
	{
		return false;
	}
    
	// Get the full tag string representation
	FString TagString = Tag.ToString();

	// Check if the tag string ends with the specified suffix
	return TagString.EndsWith(String, ESearchCase::IgnoreCase);
}

bool UAlsxtBlueprintFunctionLibrary::DoesGameplayTagContainerHaveTagWithSuffix(const FGameplayTagContainer& InTagContainer, const FString& Suffix)
{
	for (const FGameplayTag& Tag : InTagContainer)
	{
		if (Tag.ToString().EndsWith(Suffix))
		{
			return true;
		}
	}

	return false;
}

TArray<FGameplayTag> UAlsxtBlueprintFunctionLibrary::GetOverlayAnimationInfoKeys(const TMap<FGameplayTag, FAlsxtOverlayAnimationInfo>& InMap)
{
	TArray<FGameplayTag> Keys;
	InMap.GetKeys(Keys);
	return Keys;
}

void UAlsxtBlueprintFunctionLibrary::GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side)
{
	float DotProduct{ Hit.OriginHitResult.HitResult.GetActor()->GetDotProductTo(Hit.HitResult.HitResult.GetActor()) };

	// 1 is face to face, 0 is side,, -1 is behind

	FVector CrossProduct{ FVector::CrossProduct(Hit.HitResult.Impulse, Hit.HitResult.Impulse) };
	Side = ALSXTImpactSideTags::Left;
}

void UAlsxtBlueprintFunctionLibrary::GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength)
{
	float HitMass = Hit.HitResult.Mass;
	FVector HitVelocity = Hit.HitResult.Velocity;
	FVector HitMomentum = HitMass * HitVelocity;

	float SelfMass = Hit.OriginHitResult.Mass;
	FVector SelfVelocity = Hit.OriginHitResult.Velocity;
	FVector SelfMomentum = SelfMass * SelfVelocity;

	FVector MomemtumSum = HitMomentum + SelfMomentum;

	Strength = ALSXTActionStrengthTags::Light;
}