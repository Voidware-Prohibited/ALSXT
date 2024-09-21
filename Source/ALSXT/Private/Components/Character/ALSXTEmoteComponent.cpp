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
	if (IALSXTCharacterInterface::Execute_CanEmote(GetOwner()))
	{
		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerAddDesiredEmote(Emote);
		}
		else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
		{
			AddDesiredEmote(Emote);
		}
	}
}

void UALSXTEmoteComponent::ServerAddDesiredEmote_Implementation(const FGameplayTag& Emote)
{
	AddDesiredEmote(Emote);
}

void UALSXTEmoteComponent::AddEmote(const FGameplayTag& Emote)
{

	if (IsValid(EmoteSettings) && IALSXTCharacterInterface::Execute_CanEmote(GetOwner()))
	{
		if (UAnimMontage* FoundMontage = EmoteSettings->Emotes.Find(Emote)->Montage)
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FoundMontage);
			OnEmote(Emote);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "No Animation Found");
	}
}

void UALSXTEmoteComponent::OnEmote_Implementation(const FGameplayTag& Emote) 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "OnEmote");
}
