// MIT

#include "AbilitySystem/AbilityTasks/AlsxtAbilityTaskPerformTargeting.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UAlsxtAbilityTaskPerformTargeting::UAlsxtAbilityTaskPerformTargeting(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UAlsxtAbilityTaskPerformTargeting* UAlsxtAbilityTaskPerformTargeting::CustomTargetDataTask(UGameplayAbility* OwningAbility, FName TaskName)
{
    UAlsxtAbilityTaskPerformTargeting* MyObj = NewAbilityTask<UAlsxtAbilityTaskPerformTargeting>(OwningAbility, TaskName);
    return MyObj;
}

void UAlsxtAbilityTaskPerformTargeting::Activate()
{
    Super::Activate();
    // Perform targeting immediately or after a short delay, depending on your needs.
    PerformTargeting();
}

void UAlsxtAbilityTaskPerformTargeting::OnDestroy(bool AbilityEnded)
{
    Super::OnDestroy(AbilityEnded);
}

void UAlsxtAbilityTaskPerformTargeting::PerformTargeting()
{
    FGameplayAbilityTargetDataHandle TargetDataHandle = ProduceTargetData();
    if (TargetDataHandle.IsValid(0))
    {
        OnTargetDataReady.Broadcast(TargetDataHandle);
        // End the task when data is ready.
        EndTask();
    }
    else
    {
        // Handle failure if needed.
        EndTask();
    }
}

FGameplayAbilityTargetDataHandle UAlsxtAbilityTaskPerformTargeting::ProduceTargetData()
{
    FGameplayAbilityTargetDataHandle TargetDataHandle;
    AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
    if (!IsValid(AvatarActor))
    {
        return TargetDataHandle;
    }

    FVector StartLocation = AvatarActor->GetActorLocation();
    FVector EndLocation = StartLocation + AvatarActor->GetActorForwardVector() * 1000.0f; // Example trace distance

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(AvatarActor);

    // Perform a line trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);

    // Optional: Draw debug line to visualize the trace
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHit ? FColor::Green : FColor::Red, false, 2.0f);

    if (bHit)
    {
        // Create FGameplayTargetData_SingleTargetHit from the hit result
    	FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
        NewTargetData->HitResult = HitResult;
        TargetDataHandle.Add(NewTargetData);
    }
    else
    {
        // If no hit, you might want to return a target data with the end location
        FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
        NewTargetData->HitResult.Location = EndLocation; // Just an example, maybe not what you want
        TargetDataHandle.Add(NewTargetData);
    }
    
    return TargetDataHandle;
}