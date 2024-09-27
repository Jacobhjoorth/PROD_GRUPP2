#include "AAudioController.h"
#include "Sound/SoundMix.h"  // Include the necessary header for SoundMix

AAudioController::AAudioController()
{
	// Initialize the active audio components array and currently playing sound
	ActiveVoiceLines = TArray<UAudioComponent*>();
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

void AAudioController::PlayVoiceLine(USoundBase* SoundToPlay)
{
	if(!SoundToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
		return;
	}

	// Cast to USoundCue to ensure it's the correct type
	USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay);
	
	// Spawn and play the new sound
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);
	
	if (AudioComponent)
	{
		// Play the audio component
		AudioComponent->Play();
		ActiveVoiceLines.Add(AudioComponent); // Add to active components
		CurrentSoundCue = SoundCue; // Update current sound cue

		// Adjust the sound class volumes
		//AdjustSoundClassVolume(Ambient, 0.1f);
		//AdjustSoundClassVolume(FX, 0.1f);

		// Bind the function to restore volume when the sound finishes
		//AudioComponent->OnAudioFinished.AddDynamic(this, &AAudioController::RestoreSoundClassVolume);
	}
}

void AAudioController::StopCurrentVoiceLine()
{
	// Stop all active audio components
	for (UAudioComponent* ActiveComponent : ActiveVoiceLines)
	{
		if (ActiveComponent)
		{
			ActiveComponent->Stop();
		}
	}
	ActiveVoiceLines.Empty(); // Clear the array
	CurrentSoundCue = nullptr; // Reset current sound cue
}

void AAudioController::AdjustSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	// Apply the sound mix override for the sound class
	UGameplayStatics::SetSoundMixClassOverride(this, nullptr, SoundClass, Volume, 1.0f, 0.0f, true);
}

void AAudioController::RestoreSoundClassVolume()
{
	// Restore volumes to original levels when the audio finishes
	AdjustSoundClassVolume(Ambient, 1.0f);
	AdjustSoundClassVolume(FX, 1.0f);
}
