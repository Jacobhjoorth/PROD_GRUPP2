// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleData.h"
#include "PuzzleManager.generated.h"

UCLASS()
class PROD_GRUPP2_API APuzzleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleManager();

	// Array that holds the puzzle data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle")
	TArray<FPuzzleData> PuzzleList;

	// Function to mark a puzzle as completed
	UFUNCTION(BlueprintCallable, Category="Puzzle")
	void CompletePuzzle(FName PuzzleName);

	// Function to check if a puzzle is completed
	UFUNCTION(BlueprintCallable, Category="Puzzle")
	bool IsPuzzleCompleted(FName PuzzleName) const;

	// Function to check if a puzzle is completed
	UFUNCTION(BlueprintCallable, Category="Puzzle")
	void StartPuzzle(FName PuzzleName) ;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
