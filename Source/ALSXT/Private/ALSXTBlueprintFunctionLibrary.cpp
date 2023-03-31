// MIT


#include "ALSXTBlueprintFunctionLibrary.h"

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