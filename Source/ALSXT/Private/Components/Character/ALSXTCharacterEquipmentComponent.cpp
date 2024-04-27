// MIT


#include "Components/Character/ALSXTCharacterEquipmentComponent.h"
#include "Components/Mesh/ALSXTPaintableSkeletalMeshComponent.h"

// Sets default values for this component's properties
UALSXTCharacterEquipmentComponent::UALSXTCharacterEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UALSXTCharacterEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<UALSXTPaintableSkeletalMeshComponent*> UALSXTCharacterEquipmentComponent::GetAllComponents()
{
	TArray<UALSXTPaintableSkeletalMeshComponent*> AllComponents;
	return AllComponents;
}

TArray<UALSXTPaintableSkeletalMeshComponent*> UALSXTCharacterEquipmentComponent::GetHighlightableComponents()
{
	TArray<UALSXTPaintableSkeletalMeshComponent*> AllComponents{ GetAllComponents() };
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
