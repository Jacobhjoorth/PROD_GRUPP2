// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleManager.h"
#include "PuzzleData.h"

// Sets default values
APuzzleManager::APuzzleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleManager::CompletePuzzle(FName PuzzleName)
{
	for(FPuzzleData& Puzzle : PuzzleList)
	{
		if(Puzzle.PuzzleName == PuzzleName)
		{
			Puzzle.bIsCompleted = true;
			UE_LOG(LogTemp, Warning, TEXT("Puzzle %s marked as completed"), *PuzzleName.ToString());
			break;
		}
	}
}

bool APuzzleManager::IsPuzzleCompleted(FName PuzzleName) const
{
	for(const FPuzzleData& Puzzle : PuzzleList)
	{
		if(Puzzle.PuzzleName == PuzzleName)
		{
			return Puzzle.bIsCompleted;
		}
	}
	return false;
}



