// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"
#include "AAudioController.generated.h"


UCLASS()
class PROD_GRUPP2_API AAudioController : public AActor
{ 
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAudioController();

	UFUNCTION(BlueprintCallable, Category="Audio")
	void PlayOrStopSound(USoundBase* SoundToPlay, bool bShouldPlay);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopCurrentSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UAudioComponent*> ActiveAudioComponents; // Array to track active audio components
	USoundBase* CurrentSoundCue; // Currently playing sound cue
};
