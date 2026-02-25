// MIT


#include "AlsxtModularGameplayGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

void AAlsxtModularGameplayGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (!ServerOwnerPlayerState)
	{
		if (NewPlayer->PlayerState->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()) )
		{
			if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(NewPlayer->PlayerState))
			{
				if (UAbilitySystemComponent* ServerOwnerASC = ASCInterface->GetAbilitySystemComponent())
				{
					if (ServerOwnerAbility)
					{
						// Grant the ability. This function is server-only.
						FGameplayAbilitySpec ServerOwnerAbilitySpec(ServerOwnerAbility, 1, INDEX_NONE, NewPlayer->PlayerState);
						ServerOwnerASC->GiveAbility(ServerOwnerAbilitySpec);
						ServerOwnerPlayerState = NewPlayer->PlayerState;
						UE_LOG(LogTemp, Warning, TEXT("Granted ability to server owner!"));
					}
				}
			}
		}

	}
}