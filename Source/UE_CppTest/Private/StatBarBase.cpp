// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBarBase.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UStatBarBase::NativeOnInitialized() {
  Super::NativeOnInitialized();
  UE_LOG(LogTemp, Warning, TEXT("NativeOnInitialized()"));
  UpdateWidget();
}

void UStatBarBase::OnIntStatUpdated(int32 OldValue, int32 NewValue,
                                    int32 MaxValue) {
  // Just use the float version of the function
  OnFloatStatUpdated(static_cast<float>(OldValue), static_cast<float>(NewValue),
                     static_cast<float>(MaxValue));
}

void UStatBarBase::OnFloatStatUpdated(float OldValue, float NewValue,
                                      float MaxValue) {
  // For now we can ignore the OldValue, this will be used for animating the bar

  // Calculate the new percentage, and clamp the value between 0 and 1

  // Prevent dividing by zero errors. KINDA_SMALL_NUMBER is official constant name.
  if (MaxValue == 0.f) MaxValue = KINDA_SMALL_NUMBER;

  CurrentPercentage = FMath::Clamp(NewValue / MaxValue, 0.f, 1.f);
  CurrentValue = NewValue;
  UpdateWidget();
}

void UStatBarBase::ProcessCurrentValueText() {
  FString FloatString;

  if (CurrentValue < 1000.f) {
    FloatString = FString::SanitizeFloat(CurrentValue);

    if (CurrentValue < 100.f) {
      int32 Stringlen = FloatString.Len();
      if (Stringlen > 4) {
        FloatString = FloatString.Left(4);
      } else if (Stringlen < 4) {
        FloatString = FloatString.Append("0", 4 - Stringlen);
      }
    } else {
      FloatString = FloatString.Left(3);
    }
  } else {
    // scaled value
    float ScaledValue = CurrentValue / 1000.f;
    FloatString = FString::SanitizeFloat(ScaledValue);
    if (ScaledValue < 10.f) {
      FloatString = FloatString.Left(3).Append(TEXT("k"));
    } else {
      FloatString = FloatString.Left(2).Append(TEXT("k"));
    }
  }

  CurrentValueText = FText::FromString(FloatString);
}

void UStatBarBase::UpdateWidget() {
  // Check that the controls we want actually exist
  if (!PercentBar_Filled || !PercentBar_Empty) return;

  FSlateChildSize EmptySize = FSlateChildSize(ESlateSizeRule::Fill);
  EmptySize.Value = 1.f - CurrentPercentage;
  
  FSlateChildSize FilledSize = FSlateChildSize(ESlateSizeRule::Fill);
  EmptySize.Value = CurrentPercentage;

  if (UVerticalBoxSlot* FilledSlot = Cast<UVerticalBoxSlot>(PercentBar_Filled->Slot)) {
    FilledSlot->SetSize(FilledSize);
  }
  
  if (UVerticalBoxSlot* EmptySlot = Cast<UVerticalBoxSlot>(PercentBar_Empty->Slot)) {
    EmptySlot->SetSize(EmptySize);
  }

  MainBorder->SetBrushColor(BarBackgroundColor);
  PercentBar_Filled->SetBrushColor(BarForegroundColor);
  IconImage->SetBrush(IconBrush);

  ProcessCurrentValueText();

  ValueText->SetText(CurrentValueText);

  PercentBars->SetVisibility(IsFullSize ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

#if WITH_EDITOR

void UStatBarBase::OnDesignerChanged(const FDesignerChangedEventArgs &EventArgs) {
  Super::OnDesignerChanged(EventArgs);
  // Update the widget, after editor changes due to layout
  // for example, resizing the widget, or a container that the widget is in
  UpdateWidget();
}

void UStatBarBase::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
  Super::PostEditChangeProperty(PropertyChangedEvent);

  // Update the widget, after its properties have been changed
  // In this case it would be sufficient to just call UpdateWidget()
  // but the following code demonstrates one way, you could run specific code
  // when individual properties are changed.

  const FString PropertyName = ((PropertyChangedEvent.Property != nullptr)
                                ? PropertyChangedEvent.Property->GetFName()
                                : NAME_None).ToString();

  if (PropertyName == TEXT("CurrentPercentage") || PropertyName == TEXT("CurrentValue")) {
    UpdateWidget();
  }
}

#endif
