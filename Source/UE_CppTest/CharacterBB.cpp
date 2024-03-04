// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBB.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACharacterBB::ACharacterBB() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  SetActorTickInterval(0.5f);
  AActor::SetActorTickEnabled(true);
}

// Called when the game starts or when spawned
void ACharacterBB::BeginPlay()
{
  Super::BeginPlay();
  if (GetMovementComponent()) GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

  BroadcastCurrentStats();
}

void ACharacterBB::AddMovementInput(FVector WorldDirection, float ScaleValue,
                                    bool bForce) {
  // If the player is running, check that they have stamina available,
  // other wise kick them out of running mode
  if (bIsRunning && CurrentStamina <= 0) {
    SetRunning(false);
  }

  Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

  // Set the flag to indicate if the character ran
  if (bIsRunning)
    bHasRan = true;
}
void ACharacterBB::Jump() {
  // Jump requires stamina
  if (CurrentStamina - JumpStaminaCost >= 0.f) {
    UnCrouch();
    Super::Jump();
    bHasJumped = true;
  }
}
void ACharacterBB::Crouch(bool bClientSimulation) {
  SetRunning(false);
  Super::Crouch(bClientSimulation);
}

// Called every frame
void ACharacterBB::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // There are 2 things which can recover over time:
  //    - Stamina
  //    - Psi Power

#pragma region Update Stamina
  // Stamina affected, from the worst to best case:
  // if they jumped, otherwise if they ran, otherwise if they rested.
  // and if they did none of those they get the default recharge rate.

  float ActualStaminaRecuperationFactor = StaminaRecuperationFactor;

  if (bHasJumped) ActualStaminaRecuperationFactor = -JumpStaminaCost;
  else if (bHasRan) ActualStaminaRecuperationFactor = -RunStaminaCost;
  if (bIsCrouched) ActualStaminaRecuperationFactor = RestStaminaRebate;

  // Keep track of the value before it is changed
  const float PreviousStamina = CurrentStamina;

  // Update the current value, capping it to the min and max allowable
  CurrentStamina = FMath::Clamp(CurrentStamina + ActualStaminaRecuperationFactor, 0.f, MaxStamina);

  // If the value has actually changed, we need to notify any listeners
  if (CurrentStamina != PreviousStamina) {
    OnStaminaChanged.Broadcast(PreviousStamina, CurrentStamina, MaxStamina);
  }

  // Rest the flags indicating physical exertion
  bHasRan = false;
  bHasJumped = false;
#pragma endregion

#pragma region Update Psi Power
  // Nothing here needed to deduct from the current value
  // only restore it, and at a constant rate
  
  if (CurrentPsiPower != MaxPsiPower) {
    // Keep track of the value before
    const float PreviousPsiPower = CurrentPsiPower;

    CurrentPsiPower = FMath::Clamp(CurrentPsiPower+PsiRechargeRate, 0, MaxPsiPower);
    OnPsiPowerChanged.Broadcast(PreviousPsiPower, CurrentPsiPower, MaxPsiPower);
  }
#pragma endregion

  // Temp display debug information
  GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Silver,
  *(FString::Printf(TEXT("Movement - IsCrouched:%d | IsSprinting:%d"),bIsCrouched, bIsRunning)));
  GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Red,
  *(FString::Printf(TEXT("Health - Current:%d | Maximum:%d"),CurrentHealth, MaxHealth)));
  GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Green,
  *(FString::Printf(TEXT("Stamina - Current:%f | Maximum:%f"),CurrentStamina, MaxStamina)));
  GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Cyan,
  *(FString::Printf(TEXT("PsiPower - Current:%f | Maximum:%f"),CurrentPsiPower, MaxPsiPower)));
  GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Orange,
    *(FString::Printf(TEXT("Keys - %d keys currently held"),KeyWallet.Num())));
}

// Called to bind functionality to input
void ACharacterBB::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void ACharacterBB::SetRunning(bool IsRunning) {
  bIsRunning = IsRunning;

  GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunningMaxWalkSpeed : NormalMaxWalkSpeed;
}

void ACharacterBB::BroadcastCurrentStats()
{
  OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth, MaxHealth);
  OnStaminaChanged.Broadcast(CurrentStamina, CurrentStamina, MaxStamina);
  OnPsiPowerChanged.Broadcast(CurrentPsiPower, CurrentPsiPower, MaxPsiPower);

  // Make a string of all the keys
  // If there are ANY members, the string will end with a trailing comma ','
  // We dont care to remove that here, it doesnt matter.
  FString AllKeys = FString();
  for (FString Key : KeyWallet)
  {
    AllKeys.Appendf(TEXT("%s,"), &Key);
  }

  //OnKeyWalletAction.Broadcast(AllKeys, EPlayerKeyAction::CountKeys, true);
}

int ACharacterBB::GetHealth() { return CurrentHealth; }

int ACharacterBB::GetMaxHealth() { return MaxHealth;}

void ACharacterBB::UpdateHealth(int DeltaHealth) {
  // If already dead, their health cannot be modified again
  // This prevents multiple effects "stacking" and the player becoming
  // dead and instantly reviving.
  if (CurrentHealth <= 0.f) { return; }
  
  int OldValue = CurrentHealth;
  
  CurrentHealth += DeltaHealth;
  
  // Make sure the new CurrentHealth is inside an acceptable range
  // In this case it will never be < -1, or > MaxHealth
  CurrentHealth = FMath::Clamp(CurrentHealth, -1.f, MaxHealth);

  if (CurrentHealth != OldValue) {
    OnHealthChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
  }

  // Did the player just die?
  if (CurrentHealth <= 0.f) {
    // Player is dead! Do something!
    OnPlayerDied.Broadcast();
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
    OnKeyWalletAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, false);
  } else {
    KeyWallet.Add(KeyToAdd);
    // And maybe play a sound effect?
    OnKeyWalletAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, true);
  }
}

void ACharacterBB::RemoveKey(FString KeyToRemove) {
  if (KeyWallet.Contains(KeyToRemove)) {
    KeyWallet.Remove(KeyToRemove);
    OnKeyWalletAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, true);
  } else {
    OnKeyWalletAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, true);
  }
}

bool ACharacterBB::IsPlayerCarryingKey(FString DesiredKey) {
  bool Result = KeyWallet.Contains(DesiredKey);
  
  OnKeyWalletAction.Broadcast(DesiredKey, EPlayerKeyAction::TestKey, Result);
  
  return Result;
}

