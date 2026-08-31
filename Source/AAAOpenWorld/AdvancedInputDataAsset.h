// Copyright 2024 AAA Open World Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"
#include "AdvancedInputDataAsset.generated.h"

class UInputAction;

/**
 * Data Asset for storing input configuration
 * Allows designers to easily configure input actions without code changes
 * Part of Phase 1: Advanced Movement & Input System
 */
UCLASS(Blueprintable, Const)
class AAAOPENWORLD_API UAdvancedInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAdvancedInputDataAsset();

	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	// Movement Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* MantleAction;

	// Combat Actions (Reserved for future phases)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Combat")
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Combat")
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Combat")
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Combat")
	UInputAction* DodgeAction;

	// Interaction Actions (Reserved for future phases)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Interaction")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Interaction")
	UInputAction* PickUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions|Interaction")
	UInputAction* UseItemAction;
};
