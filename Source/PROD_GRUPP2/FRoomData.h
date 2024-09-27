#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h" // Include for USoundCue
#include "FRoomData.generated.h"

USTRUCT(BlueprintType)
struct PROD_GRUPP2_API FRoomData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool bIsPlayerInRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	USoundCue* RoomSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FName Name;

	FRoomData()
	{
		bIsPlayerInRoom = false;
		RoomSoundCue = nullptr; // Här kan man ha ett standard ljud som ska spelas när det inte finns någon tutorial
		Name = "";
	}
};

