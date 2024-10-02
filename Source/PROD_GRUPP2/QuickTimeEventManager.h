// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuickTimeEventManager.generated.h"

class USoundCue;



USTRUCT(BlueprintType)
struct FQtEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundCue* Audio;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundCue* initAudio;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float StartDelay = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TimeFrame = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActive = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsInit = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSucceeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROD_GRUPP2_API UQuickTimeEventManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UQuickTimeEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSucceeded OnSucceeded;

	UPROPERTY(BlueprintCallable,  BlueprintAssignable)
	FOnFailed OnFailed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FailSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SucceededSound;
	
	UFUNCTION(BlueprintCallable)
	bool AddQTEvent(FQtEvent Qt);

	UFUNCTION(BlueprintCallable)
	bool AddQtEventIter(FQtEvent Qt, int num);

	UFUNCTION(BlueprintCallable)
	bool AddAllQTEvent(TArray<FQtEvent> Qt);
private:

	


	UFUNCTION()
	void ReactionEvaluation();

	UFUNCTION(BlueprintCallable)
	void Update();

	void DoEventActions(FQtEvent& Qt);

	bool CheckEvent();

	void FailEvent();

	void SucceededEvent();
	
	TArray<FQtEvent> ListOfQTs;

	bool bKeyPressed = false;
	

};
