// MIT

#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/ALSXTStructs.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTCharacterSoundComponentInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UALSXTCharacterSoundComponent::UALSXTCharacterSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UALSXTCharacterSoundComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, VocalizationMixerAudioComponent, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CharacterMovementSoundMixer, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, WeaponMovementAudioComponent, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, WeaponActionAudioComponent, Parameters)
}


void UALSXTCharacterSoundComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AALSXTCharacter>(GetOuter());
	AlsCharacter = Cast<AAlsCharacter>(GetOuter());
	IALSXTCharacterInterface::Execute_GetStaminaThresholds(GetOuter(), StaminaOptimalThreshold, StaminaLowThreshold);
	BreathParticleSettings = Character->ALSXTSettings->BreathEffects;
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	CurrentBreathType = IALSXTCharacterInterface::Execute_GetBreathType(GetOwner());

	if (IsValid(Settings))
	{
		SpawnAudioComponent(VocalizationMixerAudioComponent, Settings->VocalizationMixer, Character->GetMesh(), Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName), Character->GetMesh()->GetSocketRotation(GeneralCharacterSoundSettings.VoiceSocketName), 1.0f, GeneralCharacterSoundSettings.VoiceSocketName);

		// Setup Delegate for each time a vocalization plays
		// VocalizationMixerAudioComponent->OnAudioPlayStateChanged
	}
}


void UALSXTCharacterSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(VocalizationMixerAudioComponent))
	{
		CurrentBreathType = IALSXTCharacterInterface::Execute_GetBreathType(GetOwner());		
	}
}

void UALSXTCharacterSoundComponent::UpdateStaminaThresholds()
{
	float NewStaminaOptimalThreshold;
	float NewStaminaLowThreshold;
	IALSXTCharacterInterface::Execute_GetStaminaThresholds(GetOwner(), NewStaminaOptimalThreshold, NewStaminaLowThreshold);
	(NewStaminaOptimalThreshold != StaminaOptimalThreshold) ? StaminaOptimalThreshold = NewStaminaOptimalThreshold : StaminaOptimalThreshold;
	(NewStaminaLowThreshold != StaminaLowThreshold) ? StaminaLowThreshold = NewStaminaLowThreshold : StaminaLowThreshold;
}

void UALSXTCharacterSoundComponent::UpdateStamina(bool& StaminaTagChanged)
{
	float NewStamina = IALSXTCharacterInterface::Execute_GetStamina(GetOwner());
	bool bStaminaChanged { CurrentStamina != NewStamina };
	(bStaminaChanged) ? CurrentStamina = NewStamina : CurrentStamina;
	FGameplayTag NewStaminaTag{ConvertStaminaToStaminaTag(NewStamina)};
	(NewStaminaTag != CurrentStaminaTag) ? CurrentStaminaTag = NewStaminaTag, StaminaTagChanged = true : StaminaTagChanged = false;

	if (bStaminaChanged && IsValid(VocalizationMixerAudioComponent))
	{
		VocalizationMixerAudioComponent->SetFloatParameter("Stamina", CurrentStamina);
	}
}

void UALSXTCharacterSoundComponent::UpdateVoiceSocketLocation()
{
	VoiceSocketLocation = Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName);
}

void UALSXTCharacterSoundComponent::UpdateVoiceSocketRotation()
{
	VoiceSocketRotation = Character->GetMesh()->GetSocketRotation(GeneralCharacterSoundSettings.VoiceSocketName);
}

void UALSXTCharacterSoundComponent::DetermineNewSound(TArray<FSound> Sounds, TArray<UObject*> PreviousAssetsReferences, FSound& ResultSound)
{
	FSound NewSoundResult;
	UObject* NewAssetResult {nullptr};
	TArray<FSound> FilteredSounds;
	
	// 	// Return if Return is there are no filtered sounds
	if (Sounds.Num() < 1)
	{
		return;
	}

	// If more than one result, avoid duplicates
	if (Sounds.Num() > 1)
	{
		
		for (FSound Sound : Sounds)
		{		
			if (!PreviousAssetsReferences.Contains(Sound.Sound))
			{
				FilteredSounds.Add(Sound);
			}
		}
		
		//Shuffle Array
		for (int m = FilteredSounds.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredSounds.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredSounds.Num() - 1));
		NewSoundResult = FilteredSounds[RandIndex];
		NewAssetResult = NewSoundResult.Sound;
		ResultSound = NewSoundResult;
	}
	else
	{
		NewSoundResult = FilteredSounds[0];
		NewAssetResult = NewSoundResult.Sound;
		ResultSound = NewSoundResult;
	}
}

void UALSXTCharacterSoundComponent::SetNewSound(UObject* Sound, TArray<UObject*> PreviousAssetsReferences, int NoRepeats)
{
	if (PreviousAssetsReferences.Num() >= abs(NoRepeats))
	{
		PreviousAssetsReferences.Add(Sound);
		PreviousAssetsReferences.RemoveAt(0, 1, true);
	}
	else
	{
		PreviousAssetsReferences.Add(Sound);
	}
}

void UALSXTCharacterSoundComponent::PlayCharacterBreathEffects(const FGameplayTag& StaminaOverride)
{	
	if (IsValid(VocalizationMixerAudioComponent))
	{	
		UpdateStaminaThresholds();
		bool StaminaTagChanged;
		UpdateStamina(StaminaTagChanged);
		FGameplayTag StaminaToUse{ (StaminaOverride != FGameplayTag::EmptyTag) ? StaminaOverride : CurrentStaminaTag};
		UpdateVoiceSocketLocation();

		// If New
		if (StaminaTagChanged || StaminaToUse != CurrentStaminaTag || CurrentBreathSounds.IsEmpty())
		{				
			if (CanPlayBreathSound() && ShouldPlayBreathSound() && IsValid(VocalizationMixerAudioComponent))
			{
				// Get and Set New Sounds
				UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
				CurrentBreathSounds = SelectBreathSoundsNew(Settings, Character->GetDesiredSex(), ALSXTVoiceVariantTags::Default, ALSXTBreathTypeTags::Regular, StaminaOverride);
				TArray<FSound> Sounds;

				for (FALSXTBreathSound BS : CurrentBreathSounds)
				{
					Sounds.Append(BS.Sounds);
				}

				if (Sounds.IsValidIndex(0))
				{
					FSound NewBreathSound;
					DetermineNewSound(Sounds, PreviousBreathsAssets, NewBreathSound);
					float Pitch = FMath::RandRange(NewBreathSound.PitchRange.X, NewBreathSound.PitchRange.Y);
					CurrentBreathSound = NewBreathSound;
					SetNewSound(NewBreathSound.Sound, PreviousBreathsAssets, GeneralCharacterSoundSettings.BreathNoRepeats);
					VocalizationMixerAudioComponent->SetObjectParameter("BreathSound", NewBreathSound.Sound);
					VocalizationMixerAudioComponent->SetFloatParameter("Pitch", Pitch);

					if (Character->GetRemoteRole() == ROLE_Authority)
					{
						VocalizationMixerAudioComponent->SetBoolParameter("Player", true);
					}

					VocalizationMixerAudioComponent->SetTriggerParameter("BreathInput");
					Character->MakeNoise(1.0, Character, Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName), 10, "Vocal");
					OnVocalization.Broadcast(NewBreathSound);
					//OnVocalization(NewBreathSound);
				}
			}
			if (BreathParticleSettings.VisibleBreathTypes.HasTag(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner())))
			{
				UE_LOG(LogTemp, Warning, TEXT("SelectNewBreathParticles"));
				CurrentBreathParticles = SelectBreathParticles(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner()), StaminaToUse);
				if (CurrentBreathParticles.IsValidIndex(0))
				{
					UpdateVoiceSocketRotation();
					UNiagaraSystem* NiagaraSystem { (CurrentBreathParticles.Num() > 1)? DetermineNewBreathParticle() : CurrentBreathParticles [0]};
					ServerPlayBreathParticle(NiagaraSystem);
				}
			}
		}
		else // If Same
		{
			
			if (CanPlayBreathSound() && ShouldPlayBreathSound() && IsValid(VocalizationMixerAudioComponent))
			{
				TArray<FSound> Sounds;

				for (FALSXTBreathSound BS : CurrentBreathSounds)
				{
					Sounds.Append(BS.Sounds);
				}

				if (Sounds.IsValidIndex(0))
				{
					FSound NewBreathSound;
					DetermineNewSound(Sounds, PreviousBreathsAssets, NewBreathSound);
					float Pitch = FMath::RandRange(NewBreathSound.PitchRange.X, NewBreathSound.PitchRange.Y);
					CurrentBreathSound = NewBreathSound;
					SetNewSound(NewBreathSound.Sound, PreviousBreathsAssets, GeneralCharacterSoundSettings.BreathNoRepeats);
					VocalizationMixerAudioComponent->SetObjectParameter("BreathSound", NewBreathSound.Sound);
					VocalizationMixerAudioComponent->SetFloatParameter("Pitch", Pitch);

					if (Character->GetRemoteRole() == ROLE_Authority)
					{
						VocalizationMixerAudioComponent->SetBoolParameter("Player", true);
					}

					VocalizationMixerAudioComponent->SetTriggerParameter("BreathInput");
					Character->MakeNoise(1.0, Character, VoiceSocketLocation, 10, "Vocal");
					// OnVocalization(NewBreathSound);
					OnVocalization.Broadcast(NewBreathSound);
				}
			}
			if (BreathParticleSettings.VisibleBreathTypes.HasTag(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner())))
			{
				if (CurrentBreathParticles.IsValidIndex(0))
				{
					UpdateVoiceSocketRotation();
					UE_LOG(LogTemp, Warning, TEXT("SelectFromExistingBreathParticles"));
					UNiagaraSystem* NiagaraSystem{ (CurrentBreathParticles.Num() > 1) ? DetermineNewBreathParticle() : CurrentBreathParticles[0] };
					ServerPlayBreathParticle(NiagaraSystem);
				}
			}
	
		}
	}
}

void UALSXTCharacterSoundComponent::ServerPlayBreathParticle_Implementation(UNiagaraSystem* NiagaraSystem)
{
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, Character->GetMesh(), GeneralCharacterSoundSettings.VoiceSocketName, Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName), Character->GetMesh()->GetSocketRotation(GeneralCharacterSoundSettings.VoiceSocketName), EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
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

void UALSXTCharacterSoundComponent::StartTimeSinceLastWeaponMovementSoundTimer(const float Delay)
{
	ResetTimeSinceLastWeaponMovementSoundTimer();
	TimeSinceLastWeaponMovementSound = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastWeaponMovementSoundTimer, this, &UALSXTCharacterSoundComponent::IncrementTimeSinceLastWeaponMovementSound, Delay, false);
}

void UALSXTCharacterSoundComponent::IncrementTimeSinceLastWeaponMovementSound()
{
	TimeSinceLastWeaponMovementSound = TimeSinceLastWeaponMovementSound + 0.1f;

	if (TimeSinceLastWeaponMovementSound >= CurrentWeaponMovementSoundDelay)
	{
		ResetTimeSinceLastWeaponMovementSoundTimer();
	}
}

void UALSXTCharacterSoundComponent::ResetTimeSinceLastWeaponMovementSoundTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeSinceLastWeaponMovementSoundTimer);
	CurrentWeaponMovementSoundDelay = 0.0f;
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
	if (Stamina >= GeneralCharacterSoundSettings.StaminaOptimalThreshold)
	{
		return ALSXTStaminaTags::Optimal;
	}
	else if ((Stamina <= GeneralCharacterSoundSettings.StaminaHalfPoint && Stamina > GeneralCharacterSoundSettings.StaminaLowThreshold))
	{
		return ALSXTStaminaTags::Half;
	}
	else if ((Stamina <= GeneralCharacterSoundSettings.StaminaLowThreshold && Stamina > .15))
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

TArray<FALSXTBreathSound> UALSXTCharacterSoundComponent::SelectBreathSoundsNew(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& BreathType, const FGameplayTag& Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTBreathSound> BreathSounds = Settings->BreathSounds;
	TArray<FALSXTBreathSound> FilteredBreathSounds;
	TagsContainer.AddTag(Sex);
	TagsContainer.AddTag(Variant);
	TagsContainer.AddTag(BreathType);
	TagsContainer.AddTag(Stamina);

	// Return if there are no sounds
	if (BreathSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Breath Sounds");
		return FilteredBreathSounds;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTBreathSound BreathSound : BreathSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(BreathSound.Sex);
		CurrentTagsContainer.AppendTags(BreathSound.Variant);
		CurrentTagsContainer.AppendTags(BreathSound.BreathType);
		CurrentTagsContainer.AppendTags(BreathSound.Stamina);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredBreathSounds.Add(BreathSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredBreathSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Filtered Breath Sounds");
		return FilteredBreathSounds;
	}

	return FilteredBreathSounds;
}

TArray<UNiagaraSystem*> UALSXTCharacterSoundComponent::SelectBreathParticles(const FGameplayTag& BreathType, const FGameplayTag& Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTBreathParticle> BreathParticles = BreathParticleSettings.BreathParticles;
	TArray<UNiagaraSystem*> FilteredBreathParticles;
	TagsContainer.AddTag(BreathType);
	TagsContainer.AddTag(Stamina);

	// Return if there are no sounds
	if (BreathParticles.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Breath Sounds");
		return FilteredBreathParticles;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTBreathParticle BreathParticle : BreathParticles)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(BreathParticle.BreathType);
		CurrentTagsContainer.AppendTags(BreathParticle.Stamina);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredBreathParticles.Append(BreathParticle.Particles);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredBreathParticles.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Filtered Breath Sounds");
		return FilteredBreathParticles;
	}

	return FilteredBreathParticles;
}

UNiagaraSystem* UALSXTCharacterSoundComponent::DetermineNewBreathParticle()
{
	if (CurrentBreathParticles.IsValidIndex(0))
	{
		if (CurrentBreathParticles.Num() < 1)
		{
			return CurrentBreathParticles[0];
		}
		else
		{
			TArray<UNiagaraSystem*> FilteredParticles = CurrentBreathParticles;
			UNiagaraSystem* SelectedParticle{ nullptr };
			if (FilteredParticles.Contains(LastBreathParticle))
			{
				int IndexToRemove = FilteredParticles.Find(LastBreathParticle);
				FilteredParticles.RemoveAt(IndexToRemove, 1, true);
			}

			//Shuffle Array
			for (int m = FilteredParticles.Num() - 1; m >= 0; --m)
			{
				int n = FMath::Rand() % (m + 1);
				if (m != n) FilteredParticles.Swap(m, n);
			}

			// Select Random Array Entry
			int RandIndex = FMath::RandRange(0, (FilteredParticles.Num() - 1));
			SelectedParticle = FilteredParticles[RandIndex];
			LastBreathParticle = SelectedParticle;
			return SelectedParticle;
		}
	}
	else
	{
		return nullptr;
	}
}

TArray<FALSXTBreathSound> UALSXTCharacterSoundComponent::SelectBreathSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& BreathType, const float Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTBreathSound> BreathSounds = Settings->BreathSounds;
	TArray<FALSXTBreathSound> FilteredBreathSounds;
	FGameplayTag StaminaTag = ConvertStaminaToStaminaTag(Stamina);
	TagsContainer.AddTag(Sex);
	TagsContainer.AddTag(Variant);
	TagsContainer.AddTag(BreathType);
	TagsContainer.AddTag(StaminaTag);

	// Return if there are no sounds
	if (BreathSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Breath Sounds");
		return FilteredBreathSounds;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTBreathSound BreathSound : BreathSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(BreathSound.Sex);
		CurrentTagsContainer.AppendTags(BreathSound.Variant);
		CurrentTagsContainer.AppendTags(BreathSound.BreathType);
		CurrentTagsContainer.AppendTags(BreathSound.Stamina);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredBreathSounds.Add(BreathSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredBreathSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Filtered Breath Sounds");
		return FilteredBreathSounds;
	}

	return FilteredBreathSounds;
}

TArray<FALSXTCharacterMovementSound> UALSXTCharacterSoundComponent::SelectCharacterMovementSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight)
{
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCharacterMovementSounds> MovementSoundsMap = SelectCharacterSoundSettings()->MovementSounds;
	TEnumAsByte<EPhysicalSurface> FoundSurface;
	IALSXTCharacterInterface::Execute_GetClothingSurfaceForMovement(Character, FoundSurface, Type);
	TArray<FALSXTCharacterMovementSound> Sounds = MovementSoundsMap.FindRef(FoundSurface).Sounds;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UKismetSystemLibrary::GetDisplayName(Sounds[0].Sound.Sound.Sound));
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	TagsContainer.AddTag(Weight);

	TArray<FALSXTCharacterMovementSound> FilteredSounds;

	// Return if there are no sounds
	if (Sounds.Num() < 1)
	{
		return FilteredSounds;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTCharacterMovementSound Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Type);
		CurrentTagsContainer.AppendTags(Sound.Weight);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UKismetSystemLibrary::GetDisplayName(Sound.Sound.Sound.Sound));
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1)
	{
		return FilteredSounds;
	}

	return FilteredSounds;
}

TArray<FALSXTCharacterMovementSound> UALSXTCharacterSoundComponent::SelectCharacterMovementAccentSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type, const FGameplayTag& Weight)
{
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTCharacterMovementSounds> MovementAccentSoundsMap = SelectCharacterSoundSettings()->MovementAccentSounds;
	TEnumAsByte<EPhysicalSurface> AccentSurface;
	IALSXTCharacterInterface::Execute_GetAccentSurfaceForMovement(Character, AccentSurface, Type);
	TArray<FALSXTCharacterMovementSound> Sounds = MovementAccentSoundsMap.FindRef(AccentSurface).Sounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	TagsContainer.AddTag(Weight);

	TArray<FALSXTCharacterMovementSound> FilteredSounds;

	// Return if there are no sounds
	if (Sounds.Num() < 1)
	{
		return FilteredSounds;
	}

	// Filter sounds based on Tag parameters
	for (auto Sound : Sounds)
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
	if (FilteredSounds.Num() < 1)
	{
		return FilteredSounds;
	}

	return FilteredSounds;
}

TArray<FALSXTWeaponMovementSound> UALSXTCharacterSoundComponent::SelectWeaponMovementSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Weapon, const FGameplayTag& Type)
{
	TArray<FALSXTWeaponMovementSound> Sounds = SelectWeaponSoundSettings()->WeaponMovementSounds;
	TArray<FALSXTWeaponMovementSound> FilteredSounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Weapon);
	TagsContainer.AddTag(Type);

	// Return if there are no sounds
	if (Sounds.Num() < 1)
	{
		return FilteredSounds;
	}

	// Filter sounds based on Tag parameters
	for (auto Sound : Sounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Sound.Weapon);
		CurrentTagsContainer.AppendTags(Sound.Type);
		// FString DebugText0 = TagsContainer.ToString();
		// FString DebugText3 = CurrentTagsContainer.ToString();
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, DebugText0);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DebugText3);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredSounds.Add(Sound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "No Filtered WeaponMovementSound");
		return FilteredSounds;
	}

	return FilteredSounds;
}

FALSXTWeaponActionSound UALSXTCharacterSoundComponent::SelectWeaponActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Type)
{
	TArray<FALSXTWeaponActionSound> Sounds = SelectWeaponSoundSettings()->WeaponActionSounds;
	TArray<FALSXTWeaponActionSound> FilteredSounds;
	FGameplayTagContainer TagsContainer;
	TagsContainer.AddTag(Type);
	FALSXTWeaponActionSound SelectedSound;

	// Return if there are no sounds
	if (Sounds.Num() < 1)
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
	if (FilteredSounds.Num() < 1)
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

TArray<FALSXTCharacterActionSound> UALSXTCharacterSoundComponent::SelectActionSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTCharacterActionSound> ActionSounds = Settings->ActionSounds;
	TArray<FALSXTCharacterActionSound> FilteredActionSounds;
	FGameplayTag StaminaTag = ConvertStaminaToStaminaTag(Stamina);
	TagsContainer.AddTag(Sex);
	TagsContainer.AddTag(Variant);
	TagsContainer.AddTag(Overlay);
	TagsContainer.AddTag(Strength);
	TagsContainer.AddTag(StaminaTag);

	// Return if there are no sounds
	if (ActionSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Sounds");
		return FilteredActionSounds;
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
	if (FilteredActionSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Filtered ActionSounds");
		return FilteredActionSounds;
	}

	return FilteredActionSounds;
}

TArray<FALSXTCharacterActionSound> UALSXTCharacterSoundComponent::SelectAttackSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTCharacterActionSound> AttackSounds = Settings->AttackSounds;
	TArray<FALSXTCharacterActionSound> FilteredAttackSounds;
	FGameplayTag StaminaTag = ConvertStaminaToStaminaTag(Stamina);
	TagsContainer.AddTagFast(Sex);
	TagsContainer.AddTagFast(Variant);
	TagsContainer.AddTagFast(Overlay);
	TagsContainer.AddTagFast(Strength);
	TagsContainer.AddTagFast(StaminaTag);

	// Return if there are no sounds
	if (AttackSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Sounds");
		return FilteredAttackSounds;
	}

	// Filter sounds based on Tag parameters
	for (auto AttackSound : AttackSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(AttackSound.Sex);
		CurrentTagsContainer.AppendTags(AttackSound.Variant);
		CurrentTagsContainer.AppendTags(AttackSound.Overlay);
		CurrentTagsContainer.AppendTags(AttackSound.Strength);
		CurrentTagsContainer.AppendTags(AttackSound.Stamina);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredAttackSounds.Add(AttackSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredAttackSounds.Num() < 1)
	{
		return FilteredAttackSounds;
	}

	return FilteredAttackSounds;
}

TArray<FALSXTCharacterDamageSound> UALSXTCharacterSoundComponent::SelectDamageSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Form, const FGameplayTag& Strength)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTCharacterDamageSound> DamageSounds = Settings->DamageSounds;
	TArray<FALSXTCharacterDamageSound> FilteredDamageSounds;
	TagsContainer.AddTagFast(Sex);
	TagsContainer.AddTagFast(Variant);
	TagsContainer.AddTagFast(AttackMethod);
	// TagsContainer.AddTagFast(Overlay);
	TagsContainer.AddTagFast(Form);
	FGameplayTag DamageTag = ALSXTDamageAmountTags::Moderate;
	TagsContainer.AddTagFast(DamageTag);

	// Return if there are no sounds
	if (DamageSounds.Num() < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Damage Sounds");
		return FilteredDamageSounds;
	}

	// Filter sounds based on Tag parameters
	for (FALSXTCharacterDamageSound DamageSound : DamageSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(DamageSound.Sex);
		CurrentTagsContainer.AppendTags(DamageSound.Variant);
		CurrentTagsContainer.AppendTags(DamageSound.AttackMethod);
		CurrentTagsContainer.AppendTags(DamageSound.Form);
		CurrentTagsContainer.AppendTags(DamageSound.Damage);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredDamageSounds.Add(DamageSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredDamageSounds.Num() < 0 )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Filtered Damage Sounds");
		return FilteredDamageSounds;
	}

	return FilteredDamageSounds;

}

TArray<FALSXTCharacterDamageSound> UALSXTCharacterSoundComponent::SelectDeathSounds(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Form, const FGameplayTag& Strength)
{
	FGameplayTagContainer TagsContainer;
	TArray<FALSXTCharacterDamageSound> DeathSounds = Settings->DeathSounds;
	TArray<FALSXTCharacterDamageSound> FilteredDeathSounds;
	FALSXTCharacterDamageSound SelectedDeathSound;
	TagsContainer.AddTagFast(Sex);
	TagsContainer.AddTagFast(Variant);
	TagsContainer.AddTagFast(Overlay);
	TagsContainer.AddTagFast(Form);
	FGameplayTag DeathTag = ALSXTDamageAmountTags::Moderate;
	TagsContainer.AddTagFast(DeathTag);

	// Return if there are no sounds
	if (DeathSounds.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No Sounds");
		return FilteredDeathSounds;
	}

	// Filter sounds based on Tag parameters
	for (auto DeathSound : DeathSounds)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(DeathSound.Sex);
		CurrentTagsContainer.AppendTags(DeathSound.Variant);
		CurrentTagsContainer.AppendTags(DeathSound.Damage);
		CurrentTagsContainer.AppendTags(DeathSound.Form);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			FilteredDeathSounds.Add(DeathSound);
		}
	}

	// Return if Return is there are no filtered sounds
	if (FilteredDeathSounds.Num() < 1)
	{
		return FilteredDeathSounds;
	}

	return FilteredDeathSounds;
}

bool UALSXTCharacterSoundComponent::ShouldPlayBreathSound()
{
	if (IsValid(VocalizationMixerAudioComponent))
	{
		float Stamina = IALSXTCharacterInterface::Execute_GetStamina(GetOwner());
		bool IsPaused = VocalizationMixerAudioComponent->bIsPaused;
		// if (!IsPaused && Stamina < 0.75)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ShouldPlayBreathSound:true");
		// }
		return !IsPaused && BreathParticleSettings.AudibleBreathStaminaLevels.HasTag(ConvertStaminaToStaminaTag(Stamina));
		// return !IsPaused && Stamina < StaminaOptimalThreshold;
	}
	else
	{
		return false;
	}
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
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	FMotionSounds MotionSounds;
	float Delay = FMath::RandRange(GeneralCharacterSoundSettings.CharacterMovementSoundDelay.X, GeneralCharacterSoundSettings.CharacterMovementSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FGameplayTag MovementStrength = ConvertWeightTagToStrengthTag(Weight);

	// MOVEMENT
	if (CanPlayCharacterMovementSound())
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementSounds = SelectCharacterMovementSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementSounds = Sounds;
		}
	}

	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, MovementStrength) && AccentSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementAccentSounds = SelectCharacterMovementAccentSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementAccentSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, MovementStrength) && WeaponSound)
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	// V2
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}


	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	MulticastPlaySound(MotionSounds);
	// }
	// else
	// {
	// 	ServerPlaySound(MotionSounds);
	// }

}

void UALSXTCharacterSoundComponent::PlayWeaponMovementSound(const FGameplayTag& Weapon, const FGameplayTag& Type)
{
	FGameplayTag Weight = IALSXTCharacterInterface::Execute_GetWeightTag(Character);
	FGameplayTag Strength = ConvertWeightTagToStrengthTag(Weight);
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	if (!CanPlayWeaponMovementSound())
	{
		return;
	}
	float Delay = FMath::RandRange(GeneralCharacterSoundSettings.WeaponMovementSoundDelay.X, GeneralCharacterSoundSettings.WeaponMovementSoundDelay.Y);
	StartTimeSinceLastWeaponMovementSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FMotionSounds MotionSounds;

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, Strength))
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.WeaponMovementSounds = Sounds;
		}
	}

	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}
}

void UALSXTCharacterSoundComponent::PlayWeaponActionSound(const FGameplayTag& Type)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	if (!CanPlayWeaponActionSound())
	{
		return;
	}
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FMotionSounds MotionSounds;
	TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
	TArray<TObjectPtr<UObject>> WeaponMovementSoundAssets;
	FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
	MotionSounds.WeaponMovementSocketName = "hand_r";
	TArray<FSound> Sounds;

	for (FALSXTWeaponMovementSound WM : WeaponMovementSounds)
	{
		Sounds.Append(WM.Sounds);
		
		for (FSound WMSound : WM.Sounds)
		{
			WeaponMovementSoundAssets.Add(WMSound.Sound);
		}
	}

	if (Sounds.IsValidIndex(0))
	{
		MotionSounds.WeaponMovementSounds = Sounds;
	}

	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}
}

void UALSXTCharacterSoundComponent::PlayActionSound(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& Type, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{	
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	FGameplayTag Weight = IALSXTCharacterInterface::Execute_GetWeightTag(Character);
	FMotionSounds MotionSounds;
	float Delay = FMath::RandRange(GeneralCharacterSoundSettings.ActionSoundDelay.X, GeneralCharacterSoundSettings.ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FGameplayTag MovementStrength = ConvertWeightTagToStrengthTag(Weight);

	// MOVEMENT
	if (CanPlayCharacterMovementSound() && MovementSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementSounds = SelectCharacterMovementSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementSounds = Sounds;
		}
	}

	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, MovementStrength) && AccentSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementAccentSounds = SelectCharacterMovementAccentSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CMA : CharacterMovementAccentSounds)
		{
			Sounds.Append(CMA.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	//ACTION
	if (ShouldPlayActionSound(MovementStrength, Stamina))
	{
		TArray<FALSXTCharacterActionSound> ActionSounds = SelectActionSounds(Settings, Character->GetDesiredSex(), ALSXTVoiceVariantTags::Default, Character->GetOverlayMode(), MovementStrength, Stamina);
		TArray<FSound> Sounds;

		for (FALSXTCharacterActionSound AC : ActionSounds)
		{
			Sounds.Append(AC.Sounds);			
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.VocalSounds = Sounds;
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, MovementStrength) && WeaponSound)
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.WeaponMovementSounds = Sounds;
		}
	}

	// V2
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}

	// if (Character->GetLocalRole() >= ROLE_Authority)
	// {
	// 	MulticastPlaySound(MotionSounds);
	// }
	// else
	// {
	// 	ServerPlaySound(MotionSounds);
	// }
}

void UALSXTCharacterSoundComponent::PlayAttackSound(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina)
{
	if (Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	FGameplayTag Weight = IALSXTCharacterInterface::Execute_GetWeightTag(Character);
	FGameplayTag Type = ALSXTCharacterMovementSoundTags::Jumping;
	FMotionSounds MotionSounds;
	float Delay = FMath::RandRange(GeneralCharacterSoundSettings.ActionSoundDelay.X, GeneralCharacterSoundSettings.ActionSoundDelay.Y);
	StartTimeSinceLastActionSoundTimer(Delay);
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();

	// MOVEMENT
	if (CanPlayCharacterMovementSound() && MovementSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementSounds = SelectCharacterMovementSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementSounds = Sounds;
		}
	}

	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, Strength) && AccentSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementAccentSounds = SelectCharacterMovementAccentSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CMA : CharacterMovementAccentSounds)
		{
			Sounds.Append(CMA.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	//ATTACK
	if (ShouldPlayAttackSound(AttackMode, Strength, Stamina))
	{
		TArray<FALSXTCharacterActionSound> AttackSounds = SelectAttackSounds(Settings, Sex, Variant, Overlay, Strength, Stamina);
		TArray<FSound> Sounds;

		for (FALSXTCharacterActionSound AC : AttackSounds)
		{
			Sounds.Append(AC.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.VocalSounds = Sounds;
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, Strength) && WeaponSound)
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.WeaponMovementSounds = Sounds;
		}
	}

	// V2
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}

}

void UALSXTCharacterSoundComponent::PlayDamageSound(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FGameplayTag Weight = IALSXTCharacterInterface::Execute_GetWeightTag(Character);
	FGameplayTag Type = ALSXTCharacterMovementSoundTags::Impact;
	FMotionSounds MotionSounds;

	// MOVEMENT
	if (CanPlayCharacterMovementSound() && MovementSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementSounds = SelectCharacterMovementSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementSounds = Sounds;
		}
	}

	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, Strength) && AccentSound)
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementAccentSounds = SelectCharacterMovementAccentSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CMA : CharacterMovementAccentSounds)
		{
			Sounds.Append(CMA.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	//DAMAGE
	if (ShouldPlayDamageSound(AttackMethod, Strength, AttackForm, Damage))
	{
		TArray<FALSXTCharacterDamageSound> DamageSounds = SelectDamageSounds(Settings, Sex, Variant, Overlay, AttackMethod, AttackForm, Strength);
		TArray<FSound> Sounds;

		for (FALSXTCharacterDamageSound AC : DamageSounds)
		{
			Sounds.Append(AC.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.VocalSounds = Sounds;
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, Strength) && WeaponSound)
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.WeaponMovementSounds = Sounds;
		}
	}

	// V2
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}
}

void UALSXTCharacterSoundComponent::PlayDeathSound(const FGameplayTag& Sex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	FGameplayTag Weight = IALSXTCharacterInterface::Execute_GetWeightTag(Character);
	FGameplayTag Type = ALSXTCharacterMovementSoundTags::Impact;
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	FMotionSounds MotionSounds;

	// MOVEMENT
	if (CanPlayCharacterMovementSound())
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementSounds = SelectCharacterMovementSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CM : CharacterMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementSounds = Sounds;
		}
	}

	// ACCENT
	if (ShouldPlayMovementAccentSound(Type, Strength))
	{
		TArray<FALSXTCharacterMovementSound> CharacterMovementAccentSounds = SelectCharacterMovementAccentSounds(Settings, Type, Weight);
		TArray<FSound> Sounds;

		for (FALSXTCharacterMovementSound CMA : CharacterMovementAccentSounds)
		{
			Sounds.Append(CMA.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.CharacterMovementAccentSounds = Sounds;
		}
	}

	//DEATH
	if (!CanPlayDeathSound() || !ShouldPlayDeathSound(AttackMethod, Strength, AttackForm, Damage))
	{
		TArray<FALSXTCharacterDamageSound> DeathSounds = SelectDeathSounds(Settings, Sex, Variant, Overlay, AttackForm, Strength);
		TArray<FSound> Sounds;
		TArray<TObjectPtr<UObject>> DeathSoundAssets;

		for (FALSXTCharacterDamageSound AC : DeathSounds)
		{
			Sounds.Append(AC.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.VocalSounds = Sounds;
		}
	}

	//WEAPON
	if (ShouldPlayWeaponMovementSound(Type, Strength))
	{
		TArray<FALSXTWeaponMovementSound> WeaponMovementSounds = SelectWeaponMovementSounds(Settings, ALSXTWeaponTags::M4, Type);
		FName WeaponMovementSoundSocket = GetSocketForMovement(Type);
		MotionSounds.WeaponMovementSocketName = "hand_r";
		TArray<FSound> Sounds;

		for (FALSXTWeaponMovementSound CM : WeaponMovementSounds)
		{
			Sounds.Append(CM.Sounds);
		}

		if (Sounds.IsValidIndex(0))
		{
			MotionSounds.WeaponMovementSounds = Sounds;
		}
	}

	// V2
	if (Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		ServerPlaySound(MotionSounds);
	}
	else if (Character->GetLocalRole() == ROLE_SimulatedProxy && Character->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		PlaySound(MotionSounds);
	}
}

void UALSXTCharacterSoundComponent::PlaySound(FMotionSounds MotionSounds)
{
	UALSXTCharacterSoundSettings* Settings = SelectCharacterSoundSettings();
	
	if (GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Play Sound in Editor");
		// UGameplayStatics::PlaySoundAtLocation(Character->GetWorld(), Sound.Sound.Sound, Location, Rotation, 1.0f, 1.0f);
	}
	else
	{
		bool BreathSound = MotionSounds.BreathSounds.IsValidIndex(0);
		bool CharacterVocalSound = MotionSounds.VocalSounds.IsValidIndex(0);
		bool CharacterMovementSound = MotionSounds.CharacterMovementSounds.IsValidIndex(0);
		bool CharacterMovementAccentSound = MotionSounds.CharacterMovementAccentSounds.IsValidIndex(0);
		bool WeaponMovementSound = MotionSounds.WeaponMovementSounds.IsValidIndex(0);

		//CHARACTER MOVEMENT
		if (CharacterMovementSound || CharacterMovementAccentSound)
		{
			FVector CharacterMovementSocketLocation = Character->GetMesh()->GetSocketLocation(MotionSounds.CharacterMovementSocketName);
			FRotator CharacterMovementSocketRotation = Character->GetMesh()->GetSocketRotation(MotionSounds.CharacterMovementSocketName);
			CharacterMovementSoundMixer = UGameplayStatics::SpawnSoundAtLocation(Character->GetWorld(), Settings->CharacterMovementSoundMixer, CharacterMovementSocketLocation, CharacterMovementSocketRotation, 10.0f, 1.0f);	

			if (IsValid(CharacterMovementSoundMixer))
			{
				CharacterMovementSoundMixer->SetIsReplicated(true);
				CharacterMovementSoundMixer->Stop();
				if (CharacterMovementSound)
				{
					FSound NewCharacterMovementSound;
					DetermineNewSound(MotionSounds.CharacterMovementSounds, PreviousCharacterMovementAssets, NewCharacterMovementSound);
					float NewCharacterMovementSoundPitch = FMath::RandRange(NewCharacterMovementSound.PitchRange.X, NewCharacterMovementSound.PitchRange.Y);
					SetNewSound(NewCharacterMovementSound.Sound, PreviousCharacterMovementAssets, GeneralCharacterSoundSettings.CharacterMovementNoRepeats);
					CurrentCharacterMovementSound = NewCharacterMovementSound;
					VocalizationMixerAudioComponent->SetObjectParameter("MovementSound", NewCharacterMovementSound.Sound);
				}

				if (CharacterMovementAccentSound)
				{
					FSound NewCharacterMovementAccentSound;
					DetermineNewSound(MotionSounds.CharacterMovementAccentSounds, PreviousCharacterMovementAccentAssets, NewCharacterMovementAccentSound);
					float NewCharacterMovementAccentSoundPitch = FMath::RandRange(NewCharacterMovementAccentSound.PitchRange.X, NewCharacterMovementAccentSound.PitchRange.Y);
					SetNewSound(NewCharacterMovementAccentSound.Sound, PreviousCharacterMovementAccentAssets, GeneralCharacterSoundSettings.CharacterMovementAccentNoRepeats);
					CurrentCharacterMovementAccentSound = NewCharacterMovementAccentSound;
					VocalizationMixerAudioComponent->SetObjectParameter("AccentSound", NewCharacterMovementAccentSound.Sound);
				}
				CharacterMovementSoundMixer->Play();
				CharacterMovementSoundMixer->SetTriggerParameter("UE.Source.OnPlay");
				Character->MakeNoise(1.0, Character, CharacterMovementSocketLocation, 10, "Noise");
			}
		}			

		if (WeaponMovementSound)
		{
			// WEAPON SOUND
			FVector WeaponMovementSocketLocation = Character->GetMesh()->GetSocketLocation(MotionSounds.WeaponMovementSocketName);
			FRotator WeaponMovementSocketRotation = Character->GetMesh()->GetSocketRotation(MotionSounds.WeaponMovementSocketName);
			WeaponMovementAudioComponent = UGameplayStatics::SpawnSoundAtLocation(Character->GetWorld(), Settings->WeaponMovementMixer, WeaponMovementSocketLocation, WeaponMovementSocketRotation, 10.0f, MotionSounds.WeaponMovementSoundsPitch);
			if (IsValid(WeaponMovementAudioComponent))
			{
				WeaponMovementAudioComponent->SetIsReplicated(true);
				FSound NewWeaponMovementSound;
				DetermineNewSound(MotionSounds.WeaponMovementSounds, PreviousWeaponMovementAssets, NewWeaponMovementSound);
				float NewCharacterMovementSoundPitch = FMath::RandRange(NewWeaponMovementSound.PitchRange.X, NewWeaponMovementSound.PitchRange.Y);
				SetNewSound(NewWeaponMovementSound.Sound, PreviousWeaponMovementAssets, GeneralCharacterSoundSettings.WeaponMovementNoRepeats);
				CurrentVocalizationSound = NewWeaponMovementSound;
				WeaponMovementAudioComponent->SetObjectParameter("WeaponMovementSound", NewWeaponMovementSound.Sound);

				WeaponMovementAudioComponent->Stop();
				WeaponMovementAudioComponent->Play();
				WeaponMovementAudioComponent->SetTriggerParameter("UE.Source.OnPlay");
			}

			
			Character->MakeNoise(1.0, Character, WeaponMovementSocketLocation, 10, "Noise");
		}

		// Breath
		if (IsValid(VocalizationMixerAudioComponent) && (BreathSound) && (!CharacterVocalSound))
		{
			FSound NewBreathSound;
			DetermineNewSound(MotionSounds.BreathSounds, PreviousBreathsAssets, NewBreathSound);
			float Pitch = FMath::RandRange(NewBreathSound.PitchRange.X, NewBreathSound.PitchRange.Y);
			CurrentBreathSound = NewBreathSound;
			SetNewSound(NewBreathSound.Sound, PreviousBreathsAssets, GeneralCharacterSoundSettings.BreathNoRepeats);
			FVector NewVoiceSocketLocation = Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName);
			FRotator NewVoiceSocketRotation = Character->GetMesh()->GetSocketRotation(GeneralCharacterSoundSettings.VoiceSocketName);
			VocalizationMixerAudioComponent->SetObjectParameter("BreathSound", NewBreathSound.Sound);
			VocalizationMixerAudioComponent->SetFloatParameter("Pitch", Pitch);

			if (Character->GetRemoteRole() == ROLE_Authority)
			{
				VocalizationMixerAudioComponent->SetBoolParameter("Player", true);
			}

			VocalizationMixerAudioComponent->SetTriggerParameter("BreathInput");
			Character->MakeNoise(1.0, Character, VoiceSocketLocation, 10, "Vocal");
			// OnVocalization(NewBreathSound);
			OnVocalization.Broadcast(NewBreathSound);
			// CurrentBreathParticles = SelectBreathParticles(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner()), CurrentStaminaTag);
			// UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(DetermineNewBreathParticle(), Character->GetMesh(), GeneralCharacterSoundSettings.VoiceSocketName, NewVoiceSocketLocation, NewVoiceSocketRotation, EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);
			
		}

		// VOCAL
		if (IsValid(VocalizationMixerAudioComponent) && (CharacterVocalSound))
		{
			FSound NewVocalizationSound;
			DetermineNewSound(MotionSounds.VocalSounds, PreviousVocalizationsAssets, NewVocalizationSound);
			float Pitch = FMath::RandRange(NewVocalizationSound.PitchRange.X, NewVocalizationSound.PitchRange.Y);
			CurrentVocalizationSound = NewVocalizationSound;
			SetNewSound(NewVocalizationSound.Sound, PreviousVocalizationsAssets, GeneralCharacterSoundSettings.VocalizationNoRepeats);
			FVector NewVoiceSocketLocation = Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName);
			VocalizationMixerAudioComponent->SetObjectParameter("VocalizationSound", NewVocalizationSound.Sound);
			VocalizationMixerAudioComponent->SetFloatParameter("Pitch", Pitch);

			if (Character->GetRemoteRole() == ROLE_Authority)
			{
				VocalizationMixerAudioComponent->SetBoolParameter("Player", true);
			}

			VocalizationMixerAudioComponent->SetTriggerParameter("VocalizationInput");
			Character->MakeNoise(1.0, Character, NewVoiceSocketLocation, 10, "Vocal");
			// OnVocalization(NewVocalizationSound);
			OnVocalization.Broadcast(NewVocalizationSound);

			if (BreathParticleSettings.VisibleBreathTypes.HasTag(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner())))
			{
				UE_LOG(LogTemp, Warning, TEXT("SelectNewBreathParticles"));
				FGameplayTag NewStaminaTag{ ConvertStaminaToStaminaTag(IALSXTCharacterInterface::Execute_GetStamina(GetOwner())) };
				CurrentBreathParticles = SelectBreathParticles(IALSXTCharacterInterface::Execute_GetBreathType(GetOwner()), NewStaminaTag);
				if (CurrentBreathParticles.IsValidIndex(0))
				{
					UpdateVoiceSocketRotation();
					UNiagaraSystem* NiagaraSystem{ (CurrentBreathParticles.Num() > 1) ? DetermineNewBreathParticle() : CurrentBreathParticles[0] };
					UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, Character->GetMesh(), GeneralCharacterSoundSettings.VoiceSocketName, Character->GetMesh()->GetSocketLocation(GeneralCharacterSoundSettings.VoiceSocketName), Character->GetMesh()->GetSocketRotation(GeneralCharacterSoundSettings.VoiceSocketName), EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
					// ServerPlayBreathParticle(NiagaraSystem);
				}
			}
		}
	}
}

void UALSXTCharacterSoundComponent::ServerPlaySound_Implementation(FMotionSounds MotionSounds)
{
	MulticastPlaySound(MotionSounds);
}

void UALSXTCharacterSoundComponent::MulticastPlaySound_Implementation(FMotionSounds MotionSounds)
{
	PlaySound(MotionSounds);
}

void UALSXTCharacterSoundComponent::SpawnAudioComponent(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket)
{
	
	if (!AttachmentSocket.IsNone())
	{
		VocalizationMixerAudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, Component, AttachmentSocket, Location, EAttachLocation::KeepWorldPosition, true, Volume, 1.0f, 0.0f, nullptr, nullptr, false);

		if (IsValid(VocalizationMixerAudioComponent))
		{
			VocalizationMixerAudioComponent->SetIsReplicated(true);
		}
	}
	else
	{
		AudioComponent = UGameplayStatics::SpawnSoundAtLocation(Cast<AALSXTCharacter>(GetOwner())->GetWorld(), Sound, Location, Rotation, Volume, 1.0f);
	}
}

void UALSXTCharacterSoundComponent::ServerSpawnAudioComponent_Implementation(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket)
{
	SpawnAudioComponent(AudioComponent, Sound, Component, Location, Rotation, Volume, AttachmentSocket);
}

void UALSXTCharacterSoundComponent::ClientSpawnAudioComponent_Implementation(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket)
{
	SpawnAudioComponent(AudioComponent, Sound, Component, Location, Rotation, Volume, AttachmentSocket);
}

void UALSXTCharacterSoundComponent::MulticastSpawnAudioComponent_Implementation(UAudioComponent* AudioComponent, USoundBase* Sound, USceneComponent* Component, FVector Location, FRotator Rotation, float Volume, FName AttachmentSocket)
{
	SpawnAudioComponent(AudioComponent, Sound, Component, Location, Rotation, Volume, AttachmentSocket);
}