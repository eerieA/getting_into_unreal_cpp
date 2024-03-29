﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBBBase.h"
#include "ModerateLayoutBase.generated.h"

class UHSPBarBase;
class UImage;

/**
 * 
 */
UCLASS(Abstract)
class UE_CPPTEST_API UModerateLayoutBase : public UWidgetBBBase {
public:
  UPROPERTY(BlueprintReadOnly, Category="Constituent Controls", meta=(BindWidget))
  TObjectPtr<UHSPBarBase> HSPBar = nullptr;
  
  UPROPERTY(BlueprintReadOnly, Category="Constituent Controls", meta=(BindWidget))
  TObjectPtr<UImage> Crosshair = nullptr;
  
private:
  GENERATED_BODY()
};
