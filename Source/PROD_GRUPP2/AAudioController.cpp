#include "AAudioController.h"

AAudioController::AAudioController()
{
    // Initialize the active audio components array and currently playing sound
    ActiveVoiceLines = TArray<UAudioComponent*>();
    CurrentSoundCue = nullptr; // Initialize to nullptr

    // Load SoundClass and SoundMix used for volume changes
    AmbientSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Audio/Audio_Classes/Ambient.Ambient"));
    FXSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Audio/Audio_Classes/FX.FX"));
    SoundMix = NewObject<USoundMix>();
}

void AAudioController::BeginPlay()
{
    Super::BeginPlay();
}

void AAudioController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AAudioController::PlayVoiceLine(USoundBase* SoundToPlay)
{
    if (!SoundToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
        return false;
    }

    // Cast to USoundCue to ensure it's the correct type
    USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay);
    
    // Spawn and play the new sound
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);
    
    // Bind the OnAudioFinished delegate to restore volume
    AudioComponent->OnAudioFinished.AddDynamic(this, &AAudioController::RestoreSoundClassVolume);

    if (AudioComponent)
    {
        // Play the audio component
        AudioComponent->Play();
        ActiveVoiceLines.Add(AudioComponent); // Add to active components
        CurrentSoundCue = SoundCue; // Update current sound cue

        // Lower the volume of the ambient and FX sound classes
        AdjustSoundClassVolume(AmbientSoundClass, 0.2f);
        AdjustSoundClassVolume(FXSoundClass, 0.2f);
        return true;
    }
    return false;
}

void AAudioController::StopCurrentVoiceLine()
{
    // Stop all active audio components
    for (UAudioComponent* ActiveComponent : ActiveVoiceLines)
    {
        if (ActiveComponent)
        {
            ActiveComponent->Stop();
            // Unbind the OnAudioFinished delegate to prevent calling RestoreSoundClassVolume after stopping
            ActiveComponent->OnAudioFinished.RemoveDynamic(this, &AAudioController::RestoreSoundClassVolume);
        }
    }

    ActiveVoiceLines.Empty(); // Clear the array
    CurrentSoundCue = nullptr; // Reset current sound cue

    // Restore the sound classes to normal after stopping
    RestoreSoundClassVolume();
}

void AAudioController::AdjustSoundClassVolume(USoundClass* SoundClass, float Volume) const
{
    if (SoundClass)
    {
        // Set up the sound class adjuster to modify the volume
        FSoundClassAdjuster Adjuster;
        Adjuster.SoundClassObject = SoundClass;
        Adjuster.VolumeAdjuster = Volume;

        // Clear existing effects for this sound class
        SoundMix->SoundClassEffects.RemoveAll([SoundClass](const FSoundClassAdjuster& InAdjuster) {
            return InAdjuster.SoundClassObject == SoundClass;
        });

        // Add the adjuster to the sound mix
        SoundMix->SoundClassEffects.Add(Adjuster);

        // Apply the sound mix modifier to the world
        UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
    }
}

void AAudioController::RestoreSoundClassVolume()
{
    // Restore the sound classes to normal volume
    AdjustSoundClassVolume(AmbientSoundClass, 1.0f);
    AdjustSoundClassVolume(FXSoundClass, 1.0f);
}

