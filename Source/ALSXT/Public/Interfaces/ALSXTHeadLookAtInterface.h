#pragma once

#include "GameplayTagContainer.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "State/ALSXTHeadLookAtState.h"
#include "ALSXTHeadLookAtInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTHeadLookAtInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTHeadLookAtInterface {
	GENERATED_BODY()

public:

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  bool CanHeadLookAt() const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  bool CanBeHeadLookedAt() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Head Look At Interface")
  void IsInFrontOf(bool& IsInFrontOf, FVector LookAtActorLocation) const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  FALSXTHeadLookAtActor GetHeadLookAtActor(FVector FromLocation) const;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  FVector GetHeadLookAtLocation();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  FTransform GetHeadLookAtTransform();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  FALSXTHeadLookAtSettings GetLookAtSettings();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  void AddHeadLookAtEntry(FALSXTHeadLookAtEntry HeadLookAtEntry);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  void RemoveHeadLookAtEntry(FALSXTHeadLookAtEntry HeadLookAtEntry);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  void ResetHeadLookAt();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  FALSXTHeadLookAtEntry GetBestHeadLookAtEntry() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  FALSXTHeadLookAtEntry GetNewBestGazeHeadLookAtEntry() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  void BeginHeadLookAt(FALSXTHeadLookAtEntry HeadLookAtEntry);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  void OnBeginHeadLookAt();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Look At Interface")
  void EndHeadLookAt();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Head Look At Interface")
  UPARAM(meta = (Categories = "Als.Facial Expression")) FGameplayTagContainer GetFacialReactions();
};