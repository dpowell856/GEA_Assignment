// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "VehicleTemplateGameMode.h"
#include "VehicleTemplatePawn.h"
#include "VehicleTemplateHud.h"

AVehicleTemplateGameMode::AVehicleTemplateGameMode()
{
	DefaultPawnClass = AVehicleTemplatePawn::StaticClass();
	HUDClass = AVehicleTemplateHud::StaticClass();
	// 0 is Point to point
	// 1 is lap based
	RaceType = 0;
	RaceLaps = 3;
}

int AVehicleTemplateGameMode::GetRaceType()
{
	return this->RaceType;
}

int AVehicleTemplateGameMode::GetRaceLaps()
{
	return this->RaceLaps;
}
