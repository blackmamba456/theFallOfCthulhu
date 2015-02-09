// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "SettingsFunctions.h"
#include "Engine.h"

USettingsFunctions::USettingsFunctions(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	

}


void USettingsFunctions::changeVolume(USoundClass* soundClass, float volume)
{
	soundClass->Properties.Volume = volume;
}


void USettingsFunctions::saveSoundSettings(ASoundConfigs* settings)
{
	settings->SaveConfig();
}

void USettingsFunctions::loadSoundSettings(ASoundConfigs* settings)
{
	settings->LoadConfig();
}


void USettingsFunctions::setResolution(int32 x, int32 y, int fullscreen)
{
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(x, y));
	GEngine->GetGameUserSettings()->SetFullscreenMode((EWindowMode::Type)fullscreen);
}

void USettingsFunctions::applyGraphics()
{
	GEngine->GetGameUserSettings()->ApplySettings(true);
}


FResolution USettingsFunctions::getResolution()
{
	FResolution resolution;
	FIntPoint res = GEngine->GetGameUserSettings()->GetScreenResolution();
	if (res == FIntPoint(0, 0))
		res = GEngine->GetGameUserSettings()->GetDefaultResolution();


	resolution.resolutionX = res.X;
	resolution.resolutionY = res.Y;
	resolution.fullscreen = (int32)GEngine->GetGameUserSettings()->GetFullscreenMode();

	return resolution;
}