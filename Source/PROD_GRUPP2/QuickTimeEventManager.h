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
	float StartDelay = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TimeFrame = 0;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FailSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SucceededSound;

	bool AddQTEvent(FQtEvent Qt);

private:

	UFUNCTION()
	void ReactionEvaluation(float Value);

	UFUNCTION(BlueprintCallable)
	void Update();

	bool CheckEvent();

	void FailEvent();

	void SucceededEvent();
	
	TArray<FQtEvent> ListOfQTs;

	bool bKeyPressed = false;
	

};
