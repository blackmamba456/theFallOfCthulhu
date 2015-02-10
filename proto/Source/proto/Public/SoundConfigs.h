// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SoundConfigs.generated.h"






/**
 * 
 */
UCLASS(Config=Game)
class PROTO_API ASoundConfigs : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = Default)
		float master;
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = Default)
		float music;
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = Default)
		float effects;
	
};
