// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibraryA.h"

FString UBlueprintFunctionLibraryA::GetPointlessMessage() {
	return FString(TEXT("This is a message, but it is rather pointless."));
}

double UBlueprintFunctionLibraryA::GetMyPi() {
	return Pi;
}

FAverages UBlueprintFunctionLibraryA::CalculateAverages(const TArray<int32>& InValues) {

	/* Make a copy of the input array */
	TArray<int32> TmpArray = InValues;
	TMap<int32, int32> ModalCount;

	float CalcMean = 0.0f;
	float CalcModal = 0.0f;
	float CalcMedian = 0.0f;
	float Sum = 0;
	const int32 Count = TmpArray.Num();

	/* In case some empty array is passed in. */
	if (Count == 0) return FAverages(0.0f, 0.0f, 0.0f);

	TmpArray.Sort();

	for (int i = 0; i < Count; ++i) {
		Sum += TmpArray[i];

		if (ModalCount.Contains(TmpArray[i])) {
			ModalCount[TmpArray[i]] ++;		/* Value is already there, add 1 to it*/
		}
		else {
			ModalCount.Add(TmpArray[i], 1);	/* Value was not there, add it*/
		}

		CalcMean = Sum / static_cast<float>(Count);

		CalcMedian = Count % 2 != 0
			? TmpArray[Count / 2]
			: static_cast<float>((TmpArray[(Count / 2) - 1] + TmpArray[Count / 2])) / 2;

		/* Use a lambda compare func to help sort values from high to low*/
		ModalCount.ValueSort([](int32 A, int32 B) { return A > B; });
		TArray<TPair<int32, int32>> Modals = ModalCount.Array();

		if (Modals[0].Value == 1) {
			/* If the 1st key has a count (value) of 1, it means each key is unique, then just use the mean */
			CalcModal = CalcMean;
		}
		else {
			int32 ModalEntries = 1;
			int32 ModalSum = Modals[0].Key;

			/* The array ModalCount was sorted, element 0 is either the modal value,
			or the first in several of values who have the same frequency */
			for (int j = 1; j < Modals.Num(); ++j) {
				if (Modals[j].Value != Modals[0].Value) break;
				ModalSum += Modals[j].Key;
				ModalEntries++;
			}

			CalcModal = static_cast<float>(ModalSum) / static_cast<float>(ModalEntries);
		}
	}

	return FAverages(CalcMean, CalcModal, CalcMedian);
}