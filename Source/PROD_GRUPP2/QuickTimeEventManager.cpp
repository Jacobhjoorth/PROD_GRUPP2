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
	player->InputComponent->BindAxis(TEXT("Spacebar"),this, &UQuickTimeEventManager::ReactionEvaluation);
	
}

void UQuickTimeEventManager::Update()
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if(ListOfQTs.IsValidIndex(0)) return;

	if((ListOfQTs[0].StartDelay -= DeltaTime) <= 0)
	{
		ListOfQTs[0].TimeFrame -= DeltaTime;
		CheckEvent();
	}
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

void UQuickTimeEventManager::ReactionEvaluation(float Value)
{
	if(Value != 1){return;}
	bKeyPressed = true;
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
		return true;
	}
	
	return false;
}

void  UQuickTimeEventManager::FailEvent()
{
	ListOfQTs.Empty();
	UGameplayStatics::PlaySound2D(GetWorld(), FailSound);
}

void  UQuickTimeEventManager::SucceededEvent()
{
	ListOfQTs.RemoveAt(0);
	UGameplayStatics::PlaySound2D(GetWorld(), SucceededSound);
}

