// Copyright 2024 AAA Open World Studio. All Rights Reserved.

#include "AdvancedCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAdvancedCharacter::AAdvancedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize state flags
	bIsSprinting = false;
	bIsCrouching = false;
	bIsMantling = false;

	// Create camera components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AAdvancedCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AAdvancedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle mantle animation/interpolation
	if (bIsMantling)
	{
		float ElapsedTime = GetWorld()->GetTimeSeconds() - MantleStartTime;
		float Alpha = FMath::Clamp(ElapsedTime / MantleDuration, 0.0f, 1.0f);
		
		// Smooth interpolation for mantle
		FVector NewLocation = FMath::Lerp(MantleStartLocation, MantleEndLocation, Alpha);
		SetActorLocation(NewLocation);

		if (Alpha >= 1.0f)
		{
			bIsMantling = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void AAdvancedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up Enhanced Input
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement input
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdvancedCharacter::Move);
		}

		// Look input
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdvancedCharacter::Look);
		}

		// Sprint input
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AAdvancedCharacter::Sprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAdvancedCharacter::StopSprinting);
		}

		// Crouch input
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAdvancedCharacter::Crouch);
		}

		// Jump input
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AAdvancedCharacter::Jump);
		}

		// Mantle input
		if (MantleAction)
		{
			EnhancedInput->BindAction(MantleAction, ETriggerEvent::Started, this, &AAdvancedCharacter::StartMantleCheck);
		}
	}
}

void AAdvancedCharacter::Move(const FInputActionValue& Value)
{
	if (bIsMantling) return;

	// Get input vector (2D from gamepad/analog stick)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get control rotation and remove pitch component
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Calculate forward and right directions
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Apply movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAdvancedCharacter::Look(const FInputActionValue& Value)
{
	// Get look input value (2D from gamepad/analog stick)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAdvancedCharacter::Sprint(const FInputActionValue& Value)
{
	if (bIsMantling || bIsCrouching) return;

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AAdvancedCharacter::StopSprinting(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AAdvancedCharacter::Crouch(const FInputActionValue& Value)
{
	if (bIsMantling) return;

	bIsCrouching = !bIsCrouching;

	if (bIsCrouching)
	{
		Crouch();
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		bIsSprinting = false; // Stop sprinting when crouching
	}
	else
	{
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AAdvancedCharacter::Jump(const FInputActionValue& Value)
{
	if (bIsMantling || bIsCrouching) return;

	ACharacter::Jump();
}

void AAdvancedCharacter::StartMantleCheck()
{
	if (bIsMantling) return;

	TraceMantleLocation();
}

bool AAdvancedCharacter::CanMantle() const
{
	// Check if mantle end location is valid
	return !MantleEndLocation.IsZero();
}

void AAdvancedCharacter::TraceMantleLocation()
{
	// Get forward direction from controller
	if (!Controller) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, ControlRotation.Yaw, 0.0f));

	// Start trace from character location
	FVector StartLocation = GetActorLocation();
	StartLocation.Z += GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	FVector EndLocation = StartLocation + (ForwardDirection * MantleReachDistance);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Check if the hit object is a valid ledge
		FVector LedgeLocation = HitResult.Location;
		FVector UpVector = FVector(0.0f, 0.0f, 1.0f);
		
		// Trace upward to find ledge height
		FVector LedgeStart = LedgeLocation;
		FVector LedgeEnd = LedgeLocation + (UpVector * MantleHeightThreshold);
		
		FHitResult LedgeHit;
		bool bLedgeHit = GetWorld()->LineTraceSingleByChannel(
			LedgeHit,
			LedgeStart,
			LedgeEnd,
			ECC_Visibility,
			QueryParams
		);

		if (bLedgeHit)
		{
			// Valid mantle detected
			MantleStartLocation = GetActorLocation();
			MantleEndLocation = LedgeHit.Location;
			MantleEndLocation.Z = LedgeHit.Location.Z; // Keep Z at ground level of top surface
			
			// Offset to place character on top of ledge
			MantleEndLocation -= ForwardDirection * 50.0f;

			PerformMantle();
		}
	}
}

void AAdvancedCharacter::PerformMantle()
{
	if (!CanMantle()) return;

	bIsMantling = true;
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	// Calculate mantle duration based on distance
	float Distance = FVector::Dist(MantleStartLocation, MantleEndLocation);
	MantleDuration = FMath::Clamp(Distance / 300.0f, 0.5f, 1.5f);
	MantleStartTime = GetWorld()->GetTimeSeconds();

	// Note: In production, trigger mantle animation montage here
	// PlayAnimMontage(MantleMontage);
}
