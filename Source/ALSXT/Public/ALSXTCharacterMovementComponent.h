// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterMovementComponent.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTCharacterMovementComponent : public UAlsCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual FVector ConsumeInputVector() override;
	
};
