// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBB.h"
#include "CharacterBB.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

void APlayerControllerBBBase::OnPossess(APawn *aPawn) {
  Super::OnPossess(aPawn);

  // Store reference to the Player's pawn
  PlayerCharacter = Cast<ACharacterBB>(aPawn);
  checkf(PlayerCharacter, TEXT("APlayerControllerBBBase derived classes should only posess ACharacterBBBase derived pawns."));

  // Store a reference to the HUD
  PlayerHud = Cast<AHudBB>(GetHUD());
  checkf(PlayerHud, TEXT("Unable to get reference to the HUD"));
  
  // Get a reference to the EnhancedInputcomponent
  EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
  checkf(EnhancedInputComponent, TEXT("Unable to get reference to the EnhancedInputComponent."));

  // Get a reference to the local player subsystem
  UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  checkf(InputSubSystem, TEXT("Unable to get reference to the UEnhancedInputLocalPlayerSubsystem."));

  // Wipe existing mappings, and add our mapping
  checkf(InputMappingContext, TEXT("InputMappingContext was not specified."))
  InputSubSystem->ClearAllMappings();
  InputSubSystem->AddMappingContext(InputMappingContext, 0);
  
  // Bind the input actions.
  // Only attempt to bind if a valid value was provided
  if (ActionMove)
    EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleMove);
  
  if (ActionLook)
    EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleLook);
  
  if (ActionJump)
    EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleJump);

  if (ActionPsiBlast)
    EnhancedInputComponent->BindAction(ActionPsiBlast, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandlePsiBlast);
  
  if (ActionToggleCrouch)
    EnhancedInputComponent->BindAction(ActionToggleCrouch, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleToggleCrouch);
  
  if (ActionToggleSprint)
    EnhancedInputComponent->BindAction(ActionToggleSprint, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleToggleSprint);
  
  if (ActionCycleUIMode)
    EnhancedInputComponent->BindAction(ActionCycleUIMode, ETriggerEvent::Triggered, this, &APlayerControllerBBBase::HandleCycleUIMode);
  
}
void APlayerControllerBBBase::OnUnPossess() {
  // Unbind things
  EnhancedInputComponent->ClearActionBindings();

  // Call the parent method in case it needs to do anything
  Super::OnUnPossess();
}

void APlayerControllerBBBase::HandleMove(const FInputActionValue &InputActionValue) {
  // Value is a Vector2D
  const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

  // Add movement to the Player's Character pawn
  if (PlayerCharacter) {
    PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
    PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
  }
}

void APlayerControllerBBBase::HandleLook(
    const FInputActionValue &InputActionValue) {
  // Input is a Vector2D
  const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

  // add yaw and pitch input to controller
  AddYawInput(LookAxisVector.X);
  AddPitchInput(LookAxisVector.Y);
}

void APlayerControllerBBBase::HandleJump() {
  // Input is Digital (value not used here)

  // Make the Player's Character Pawn jump, disabling crouch if it was active
  if (PlayerCharacter) {
    PlayerCharacter->UnCrouch();
    PlayerCharacter->Jump();
  }
}
void APlayerControllerBBBase::HandlePsiBlast() {
  if (PlayerCharacter) {
    PlayerCharacter->PsiBlast();
  }
}

void APlayerControllerBBBase::HandleToggleSprint() {
  // Input is Digital (value not used here)

  // Make the Player's Character Pawn sprint, if not already sprinting
  if (PlayerCharacter && PlayerCharacter->bIsRunning) {
    PlayerCharacter->SetRunning(false);
  } else {
    PlayerCharacter->SetRunning(true);
  }
}

void APlayerControllerBBBase::HandleToggleCrouch() {
  // Input is Digital (value not used here)

  // Make the Player's Character Pawn crouch, if not already crouching
  if (PlayerCharacter && PlayerCharacter->bIsCrouched) {
    PlayerCharacter->UnCrouch();
  } else {
    PlayerCharacter->Crouch();
  }
}

void APlayerControllerBBBase::HandleCycleUIMode() {
  if (PlayerHud) {
    PlayerHud->CycleToNextViewMode();
  }
}
