#include "AlsxtTest_BasicMovement.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"

// Latent command to simulate holding "Forward" for a specific duration
DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FAlsxtSimulateMoveForward, ACharacter*, Character, float, Duration, float, ElapsedTime);

bool FAlsxtSimulateMoveForward::Update()
{
	float DeltaTime = FApp::GetDeltaTime();
	ElapsedTime += DeltaTime;

	if (Character)
	{
		// ALSXT characters still use standard AddMovementInput for basic velocity
		Character->AddMovementInput(Character->GetActorForwardVector(), 1.0f);
	}

	return ElapsedTime >= Duration;
}

bool FAlsxtTest_BasicMovement::RunTest(const FString& Parameters)
{
	// 1. Load a basic test level or use current
	AutomationOpenMap(TEXT("/ALSXT/ALSExtras/Levels/L_TestMap")); // Replace with your actual test map path

	UWorld* World = GEngine->GetWorldContexts()[0].World();
	if (!World) return false;

	// 2. Get or Spawn Character
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!Character) return false;

	FVector StartLocation = Character->GetActorLocation();

	// 3. Queue movement simulation (Latent command)
	ADD_LATENT_AUTOMATION_COMMAND(FAlsxtSimulateMoveForward(Character, 1.0f, 0.0f));

	// 4. Verify movement after latent command finishes
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Character, StartLocation]()
	{
		FVector EndLocation = Character->GetActorLocation();
		float DistanceMoved = FVector::Dist(StartLocation, EndLocation);

		// Assert that the character moved at least 100 units forward
		TestTrue(TEXT("Character should have moved forward"), DistanceMoved > 100.0f);
		return true;
	}));

	return true;
}