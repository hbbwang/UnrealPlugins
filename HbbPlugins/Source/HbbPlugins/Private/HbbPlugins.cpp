// Copyright Epic Games, Inc. All Rights Reserved.

#include "HbbPlugins.h"

#include "AssetSearchTool.h"
#include "HbbPluginsStyle.h"
#include "LevelEditor.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Editor/LevelEditor/Private/SLevelEditor.h"

static const FName HbbPluginsTabName("HbbPlugins");

#define LOCTEXT_NAMESPACE "FHbbPluginsModule"

void FHbbPluginsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FHbbPluginsStyle::Initialize();
	FHbbPluginsStyle::ReloadTextures();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHbbPluginsModule::RegisterMenus));
}

void FHbbPluginsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHbbPluginsStyle::Shutdown();
}

void FHbbPluginsModule::AddButton(FText ButtonName, FOnClicked onClickedFunc ,FText toolTip) 
{
	auto button =
		SNew(SButton).Text(ButtonName)
		.ToolTipText(toolTip)
		.OnClicked_Lambda([this]()
		{
			FAssetSearchToolModule& Module = FModuleManager::LoadModuleChecked<FAssetSearchToolModule>("AssetSearchTool");
			Module.ShowToolWindow();
			return FReply::Handled(); 
		});
	vBox->AddSlot().AutoHeight().AttachWidget(button);
}

void FHbbPluginsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry AddContentEntry = FToolMenuEntry::InitComboButton(
					"AddContent",
					FUIAction(),
					FOnGetContent::CreateLambda([this]()
					{
						SAssignNew(vBox , SVerticalBox);
						
						//Asset Search Tool
						AddButton(LOCTEXT("AssetSearchToolButtonName", "Asset Search Tool") , FOnClicked::CreateLambda(
			[this]()
							{
								FAssetSearchToolModule& Module = FModuleManager::LoadModuleChecked<FAssetSearchToolModule>("AssetSearchTool");
								Module.ShowToolWindow();
								return FReply::Handled(); 
							}),
							FText::FromString(TEXT("资产检索工具"))
						);
						
						return vBox.ToSharedRef();
					}),
					LOCTEXT("HbbPluginsLabel", "Hbb Plugins"),
					LOCTEXT("HbbPluginsToolTip", "Hbb unreal engine editor tools."),
					FSlateIcon(FHbbPluginsStyle::Get().GetStyleSetName(), "HbbPlugins.PluginAction")
				);
				Section.AddEntry(AddContentEntry);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHbbPluginsModule, HbbPlugins)