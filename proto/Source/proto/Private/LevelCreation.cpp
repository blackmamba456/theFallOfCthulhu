// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "LevelCreation.h"
#include "RandomStream.h"


#include <sstream>


ALevelCreation::ALevelCreation(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}






void ALevelCreation::setRoom(int x, int y, int idx)
{

	// set room idx and entries


	AIRoom* room = rooms[idx];

	for (int i = 0; i < room->getWidth(); i++)
	{
		for (int j = 0; j < room->getHeight(); j++)
		{
			get(x + i, y + j)->idx = -2;

			//
		}
	}
	get(x, y)->idx = idx;


	// set and add next rooms
	for (int i = 0; i < room->getWidth(); i++)
	{
		for (int j = 0; j < room->getHeight(); j++)
		{

			FSubRoom& subRoom = room->getSubRoom(i, j);


			if (subRoom.exitLeft && get(x + i - 1, y + j))
			{
				RoomStr* room = get(x + i - 1, y + j);
				if (room->idx == -1)
				{
					room->entries |= RoomStr::RIGHT;
					m_NextRooms.push({ x + i - 1, y + j });
				}
			}

			if (subRoom.exitRight && get(x + i + 1, y + j))
			{
				RoomStr* room = get(x + i + 1, y + j);
				if (room->idx == -1)
				{
					room->entries |= RoomStr::LEFT;
					m_NextRooms.push({ x + i + 1, y + j });
				}
			}

			if (subRoom.exitUp && get(x + i, y + j - 1))
			{
				RoomStr* room = get(x + i, y + j - 1);
				if (room->idx == -1)
				{
					room->entries |= RoomStr::DOWN;
					m_NextRooms.push({ x + i, y + j - 1 });
				}
			}

			if (subRoom.exitDown && get(x + i, y + j + 1))
			{
				RoomStr* room = get(x + i, y + j + 1);
				if (room->idx == -1)
				{
					room->entries |= RoomStr::UP;
					m_NextRooms.push({ x + i, y + j + 1 });
				}
			}

		}
	}
}

struct RoomPriority
{
	int idx = -1;
	float prio = 0.0f;
	int subRoomX = 0;
	int subRoomY = 0;
};

void ALevelCreation::checkRooms(int x, int y)
{
	RoomPriority bestRoom;

	// TODO set actual exit
	if (!m_Exit && y == (m_LevelSize - 1))
		m_Exit = true;


	int start = m_Random.RandRange(0, rooms.Num() - 1);
	for (int i = 0; i < rooms.Num(); i++)
	{
		// check how well this room fits
		// if it fits perfectly choose it else put it on the queue

		AIRoom* room = rooms[(i + start)%rooms.Num()];


		for (int startX = 0; startX < room->getWidth(); startX++)
		{
			for (int startY = 0; startY < room->getHeight(); startY++)
			{
				int numTotalExits = 0;
				int numFittingExits = 0;
				int numFreeExits = 0;

				bool br = false;

				// room lays on edge
				bool edge = false;
				// room has an free exit which is not upward
				bool down = false;
				

				//check each subroom
				for (int subRoomX = 0; subRoomX < room->getWidth() && !br; subRoomX++)
				{
					for (int subRoomY = 0; subRoomY < room->getHeight() && !br; subRoomY++)
					{

						int _x = x - startX + subRoomX;
						int _y = y - startY + subRoomY;

						if (_x == 0 || _x == (m_LevelSize - 1))
							edge = true;

						RoomStr* layoutRoom = get(_x, _y);

						// room is out of boundaries or already occupied
						if (!layoutRoom || layoutRoom->idx != -1)
						{
							br = true;
							break;
						}


						if (room->getSubRoom(subRoomX, subRoomY).exitDown)
						{
							numTotalExits++;
							if (layoutRoom->entries & RoomStr::DOWN)
							{
								numFittingExits++;
							}
							else if (get(_x, _y + 1) && get(_x, _y + 1)->idx == -1)
							{
								numFreeExits++;
								down = true;
							}

						}

						if (room->getSubRoom(subRoomX, subRoomY).exitLeft)
						{
							numTotalExits++;
							if (layoutRoom->entries & RoomStr::LEFT)
							{
								numFittingExits++;
							}
							else if (get(_x - 1, _y) && get(_x - 1, _y)->idx == -1)
							{
								numFreeExits++;
								down = true;
							}
						}

						if (room->getSubRoom(subRoomX, subRoomY).exitRight)
						{
							numTotalExits++;
							if (layoutRoom->entries & RoomStr::RIGHT)
							{
								numFittingExits++;
							}
							else if (get(_x + 1, _y) && get(_x + 1, _y)->idx == -1)
							{
								numFreeExits++;
								down = true;
							}
						}

						if (room->getSubRoom(subRoomX, subRoomY).exitUp)
						{
							numTotalExits++;
							if (layoutRoom->entries & RoomStr::UP)
								numFittingExits++;
							else if (get(_x, _y - 1) && get(_x, _y - 1)->idx == -1)
								numFreeExits++;;
						}

					} 
				}


				// no fit or no space
				if (br || numFittingExits == 0)
					continue;

				// ensures level goes to the lowest level
				// and doesnt end in a deadend
				if (!m_Exit && !down)
					continue;

				//perfect fit
				if (numTotalExits == (numFittingExits + numFreeExits))
				{
					setRoom(x - startX, y - startY, (i + start) % rooms.Num());
					return;
				}
				else if (bestRoom.prio < (numFittingExits + numFreeExits) / (float)numTotalExits) // check if its the next best room
				{
					bestRoom.idx = (i + start)%rooms.Num();
					bestRoom.prio = (numFittingExits + numFreeExits) / (float)numTotalExits;
					bestRoom.subRoomX = startX;
					bestRoom.subRoomY = startY;
				}


			}
		}
	}

	if (bestRoom.idx != -1)
	{
		setRoom(x - bestRoom.subRoomX, y - bestRoom.subRoomY, bestRoom.idx);
	}


}


void ALevelCreation::createLevel(int32 levelSize)
{
	m_Random.GenerateNewSeed();


	if (levelSize > MAX_LEVELSIZE) levelSize = MAX_LEVELSIZE;

	m_LevelSize = levelSize;

	m_Exit = false;

	int startPosition = levelSize / 2;

	int startId = m_Random.RandRange(0, rooms.Num() - 1);

	setRoom(startPosition, 0, startId);


	while (!m_NextRooms.empty())
	{
		//std::pair<int, int> nextRoom = nextRooms.front();
		std::pair<int, int> nextRoom = m_NextRooms.top();
		m_NextRooms.pop();

		if (get(nextRoom.first, nextRoom.second)->idx == -1)
		{
			checkRooms(nextRoom.first, nextRoom.second);
		}
		
	}


	//construct level

	UWorld* const world = GetWorld();
	for (int j = 0; j < levelSize; j++)
	{
		for (int i = 0; i < levelSize; i++)
		{
			if (get(i, j)->idx >= 0)
			{
				FVector location(0, -i*roomSize + levelSize/2*roomSize, -j * roomSize);
				FRotator rotation(0);

				std::stringstream ss;
				ss << "Room" << get(i, j)->idx << "_" << i << "|" << j;

				FActorSpawnParameters params;
				params.Name = FName(ss.str().c_str());
				params.Owner = this;

				AActor* newRoom = world->SpawnActor(rooms[get(i, j)->idx]->GetClass(), &location, &rotation, params);


			}
		}
	}



}