// MIT

#include "ALSXTCharacterMovementComponent.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Interfaces/ALSXTCharacterInterface.h"

void UALSXTCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UALSXTCharacterMovementComponent::ConsumeInputVector()
{
	// return Super::ConsumeInputVector();

	if (IALSXTCharacterInterface::Execute_GetCharacterStatus(GetOwner()) == ALSXTStatusTags::Normal)
	{
		return Super::ConsumeInputVector();
	}
	else
	{
		return FVector::ZeroVector;
	}
}