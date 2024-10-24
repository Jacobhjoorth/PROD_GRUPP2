// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTimeEventSubSystem.h"

#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"



DEFINE_LOG_CATEGORY(LogGameActions);


void UQuickTimeEventSubSystem::Deinitialize()
{
	LogTextDataToFile();
}

void UQuickTimeEventSubSystem::Start()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	player->InputComponent->BindAction(TEXT("Reaction"), IE_Pressed, this, &UQuickTimeEventSubSystem::ReactionEvaluation);
	
	bKeyPressed = false;
}

void UQuickTimeEventSubSystem::Update()
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if(ListOfQTs.IsEmpty()) return;

	if(!ListOfQTs[0].bIsInit)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ListOfQTs[0].initAudio);
	
		ListOfQTs[0].bIsInit = true;
	}

	
	if((ListOfQTs[0].StartDelay -= DeltaTime) <= 0)
	{
	
		ListOfQTs[0].TimeFrame -= DeltaTime;
		
		if(CheckEvent())
		{
			
			DoEventActions(ListOfQTs[0]);
		
		}
		return;
	}
	if(bKeyPressed)
	{
		
		FailEvent();
		bKeyPressed = false;
	}
}

bool UQuickTimeEventSubSystem::AddQtEventIter(FQtEvent Qt, int num)
{
	for(int i = 0; i < num; i++)
	{
		AddQTEvent(Qt);
	}
	return true;
}

bool UQuickTimeEventSubSystem::AddAllQTEvent(TArray<FQtEvent> Qt)
{
	for (FQtEvent Element : Qt)
	{
		AddQTEvent(Element);
	}
	return true;
}

bool UQuickTimeEventSubSystem::AddQTEvent(FQtEvent Qt)
{
	if(Qt.Audio != nullptr)
	{
		ListOfQTs.Push(Qt);
		return true;
	} 
	return false;
}

void UQuickTimeEventSubSystem::ReactionEvaluation()
{
	if(!ListOfQTs.IsEmpty())
	{
		bKeyPressed = true;
	
	}

}

bool UQuickTimeEventSubSystem::CheckEvent()
{
	if(ListOfQTs.IsEmpty()){
		return false;
	}
	
	FQtEvent& CurrentEvent = ListOfQTs[0];

	if(CurrentEvent.TimeFrame <= 0){
		FailEvent();
		bKeyPressed = false;
		return false;
	}
	if(bKeyPressed){
		SucceededEvent();
		bKeyPressed = false;
		return false;
	}
	
	return true;
}

void  UQuickTimeEventSubSystem::FailEvent()
{
	Clear();
	UGameplayStatics::PlaySound2D(GetWorld(), FailSound);
	OnFailed.Broadcast();
	OnFailed.Clear();
	

}

void  UQuickTimeEventSubSystem::SucceededEvent()
{
	ListOfQTs.RemoveAt(0);
	UGameplayStatics::PlaySound2D(GetWorld(), SucceededSound);
	if(ListOfQTs.IsEmpty())
	{
		OnSucceeded.Broadcast();
		OnSucceeded.Clear();
	}

}

void UQuickTimeEventSubSystem::DoEventActions(FQtEvent& Qt)
{
	if(!Qt.bIsActive)
	{
	
		UGameplayStatics::PlaySound2D(GetWorld(), Qt.Audio);
		Qt.bIsActive = true;
	}
}
void UQuickTimeEventSubSystem::Clear()
{
	for (int i = 0; i < ListOfQTs.Num(); i++)
	{
		ListOfQTs.RemoveAt(i);
		ListOfQTs.Reset(0);
	}
}


void UQuickTimeEventSubSystem::LogToText(FString string, int PlayNum)
{
	FString date = FDateTime::Now().GetDate().ToString() + ", ";
	FString time =  FDateTime::Now().GetTimeOfDay().ToString() + ", ";
	FString playerSession = "PlayerSession: " + playerSession.FromInt(PlayNum);
	
	Data +=  playerSession + ", " + "Date: " + date + "Time" + time + "#" + string + "\n"; 
}
void UQuickTimeEventSubSystem::LogTextDataToFile()
{
	FString time = time.FromInt(FDateTime::Now().GetHour()) + "."+ time.FromInt(FDateTime::Now().GetMinute()) + "." + time.FromInt(FDateTime::Now().GetSecond());
	FString day = day.FromInt(FDateTime::Now().GetYear()) + "." + day.FromInt(FDateTime::Now().GetMonth()) +"."+ day.FromInt(FDateTime::Now().GetDay());
	FString FilePath = FPaths::ProjectDir() + "Saved/Logs/PlaytestLog_" + day + "-" + time + ".txt";
	
	// Create folder if it doesn't exist
	IFileManager& FileManager = IFileManager::Get();
	
	// Write the event to the file
	FFileHelper::SaveStringToFile(Data, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

