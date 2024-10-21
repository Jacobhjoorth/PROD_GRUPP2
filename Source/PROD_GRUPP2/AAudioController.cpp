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

    // Check if the sound classes were loaded successfully
    if (!AmbientSoundClass || !FXSoundClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load sound classes"));
    }

    SoundMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/Audio/Audio_Classes/FX_Mix.FX_Mix"));

    // Check if SoundMix was created successfully
    if (!SoundMix)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create SoundMix"));
    }
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
    // Check if the provided sound is valid
    if (!SoundToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
        return; // Return early if no sound
    }

    StopCurrentVoiceLine();
    
    // Cast to USoundCue to ensure it's the correct type
    USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay);

    if (!SoundCue)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast SoundBase to SoundCue"));
        return;
    }

    // Spawn and play the new sound
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);

    if (AudioComponent && !bSoundIsTriggered)
    {
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
        UE_LOG(LogTemp, Warning, TEXT("SoundMix is null"));
        return;
    }

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
    PlayFeedbackSound(false);

    // Restore the sound classes to normal volume
    AdjustSoundClassVolume(AmbientSoundClass, 1.0f);
    AdjustSoundClassVolume(FXSoundClass, 1.0f);
    bSoundIsTriggered = false;
}

void AAudioController::PlayFeedbackSound(bool IsStartSound)
{
    // Find and play the sound using its asset path
    if(IsStartSound)
    {
        USoundBase* FeedbackSoundStart = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Audio/Sounds/FX/StartAudio.StartAudio")));
        if (FeedbackSoundStart)
        {
            UGameplayStatics::SpawnSound2D(this, FeedbackSoundStart); 
        } 
    }else
    {
        USoundBase* FeedbackSoundStop = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Audio/Sounds/FX/StopAudio.StopAudio")));
        if (FeedbackSoundStop)
        {
            UGameplayStatics::SpawnSound2D(this, FeedbackSoundStop);
        } 
    }
}


    

