// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBB.h"

#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACharacterBB::ACharacterBB() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterBB::BeginPlay() {
  Super::BeginPlay();
  
  // Set the character as can crouch in the NavAgent; otherwise it can not crouch
  if (GetMovementComponent()) {
    GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
  }
}

// Called every frame
void ACharacterBB::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ACharacterBB::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int ACharacterBB::GetHealth() { return CurrentHealth; }

int ACharacterBB::GetMaxHealth() { return MaxHealth;}

void ACharacterBB::UpdateHealth(int DeltaHealth) {
  CurrentHealth += DeltaHealth;

  // Make sure the new CurrentHealth is inside an acceptable range
  // In this case it will never be < -1, or > MaxHealth
  CurrentHealth = FMath::Clamp(CurrentHealth, -1.f, MaxHealth);

  if (CurrentHealth <= 0.f) {
    // TODO: Something when the player dies
  }
}

void ACharacterBB::RestoreToFullHealth() {
  CurrentHealth = MaxHealth;
}

void ACharacterBB::SetMaxHealth(int NewMaxHealth) {
  // We just assume that the new value is within an acceptable range.
  // Might be better if we had some range checking.
  MaxHealth = NewMaxHealth;
}

float ACharacterBB::GetStamina() { return CurrentStamina; }

float ACharacterBB::GetStaminaRecuperationFactor() { return StaminaRecuperationFactor;}

void ACharacterBB::SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor) {
  // Might be sensible to check this is a +ve value within some sensible range.
  StaminaRecuperationFactor = NewStaminaRecuperationFactor;
}

float ACharacterBB::GetPsiPower() { return CurrentPsiPower; }

void ACharacterBB::PsiBlast() {
  // The cost of psi blast is a known number
  // Check we have at least that much before allowing this func to work
  if (CurrentPsiPower >= PsiBlastCost) {
    // Do the Psi Blast

    //Deduct the power used
    CurrentPsiPower -= PsiBlastCost;
  }
}

void ACharacterBB::AddKey(FString KeyToAdd) {
  if (KeyWallet.Contains(KeyToAdd)) {
    // Key already in there, play a noise
  } else {
    KeyWallet.Add(KeyToAdd);
    // And maybe play a sound effect?
  }
}

void ACharacterBB::RemoveKey(FString KeyToRemove) {
  if (KeyWallet.Contains(KeyToRemove)) {
    KeyWallet.Remove(KeyToRemove);
  }
}

bool ACharacterBB::IsPlayerCarryingKey(FString DesiredKey) {
  return KeyWallet.Contains(DesiredKey);
}
