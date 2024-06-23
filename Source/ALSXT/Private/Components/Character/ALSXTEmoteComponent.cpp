// MIT


#include "Components/Character/ALSXTEmoteComponent.h"

// Sets default values for this component's properties
UALSXTEmoteComponent::UALSXTEmoteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTEmoteComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UALSXTEmoteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Emote

void UALSXTEmoteComponent::AddDesiredEmote(const FGameplayTag& Emote)
{
	if (CanEmote())
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerAddDesiredEmote(Emote);
			}
	}
}

void UALSXTEmoteComponent::ServerAddDesiredEmote_Implementation(const FGameplayTag& Emote)
{
	AddDesiredEmote(Emote);
}

void UALSXTEmoteComponent::AddEmote(const FGameplayTag& Emote)
{

	if (IsValid(EmoteSettings) && CanEmote())
	{
		if (UAnimMontage* FoundMontage = EmoteSettings->Emotes.Find(Emote)->Montage)
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FoundMontage);
			OnEmote(Emote);
		}
	}
}

void UALSXTEmoteComponent::OnEmote_Implementation(const FGameplayTag& Emote) {}
