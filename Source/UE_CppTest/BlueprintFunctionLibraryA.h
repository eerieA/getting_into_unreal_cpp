// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionLibraryA.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAverages
{
	FAverages() :
		Mean(0.0f),
		Modal(0.0f),
		Median(0.0f) {};

	FAverages(float InMean, float InModal, float InMedian) :
		Mean(InMean),
		Modal(InModal),
		Median(InMedian) {};

	UPROPERTY(BlueprintReadOnly)
	float Mean = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Modal = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Median = 0.0f;

	GENERATED_BODY()
};

UCLASS()
class UE_CPPTEST_API UBlueprintFunctionLibraryA : public UBlueprintFunctionLibrary
{
public:
	UFUNCTION(BlueprintCallable)
	static FString GetPointlessMessage();

	UFUNCTION(BlueprintCallable, BlueprintPure,
		meta = (Tooltip = "3.141blahblah", CompactNodeTitle = "π"))
	static double GetMyPi();

	UFUNCTION(BlueprintCallable,
		meta = (Tooltip = "Calculate mean, mode and median for an array of integers."))
	static FAverages CalculateAverages(const TArray<int32>& InValues);

protected:

private:
	static constexpr double Pi = 3.1415926535897932384626433832795;
	GENERATED_BODY()
	
};
