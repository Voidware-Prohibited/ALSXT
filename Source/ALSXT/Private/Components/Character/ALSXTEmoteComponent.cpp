// MIT

#include "Net/UnrealNetwork.h"
#include "Components/Character/ALSXTEmoteComponent.h"

// Sets default values for this component's properties
UALSXTEmoteComponent::UALSXTEmoteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UALSXTEmoteComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UALSXTEmoteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UALSXTEmoteComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Emote, Parameters)
}

// Emote

FGameplayTag UALSXTEmoteComponent::GetEmote() const
{
	return Emote;
}

void UALSXTEmoteComponent::AddDesiredEmote(const FGameplayTag& NewEmote)
{
	if (IALSXTCharacterInterface::Execute_CanEmote(GetOwner()))
	{
		if (EmoteSettings->bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "AddEmote");
		}

		// AddEmote(Emote);
		// MulticastAddDesiredEmote(Emote);
		// ServerAddDesiredEmote(Emote);

		// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "ServerAddDesiredEmote");
		// 	MulticastAddDesiredEmote(Emote);
		// 	ServerAddDesiredEmote(Emote);
		// }
		// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "AddDesiredEmote");
		// 	AddEmote(Emote);
		// 	MulticastAddDesiredEmote(Emote);
		// }

		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			// Server
			if (EmoteSettings->bDebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Server");
			}
			ServerAddDesiredEmote(Emote);
		}
		else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
		{
			// Reg
			if (EmoteSettings->bDebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Reg");
			}
			AddEmote(Emote);
		}
		else if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
		{
			// AI
			if (EmoteSettings->bDebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "AI");
			}
			ServerAddDesiredEmote(Emote);
		}
	}
	else
	{
		if (EmoteSettings->bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Cannot Emote");
		}
	}
}

void UALSXTEmoteComponent::ServerAddDesiredEmote_Implementation(const FGameplayTag& NewEmote)
{
	if (EmoteSettings->bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "ServerAddDesiredEmote");
	}
	AddDesiredEmote(Emote);
}

void UALSXTEmoteComponent::MulticastAddDesiredEmote_Implementation(const FGameplayTag& NewEmote)
{
	if (EmoteSettings->bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "MulticastAddDesiredEmote");
	}
	AddEmote(Emote);
}

void UALSXTEmoteComponent::AddEmote(const FGameplayTag& NewEmote)
{

	if (IsValid(EmoteSettings) && IALSXTCharacterInterface::Execute_CanEmote(GetOwner()))
	{
		// for (Emote : EmoteSettings->Emotes)
		
		if (UAnimMontage* FoundMontage = EmoteSettings->Emotes.Find(Emote)->Montage)
		{
			if (EmoteSettings->bDebugMode)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FoundMontage->GetName());
			}
			IALSXTCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(FoundMontage);
			OnEmote(Emote);
		}
	}
	else
	{
		if (EmoteSettings->bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Cannot Emote");
		}
	}
}

void UALSXTEmoteComponent::OnEmote_Implementation(const FGameplayTag& NewEmote)
{
	if (EmoteSettings->bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "OnEmote");
	}	
}
