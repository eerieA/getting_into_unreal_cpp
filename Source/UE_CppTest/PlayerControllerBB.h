// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HudBB.h"
#include "InputActionValue.h"
#include "PlayerControllerBB.generated.h"

// Forward declarations
class UInputComponent;
class UInputAction;
class UEnhancedInputComponent;
class ACharacterBB;
class UInputMappingContext;

UCLASS(Abstract)
class UE_CPPTEST_API APlayerControllerBBBase : public APlayerController {

public:
  // The Input Action to map to movement
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionMove = nullptr;
  
  // The Input Action to map to looking around
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionLook = nullptr;
  
  // The Input Action to map to jump
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionJump = nullptr;

  // The Input Action to map to crouch
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionToggleCrouch = nullptr;

  // The Input Action to map to sprint
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionToggleSprint = nullptr;
  
  // The Input Action to map to psi blast
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputAction> ActionPsiBlast = nullptr;
  
  // The Input Action to map to cycle UI viewmode
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|UI")
  TObjectPtr<UInputAction> ActionCycleUIMode = nullptr;
  
  // The Input Mapping Context to use
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
  TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
  
protected:
  virtual void OnPossess(APawn *aPawn) override;
  virtual void OnUnPossess() override;
  void HandleMove(const FInputActionValue& InputActionValue);
  void HandleLook(const FInputActionValue& InputActionValue);
  void HandleJump();
  void HandlePsiBlast();
  void HandleToggleSprint();
  void HandleToggleCrouch();
  void HandleCycleUIMode();

private:
  
  // Used to store a reference to the InputComponent cast to an Enhanced Inputcomponent
  UPROPERTY()
  TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;
  
  // Used to store a reference to the pawn we are controlling
  UPROPERTY()
  TObjectPtr<ACharacterBB> PlayerCharacter = nullptr;
  
  // Used to store a reference to the pawn we are controlling
  UPROPERTY()
  TObjectPtr<AHudBB> PlayerHud = nullptr;
  
  GENERATED_BODY()
};
