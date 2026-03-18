// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtAbilitySystemComponentBase.h"
#include "AlsxtOverlayObjectAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSXT_API UAlsxtOverlayObjectAbilitySystemComponent : public UAlsxtAbilitySystemComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAlsxtOverlayObjectAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
