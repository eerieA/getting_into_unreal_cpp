// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBBBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_CPPTEST_API UWidgetBBBase : public UUserWidget {
public:
#if WITH_EDITOR
  virtual const FText GetPaletteCategory() override;
#endif
  
protected:

private:
  GENERATED_BODY()
};
