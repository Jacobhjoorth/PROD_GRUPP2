// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QuickTimeEventManager.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuickTimeEventSubSystem.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogGameActions, Log, All);



/**
 * 
 */
UCLASS()
class PROD_GRUPP2_API UQuickTimeEventSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Start();


	void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	void Clear();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSucceeded OnSucceeded;

	UPROPERTY(BlueprintCallable,  BlueprintAssignable)
	FOnFailed OnFailed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FailSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SucceededSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform InitialPlayerTransform;

	UFUNCTION(BlueprintCallable)
	void LogToText(FString string, int PlayNum);
	
	UFUNCTION(BlueprintCallable)
	bool AddQTEvent(FQtEvent Qt);

	UFUNCTION(BlueprintCallable)
	bool AddQtEventIter(FQtEvent Qt, int num);

	UFUNCTION(BlueprintCallable)
	bool AddAllQTEvent(TArray<FQtEvent> Qt);

private:

	
   void LogTextDataToFile();

	UFUNCTION(BlueprintCallable)
	void ReactionEvaluation();

	UFUNCTION(BlueprintCallable)
	void Update();

	void DoEventActions(FQtEvent& Qt);

	bool CheckEvent();

	void FailEvent();

	void SucceededEvent();
	
	TArray<FQtEvent> ListOfQTs;

	bool bKeyPressed = false;

	FString Data; 


};
