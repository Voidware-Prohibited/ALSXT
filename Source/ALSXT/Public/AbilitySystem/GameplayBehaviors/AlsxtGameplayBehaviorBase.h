// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehavior.h"
#include "AlsxtGameplayBehaviorBase.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayBehaviorBase : public UGameplayBehavior
{
	GENERATED_BODY()

public:
	virtual bool Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config = nullptr, AActor* SmartObjectOwner = nullptr) override;
};
