// Copyright Epic Games, Inc. All Rights Reserved.

#include "HbbEditorPluginsStyle.h"
#include "HbbEditorPlugins.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FHbbEditorPluginsStyle::StyleInstance = nullptr;

void FHbbEditorPluginsStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FHbbEditorPluginsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FHbbEditorPluginsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("HbbEditorPluginsStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef< FSlateStyleSet > FHbbEditorPluginsStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("HbbEditorPluginsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("HbbEditorPlugins")->GetBaseDir() / TEXT("Resources"));
	Style->Set("HbbEditorPlugins.PluginAction", new IMAGE_BRUSH(TEXT("Icon128"), Icon40x40));
	return Style;
}

void FHbbEditorPluginsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FHbbEditorPluginsStyle::Get()
{
	return *StyleInstance;
}
