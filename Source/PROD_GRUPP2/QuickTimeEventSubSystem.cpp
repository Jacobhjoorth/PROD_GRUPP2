// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTimeEventSubSystem.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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
