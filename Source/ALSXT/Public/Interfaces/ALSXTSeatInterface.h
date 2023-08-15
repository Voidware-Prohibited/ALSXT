#pragma once

#include "ALSXTSeatInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTSeatInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTSeatInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	const FGameplayTag GetStationaryMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	void GetSeatLocationAndRotation(FVector& Location, FRotator& Rotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	bool CanUseHandsWhileSeated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	bool CanSeatBeOccupied();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	bool IsSeatOccupied();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	void OccupySeat();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	TArray<AActor*> GetSwitchableSeats();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Seat Interface")
	void UnoccupySeat();

};