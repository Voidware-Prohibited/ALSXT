// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtOverlayObjectActor.h"
#include "AlsxtOverlayObjectActorFirearm.generated.h"

UCLASS()
class ALSXT_API AAlsxtOverlayObjectActorFirearm : public AAlsxtOverlayObjectActor, public IAlsxtFirearmInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAlsxtOverlayObjectActorFirearm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UAlsxtFirearmDischargeSettingsDataAsset> FirearmDischargeSettings;
};
