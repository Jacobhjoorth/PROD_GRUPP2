// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "AAudioController.generated.h"

UCLASS()
class PROD_GRUPP2_API AAudioController : public AActor
{ 
	GENERATED_BODY()
	
public:
	AAudioController();

	UFUNCTION(BlueprintCallable, Category="Audio")
	void PlayVoiceLine(USoundBase* SoundToPlay);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopCurrentVoiceLine();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayFeedbackSound(bool IsStartSound);
	
	void AdjustSoundClassVolume(USoundClass* SoundClass, float Volume) const;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void RestoreSoundClassVolume();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	float ChangeVolume(USoundClass* SoundClass, bool ShouldTurnUpVolume);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bSoundIsTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio");
	USoundMix* SoundMix;

private:
	UPROPERTY()
	TArray<UAudioComponent*> ActiveVoiceLines; // Array to track active audio components

	UPROPERTY()
	USoundBase* CurrentSoundCue; // Currently playing sound cue

	// Load SoundClass and SoundMix used for volume changes
	UPROPERTY()
	USoundClass* AmbientSoundClass;

	UPROPERTY()
	USoundClass* FXSoundClass;

	UPROPERTY()
	USoundClass* MasterSoundClass;

	UPROPERTY()
	TMap<USoundClass*, float> SoundClassVolumes;
};
