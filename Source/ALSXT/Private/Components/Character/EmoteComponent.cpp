// MIT


#include "Components/Character/EmoteComponent.h"

// Sets default values for this component's properties
UEmoteComponent::UEmoteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEmoteComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UEmoteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Emote

void UEmoteComponent::AddDesiredEmote(const FGameplayTag& Emote)
{
	if (CanEmote())
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerAddDesiredEmote(Emote);
			}
	}
}

void UEmoteComponent::ServerAddDesiredEmote_Implementation(const FGameplayTag& Emote)
{
	AddDesiredEmote(Emote);
}

void UEmoteComponent::AddEmote(const FGameplayTag& Emote)
{

	if (CanEmote())
	{
		OnEmote(Emote);
	}
}

void UEmoteComponent::OnEmote_Implementation(const FGameplayTag& Emote) {}
