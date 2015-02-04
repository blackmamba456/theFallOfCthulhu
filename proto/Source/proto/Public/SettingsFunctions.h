// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingsFunctions.generated.h"


/**
 * 
 */
UCLASS()
class PROTO_API USettingsFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "SoundSettings")
	static void changeVolume(USoundClass* soundClass, float volume);


};

