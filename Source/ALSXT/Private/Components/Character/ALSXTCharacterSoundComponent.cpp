// MIT

#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UALSXTCharacterSoundComponent::UALSXTCharacterSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UALSXTCharacterSoundComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
}


void UALSXTCharacterSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UALSXTCharacterSoundComponent::StartTimeSinceLastActionSoundTimer(const float Delay)
{
	ResetTimeSinceLastActionSoundTimer();
	TimeSinceLastActionSound = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastActionSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastActionSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastActionSound()
{
	TimeSinceLastActionSound = TimeSinceLastActionSound + 0.1f;
	
	if (TimeSinceLastActionSound >= CurrentActionSoundDelay)
	{
		ResetTimeSinceLastActionSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastActionSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastActionSoundTimer);
	CurrentActionSoundDelay = 0.0f;
}

void UALSXTCharacterSoundComponent::StartTimeSinceLastAttackSoundTimer(const float Delay)
{
	ResetTimeSinceLastAttackSoundTimer();
	TimeSinceLastAttackSound = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastAttackSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastAttackSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastAttackSound()
{
	TimeSinceLastAttackSound = TimeSinceLastAttackSound + 0.1f;

	if (TimeSinceLastAttackSound >= CurrentAttackSoundDelay)
	{
		ResetTimeSinceLastAttackSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastAttackSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastAttackSoundTimer);
	CurrentAttackSoundDelay = 0.0f;
}

void UALSXTCharacterSoundComponent::StartTimeSinceLastDamageSoundTimer(const float Delay)
{
	ResetTimeSinceLastDamageSoundTimer();
	TimeSinceLastDamageSound = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastDamageSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastDamageSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastDamageSound()
{
	TimeSinceLastDamageSound = TimeSinceLastDamageSound + 0.1f;

	if (TimeSinceLastDamageSound >= CurrentDamageSoundDelay)
	{
		ResetTimeSinceLastDamageSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastDamageSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastDamageSoundTimer);
	CurrentDamageSoundDelay = 0.0f;
}

FGameplayTag UALSXTCharacterSoundComponent::ConvertStaminaToStaminaTag(const float Stamina)
{
	if (Stamina >= 0.75)
	{
		return ALSXTStaminaTags::Optimal;
	}
	else if ((Stamina <= 0.5 && Stamina > .25))
	{
		return ALSXTStaminaTags::Half;
	}
	else if ((Stamina <= 0.25 && Stamina > .15))
	{
		return ALSXTStaminaTags::Low;
	}
	else if ((Stamina <= .15))
	{
		return ALSXTStaminaTags::Empty;
	}
	else
	{
		return ALSXTStaminaTags::Empty;
	}
}

FALSXTCharacterActionSound UALSXTCharacterSoundComponent::SelectActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	TArray<FALSXTCharacterActionSound> ActionSounds = Settings->ActionSounds;
	TArray<FALSXTCharacterActionSound> FilteredActionSounds = Settings->ActionSounds;
	FGameplayTag StaminaTag = ConvertStaminaToStaminaTag(Stamina);
	TArray<FGameplayTag> TagsArray = {Strength, StaminaTag};
	FGameplayTagContainer TagsContainer = FGameplayTagContainer::CreateFromArray(TagsArray);
	FALSXTCharacterActionSound SelectedActionSound;

	// Return is there are no sounds
	if (ActionSounds.Num() < 1 || !ActionSounds[0].Sound.Sound.Sound)
	{
		return SelectedActionSound;
	}
	
	// Filter sounds based on Tag parameters
	for (auto ActionSound : ActionSounds)
	{
		TArray<FGameplayTag> CurrentTagsArray;
		CurrentTagsArray.Append(ActionSound.Strength);
		CurrentTagsArray.Append(ActionSound.Stamina);
		FGameplayTagContainer CurrentTagsContainer = FGameplayTagContainer::CreateFromArray(CurrentTagsArray);
		
		if (CurrentTagsContainer.HasAllExact(TagsContainer))
		{
			FilteredActionSounds.Add(ActionSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredActionSounds.Num() < 1 || !FilteredActionSounds[0].Sound.Sound.Sound)
	{
		return SelectedActionSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredActionSounds.Num() > 1)
	{
		// If FilteredActionSounds contains LastCharacterActionSound, remove it from FilteredActionSounds array to avoid duplicates
		if (FilteredActionSounds.Contains(LastCharacterActionSound))
		{
			FilteredActionSounds.Remove(LastCharacterActionSound);
		}

		//Shuffle Array
		for (int m = FilteredActionSounds.Max(); m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredActionSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = rand() % FilteredActionSounds.Max();
		SelectedActionSound = FilteredActionSounds[RandIndex];
		return SelectedActionSound;
	}
	else
	{
		SelectedActionSound = FilteredActionSounds[0];
		return SelectedActionSound;
	}

	return SelectedActionSound;
}

FALSXTCharacterActionSound UALSXTCharacterSoundComponent::SelectAttackSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	FALSXTCharacterActionSound SelectedAttackSound;

	//..

	return SelectedAttackSound;
}

FALSXTCharacterDamageSound UALSXTCharacterSoundComponent::SelectDamageSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Form, const float Damage)
{
	FALSXTCharacterDamageSound SelectedDamageSound;

	//..

	return SelectedDamageSound;
}

FALSXTCharacterDamageSound UALSXTCharacterSoundComponent::SelectDeathSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Form, const float Damage)
{
	FALSXTCharacterDamageSound SelectedDeathSound;

	//..

	return SelectedDeathSound;
}

bool UALSXTCharacterSoundComponent::ShouldPlayActionSound(const FGameplayTag& Strength, const float Stamina)
{
		return (TimeSinceLastActionSound >= CurrentActionSoundDelay);
}

bool UALSXTCharacterSoundComponent::ShouldPlayAttackSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const float Stamina)
{
	return true;
}

bool UALSXTCharacterSoundComponent::ShouldPlayDamageSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	return true;
}

void UALSXTCharacterSoundComponent::PlayActionSound(const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{	
	if (!CanPlayActionSound() || !ShouldPlayActionSound(Strength, Stamina))
	{
		return;
	}

	float Delay = FMath::RandRange(ActionSoundDelay.X, ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTCharacterActionSound Sound = SelectActionSound(Settings, Overlay, Strength, Stamina);

	if (!IsValid(Sound.Sound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.Sound);
	}
	else
	{
		ServerPlaySound(Sound.Sound);
	}
}

void UALSXTCharacterSoundComponent::PlayAttackSound(const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina) {}

void UALSXTCharacterSoundComponent::PlayDamageSound(const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage) {}

void UALSXTCharacterSoundComponent::PlayDeathSound(const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage) {}

void UALSXTCharacterSoundComponent::PlaySound(FALSXTCharacterSound Sound)
{
	if (Sound.Sound.Sound)
	{
		UAudioComponent* AudioComponent{ nullptr };
		FVector SocketLocation = Character->GetMesh()->GetSocketLocation(VoiceSocketName);
		FRotator PlayerRotation = Character->GetControlRotation();
		if (GetWorld()->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound.Sound.Sound, SocketLocation, 1.0f, 1.0f);
		}
		else
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound.Sound.Sound, SocketLocation, PlayerRotation, 1.0f, 1.0f);
		}
	}
}

void UALSXTCharacterSoundComponent::ServerPlaySound_Implementation(FALSXTCharacterSound Sound) 
{
	MulticastPlaySound(Sound);
}

void UALSXTCharacterSoundComponent::MulticastPlaySound_Implementation(FALSXTCharacterSound Sound)
{
	PlaySound(Sound);
}