#include "ARoomManager.h"

AARoomManager::AARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

USoundCue* AARoomManager::GetTutorialVoiceLine() const
{
	for(const FRoomData& Room : Rooms)
	{
		if(Room.bIsPlayerInRoom)
		{
			return Room.RoomSoundCue;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("GetTutorialVoiceLine: No room has the player in it or no sound found!"));
	return nullptr;
}

void AARoomManager::AddRoom(FName Name, USoundCue* SC)
{
		FRoomData NewRoomData;
		NewRoomData.Name = Name;
		NewRoomData.RoomSoundCue = SC; 
		Rooms.Add(NewRoomData); 
}

void AARoomManager::SetPlayerInRoom(FName Name, bool bIsInRoom)
{
	for (FRoomData& Room : Rooms) 
	{
		if (Room.Name == Name) 
		{
			Room.bIsPlayerInRoom = bIsInRoom;
			break; 
		}
	}
}




