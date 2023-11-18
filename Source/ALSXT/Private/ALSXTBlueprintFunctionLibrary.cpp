// MIT


#include "ALSXTBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/ALSXTControllerRenderInterface.h"

FQuat UALSXTBlueprintFunctionLibrary::ConvertEulerToQuaternion(FRotator CurrentRotation)
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

bool UALSXTBlueprintFunctionLibrary::GetAdjustedRenderMatrix(const UMeshComponent* Component,
	APlayerController* PlayerController, float DesiredFOV, FMatrix& AdjustedRenderMatrix)
{
	if (Component && PlayerController && PlayerController->PlayerCameraManager)
	{
		if (PlayerController->GetClass()->ImplementsInterface(UALSXTControllerRenderInterface::StaticClass()))
		{
			if (IALSXTControllerRenderInterface::Execute_IsSeparateFirstPersonFOVEnabled(PlayerController))
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