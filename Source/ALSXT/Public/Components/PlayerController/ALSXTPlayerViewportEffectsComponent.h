// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSXTPlayerViewportEffectsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTPlayerViewportEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTPlayerViewportEffectsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UPlayerCameraManager* CameraManager;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddSuppressionEffect(float Amount);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ClearSuppressionEffect();

		
};
