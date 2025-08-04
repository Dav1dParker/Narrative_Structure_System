// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "NarrativeStructurePluginStyle.h"

class FNarrativeStructurePluginCommands : public TCommands<FNarrativeStructurePluginCommands>
{
public:

	FNarrativeStructurePluginCommands()
		: TCommands<FNarrativeStructurePluginCommands>(TEXT("NarrativeStructurePlugin"), NSLOCTEXT("Contexts", "NarrativeStructurePlugin", "NarrativeStructurePlugin Plugin"), NAME_None, FNarrativeStructurePluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};