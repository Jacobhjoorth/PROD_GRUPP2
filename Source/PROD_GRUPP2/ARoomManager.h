// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRoomData.h"
#include "ARoomManager.generated.h"

UCLASS()
class PROD_GRUPP2_API AARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AARoomManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rooms")
	TArray<FRoomData> Rooms;

	UFUNCTION(BlueprintCallable, Category="Room")
	USoundCue* GetCurrentRoomSoundCue() const;

	UFUNCTION(BlueprintCallable, Category="Room")
	void AddRoom(FName Name, USoundCue* SC);

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetPlayerInRoom(FName Name, bool bIsInRoom);
};
