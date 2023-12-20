#pragma once

#include "ALSXTStationaryModeComponentInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTStationaryModeComponentInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTStationaryModeComponentInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	const FGameplayTag GetStationaryMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	void GetSeatLocationAndRotation(FVector& Location, FRotator& Rotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	bool CanUseHandsWhileSeated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	bool CanSeatBeOccupied();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	bool IsSeatOccupied();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	void OccupySeat();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	TArray<AActor*> GetSwitchableSeats();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stationary Mode Component Interface")
	void UnoccupySeat();

};