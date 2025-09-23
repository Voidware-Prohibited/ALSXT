// MIT

#include "Components/Character/AlsxtIdleAnimationComponent.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtIdleAnimationComponentInterface.h"
#include "AlsxtAnimationInstance.h"

// Sets default values for this component's properties
UAlsxtIdleAnimationComponent::UAlsxtIdleAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UAlsxtIdleAnimationComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentIdleMontage, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PreviousMontages, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IdleCounterCurrent, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IdleCounterTarget, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bIsIdle, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentTimeBetweenAnimations, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, TargetTimeBetweenAnimations, Parameters)
}

// Called when the game starts
void UAlsxtIdleAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		AnimInstance = IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance();
	}
	
	if (GetOwner() && IdleAnimationSettings.EnableIdleAnimations)
	{
		SetIdleCounterTarget();
		GetWorld()->GetTimerManager().SetTimer(PreCountIdleCounterTimerHandle, this, &UAlsxtIdleAnimationComponent::StartIdleCounterTimer, GetIdleState().TargetTime, false);
	}
}

// Called every frame
void UAlsxtIdleAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IdleAnimationSettings.EnableIdleAnimations)
	{
		StatusState = IAlsxtCharacterInterface::Execute_GetCharacterStatusState(GetOwner());
		if (IsValid(GetIdleState().CurrentIdleMontage) && !IsPlayerInputIdle())
		{
			StopIdle();
		}
	}
}

void UAlsxtIdleAnimationComponent::SetIdleState(const FAlsxtIdleState& NewIdleState)
{
	const auto PreviousIdleState{ IdleState };
	IdleState = NewIdleState;
	OnIdleStateChanged(PreviousIdleState);

	if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
	{
		ServerSetIdleState(NewIdleState);
	}

	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetIdleState(NewIdleState);
	// }

	// if (GetLocalRole() >= ROLE_Authority)
	// {
	// 	IdleState = NewIdleState;
	// }
	// else
	// {
	// 	ServerSetIdleState(NewIdleState);
	// }

	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, IdleState, this)

}

void UAlsxtIdleAnimationComponent::ServerSetIdleState_Implementation(const FAlsxtIdleState& NewIdleState)
{
	SetIdleState(NewIdleState);
}

void UAlsxtIdleAnimationComponent::ClientSetIdleState_Implementation(const FAlsxtIdleState& NewIdleState)
{
	SetIdleState(NewIdleState);
}

void UAlsxtIdleAnimationComponent::ServerProcessNewIdleState_Implementation(const FAlsxtIdleState& NewIdleState)
{
	ProcessNewIdleState(NewIdleState);
}

void UAlsxtIdleAnimationComponent::OnReplicate_IdleState(const FAlsxtIdleState& PreviousIdleState)
{
	OnIdleStateChanged(PreviousIdleState);
}

void UAlsxtIdleAnimationComponent::OnIdleStateChanged_Implementation(const FAlsxtIdleState& PreviousIdleState) {}

void UAlsxtIdleAnimationComponent::SetIdleCounterTarget_Implementation()
{
	FAlsxtIdleState NewIdleState = GetIdleState();
	NewIdleState.TargetTime = FMath::RandRange(IdleAnimationSettings.TimeDelayBeforeIdle.X, IdleAnimationSettings.TimeDelayBeforeIdle.Y);
	NewIdleState.CurrentTime = 0.0f;
	NewIdleState.GazingTargetTime = FMath::RandRange(IdleAnimationSettings.TimeDelayBeforeGazing.X, IdleAnimationSettings.TimeDelayBeforeGazing.Y);
	NewIdleState.GazingCurrentTime = 0.0f;
	SetIdleState(NewIdleState);	
}

bool UAlsxtIdleAnimationComponent::IsPlayerIdle()
{
	return bIsIdle;
}

void UAlsxtIdleAnimationComponent::SetPlayerIdle_Implementation(bool NewIdle)
{
	bIsIdle = NewIdle;
}

bool UAlsxtIdleAnimationComponent::IsPlayerInputIdle()
{
	bool bIsInputIdle = (GetOwner()->GetVelocity().Size() == 0.0) && (IAlsxtCharacterInterface::Execute_GetCharacterControlRotation(GetOwner()) == PreviousControlRotation);
	bool bIsMovementIdle = IAlsxtCharacterInterface::Execute_GetCharacterLocomotionMode(GetOwner()) == AlsLocomotionModeTags::Grounded;
	bool bIsActionIdle = IAlsxtCharacterInterface::Execute_GetCharacterLocomotionAction(GetOwner()) == FGameplayTag::EmptyTag;
	PreviousControlRotation = IAlsxtCharacterInterface::Execute_GetCharacterControlRotation(GetOwner());
	return bIsInputIdle && bIsMovementIdle && bIsActionIdle;
}

TArray<FIdleAnimation> UAlsxtIdleAnimationComponent::SelectIdleAnimations(const FGameplayTag& Sex, const FGameplayTag& Stance, const FGameplayTag& Overlay, const FGameplayTag& Injury, const FGameplayTag& CombatStance)
{
	UAlsxtIdleAnimationSettings* Settings = IAlsxtIdleAnimationComponentInterface::Execute_SelectIdleSettings(GetOwner());
	TArray<FIdleAnimation> Animations = Settings->IdleAnimations;
	FGameplayTagContainer TagsContainer;
	TArray<FIdleAnimation> SelectedAnimations;
	TagsContainer.AddTag(Sex);
	TagsContainer.AddTag(Stance);
	TagsContainer.AddTag(Overlay);
	TagsContainer.AddTag(Injury);
	TagsContainer.AddTag(CombatStance);

	// Return if there are no sounds
	if (Animations.Num() < 1)
	{
		if (IdleAnimationSettings.bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Animations");
		}
		return SelectedAnimations;
	}

	// Filter sounds based on Tag parameters
	for (FIdleAnimation Animation : Animations)
	{
		FGameplayTagContainer CurrentTagsContainer;
		CurrentTagsContainer.AppendTags(Animation.Sex);
		CurrentTagsContainer.AppendTags(Animation.Stance);
		CurrentTagsContainer.AppendTags(Animation.Overlay);
		CurrentTagsContainer.AppendTags(Animation.Injury);
		CurrentTagsContainer.AppendTags(Animation.CombatStance);

		if (CurrentTagsContainer.HasAll(TagsContainer))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Animation Found");
			SelectedAnimations.Add(Animation);
		}
	}

	// Return if Return is there are no filtered sounds
	if (SelectedAnimations.Num() < 1)
	{
		if (IdleAnimationSettings.bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Animations");
		}
		return SelectedAnimations;
	}
	return SelectedAnimations;
}

UAnimMontage* UAlsxtIdleAnimationComponent::GetNewIdleAnimation(TArray<FIdleAnimation> IdleAnimations)
{
	TArray<FIdleAnimation> FilteredAnimations;
	FIdleAnimation NewAnimationResult;
	UAnimMontage* NewAssetResult{ nullptr };

	// 	// Return if Return is there are no filtered sounds
	if (IdleAnimations.Num() < 1)
	{
		return NewAssetResult;
	}

	// If more than one result, avoid duplicates
	if (IdleAnimations.Num() > 1)
	{

		for (FIdleAnimation IdleAnimation : IdleAnimations)
		{
			if (!PreviousMontages.Contains(IdleAnimation.Montage))
			{
				FilteredAnimations.Add(IdleAnimation);
			}
		}

		//Shuffle Array
		for (int m = FilteredAnimations.Num() - 1; m >= 0; --m)
		{
			int n = FMath::Rand() % (m + 1);
			if (m != n) FilteredAnimations.Swap(m, n);
		}

		// Select Random Array Entry
		int RandIndex = FMath::RandRange(0, (FilteredAnimations.Num() - 1));
		NewAnimationResult = FilteredAnimations[RandIndex];
		NewAssetResult = NewAnimationResult.Montage;
		return NewAssetResult;
	}
	else
	{
		NewAnimationResult = FilteredAnimations[0];
		NewAssetResult = NewAnimationResult.Montage;
		return NewAssetResult;
	}
}

void UAlsxtIdleAnimationComponent::ServerSetNewAnimation_Implementation(UAnimMontage* Animation, int NoRepeats)
{
	SetNewAnimationImplementation(Animation, NoRepeats);
}

void UAlsxtIdleAnimationComponent::SetNewAnimation(UAnimMontage* Animation, int NoRepeats)
{
	// if (PreviousMontages.Num() >= abs(NoRepeats))
	// {
	// 	CurrentIdleMontage = Animation;
	// 	PreviousMontages.Add(Animation);
	// 	PreviousMontages.RemoveAt(0, 1, true);
	// }
	// else
	// {
	// 	CurrentIdleMontage = Animation;
	// 	PreviousMontages.Add(Animation);
	// }

	// if ((GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) && Cast<ACharacter>(GetOwner())->IsLocallyControlled())
	// {
	// 	ServerSetNewAnimation(Animation, NoRepeats);
	// }

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSetNewAnimation(Animation, NoRepeats);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		SetNewAnimationImplementation(Animation, NoRepeats);
	}
	
}

void UAlsxtIdleAnimationComponent::SetNewAnimationImplementation(UAnimMontage* Animation, int NoRepeats)
{
	// CurrentIdleMontage = Animation;
	
	if (PreviousMontages.Num() >= abs(NoRepeats))
	{
		// CurrentIdleMontage = Animation;
		FAlsxtIdleState NewIdleState = GetIdleState();
		NewIdleState.CurrentIdleMontage = Animation;
		SetIdleState(NewIdleState);
		PreviousMontages.Add(Animation);
		PreviousMontages.RemoveAt(0, 1, EAllowShrinking::Yes);
	}
	else
	{
		// CurrentIdleMontage = Animation;
		FAlsxtIdleState NewIdleState = GetIdleState();
		NewIdleState.CurrentIdleMontage = Animation;
		SetIdleState(NewIdleState);
		PreviousMontages.Add(Animation);
	}

	if (IdleAnimationSettings.bDebugMode)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GetIdleState().CurrentIdleMontage->GetName());
	}
}

void UAlsxtIdleAnimationComponent::ServerStartIdleCounterTimer_Implementation()
{
	StartIdleCounterTimerImplementation();
}

void UAlsxtIdleAnimationComponent::StartIdleCounterTimer()
{
	// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerStartIdleCounterTimer();
	// }
	// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	// {
	// 	StartIdleCounterTimerImplementation();
	// }

	StartIdleCounterTimerImplementation();
}

void UAlsxtIdleAnimationComponent::StartIdleCounterTimerImplementation()
{
	GetWorld()->GetTimerManager().ClearTimer(PreCountIdleCounterTimerHandle);
	SetIdleCounterTarget();
	GetWorld()->GetTimerManager().SetTimer(IdleCounterTimerHandle, this, &UAlsxtIdleAnimationComponent::IdleCounterTimer, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(GazingCounterTimerHandle, this, &UAlsxtIdleAnimationComponent::GazeCounterTimer, 0.01f, true);
}

void UAlsxtIdleAnimationComponent::IdleCounterTimer()
{
	if (IsPlayerInputIdle())
	{
		FAlsxtIdleState NewIdleState = GetIdleState();
		NewIdleState.CurrentTime = NewIdleState.CurrentTime + 0.01;
		SetIdleState(NewIdleState);

		// if (IdleAnimationSettings.bDebugMode)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Yellow, FString::SanitizeFloat(GetIdleState().CurrentTime));
		// }
		
		if (GetIdleState().CurrentTime >= GetIdleState().TargetTime && IdleAnimationSettings.EligibleStaminaLevels.HasTag(StatusState.CurrentStaminaTag) && IAlsxtIdleAnimationComponentInterface::Execute_ShouldIdle(GetOwner()))
		{
			GetWorld()->GetTimerManager().ClearTimer(DelayBetweenAnimationsTimerHandle);
			ResetIdleCounterTimer();
			SetPlayerIdle(true);
			StartIdle();
		}
	}
	else
	{
		StopIdle();
		SetIdleCounterTarget();
		StartIdleCounterTimer();
	}	
}

void UAlsxtIdleAnimationComponent::GazeCounterTimer()
{
	if (IsPlayerInputIdle())
	{
		FAlsxtIdleState NewIdleState = GetIdleState();
		NewIdleState.GazingTargetTime = NewIdleState.GazingTargetTime + 0.01;
		SetIdleState(NewIdleState);

		// if (IdleAnimationSettings.bDebugMode)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Yellow, FString::SanitizeFloat(GetIdleState().CurrentTime));
		// }

		if (GetIdleState().GazingTargetTime >= GetIdleState().GazingTargetTime && IdleAnimationSettings.EligibleStaminaLevels.HasTag(StatusState.CurrentStaminaTag) && IAlsxtIdleAnimationComponentInterface::Execute_ShouldIdle(GetOwner()))
		{
			GetWorld()->GetTimerManager().ClearTimer(GazingCounterTimerHandle);
			ResetIdleCounterTimer();
			SetPlayerIdle(true);
			StartIdle();
		}
	}
	else
	{
		StopIdle();
		SetIdleCounterTarget();
		StartIdleCounterTimer();
	}	
}

void UAlsxtIdleAnimationComponent::ResetIdleCounterTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(PreCountIdleCounterTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(IdleCounterTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DelayBetweenAnimationsTimerHandle);

	FAlsxtIdleState NewIdleState = GetIdleState();
	NewIdleState.TargetTime = 0.0;
	NewIdleState.CurrentTime = 0.0;
	NewIdleState.GazingTargetTime = 0.0;
	NewIdleState.GazingCurrentTime = 0.0;
	SetIdleState(NewIdleState);

	if (IAlsxtIdleAnimationComponentInterface::Execute_ShouldIdle(GetOwner()))
	{
		SetIdleCounterTarget();
		StartIdleCounterTimer();
		// StartDelayBetweenAnimationsTimer(0.0f);
	}	
}

void UAlsxtIdleAnimationComponent::StartDelayBetweenAnimationsTimer(float InitialDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(PreCountIdleCounterTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(IdleCounterTimerHandle);

	FAlsxtIdleState NewIdleState = GetIdleState();
	NewIdleState.TargetTime = 0.0;
	NewIdleState.CurrentTime = 0.0;
	NewIdleState.CurrentTimeBeforeNext = 0.0;
	NewIdleState.TargetTimeBeforeNext = FMath::RandRange(IdleAnimationSettings.TimeDelayBetweenAnimations.X, IdleAnimationSettings.TimeDelayBetweenAnimations.Y);
	SetIdleState(NewIdleState);
	GetWorld()->GetTimerManager().SetTimer(DelayBetweenAnimationsTimerHandle, this, &UAlsxtIdleAnimationComponent::DelayBetweenAnimationsTimer, 0.01f, true, InitialDelay);
}

void UAlsxtIdleAnimationComponent::DelayBetweenAnimationsTimer()
{
	if (IsPlayerInputIdle())
	{
		FAlsxtIdleState NewIdleState = GetIdleState();
		NewIdleState.CurrentTimeBeforeNext = NewIdleState.CurrentTimeBeforeNext + 0.01;
		SetIdleState(NewIdleState);
		
		if (GetIdleState().CurrentTimeBeforeNext >= GetIdleState().TargetTimeBeforeNext && IAlsxtIdleAnimationComponentInterface::Execute_ShouldIdle(GetOwner()))
		{
			ResetDelayBetweenAnimationsTimer();
			StartIdle();
		}
	}
	else
	{
		StopIdle();
		// SetPlayerIdle(false);
		StartDelayBetweenAnimationsTimer(0.0);
	}	
}

void UAlsxtIdleAnimationComponent::ResetDelayBetweenAnimationsTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayBetweenAnimationsTimerHandle);
	FAlsxtIdleState NewIdleState = GetIdleState();
	NewIdleState.CurrentTimeBeforeNext = 0.0;
	NewIdleState.TargetTimeBeforeNext = 0.0;
	SetIdleState(NewIdleState);
}

void UAlsxtIdleAnimationComponent::StartCameraRotationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CameraRotationTimerHandle, CameraRotationTimerDelegate, 0.01f, true);
}

void UAlsxtIdleAnimationComponent::CameraRotationTimer()
{
	// ...
}

void UAlsxtIdleAnimationComponent::ResetCameraRotationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CameraRotationTimerHandle);
	CameraOffset = FVector::ZeroVector;
}

void UAlsxtIdleAnimationComponent::ServerStartIdle_Implementation()
{
	StartIdleImplementation();
}

void UAlsxtIdleAnimationComponent::MulticastStartIdle_Implementation()
{
	StartIdleImplementation();
}

void UAlsxtIdleAnimationComponent::StartIdle()
{
	// StartIdleImplementation();
	// ServerStartIdle();

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Server
		MulticastStartIdle();
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		// Reg
		StartIdleImplementation();
	}
	else if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		// AI
		MulticastStartIdle();
	}

	
	// if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	ServerStartIdle();
	// }
	// else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy && GetOwner()->GetRemoteRole() == ROLE_Authority)
	// {
	// 	StartIdleImplementation();
	// }
}

void UAlsxtIdleAnimationComponent::StartIdleImplementation()
{
	if (IdleAnimationSettings.bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "StartIdle");
	}
	
	TArray<FIdleAnimation> SelectedAnimations = SelectIdleAnimations(IAlsxtCharacterInterface::Execute_GetCharacterSex(GetOwner()), IAlsxtCharacterInterface::Execute_GetCharacterStance(GetOwner()), IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(GetOwner()), IAlsxtCharacterInterface::Execute_GetCharacterInjury(GetOwner()), IAlsxtCharacterInterface::Execute_GetCharacterCombatStance(GetOwner()));
	UAnimMontage* FoundIdleAnimation = GetNewIdleAnimation(SelectedAnimations);
	SetNewAnimation(FoundIdleAnimation, 1);
	float MontageLength{ 0.0f };

	if (IsValid(GetIdleState().CurrentIdleMontage) && IsValid(IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()))
	{
		if (IdleAnimationSettings.bDebugMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, GetIdleState().CurrentIdleMontage->GetName());
		}
		MontageLength = IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Play(GetIdleState().CurrentIdleMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		// MontageLength = CurrentIdleMontage->GetPlayLength();
		ResetIdleCounterTimer();
		GetWorld()->GetTimerManager().ClearTimer(IdleCounterTimerHandle);
		ResetDelayBetweenAnimationsTimer();
		StartDelayBetweenAnimationsTimer(MontageLength);
	}
	else
	{
		StartDelayBetweenAnimationsTimer(3);
		if (IdleAnimationSettings.bDebugMode)
		{
			if (!IsValid(GetIdleState().CurrentIdleMontage))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Invalid Montage");
			}
			if (!IsValid(IAlsxtCharacterInterface::Execute_GetCharacterAnimInstance(GetOwner())))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Invalid AnimInstance");
			}
		}	
	}
}

void UAlsxtIdleAnimationComponent::StopIdle()
{
	if (IsValid(GetIdleState().CurrentIdleMontage))
	{
		IAlsxtCharacterInterface::Execute_GetCharacterMesh(GetOwner())->GetAnimInstance()->Montage_Stop(0.5f, GetIdleState().CurrentIdleMontage);
		SetPlayerIdle(false);

		if (IAlsxtIdleAnimationComponentInterface::Execute_ShouldIdle(GetOwner()))
		{
			StartDelayBetweenAnimationsTimer(3.0);
		}	
	}
}