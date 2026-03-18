// MIT


#include "AlsxtAimableOverlayObjectActor.h"


// Sets default values
AAlsxtAimableOverlayObjectActor::AAlsxtAimableOverlayObjectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAlsxtAimableOverlayObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlsxtAimableOverlayObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

