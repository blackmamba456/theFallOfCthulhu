// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "LevelCreation.h"
#include "RandomStream.h"
#include "Engine.h"


#include <sstream>


ALevelCreation::ALevelCreation(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}






void ALevelCreation::setRoom(int x, int y, AIRoom* room, int idx, int flags)
{

	// set room idx and entries

	for (int i = 0; i < room->getWidth(); i++)
	{
		for (int j = 0; j < room->getHeight(); j++)
		{
			//get(x + i, y + j)->roomIdx = -2;
			if (i != 0 || j != 0)
			{
				get(x + i, y + j)->mainPos = { x, y };
				get(x + i, y + j)->flags |= RoomStr::SUB;
			}
		}
	}
	get(x, y)->roomIdx = idx;
	get(x, y)->flags |= (RoomStr::MAIN | flags);


	// set and add next rooms
	for (int i = 0; i < room->getWidth(); i++)
	{
		for (int j = 0; j < room->getHeight(); j++)
		{

			FSubRoom& subRoom = room->getSubRoom(i, j);

			RoomStr* layoutRoom = get(x + i, y + j);

			if (get(x + i - 1, y + j) && !(layoutRoom->flags & RoomStr::LEFTWALL))
			{
				RoomStr* neighbour = get(x + i - 1, y + j);
				if (subRoom.exitLeft)
				{
					neighbour->flags |= RoomStr::RIGHTENTRY;
					m_NextRooms.push({ x + i - 1, y + j });
				}
				else
				{
					if (layoutRoom->flags & RoomStr::LEFTENTRY)
						layoutRoom->flags &= ~RoomStr::LEFTENTRY;

					layoutRoom->flags |= RoomStr::LEFTWALL;
					neighbour->flags |= RoomStr::RIGHTWALL;

				}
			}

			if (get(x + i + 1, y + j) && !(layoutRoom->flags & RoomStr::RIGHTWALL))
			{
				RoomStr* neighbour = get(x + i + 1, y + j);
				if (subRoom.exitRight)
				{
					neighbour->flags |= RoomStr::LEFTENTRY;
					m_NextRooms.push({ x + i + 1, y + j });
				}
				else
				{
					if (layoutRoom->flags & RoomStr::RIGHTENTRY)
						layoutRoom->flags &= ~RoomStr::RIGHTENTRY;

					layoutRoom->flags |= RoomStr::RIGHTWALL;
					neighbour->flags |= RoomStr::LEFTWALL;

				}
			}

			if (get(x + i, y + j - 1) && !(layoutRoom->flags & RoomStr::UPWALL))
			{
				RoomStr* neighbour = get(x + i, y + j - 1);
				if (subRoom.exitUp)
				{
					neighbour->flags |= RoomStr::DOWNENTRY;
					m_NextRooms.push({ x + i, y + j - 1});
				}
				else
				{
					if (layoutRoom->flags & RoomStr::UPENTRY)
						layoutRoom->flags &= ~RoomStr::UPENTRY;

					layoutRoom->flags |= RoomStr::UPWALL;
					neighbour->flags |= RoomStr::DOWNWALL;

				}
			}

			if (get(x + i, y + j + 1) && !(layoutRoom->flags & RoomStr::DOWNWALL))
			{
				RoomStr* neighbour = get(x + i, y + j + 1);
				if (subRoom.exitDown)
				{
					neighbour->flags |= RoomStr::UPENTRY;
					m_NextRooms.push({ x + i, y + j + 1});
				}
				else
				{
					if (layoutRoom->flags & RoomStr::DOWNENTRY)
						layoutRoom->flags &= ~RoomStr::DOWNENTRY;

					layoutRoom->flags |= RoomStr::DOWNWALL;
					neighbour->flags |= RoomStr::UPWALL;

				}
			}


			/*
			if (subRoom.exitRight && get(x + i + 1, y + j))
			{
				//layoutRoom->flags |= RoomStr::RIGHT;

				RoomStr* neighbour = get(x + i + 1, y + j);
				if (neighbour->roomIdx == -1)
				{
					neighbour->flags |= RoomStr::LEFT;
					m_NextRooms.push({ x + i + 1, y + j });
				}
			}

			if (subRoom.exitUp && get(x + i, y + j - 1))
			{
				//layoutRoom->flags |= RoomStr::UP;

				RoomStr* neighbour = get(x + i, y + j - 1);
				if (neighbour->roomIdx == -1)
				{
					neighbour->flags |= RoomStr::DOWN;
					m_NextRooms.push({ x + i, y + j - 1 });
				}
			}

			if (subRoom.exitDown && get(x + i, y + j + 1))
			{
				//layoutRoom->flags |= RoomStr::DOWN;

				RoomStr* neighbour = get(x + i, y + j + 1);
				if (neighbour->roomIdx == -1)
				{
					neighbour->flags |= RoomStr::UP;
					m_NextRooms.push({ x + i, y + j + 1 });
				}
			}

			*/

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


	int start = m_Random.RandRange(0, roomTemplates.Num() - 1);
	for (int i = 0; i < roomTemplates.Num(); i++)
	{
		// check how well this room fits
		// if it fits perfectly choose it else put it on the queue

		AIRoom* room = roomTemplates[(i + start) % roomTemplates.Num()]->GetDefaultObject<AIRoom>();


		for (int startX = 0; startX < room->getWidth(); startX++)
		{
			for (int startY = 0; startY < room->getHeight(); startY++)
			{
				int numTotalExits = 0;
				int numFittingExits = 0;
				int numFreeExits = 0;

				bool br = false;

				// room lies on edge
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
						FSubRoom& subRoom = room->getSubRoom(subRoomX, subRoomY);

						// room is out of boundaries or already occupied
						if (!layoutRoom || layoutRoom->flags & (RoomStr::MAIN | RoomStr::SUB)/*layoutRoom->roomIdx != -1*/)
						{
							br = true;
							break;
						}



						if (layoutRoom->flags & RoomStr::LEFTENTRY)
						{
							numTotalExits++;
							if (subRoom.exitLeft)
							{

								numFittingExits++;
							}
						}
						else if (layoutRoom->flags & RoomStr::LEFTWALL)
						{
							numTotalExits++;
							if (!subRoom.exitLeft)
								numFittingExits++;
						}
						else
						{
							if (subRoom.exitLeft)
							{
								numTotalExits++;
								if (get(_x-1, _y))
								{
									down = true;
									numFreeExits++;
								}
							}
						}

						if (layoutRoom->flags & RoomStr::RIGHTENTRY)
						{
							numTotalExits++;
							if (subRoom.exitRight)
							{
								numFittingExits++;
							}
						}
						else if (layoutRoom->flags & RoomStr::RIGHTWALL)
						{
							numTotalExits++;
							if (!subRoom.exitRight)
								numFittingExits++;
						}
						else
						{
							if (subRoom.exitRight)
							{
								numTotalExits++;
								if (get(_x + 1, _y))
								{
									down = true;
									numFreeExits++;
								}
							}
						}

						if (layoutRoom->flags & RoomStr::UPENTRY)
						{
							numTotalExits++;
							if (subRoom.exitUp)
								numFittingExits++;
						}
						else if (layoutRoom->flags & RoomStr::UPWALL)
						{
							numTotalExits++;
							if (!subRoom.exitUp)
								numFittingExits++;
						}
						else
						{
							if (subRoom.exitUp)
							{
								numTotalExits++;
								if (get(_x, _y - 1))
									numFreeExits++;
							}
						}

						if (layoutRoom->flags & RoomStr::DOWNENTRY)
						{
							numTotalExits++;
							if (subRoom.exitDown)
							{
								numFittingExits++;
							}
						}
						else if (layoutRoom->flags & RoomStr::DOWNWALL)
						{
							numTotalExits++;
							if (!subRoom.exitDown)
								numFittingExits++;
						}
						else
						{
							if (subRoom.exitDown)
							{
								numTotalExits++;
								if (get(_x, _y + 1))
								{
									down = true;
									numFreeExits++;
								}
							}
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
					setRoom(x - startX, y - startY, roomTemplates[(i+start)%roomTemplates.Num()]->GetDefaultObject<AIRoom>(), (i + start) % roomTemplates.Num());
					return;
				}
				else if (bestRoom.prio < (numFittingExits + numFreeExits) / (float)numTotalExits) // check if its the next best room
				{
					bestRoom.idx = (i + start) % roomTemplates.Num();
					bestRoom.prio = (numFittingExits + numFreeExits) / (float)numTotalExits;
					bestRoom.subRoomX = startX;
					bestRoom.subRoomY = startY;
				}


			}
		}
	}

	if (bestRoom.idx != -1)
	{
		// TODO adjust entries of neighbours
		setRoom(x - bestRoom.subRoomX, y - bestRoom.subRoomY, roomTemplates[bestRoom.idx]->GetDefaultObject<AIRoom>(), bestRoom.idx);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, "couldnt find fitting room " + FString::FromInt(x) + " " + FString::FromInt(y));
	}


}


void ALevelCreation::createLevel()
{
	m_Random.GenerateNewSeed();


	if (levelSize > MAX_LEVELSIZE) levelSize = MAX_LEVELSIZE;

	m_LevelSize = levelSize;

	m_Exit = false;

	int startPosition = levelSize / 2;

	int startId = m_Random.RandRange(0, startRoomTemplates.Num() - 1);


	setRoom(startPosition, 0, startRoomTemplates[startId]->GetDefaultObject<AIRoom>(), startId, RoomStr::START);
	

	while (!m_NextRooms.empty())
	{
		//std::pair<int, int> nextRoom = nextRooms.front();
		std::pair<int, int> nextRoom = m_NextRooms.top();
		m_NextRooms.pop();

		if (get(nextRoom.first, nextRoom.second)->roomIdx == -1)
		{
			checkRooms(nextRoom.first, nextRoom.second);
		}

	}


	//construct level

	// TODO maybe use binary search for rooms?!

	UWorld* const world = GetWorld();
	for (int j = 0; j < levelSize; j++)
	{
		for (int i = 0; i < levelSize; i++)
		{
			AIRoom* room = 0;

			RoomStr* layoutRoom = get(i, j);

			if (get(i, j)->flags & RoomStr::MAIN)
			{
				FVector location(0, -i*roomSize + levelSize / 2 * roomSize, -j * roomSize);
				FRotator rotation(0);

				std::stringstream ss;
				ss << "Room" << get(i, j)->roomIdx << "_" << i << "|" << j;

				FActorSpawnParameters params;
				params.Name = FName(ss.str().c_str());

				if (layoutRoom->flags & RoomStr::START)
				{
					room = (AIRoom*)world->SpawnActor(startRoomTemplates[get(i, j)->roomIdx], &location, &rotation, params);
					startRoom = room;
				}
				else
				{
					room = (AIRoom*)world->SpawnActor(roomTemplates[get(i, j)->roomIdx], &location, &rotation, params);
				}
				room->position = FVector2D(i, j);
				room->AttachRootComponentToActor(this);
				
				if (!room)
				{
					GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Blue, "failed building room " + FString::FromInt(i) + " " + FString::FromInt(j));
				}

				rooms.Add(room);
				
			}
			else if (get(i, j)->flags & RoomStr::SUB)
			{
				for (int k = 0; k < rooms.Num(); k++)
				{
					if (rooms[k]->position.X == layoutRoom->mainPos.x &&
						rooms[k]->position.Y == layoutRoom->mainPos.y)
					{
						room = rooms[k];
						break;
					}
				}
			}


			// connect rooms

			if (room)
			{



				FSubRoom& subRoom = room->getSubRoom(i - room->position.X, j - room->position.Y);


				
				subRoom.exitLeft = layoutRoom->flags & RoomStr::LEFTENTRY ? true : false;
				subRoom.exitRight = layoutRoom->flags & RoomStr::RIGHTENTRY ? true : false;
				subRoom.exitUp = layoutRoom->flags & RoomStr::UPENTRY ? true : false;
				subRoom.exitDown = layoutRoom->flags & RoomStr::DOWNENTRY ? true : false;

				
				if (layoutRoom->flags & RoomStr::UPENTRY)
				{
					RoomStr* neighbour = get(i, j - 1);

					int nRoomPosX = -1;
					int nRoomPosY = -1; 

					if (neighbour->flags & RoomStr::MAIN)
					{
						nRoomPosX = i;
						nRoomPosY = j - 1;
					}
					else if (neighbour->flags & RoomStr::SUB)
					{
						nRoomPosX = neighbour->mainPos.x;
						nRoomPosY = neighbour->mainPos.y;
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, "room not properly initialized " + FString::FromInt(i) + " " + FString::FromInt(j));
					}


					bool connected = false;
					for (int k = 0; k < rooms.Num(); k++)
					{
						if (rooms[k]->position.X == nRoomPosX && 
							rooms[k]->position.Y == nRoomPosY)
						{

							connected = true;

							room->getSubRoom(i - room->position.X , j - room->position.Y).nextUp = rooms[k];
							rooms[k]->getSubRoom(i - nRoomPosX, j - 1 - nRoomPosY).nextDown = room;
							break;
						}
					}

					if (!connected)
					{
						GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, "couldnt connect room " + FString::FromInt(i) + " " + FString::FromInt(j));
					}

				}

				if (layoutRoom->flags & RoomStr::LEFTENTRY)
				{

					RoomStr* neighbour = get(i - 1, j);

					int nRoomPosX = -1;
					int nRoomPosY = -1;

					if (neighbour->flags & RoomStr::MAIN)
					{
						nRoomPosX = i - 1;
						nRoomPosY = j;
					}
					else if (neighbour->flags & RoomStr::SUB)
					{
						nRoomPosX = neighbour->mainPos.x;
						nRoomPosY = neighbour->mainPos.y;
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, "room not properly initialized " + FString::FromInt(i) + " " + FString::FromInt(j));
					}


					bool connected = false;
					for (int k = 0; k < rooms.Num(); k++)
					{
						if (rooms[k]->position.X == nRoomPosX &&
							rooms[k]->position.Y == nRoomPosY)
						{
							connected = true;
							room->getSubRoom(i - room->position.X, j - room->position.Y).nextLeft = rooms[k];
							rooms[k]->getSubRoom(i - 1 - nRoomPosX, j - nRoomPosY).nextRight = room;
							break;
						}
					}

					if (!connected)
					{
						GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Blue, "couldnt connect room " + FString::FromInt(i) + " " + FString::FromInt(j));
					}

				}


			}

		}
	}



}


