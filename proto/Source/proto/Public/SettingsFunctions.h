// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SoundConfigs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingsFunctions.generated.h"

USTRUCT()
struct FResolution
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	int32 resolutionX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	int32 resolutionY;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	int32 fullscreen;
};






/**
 * 
 */
UCLASS()
class PROTO_API USettingsFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "SoundSettings")
	static void changeVolume(USoundClass* soundClass, float volume);

	UFUNCTION(BlueprintCallable, Category = "SoundSettings")
	static void saveSoundSettings(ASoundConfigs* settings);

	UFUNCTION(BlueprintCallable, Category = "SoundSettings")
	static void loadSoundSettings(ASoundConfigs* settings);


	UFUNCTION(BlueprintCallable, Category = "GraphicsSettings")
	static void setResolution(int32 x, int32 y, int32 fullscreen);

	UFUNCTION(BlueprintCallable, Category = "GraphicsSettings")
	static FResolution getResolution();


	UFUNCTION(BlueprintCallable, Category = "GraphicsSettings")
	static void applyGraphics();

};


