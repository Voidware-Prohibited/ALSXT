// MIT

#include "Components/Character/ImpactReactionComponent.h"

// Sets default values for this component's properties
UImpactReactionComponent::UImpactReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UImpactReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UImpactReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// BumpReaction

void UImpactReactionComponent::AddDesiredBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{
	if (CanReact())
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerAddDesiredBumpReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
			}
	}
}

void UImpactReactionComponent::ServerAddDesiredBumpReaction_Implementation(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{
	AddDesiredBumpReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
}

void UImpactReactionComponent::AddBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{

	if (CanReact())
	{
		OnBumpReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
	}
}

void UImpactReactionComponent::OnBumpReaction_Implementation(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType) {}

// HitReaction

void UImpactReactionComponent::AddDesiredHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{
	if (CanReact())
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerAddDesiredHitReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
			}
	}
}

void UImpactReactionComponent::ServerAddDesiredHitReaction_Implementation(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{
	AddDesiredHitReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
}

void UImpactReactionComponent::AddHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType)
{

	if (CanReact())
	{
		OnHitReaction(ImpactForm, ImpactLocation, ImpactSide, ImpactStrength, SurfaceType);
	}
}

void UImpactReactionComponent::OnHitReaction_Implementation(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType) {}

