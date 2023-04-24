// MIT

#include "ALSXTCharacterMovementComponent.h"
#include "Utility/ALSXTGameplayTags.h"

void UALSXTCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UALSXTCharacterMovementComponent::ConsumeInputVector()
{
	// return Super::ConsumeInputVector();

	if (Cast<AALSXTCharacter>(GetOwner())->GetDesiredStatus() == ALSXTStatusTags::Normal)
	{
		return Super::ConsumeInputVector();
	}
	else
	{
		return FVector::ZeroVector;
	}
}