// Fill out your copyright notice in the Description page of Project Settings.

#include "HudBB.h"

void AHudBB::BeginPlay() { Super::BeginPlay(); }

void AHudBB::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  Super::EndPlay(EndPlayReason);
}

void AHudBB::SetCurrentViewMode(EHudViewMode NewViewMode) {}

void AHudBB::CycleToNextViewMode() {
  CurrentViewMode = static_cast<EHudViewMode>( (static_cast<int>(CurrentViewMode) + 1) % 4);
  UE_LOG(LogTemp, Warning, TEXT("*UEnum::GetValueAsString(CurrentViewMode): %s"), *UEnum::GetValueAsString(CurrentViewMode));
  
  UpdateWidgets();
}

void AHudBB::UpdateWidgets() {}

void AHudBB::ClearAllHandlers() {}