// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtOverlayObjectActor.h"
#include "AlsxtAimableOverlayObjectActor.generated.h"

UCLASS()
class ALSXT_API AAlsxtAimableOverlayObjectActor : public AAlsxtOverlayObjectActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAlsxtAimableOverlayObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// This Gameplay Tag is used to retrieve Settings from the Aimable Overlay Object Lookup Table 
	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTag AimableOverlayObjectTag;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UAlsxtAimableOverlayObjectSettingsDataAsset> AimableOverlayObjectSettings;
};
