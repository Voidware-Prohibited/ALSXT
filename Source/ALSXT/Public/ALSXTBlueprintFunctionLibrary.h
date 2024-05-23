// MIT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Convert Euler To Quaternion", Keywords = "rotation, quaterion"), Category = "Quaternion Rotation")
	static FQuat ConvertEulerToQuaternion(FRotator CurrentRotation);

	static bool GetAdjustedRenderMatrix(const UMeshComponent* Component, APlayerController* PlayerController, float DesiredFOV, FMatrix& AdjustedRenderMatrix);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Side From Hit", Keywords = "hit, side"), Category = "ALS|Als Character")
	static void GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Side From Hit", Keywords = "hit, strength"), Category = "ALS|Als Character")
	static void GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength);
};
