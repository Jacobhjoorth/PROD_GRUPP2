// Fill out your copyright notice in the Description page of Project Settings.


#include "AAudioController.h"



void AAudioController::BeginPlay()
{
}

void AAudioController::Tick(float DeltaTime)
{
}

void AAudioController::PlayOrStopSound(USoundBase* SoundToPlay, bool bShouldPlay) const
{
	if(!SoundToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sound provided"));
		return;
	}

	if(USoundCue* SoundCue = Cast<USoundCue>(SoundToPlay))
	{
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue);

		if(AudioComponent)
		{
			if(bShouldPlay)
			{
				AudioComponent->Play();
				UE_LOG(LogTemp, Warning, TEXT("Playing"));			}
			else
			{
				AudioComponent->Stop();
			}
		}
	}
}
