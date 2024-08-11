// MIT


#include "Components/Character/ALSXTCharacterCustomizationComponent.h"
#include "Components/Mesh/ALSXTPaintableSkeletalMeshComponent.h"

// Sets default values for this component's properties
UALSXTCharacterCustomizationComponent::UALSXTCharacterCustomizationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTCharacterCustomizationComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UALSXTCharacterCustomizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FALSXTCharacterVoiceParameters UALSXTCharacterCustomizationComponent::GetVoiceParameters()
{
	return VoiceParameters;
}

TArray<UALSXTPaintableSkeletalMeshComponent*> UALSXTCharacterCustomizationComponent::GetAllComponents()
{
	TArray<UALSXTPaintableSkeletalMeshComponent*> AllComponents;
	return AllComponents;
}

TArray<UALSXTPaintableSkeletalMeshComponent*> UALSXTCharacterCustomizationComponent::GetHighlightableComponents()
{
	TArray<UALSXTPaintableSkeletalMeshComponent*> AllComponents {GetAllComponents()};
	TArray<UALSXTPaintableSkeletalMeshComponent*> HighlightableComponents;

	for (UALSXTPaintableSkeletalMeshComponent* Component : AllComponents)
	{
		if (IsValid(Component))
		{
			HighlightableComponents.Add(Component);
		}
	}
	return HighlightableComponents;
}

