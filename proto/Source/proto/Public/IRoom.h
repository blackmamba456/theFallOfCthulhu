// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IRoom.generated.h"


USTRUCT()
struct FSubRoom
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		bool exitLeft = false;							   
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		bool exitRight = false;							   
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		bool exitUp = false;							  
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		bool exitDown = false;
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

	FSubRoom& getSubRoom(int x, int y);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		TArray<FSubRoom> subRooms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		int32 width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = subRoom)
		int32 height;

	UFUNCTION(BlueprintImplementableEvent, Category = Creation)
		void build(int32& numAvailableChests);
	
};
