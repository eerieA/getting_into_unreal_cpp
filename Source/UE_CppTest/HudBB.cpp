// Fill out your copyright notice in the Description page of Project Settings.

#include "HudBB.h"

#include "CharacterBB.h"
#include "HSPBarBase.h"
#include "StatBarBase.h"

void AHudBB::BeginPlay() {
  Super::BeginPlay();

  World = GetWorld();
  checkf(World, TEXT("Failed to reference world."));

  // Ensure we have valid values for the 3 classes of widget used by the HUD
  checkf(MinimalLayoutClass, TEXT("Invalid MinimalLayoutClass reference."));
  checkf(ModerateLayoutClass, TEXT("Invalid ModerateLayoutClass reference."));
  checkf(OverloadLayoutClass, TEXT("Invalid OverloadLayoutClass reference."));
  
  // Create the 3 types of layout widget, and add them to the viewport
  // We could have been 'clever' here, and had maybe a single widget which "morphs"
  // based on the requirements, but this is just a tutorial
  // When creating a widget, the first parameter (owning object) must be one of the following types:
  //   UWidget, UWidgetTree, APlayerController, UGameInstance, or UWorld
  MinimalLayoutWidget = CreateWidget<UMinimalLayoutBase>(World, MinimalLayoutClass);
  MinimalLayoutWidget->AddToViewport();
  MinimalLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

  ModerateLayoutWidget = CreateWidget<UModerateLayoutBase>(World, ModerateLayoutClass);
  ModerateLayoutWidget->AddToViewport();
  ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

  OverloadLayoutWidget = CreateWidget<UOverloadLayoutBase>(World, OverloadLayoutClass);
  OverloadLayoutWidget->AddToViewport();
  OverloadLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

  // Get a reference to the character, and hook up the stat handlers
  if (APlayerController* PlayerController = GetOwningPlayerController()) {
    PlayerCharacter = Cast<ACharacterBB>(PlayerController->GetPawn());
  }
  checkf(PlayerCharacter, TEXT("Unable to get a reference to the player character."));

  UpdateWidgets();
}

void AHudBB::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  Super::EndPlay(EndPlayReason);
}

void AHudBB::SetCurrentViewMode(EHudViewMode NewViewMode) {}

void AHudBB::CycleToNextViewMode() {
  CurrentViewMode = static_cast<EHudViewMode>( (static_cast<int>(CurrentViewMode) + 1) % 4);
  UE_LOG(LogTemp, Warning, TEXT("*UEnum::GetValueAsString(CurrentViewMode): %s"), *UEnum::GetValueAsString(CurrentViewMode));
  
  UpdateWidgets();
}

void AHudBB::UpdateWidgets() {
  ClearAllHandlers();

  // Set all the widgets so we see none of them first
  MinimalLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);
  ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);
  OverloadLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

  switch (CurrentViewMode) {
  case EHudViewMode::CleanAndPristine:
    break;
  case EHudViewMode::Minimal:
    // Link the necessary delegates and set the relevant widget visible
    PlayerCharacter->OnHealthChanged.AddDynamic(MinimalLayoutWidget->HSPBar->HealthBar,
    &UStatBarBase::OnIntStatUpdated);
    PlayerCharacter->OnStaminaChanged.AddDynamic(MinimalLayoutWidget->HSPBar->StaminaBar,
    &UStatBarBase::OnFloatStatUpdated);
    PlayerCharacter->OnPsiPowerChanged.AddDynamic(MinimalLayoutWidget->HSPBar->PsiBar,
    &UStatBarBase::OnFloatStatUpdated);
    MinimalLayoutWidget->SetVisibility(ESlateVisibility::Visible);
    break;
  case EHudViewMode::Moderate:
    // Link the necessary delegates and set the relevant widget visible
    PlayerCharacter->OnHealthChanged.AddDynamic(ModerateLayoutWidget->HSPBar->HealthBar,
    &UStatBarBase::OnIntStatUpdated);
    PlayerCharacter->OnStaminaChanged.AddDynamic(ModerateLayoutWidget->HSPBar->StaminaBar,
    &UStatBarBase::OnFloatStatUpdated);
    PlayerCharacter->OnPsiPowerChanged.AddDynamic(ModerateLayoutWidget->HSPBar->PsiBar,
    &UStatBarBase::OnFloatStatUpdated);
    ModerateLayoutWidget->SetVisibility(ESlateVisibility::Visible);
    break;
  case EHudViewMode::SensoryOverload:
    // Link the necessary delegates and set the relevant widget visible
    PlayerCharacter->OnHealthChanged.AddDynamic(OverloadLayoutWidget->HSPBar->HealthBar,
    &UStatBarBase::OnIntStatUpdated);
    PlayerCharacter->OnStaminaChanged.AddDynamic(OverloadLayoutWidget->HSPBar->StaminaBar,
    &UStatBarBase::OnFloatStatUpdated);
    PlayerCharacter->OnPsiPowerChanged.AddDynamic(OverloadLayoutWidget->HSPBar->PsiBar,
    &UStatBarBase::OnFloatStatUpdated);
    OverloadLayoutWidget->SetVisibility(ESlateVisibility::Visible);
    break;
  default: ;
  }

  PlayerCharacter->BroadcastCurrentStats();
}

void AHudBB::ClearAllHandlers() {
  if (PlayerCharacter) {
    // Clear all delegates defined as possible in the CharacterBB class
    PlayerCharacter->OnHealthChanged.Clear();
    PlayerCharacter->OnStaminaChanged.Clear();
    PlayerCharacter->OnPsiPowerChanged.Clear();
    PlayerCharacter->OnKeyWalletAction.Clear();
  }
}