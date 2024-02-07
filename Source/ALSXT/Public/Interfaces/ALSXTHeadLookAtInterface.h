#pragma once

#include "GameplayTagContainer.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTHeadLookAtInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTHeadLookAtInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTHeadLookAtInterface {
	GENERATED_BODY()

public:

UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  bool CanHeadLookAt();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  bool CanBeHeadLookedAt();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  bool IsInFrontOf(FVector ActorLocation);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  float GetRating();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  void BeginHeadLookAt(AActor* Actor, bool CameraLookAt);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  void OnBeginHeadLookAt();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  FVector GetHeadLookAtLocation();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  void EndHeadLookAt();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  UPARAM(meta = (Categories = "Als.Facial Expression")) FGameplayTagContainer GetFacialReactions();
};