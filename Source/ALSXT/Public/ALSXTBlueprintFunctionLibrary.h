// MIT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ALSXTBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Convert Euler To Quaternion", Keywords = "rotation, quaterion"), Category = "Quaternion Rotation")
	static FQuat ConvertEulerToQuaternion(FRotator CurrentRotation);
	
};
