// Copyright Epic Games, Inc. All Rights Reserved.

#include "NarrativeStructurePluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FNarrativeStructurePluginStyle::StyleInstance = nullptr;

void FNarrativeStructurePluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FNarrativeStructurePluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FNarrativeStructurePluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("NarrativeStructurePluginStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FNarrativeStructurePluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("NarrativeStructurePluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("NarrativeStructurePlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("NarrativeStructurePlugin.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FNarrativeStructurePluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FNarrativeStructurePluginStyle::Get()
{
	return *StyleInstance;
}
