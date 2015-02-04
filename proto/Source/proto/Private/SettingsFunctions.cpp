// Fill out your copyright notice in the Description page of Project Settings.

#include "proto.h"
#include "SettingsFunctions.h"

USettingsFunctions::USettingsFunctions(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	

}


void USettingsFunctions::changeVolume(USoundClass* soundClass, float volume)
{
	soundClass->Properties.Volume = volume;
}