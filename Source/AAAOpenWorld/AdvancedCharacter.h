// Copyright 2024 AAA Open World Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "AdvancedCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

/**
 * Advanced Character class implementing Enhanced Input, Sprinting, Crouching, and Mantling
 * Part of Phase 1: Advanced Movement & Input System
 */
UCLASS()
class AAAOPENWORLD_API AAdvancedCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAdvancedCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Movement Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	void Crouch(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	
	// Mantling System
	void StartMantleCheck();
	bool CanMantle() const;
	void PerformMantle();
	void TraceMantleLocation();

	// State Management
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsCrouching() const { return bIsCrouching; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsMantling() const { return bIsMantling; }

private:
	// Input Assets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

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

	// Movement Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Speeds")
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Speeds")
	float SprintSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Speeds")
	float CrouchSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Mantle")
	float MantleReachDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Mantle")
	float MantleHeightThreshold = 120.0f;

	// State Flags
	bool bIsSprinting;
	bool bIsCrouching;
	bool bIsMantling;

	// Mantle Variables
	FVector MantleStartLocation;
	FVector MantleEndLocation;
	float MantleStartTime;
	float MantleDuration;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComponent;
};
