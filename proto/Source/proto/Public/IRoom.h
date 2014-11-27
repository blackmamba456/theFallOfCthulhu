// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IRoom.generated.h"


class AIRoom;

USTRUCT()
struct FSubRoom
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		bool exitLeft = false;							   
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		bool exitRight = false;							   
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		bool exitUp = false;							  
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		bool exitDown = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = subRoom)
		AIRoom* nextLeft = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = subRoom)
		AIRoom* nextRight = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = subRoom)
		AIRoom* nextUp = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = subRoom)
		AIRoom* nextDown = 0;
};

/**
 * 
 */
UCLASS()
class PROTO_API AIRoom : public AActor
{
	GENERATED_UCLASS_BODY()


	int32 getWidth();

	int32 getHeight();

	inline FSubRoom& getSubRoom(int x, int y);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = subRoom)
		TArray<FSubRoom> subRooms;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		int32 width;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = subRoom)
		int32 height;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Position)
		FVector2D position;


	UFUNCTION(BlueprintImplementableEvent, Category = Creation)
		void build(int32& numAvailableChests);


};


inline FSubRoom& AIRoom::getSubRoom(int x, int y) 
{
	return subRooms[x + y*width];
}
