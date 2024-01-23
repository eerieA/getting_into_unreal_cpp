// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBB.generated.h"

UCLASS()
class UE_CPPTEST_API ACharacterBB : public ACharacter {
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ACharacterBB();
  
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;

  // Return the player's current health
  UFUNCTION(BlueprintPure, Category="Player|Health")
  int GetHealth();
  
  // Return the player's max health
  UFUNCTION(BlueprintPure, Category="Player|Health")
  int GetMaxHealth();

  // Modify the player's health by the specified amount
  UFUNCTION(BlueprintCallable, Category="Player|Health")
  void UpdateHealth(int DeltaHealth);
  
  // Set the player's current health to maximum allowable
  UFUNCTION(BlueprintCallable, Category="Player|Health")
  void RestoreToFullHealth();
  
  // Set the maximum allowable health
  UFUNCTION(BlueprintCallable, Category="Player|Health")
  void SetMaxHealth(int NewMaxHealth);
  
  // Return the player's current stamina
  UFUNCTION(BlueprintPure, Category="Player|Stamina")
  float GetStamina();
  
  // Return the player's current stamina recuper factor
  UFUNCTION(BlueprintPure, Category="Player|Stamina")
  float GetStaminaRecuperationFactor();
  
  UFUNCTION(BlueprintCallable, Category="Player|Stamina")
  void SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor);
  
  // Return the player's current psi power
  UFUNCTION(BlueprintPure, Category="Player|PsiPower")
  float GetPsiPower();
  
  // Player unleashes a devastating blast of mind power
  UFUNCTION(BlueprintCallable, Category="Player|Stamina")
  void PsiBlast();
  
  // Add a key to the wallet if it is not already there
  UFUNCTION(BlueprintCallable, Category="Player|KeyWallet")
  void AddKey(FString KeyToAdd);
  
  // Remove a key (maybe will not happen in this practice game)
  // But if the key is not in the wallet, do nothing
  UFUNCTION(BlueprintCallable, Category="Player|KeyWallet")
  void RemoveKey(FString KeyToRemove);
  
  // Does the player have a given key
  // Return true if they do, false if they do not
  UFUNCTION(BlueprintPure, Category="Player|KeyWallet")
  bool IsPlayerCarryingKey(FString DesiredKey);
  
protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

private:
  
  // Health
  static constexpr int BaseStatValue = 100;
  int MaxHealth = BaseStatValue;
  int CurrentHealth = BaseStatValue;

  // Stamina
  static constexpr float MaxStamina = 100.0f;
  static constexpr float JumpStaminaCost = 25.0f;
  static constexpr float RunStaminaCost = 5.0f;
  static constexpr float RestStaminaRebate = 4.0f;
  float CurrentStamina = MaxStamina;
  float StaminaRecuperationFactor = 1.0f;
  
  // Psi Power
  static constexpr float MaxPsiPower = 1000.0f;
  static constexpr float PsiRechargeRate = 1.0f;
  static constexpr float PsiBlastCost = 150.0f;
  float CurrentPsiPower = MaxPsiPower;
  
  // Player Keys
  TArray<FString> KeyWallet;
};
