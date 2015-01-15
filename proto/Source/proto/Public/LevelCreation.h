// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <stack>
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "IRoom.h"

#include "LevelCreation.generated.h"


struct Vec2
{
	int x;
	int y;
};

typedef std::stack < Vec2 > NextRooms;



struct RoomStr
{
	enum Flags
	{
		LEFTENTRY = 1,
		RIGHTENTRY = 2,
		UPENTRY = 4,
		DOWNENTRY = 8,
		MAIN = 16,
		SUB = 32,
		START = 64,
		END = 128,
		LEFTWALL = 256,
		RIGHTWALL = 512,
		UPWALL = 1024,
		DOWNWALL = 2048,
		HEAL = 4096,

		SPECIAL = START | END | HEAL


	};

	int flags = 0;
	union
	{
		struct
		{
			short roomIdx;
			char roomWidth;
			char roomHeight;
		};

		struct
		{
			short x;
			short y;
		} mainPos;

	};
};


/**
*
*/
UCLASS()
class PROTO_API ALevelCreation : public AActor
{
	GENERATED_UCLASS_BODY()


		static const int32 MAX_LEVELSIZE = 32;


	UFUNCTION(BlueprintCallable, Category = Creation)
		void createLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Creation)
		void destroyLevel();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Level)
		int32 levelSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Level)
		AIRoom* startRoom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		TArray<UClass*> startRoomTemplates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		TArray<UClass*> endRoomTemplates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		TArray<UClass*> healRoomTemplates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		TArray<UClass*> roomTemplates;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		UClass* wallTemplate;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Level)
		TArray<AIRoom*> rooms;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Creation)
		int32 roomSize;

private:

	void setRoom(int x, int y, AIRoom* room, int idx, int flags = 0);
	void checkRooms(int x, int y);

	bool checkSpecialRoom(int x, int y);

	RoomStr* get(int x, int y);

	RoomStr m_Layout[MAX_LEVELSIZE * MAX_LEVELSIZE];
	int m_LevelSize;
	NextRooms m_NextRooms;
	bool m_Exit;

	FRandomStream m_Random;



};


inline RoomStr* ALevelCreation::get(int x, int y)
{
	if (x >= 0 && x < m_LevelSize &&
		y >= 0 && y < m_LevelSize)
		return &m_Layout[y * m_LevelSize + x];
	else
		return 0;
}


