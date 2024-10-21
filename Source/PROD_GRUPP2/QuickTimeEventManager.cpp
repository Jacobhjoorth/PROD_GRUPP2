// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTimeEventManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
UQuickTimeEventManager::UQuickTimeEventManager()
{
 
	
}

// Called when the game starts or when spawned
void UQuickTimeEventManager::BeginPlay()
{
	Super::BeginPlay();


	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	player->InputComponent->BindAction(TEXT("Reaction"), IE_Pressed, this, &UQuickTimeEventManager::ReactionEvaluation);
	
}

void UQuickTimeEventManager::Update()
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
	}
}

bool UQuickTimeEventManager::AddQtEventIter(FQtEvent Qt, int num)
{
	for(int i = 0; i < num; i++)
	{
		AddQTEvent(Qt);
	}
	return true;
}

bool UQuickTimeEventManager::AddAllQTEvent(TArray<FQtEvent> Qt)
{
	for (FQtEvent Element : Qt)
	{
		AddQTEvent(Element);
	}
	return true;
}

bool UQuickTimeEventManager::AddQTEvent(FQtEvent Qt)
{
	if(Qt.Audio != nullptr)
	{
		ListOfQTs.Push(Qt);
		return true;
	} 
	return false;
}

void UQuickTimeEventManager::ReactionEvaluation()
{
	if(!ListOfQTs.IsEmpty())
	{
		bKeyPressed = true;
	}
}

bool UQuickTimeEventManager::CheckEvent()
{
	if(ListOfQTs.IsEmpty() == true){
		return false;
	}
	
	FQtEvent& CurrentEvent = ListOfQTs[0];

	if(CurrentEvent.TimeFrame <= 0){
		FailEvent();
		return false;
	}
	if(bKeyPressed){
		SucceededEvent();
		bKeyPressed = false;
		return false;
	}
	
	return true;
}

void  UQuickTimeEventManager::FailEvent()
{
	ListOfQTs.Empty();
	UGameplayStatics::PlaySound2D(GetWorld(), FailSound);
	OnFailed.Broadcast();
}

void  UQuickTimeEventManager::SucceededEvent()
{
	ListOfQTs.RemoveAt(0);
	UGameplayStatics::PlaySound2D(GetWorld(), SucceededSound);
	if(ListOfQTs.IsEmpty())
	{
		OnSucceeded.Broadcast();
	}

}

void UQuickTimeEventManager::DoEventActions(FQtEvent& Qt)
{
	if(!Qt.bIsActive)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Qt.Audio);
		Qt.bIsActive = true;
	}
}


