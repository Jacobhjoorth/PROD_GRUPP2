// Fill out your copyright notice in the Description page of Project Settings.


#include "ARoomManager.h"

// Sets default values
AARoomManager::AARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

USoundCue* AARoomManager::GetCurrentRoomSoundCue() const
{
	for(const FRoomData& Room : Rooms)
	{
		if(Room.bIsPlayerInRoom)
		{
			return Room.RoomSoundCue;
		}
	}
	return nullptr;
}

void AARoomManager::AddRoom(FName Name, USoundCue* SC)
{
		FRoomData NewRoomData;
		NewRoomData.Name = Name;// Set the RoomActor reference
		NewRoomData.RoomSoundCue = SC; 
		Rooms.Add(NewRoomData); // Add the struct to the array
}

// Function to set player presence based on Room Actor reference
void AARoomManager::SetPlayerInRoom(FName Name, bool bIsInRoom)
{
	for (FRoomData& Room : Rooms) // Loop through the Rooms array
	{
		if (Room.Name == Name) // Check if the RoomActor matches
		{
			Room.bIsPlayerInRoom = bIsInRoom; // Update the boolean
			break; // Exit loop once found
		}
	}
}




