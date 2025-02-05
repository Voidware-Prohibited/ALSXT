// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ALSXTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AALSXTPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	// UPROPERTY(VisibleAnywhere, Category = "ALSXT Player Controller")
	// ACharacter* Character;
	// 
	// UPROPERTY(VisibleAnywhere, Category = "ALSXT Player Controller")
	// APawn* Pawn;
	
	UPROPERTY(VisibleAnywhere, Category = "ALSXT Player Controller")
	bool CanControlCharacter {true};

	UPROPERTY(VisibleAnywhere, Category = "ALSXT Player Controller")
	bool CanControlPawn {false};

};
