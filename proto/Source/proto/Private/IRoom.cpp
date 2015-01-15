// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "IRoom.h"

AIRoom::AIRoom(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

}


int32 AIRoom::getWidth()
{
	return width;
}

int32 AIRoom::getHeight()
{
	return height;
}

