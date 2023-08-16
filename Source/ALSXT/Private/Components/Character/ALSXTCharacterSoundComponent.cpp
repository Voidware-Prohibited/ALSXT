// MIT

#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/ALSXTStructs.h"
#include "Interfaces/ALSXTCharacterInterface.h"

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

void UALSXTCharacterSoundComponent::StartTimeSinceLastCharacterMovementSoundTimer(const float Delay)
{
	ResetTimeSinceLastCharacterMovementSoundTimer();
	TimeSinceLastCharacterMovementSound = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastCharacterMovementSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastCharacterMovementSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastCharacterMovementSound()
{
	TimeSinceLastCharacterMovementSound = TimeSinceLastCharacterMovementSound + 0.1f;

	if (TimeSinceLastCharacterMovementSound >= CurrentCharacterMovementSoundDelay)
	{
		ResetTimeSinceLastCharacterMovementSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastCharacterMovementSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastCharacterMovementSoundTimer);
	CurrentCharacterMovementSoundDelay = 0.0f;
}

void UALSXTCharacterSoundComponent::StartTimeSinceLastActionSoundTimer(const float Delay)
{
	ResetTimeSinceLastActionSoundTimer();
	TargetActionSoundDelay = FMath::RandRange(GeneralCharacterSoundSettings.ActionSoundDelay.X, GeneralCharacterSoundSettings.ActionSoundDelay.Y);
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastActionSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastActionSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastActionSound()
{
	TimeSinceLastActionSound = TimeSinceLastActionSound + 0.1f;
	
	if (TimeSinceLastActionSound >= TargetActionSoundDelay)
	{
		ResetTimeSinceLastActionSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastActionSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastActionSoundTimer);
	TimeSinceLastActionSound = 0.0f;
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

FName UALSXTCharacterSoundComponent::GetSocketForMovement(const FGameplayTag MovementType)
{
	FName FoundSocketName;
	for (FMotionSoundAreaMap SoundSourcesForMotionsEntry : GeneralCharacterSoundSettings.SoundSourcesForMotions)
	{
		if (SoundSourcesForMotionsEntry.Motions.HasTag(MovementType))
		{
			FoundSocketName = SoundSourcesForMotionsEntry.MotionSoundBone;
		}
	}
	return FoundSocketName;
}

FGameplayTag UALSXTCharacterSoundComponent::ConvertWeightTagToStrengthTag(const FGameplayTag Weight)
{
	if (Weight == ALSXTObjectWeightTags::Stealth || Weight == ALSXTObjectWeightTags::VeryLight || Weight == ALSXTObjectWeightTags::Light)
	{
		return ALSXTActionStrengthTags::Light;
	}
	else if (Weight == ALSXTObjectWeightTags::Default)
	{
		return ALSXTActionStrengthTags::Medium;
	}
	else if (Weight == ALSXTObjectWeightTags::Heavy || Weight == ALSXTObjectWeightTags::VeryHeavy || Weight == ALSXTObjectWeightTags::MassivelyHeavy)
	{
		return ALSXTActionStrengthTags::Heavy;
	}
	else
	{
		return ALSXTActionStrengthTags::Medium;
	}
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

FALSXTCharacterMovementSound UALSXTCharacterSoundComponent::SelectCharacterMovementSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight)
{
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCharacterMovementSounds> MovementSoundsMap = SelectCharacterSoundSettings()->MovementSounds;
	TEnumAsByte<EPhysicalSurface> FoundSurface;
	IALSXTCharacterInterface::Execute_GetClothingSurfaceForMovement(Character, FoundSurface, Type);
	TArray<FALSXTCharacterMovementSound> Sounds = MovementSoundsMap.FindRef(FoundSurface).Sounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	TagsContainer.AddTag(Weight);
	
	TArray<FALSXTCharacterMovementSound> FilteredSounds;
	FALSXTCharacterMovementSound SelectedSound;

	// Return is there are no sounds
	if (Sounds.Num() < 1 || !Sounds[0].Sound.Sound.Sound)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "None");
		return SelectedSound;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTCharacterMovementSound Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Type);
		CurrentTagsContainer.AppendTags(Sound.Weight);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1 || !FilteredSounds[0].Sound.Sound.Sound)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "None2");
		return SelectedSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredSounds.Num() > 1)
	{
		// If FilteredSounds contains LastCharacterMovementSound, remove it from FilteredSounds array to avoid duplicates
		if (FilteredSounds.Contains(LastCharacterMovementSound))
		{
			int IndexToRemove = FilteredSounds.Find(LastCharacterMovementSound);
			FilteredSounds.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredSounds.Num() - 1));
		SelectedSound = FilteredSounds[RandIndex];
		LastCharacterMovementSound = SelectedSound;
		return SelectedSound;
	}
	else
	{
		SelectedSound = FilteredSounds[0];
		LastCharacterMovementSound = SelectedSound;
		return SelectedSound;
	}

	return SelectedSound;
}

FALSXTCharacterMovementSound UALSXTCharacterSoundComponent::SelectCharacterMovementAccentSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight)
{
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCharacterMovementSounds> MovementAccentSoundsMap = SelectCharacterSoundSettings()->MovementAccentSounds;
	TEnumAsByte<EPhysicalSurface> AccentSurface;
	IALSXTCharacterInterface::Execute_GetAccentSurfaceForMovement(Character, AccentSurface, Type);
	TArray<FALSXTCharacterMovementSound> Sounds = MovementAccentSoundsMap.FindRef(AccentSurface).Sounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	TagsContainer.AddTag(Weight);

	TArray<FALSXTCharacterMovementSound> FilteredSounds;
	FALSXTCharacterMovementSound SelectedSound;

	// Return is there are no sounds
	if (Sounds.Num() < 1 || !Sounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// Filter sounds based on Tag parameters
	for (auto Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Type);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1 || !FilteredSounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredSounds.Num() > 1)
	{
		// If FilteredSounds contains LastCharacterMovementSound, remove it from FilteredSounds array to avoid duplicates
		if (FilteredSounds.Contains(LastCharacterMovementSound))
		{
			int IndexToRemove = FilteredSounds.Find(LastCharacterMovementSound);
			FilteredSounds.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredSounds.Num() - 1));
		SelectedSound = FilteredSounds[RandIndex];
		LastCharacterMovementSound = SelectedSound;
		return SelectedSound;
	}
	else
	{
		SelectedSound = FilteredSounds[0];
		LastCharacterMovementSound = SelectedSound;
		return SelectedSound;
	}

	return SelectedSound;
}

FALSXTWeaponMovementSound UALSXTCharacterSoundComponent::SelectWeaponMovementSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type)
{
	TArray<FALSXTWeaponMovementSound> Sounds = SelectWeaponSoundSettings()->WeaponMovementSounds;
	TArray<FALSXTWeaponMovementSound> FilteredSounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	FALSXTWeaponMovementSound SelectedSound;

	// Return is there are no sounds
	if (Sounds.Num() < 1 || !Sounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// Filter sounds based on Tag parameters
	for (auto Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Type);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1 || !FilteredSounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredSounds.Num() > 1)
	{
		// If FilteredSounds contains LastWeaponMovementSound, remove it from FilteredSounds array to avoid duplicates
		if (FilteredSounds.Contains(LastWeaponMovementSound))
		{
			int IndexToRemove = FilteredSounds.Find(LastWeaponMovementSound);
			FilteredSounds.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredSounds.Num() - 1));
		SelectedSound = FilteredSounds[RandIndex];
		LastWeaponMovementSound = SelectedSound;
		return SelectedSound;
	}
	else
	{
		SelectedSound = FilteredSounds[0];
		LastWeaponMovementSound = SelectedSound;
		return SelectedSound;
	}

	return SelectedSound;
}

FALSXTWeaponActionSound UALSXTCharacterSoundComponent::SelectWeaponActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type)
{
	TArray<FALSXTWeaponActionSound> Sounds = SelectWeaponSoundSettings()->WeaponActionSounds;
	TArray<FALSXTWeaponActionSound> FilteredSounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	FALSXTWeaponActionSound SelectedSound;

	// Return is there are no sounds
	if (Sounds.Num() < 1 || !Sounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// Filter sounds based on Tag parameters
	for (auto Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Type);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1 || !FilteredSounds[0].Sound.Sound.Sound)
	{
		return SelectedSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredSounds.Num() > 1)
	{
		// If FilteredSounds contains LastWeaponActionSound, remove it from FilteredSounds array to avoid duplicates
		if (FilteredSounds.Contains(LastWeaponActionSound))
		{
			int IndexToRemove = FilteredSounds.Find(LastWeaponActionSound);
			FilteredSounds.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredSounds.Num() - 1));
		SelectedSound = FilteredSounds[RandIndex];
		LastWeaponActionSound = SelectedSound;
		return SelectedSound;
	}
	else
	{
		SelectedSound = FilteredSounds[0];
		LastWeaponActionSound = SelectedSound;
		return SelectedSound;
	}

	return SelectedSound;
}

FALSXTCharacterActionSound UALSXTCharacterSoundComponent::SelectActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTCharacterActionSound> ActionSounds = Settings->ActionSounds;
	TArray<FALSXTCharacterActionSound> FilteredActionSounds;
	FGameplayTag StaminaTag = ConvertStaminaToStaminaTag(Stamina);
	FALSXTCharacterActionSound SelectedActionSound;
	TagsContainer.AddTagFast(Sex);
	TagsContainer.AddTagFast(Variant);
	TagsContainer.AddTagFast(Overlay);
	TagsContainer.AddTagFast(Strength);
	TagsContainer.AddTagFast(StaminaTag);

	// Return is there are no sounds
	if (ActionSounds.Num() < 1 || !ActionSounds[0].CharacterSound.Sound.Sound)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Sounds");
		return SelectedActionSound;
	}
	
	// Filter sounds based on Tag parameters
	for (auto ActionSound : ActionSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(ActionSound.Sex);
		CurrentTagsContainer.AppendTags(ActionSound.Variant);
		CurrentTagsContainer.AppendTags(ActionSound.Overlay);
		CurrentTagsContainer.AppendTags(ActionSound.Strength);
		CurrentTagsContainer.AppendTags(ActionSound.Stamina);	
		
		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredActionSounds.Add(ActionSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredActionSounds.Num() < 1 || !FilteredActionSounds[0].CharacterSound.Sound.Sound)
	{
		return SelectedActionSound;
	}

	// If more than one result, avoid duplicates
	if (FilteredActionSounds.Num() > 1)
	{
		// If FilteredActionSounds contains LastCharacterActionSound, remove it from FilteredActionSounds array to avoid duplicates
		if (FilteredActionSounds.Contains(LastCharacterActionSound))
		{
			int IndexToRemove = FilteredActionSounds.Find(LastCharacterActionSound);
			FilteredActionSounds.RemoveAt(IndexToRemove, 1, true);
		}

		//Shuffle Array
		for (int m = FilteredActionSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredActionSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredActionSounds.Num() - 1));
		SelectedActionSound = FilteredActionSounds[RandIndex];
		LastCharacterActionSound = SelectedActionSound;
		return SelectedActionSound;
	}
	else
	{
		SelectedActionSound = FilteredActionSounds[0];
		LastCharacterActionSound = SelectedActionSound;
		return SelectedActionSound;
	}

	return SelectedActionSound;
}

FALSXTCharacterActionSound UALSXTCharacterSoundComponent::SelectAttackSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	FALSXTCharacterActionSound SelectedAttackSound;

	//..

	return SelectedAttackSound;
}

FALSXTCharacterDamageSound UALSXTCharacterSoundComponent::SelectDamageSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength)
{
	FALSXTCharacterDamageSound SelectedDamageSound;

	//..

	return SelectedDamageSound;
}

FALSXTCharacterDamageSound UALSXTCharacterSoundComponent::SelectDeathSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength)
{
	FALSXTCharacterDamageSound SelectedDeathSound;

	//..

	return SelectedDeathSound;
}

bool UALSXTCharacterSoundComponent::ShouldPlayActionSound(const FGameplayTag& Strength, const float Stamina)
{
	// return (TimeSinceLastActionSound >= TargetActionSoundDelay);
	return true;
}

bool UALSXTCharacterSoundComponent::ShouldPlayAttackSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const float Stamina)
{
	return true;
}

bool UALSXTCharacterSoundComponent::ShouldPlayDamageSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	return true;
}

bool UALSXTCharacterSoundComponent::ShouldPlayDeathSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	return true;
}

void UALSXTCharacterSoundComponent::PlayCharacterMovementSound(bool AccentSound, bool WeaponSound, const FGameplayTag& Type, const FGameplayTag& Weight)
{
	if (!CanPlayCharacterMovementSound())
	{
		return;
	}

	
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTCharacterMovementSound CharacterMovementSound = SelectCharacterMovementSound(Settings, Type, Weight);

	if (!IsValid(CharacterMovementSound.Sound.Sound.Sound))
	{
		return;
	}

	float Delay = FMath::RandRange(ActionSoundDelay.X, ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);

	FName CharacterMovementSoundSocket = GetSocketForMovement(Type);
	FVector CharacterMovementSoundLocation;
	FRotator CharacterMovementSoundRotation;
	Character->GetMesh()->GetSocketWorldLocationAndRotation(CharacterMovementSoundSocket, CharacterMovementSoundLocation, CharacterMovementSoundRotation);

	
	// MOVEMENT
	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(CharacterMovementSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(CharacterMovementSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
	}

	FGameplayTag MovementStrength = ConvertWeightTagToStrengthTag(Weight);
	
	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, MovementStrength) && AccentSound)
	{
		FALSXTCharacterMovementSound CharacterMovementAccentSound = SelectCharacterMovementAccentSound(Settings, Type, Weight);

		if (!IsValid(CharacterMovementAccentSound.Sound.Sound.Sound))
		{
			return;
		}

		FName CharacterMovementAccentSoundSocket = GetSocketForMovement(Type);
		FVector CharacterMovementAccentSoundLocation;
		FRotator CharacterMovementAccentSoundRotation;
		Character->GetMesh()->GetSocketWorldLocationAndRotation(CharacterMovementAccentSoundSocket, CharacterMovementSoundLocation, CharacterMovementSoundRotation);

		if (Character->GetLocalRole() >= ROLE_Authority)
		{
			MulticastPlaySound(CharacterMovementAccentSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
		}
		else
		{
			ServerPlaySound(CharacterMovementAccentSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, MovementStrength) && WeaponSound)
	{
		FALSXTWeaponMovementSound WeaponMovementSound = SelectWeaponMovementSound(Settings, Type);

		if (!IsValid(WeaponMovementSound.Sound.Sound.Sound))
		{
			return;
		}

		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		FVector WeaponMovementSoundLocation;
		FRotator WeaponMovementSoundRotation;
		Character->GetMesh()->GetSocketWorldLocationAndRotation(WeaponMovementSoundSocket, CharacterMovementSoundLocation, CharacterMovementSoundRotation);

		if (Character->GetLocalRole() >= ROLE_Authority)
		{
			MulticastPlaySound(WeaponMovementSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
		}
		else
		{
			ServerPlaySound(WeaponMovementSound.Sound, CharacterMovementSoundLocation, CharacterMovementSoundRotation, 1.0);
		}
	}

}

void UALSXTCharacterSoundComponent::PlayWeaponMovementSound(const FGameplayTag& Type)
{
	if (!CanPlayWeaponMovementSound())
	{
		return;
	}

	float Delay = FMath::RandRange(ActionSoundDelay.X, ActionSoundDelay.Y);
	// StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTWeaponMovementSound Sound = SelectWeaponMovementSound(Settings, Type);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	// Character->Mesh->GetSocketWorldLocationAndRotation(Socket, SoundLocation, SoundRotation);

	if (!IsValid(Sound.Sound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.Sound, SoundLocation, SoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(Sound.Sound, SoundLocation, SoundRotation, 1.0);
	}
}

void UALSXTCharacterSoundComponent::PlayWeaponActionSound(const FGameplayTag& Type)
{
	if (!CanPlayWeaponActionSound())
	{
		return;
	}

	float Delay = FMath::RandRange(ActionSoundDelay.X, ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTWeaponActionSound Sound = SelectWeaponActionSound(Settings, Type);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	// Character->Mesh->GetSocketWorldLocationAndRotation(Socket, SoundLocation, SoundRotation);

	if (!IsValid(Sound.Sound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.Sound, SoundLocation, SoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(Sound.Sound, SoundLocation, SoundRotation, 1.0);
	}
}

void UALSXTCharacterSoundComponent::PlayActionSound(const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{	
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	if (!CanPlayActionSound() || !ShouldPlayActionSound(Strength, Stamina))
	{
		return;
	}

	float Delay = FMath::RandRange(GeneralCharacterSoundSettings.ActionSoundDelay.X, GeneralCharacterSoundSettings.ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTCharacterActionSound ActionSound = SelectActionSound(Settings, Sex, Variant, Overlay, Strength, Stamina);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	Character->GetMesh()->GetSocketWorldLocationAndRotation(GeneralCharacterSoundSettings.VoiceSocketName, SoundLocation, SoundRotation);

	if (!IsValid(ActionSound.CharacterSound.Sound.Sound))
	{
		return;
	}


	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(ActionSound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(ActionSound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}


	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	MulticastPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	// }
	// else
	// {
	// 	ServerPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	// }
}

void UALSXTCharacterSoundComponent::PlayAttackSound(const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina) 
{
	if (!CanPlayAttackSound() || !ShouldPlayAttackSound(AttackMode, Strength, Stamina))
	{
		return;
	}

	float Delay = FMath::RandRange(AttackSoundDelay.X, AttackSoundDelay.Y);
	StartTimeSinceLastAttackSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTCharacterActionSound Sound = SelectAttackSound(Settings, Sex, Variant, Overlay, Strength, Stamina);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	// Character->Mesh->GetSocketWorldLocationAndRotation(Socket, SoundLocation, SoundRotation);

	if (!IsValid(Sound.CharacterSound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
}

void UALSXTCharacterSoundComponent::PlayDamageSound(const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	if (!CanPlayDamageSound() || !ShouldPlayDamageSound(AttackMethod, Strength, AttackForm, Damage))
	{
		return;
	}

	float Delay = FMath::RandRange(DamageSoundDelay.X, DamageSoundDelay.Y);
	StartTimeSinceLastDamageSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FALSXTCharacterDamageSound Sound = SelectDamageSound(Settings, Sex, Variant, Overlay, AttackForm, Strength);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	// Character->Mesh->GetSocketWorldLocationAndRotation(Socket, SoundLocation, SoundRotation);

	if (!IsValid(Sound.CharacterSound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
}

void UALSXTCharacterSoundComponent::PlayDeathSound(const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	if (!CanPlayDeathSound() || !ShouldPlayDeathSound(AttackMethod, Strength, AttackForm, Damage))
	{
		return;
	}

	float Delay = FMath::RandRange(DamageSoundDelay.X, DamageSoundDelay.Y);
	StartTimeSinceLastDamageSoundTimer(Delay);
	FALSXTCharacterDamageSound Sound = SelectDeathSound(Settings, Sex, Variant, Overlay, AttackForm, Strength);

	FName Socket;
	FVector SoundLocation;
	FRotator SoundRotation;
	// Character->Mesh->GetSocketWorldLocationAndRotation(Socket, SoundLocation, SoundRotation);

	if (!IsValid(Sound.CharacterSound.Sound.Sound))
	{
		return;
	}

	if (Character->GetLocalRole() >= ROLE_Authority)
	{
		MulticastPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
	else
	{
		ServerPlaySound(Sound.CharacterSound, SoundLocation, SoundRotation, 1.0);
	}
}

void UALSXTCharacterSoundComponent::PlaySound(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch)
{
	if (Sound.Sound.Sound)
	{
		UAudioComponent* AudioComponent{ nullptr };
		float PlayPitch = FMath::RandRange(Sound.PitchRange.X, Sound.PitchRange.Y);
		float PlayRate = FMath::RandRange(Sound.PlayRateRange.X, Sound.PlayRateRange.Y);
		FVector SocketLocation = Character->GetMesh()->GetSocketLocation(VoiceSocketName);
		FRotator PlayerRotation = Character->GetControlRotation();
		if (GetWorld()->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound.Sound.Sound, SocketLocation, PlayerRotation, 1.0f, PlayPitch);
		}
		else
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound.Sound.Sound, SocketLocation, PlayerRotation, 1.0f, PlayPitch);
		}
	}
}

void UALSXTCharacterSoundComponent::ServerPlaySound_Implementation(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch)
{
	MulticastPlaySound(Sound, Location, Rotation, Pitch);
}

void UALSXTCharacterSoundComponent::MulticastPlaySound_Implementation(FALSXTCharacterSound Sound, FVector Location, FRotator Rotation, float Pitch)
{
	PlaySound(Sound, Location, Rotation, Pitch);
}