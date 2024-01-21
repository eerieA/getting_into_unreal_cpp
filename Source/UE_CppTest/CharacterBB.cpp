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

