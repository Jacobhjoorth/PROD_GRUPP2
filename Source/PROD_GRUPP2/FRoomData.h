#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h" // Include for USoundCue
#include "FRoomData.generated.h"

// Struct to store room data
USTRUCT(BlueprintType)
struct PROD_GRUPP2_API FRoomData
{
	GENERATED_BODY()

public:
	// Whether the player is in this room or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool bIsPlayerInRoom;

	// The sound cue to play in this room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	USoundCue* RoomSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FName Name;

	FRoomData()
	{
		bIsPlayerInRoom = false;
		RoomSoundCue = nullptr;
		Name = "";
	}
};

