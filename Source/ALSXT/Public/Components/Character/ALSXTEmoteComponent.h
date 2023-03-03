// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlsCharacter.h"
#include "ALSXTCharacter.h"
#include "NativeGameplayTags.h"
#include "ALSXTEmoteComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTEmoteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTEmoteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanEmote();

	// Desired Emote

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Emote", Meta = (AutoCreateRefTerm = "NewEmoteTag"))
		void AddDesiredEmote(const FGameplayTag& Emote);

private:
	UFUNCTION(Server, Reliable)
		void ServerAddDesiredEmote(const FGameplayTag& Emote);

	// Emote

private:
	void AddEmote(const FGameplayTag& Emote);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Emote")
		void OnEmote(const FGameplayTag& Emote);
};
