// Fill out your copyright notice in the Description page of Project Settings.


#include "AAudioController.h"

AAudioController::AAudioController()
{
	// Initialize the active audio components array and currently playing sound
	ActiveAudioComponents = TArray<UAudioComponent*>();
	CurrentSoundCue = nullptr; // Initialize to nullptr
}

void AAudioController::BeginPlay()
{
	Super::BeginPlay();
}

void AAudioController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAudioController::PlayOrStopSound(USoundBase* SoundToPlay, bool bShouldPlay)
{
	if(!SoundToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
		return;
	}

	// Cast to USoundCue to ensure it's the correct type
	USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay);

	if(bShouldPlay)
	{
		if(CurrentSoundCue != SoundCue)
		{
			StopCurrentSound();
		}

		// Spawn and play the new sound
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);
		if (AudioComponent)
		{
			AudioComponent->Play();
			ActiveAudioComponents.Add(AudioComponent); // Add to active components
			CurrentSoundCue = SoundCue; // Update current sound cue
			UE_LOG(LogTemp, Warning, TEXT("Playing"));
		}
	}
	else
	{
		if(CurrentSoundCue == SoundCue)
		{
			StopCurrentSound();
		}
	}
}

void AAudioController::StopCurrentSound()
{
	if (CurrentSoundCue)
	{
		// Stop all active audio components
		for (UAudioComponent* ActiveComponent : ActiveAudioComponents)
		{
			if (ActiveComponent)
			{
				ActiveComponent->Stop();
			}
		}
		ActiveAudioComponents.Empty(); // Clear the array
		CurrentSoundCue = nullptr; // Reset current sound cue
		UE_LOG(LogTemp, Warning, TEXT("Stopped Current Sound"));
	}
}
