// Copyright Epic Games, Inc. All Rights Reserved.

#include "NarrativeStructurePlugin.h"
#include "NarrativeStructurePluginStyle.h"
#include "NarrativeStructurePluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SNarrativeJsonBrowser.h"


static const FName NarrativeStructurePluginTabName("NarrativeStructurePlugin");

#define LOCTEXT_NAMESPACE "FNarrativeStructurePluginModule"

void FNarrativeStructurePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNarrativeStructurePluginStyle::Initialize();
	FNarrativeStructurePluginStyle::ReloadTextures();

	FNarrativeStructurePluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNarrativeStructurePluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNarrativeStructurePluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNarrativeStructurePluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NarrativeStructurePluginTabName, FOnSpawnTab::CreateRaw(this, &FNarrativeStructurePluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNarrativeStructurePluginTabTitle", "NarrativeStructurePlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNarrativeStructurePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNarrativeStructurePluginStyle::Shutdown();

	FNarrativeStructurePluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NarrativeStructurePluginTabName);
}

TSharedRef<SDockTab> FNarrativeStructurePluginModule::OnSpawnPluginTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SNarrativeJsonBrowser)
		];
}



void FNarrativeStructurePluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(NarrativeStructurePluginTabName);
}

void FNarrativeStructurePluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNarrativeStructurePluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNarrativeStructurePluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNarrativeStructurePluginModule, NarrativeStructurePlugin)