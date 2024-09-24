// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PuzzleData.generated.h"    // Include generated header


// Struct
USTRUCT(BlueprintType)
struct PROD_GRUPP2_API FPuzzleData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle")
	FName PuzzleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle")
	bool bIsCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle")
	bool bIsActive;

	//Constructor to initialize default values
	FPuzzleData()
	{
		PuzzleName = "DefaultPuzzle";
		bIsCompleted = false;
		bIsActive = false;
	}
};


