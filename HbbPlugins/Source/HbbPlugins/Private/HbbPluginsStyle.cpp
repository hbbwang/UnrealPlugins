// Copyright Epic Games, Inc. All Rights Reserved.

#include "HbbPluginsStyle.h"
#include "HbbPlugins.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FHbbPluginsStyle::StyleInstance = nullptr;

void FHbbPluginsStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FHbbPluginsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FHbbPluginsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("HbbPluginsStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef< FSlateStyleSet > FHbbPluginsStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("HbbPluginsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("HbbPlugins")->GetBaseDir() / TEXT("Resources"));
	Style->Set("HbbPlugins.PluginAction", new IMAGE_BRUSH(TEXT("Icon128"), Icon40x40));
	return Style;
}

void FHbbPluginsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FHbbPluginsStyle::Get()
{
	return *StyleInstance;
}
