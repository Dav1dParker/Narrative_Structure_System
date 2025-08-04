// Copyright Epic Games, Inc. All Rights Reserved.

#include "NarrativeStructurePluginCommands.h"

#define LOCTEXT_NAMESPACE "FNarrativeStructurePluginModule"

void FNarrativeStructurePluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "NarrativeStructurePlugin", "Bring up NarrativeStructurePlugin window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
