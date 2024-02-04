// Fill out your copyright notice in the Description page of Project Settings.

#include "UE_CppTest/Public/WidgetBBBase.h"

#if WITH_EDITOR
const FText UWidgetBBBase::GetPaletteCategory() {
  return NSLOCTEXT("UMG", "CustomPaletteCategory", "CppTestDev!");
}
#endif