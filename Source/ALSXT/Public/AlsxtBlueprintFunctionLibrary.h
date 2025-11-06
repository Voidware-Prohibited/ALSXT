// MIT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility/AlsxtStructs.h"
#include "AlsxtBlueprintFunctionLibrary.generated.h"

struct FAlsxtOverlayAnimationInfo;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (DisplayName = "Convert Euler To Quaternion", Keywords = "rotation, quaterion"), Category = "Alsxt|Quaternion Rotation")
	static FQuat ConvertEulerToQuaternion(FRotator CurrentRotation);

	static bool GetAdjustedRenderMatrix(const UMeshComponent* Component, APlayerController* PlayerController, float DesiredFOV, FMatrix& AdjustedRenderMatrix);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (AutoCreateRefTerm = "Tag", DisplayName = "Get Direct Child Tags", Keywords = "direct, child, tags"), Category = "Alsxt|Overlay Layers")
	static TArray<FGameplayTag> GetDirectChildTags(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (AutoCreateRefTerm = "Tag", DisplayName = "Does Gameplay Tag End With String", Keywords = "direct, child, tags"), Category = "Gameplay Tags")
	static bool DoesGameplayTagEndWithString(FGameplayTag Tag, FString String);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (AutoCreateRefTerm = "Tag", DisplayName = "Does Gameplay Tag Container Have Tag With Suffix", Keywords = "tag, container, have, tag, suffix"), Category = "Gameplay Tags")
	static bool DoesGameplayTagContainerHaveTagWithSuffix(const FGameplayTagContainer& InTagContainer, const FString& Suffix);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (DisplayName = "Get OverlayAnimationInfo Keys", Keywords = "overlay, info, keys"), Category = "Alsxt|Overlay Layers")
	static TArray<FGameplayTag> GetOverlayAnimationInfoKeys(const TMap<FGameplayTag, FAlsxtOverlayAnimationInfo>& InMap);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Side From Hit", Keywords = "hit, side"), Category = "Alsxt|Impact")
	static void GetSideFromHit(FDoubleHitResult Hit, FGameplayTag& Side);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Side From Hit", Keywords = "hit, strength"), Category = "Alsxt|Impact")
	static void GetStrengthFromHit(FDoubleHitResult Hit, FGameplayTag& Strength);
};
