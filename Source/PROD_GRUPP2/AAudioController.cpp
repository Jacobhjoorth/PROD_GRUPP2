#include "AAudioController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AAudioController::AAudioController()
{
    // Initialize the active audio components array and currently playing sound
    ActiveVoiceLines = TArray<UAudioComponent*>();
    CurrentSoundCue = nullptr; // Initialize to nullptr
    bSoundIsTriggered = false;

    // Load SoundClass and SoundMix used for volume changes
    AmbientSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Audio/Audio_Classes/Ambient.Ambient"));
    FXSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Audio/Audio_Classes/FX.FX"));
    MasterSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Audio/Audio_Classes/Master.Master"));

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
    UE_LOG(LogTemp, Warning, TEXT("Attempting to play voice line"));

    // Check if the provided sound is valid
    if (!SoundToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
        return; 
    }
    
    // Cast to USoundCue to ensure it's the correct type
    USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay);

    if (!SoundCue)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast SoundBase to SoundCue"));
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Successfully casted to SoundCue"));

    // Spawn and play the new sound
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);

    if (AudioComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Playing voice line"));

        // Bind the OnAudioFinished delegate to restore volume
        AudioComponent->OnAudioFinished.AddDynamic(this, &AAudioController::RestoreSoundClassVolume);

        // Play the audio component
        AudioComponent->Play();
        ActiveVoiceLines.Add(AudioComponent); // Add to active components
        CurrentSoundCue = SoundCue; // Update current sound cue

        // Lower the volume of the ambient and FX sound classes
        AdjustSoundClassVolume(AmbientSoundClass, 0.2f);
        AdjustSoundClassVolume(FXSoundClass, 0.2f);
        bSoundIsTriggered = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AudioComponent or Sound is already triggered"));
    }
}

void AAudioController::StopCurrentVoiceLine()
{
    UE_LOG(LogTemp, Warning, TEXT("Stopping current voice line"));

    // Stop all active audio components
    for (UAudioComponent* ActiveComponent : ActiveVoiceLines)
    {
        if (ActiveComponent)
        {
            ActiveComponent->Stop();
            UE_LOG(LogTemp, Warning, TEXT("Stopped Active AudioComponent"));

            // Unbind the OnAudioFinished delegate to prevent calling RestoreSoundClassVolume after stopping
            ActiveComponent->OnAudioFinished.RemoveDynamic(this, &AAudioController::RestoreSoundClassVolume);
            bSoundIsTriggered = false;
        }
    }

    ActiveVoiceLines.Empty(); // Clear the array
    CurrentSoundCue = nullptr; // Reset current sound cue

    // Restore the sound classes to normal after stopping
    RestoreSoundClassVolume();
}

void AAudioController::AdjustSoundClassVolume(USoundClass* SoundClass, float Volume) const
{
    if (!SoundMix)
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundMix is null, cannot adjust volume"));
        return;
    }

    if (SoundClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Adjusting volume for SoundClass: %s, New Volume: %f"), *SoundClass->GetName(), Volume);

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

        if (SoundClass == MasterSoundClass)
        {
            AdjustSoundClassVolume(FXSoundClass, Volume);   // Adjust FX volume
            AdjustSoundClassVolume(AmbientSoundClass, Volume);  // Adjust Ambient volume
            // Adjust other children sound classes as needed
        }
        
        UE_LOG(LogTemp, Warning, TEXT("SoundMix applied successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundClass is null, cannot adjust volume"));
    }
}

void AAudioController::RestoreSoundClassVolume()
{
    UE_LOG(LogTemp, Warning, TEXT("Restoring sound class volume to default"));

    PlayFeedbackSound(false);

    // Restore the sound classes to normal volume
    AdjustSoundClassVolume(AmbientSoundClass, 1.0f);
    AdjustSoundClassVolume(FXSoundClass, 1.0f);
    bSoundIsTriggered = false;
}

void AAudioController::PlayFeedbackSound(bool IsStartSound)
{
    if(IsStartSound)
    {
        USoundBase* FeedbackSoundStart = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Audio/Sounds/FX/StartAudio.StartAudio")));
        if (FeedbackSoundStart)
        {
            UE_LOG(LogTemp, Warning, TEXT("Playing feedback start sound"));
            UGameplayStatics::SpawnSound2D(this, FeedbackSoundStart); 
        } 
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load FeedbackStartSound"));
        }
    }
    else
    {
        USoundBase* FeedbackSoundStop = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Audio/Sounds/FX/StopAudio.StopAudio")));
        if (FeedbackSoundStop)
        {
            UE_LOG(LogTemp, Warning, TEXT("Playing feedback stop sound"));
            UGameplayStatics::SpawnSound2D(this, FeedbackSoundStop);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load FeedbackStopSound"));
        }
    }
}

float AAudioController::ChangeVolume(USoundClass* SoundClass, bool ShouldTurnUpVolume)
{
    if (!SoundClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundClass is null, cannot change volume"));
        return 0;
    }

    if (!SoundMix)
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundMix is null, cannot adjust volume"));
        return 0;
    }

    // Get the current volume from the map, or use a default of 1.0 if not found
    float* CurrentVolumePtr = SoundClassVolumes.Find(SoundClass);
    if (!CurrentVolumePtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundClass not found in volume map, initializing to 1.0f"));
        CurrentVolumePtr = &SoundClassVolumes.Add(SoundClass, 1.0f);
    }

    // Adjust the volume
    float CurrentVolume = *CurrentVolumePtr;
    float VolumeChange = ShouldTurnUpVolume ? 0.1f : -0.1f;
    float NewVolume = FMath::Clamp(CurrentVolume + VolumeChange, 0.0f, 1.0f);

    // Update the map with the new volume
    SoundClassVolumes[SoundClass] = NewVolume;

    // Apply the volume change
    AdjustSoundClassVolume(SoundClass, NewVolume);

    UE_LOG(LogTemp, Warning, TEXT("Changed volume for %s to %f"), *SoundClass->GetName(), NewVolume);

    return NewVolume;
}




    

