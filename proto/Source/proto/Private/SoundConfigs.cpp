// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "SoundConfigs.h"


ASoundConfigs::ASoundConfigs(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), master(1.0f), music(0.75f), effects(1.0f)
	
{

}


