// Copyright Epic Games, Inc. All Rights Reserved.

#include "HbbEditorPlugins.h"

#include "AssetSearchTool.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "HbbEditorPluginsStyle.h"
#include "IContentBrowserSingleton.h"
#include "LevelEditor.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Editor/LevelEditor/Private/SLevelEditor.h"
#include "Engine/TextureCube.h"

static const FName HbbEditorPluginsTabName("HbbEditorPlugins");

#define LOCTEXT_NAMESPACE "FHbbEditorPluginsModule"

void FHbbEditorPluginsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FHbbEditorPluginsStyle::Initialize();
	FHbbEditorPluginsStyle::ReloadTextures();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHbbEditorPluginsModule::RegisterMenus));
}

void FHbbEditorPluginsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHbbEditorPluginsStyle::Shutdown();

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser")); //先拿到内容浏览器
	TArray<FContentBrowserMenuExtender_SelectedPaths>& MenuExtenderDelegates = ContentBrowserModule.GetAllPathViewContextMenuExtenders(); //获取内容浏览器拓展代理数组
	//移除所有与我们在模块加载时创建的代理句柄一样的代理
	MenuExtenderDelegates.RemoveAll([this](const FContentBrowserMenuExtender_SelectedPaths& Delegate){
		return Delegate.GetHandle() == ContentBrowserFolderExtenderDelegateHandle; 
	});
}

void FHbbEditorPluginsModule::AddButton(FText ButtonName, FOnClicked onClickedFunc ,FText toolTip) 
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

void FHbbEditorPluginsModule::RegisterMenus()
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
					LOCTEXT("HbbEditorPluginsLabel", "Hbb Plugins"),
					LOCTEXT("HbbEditorPluginsToolTip", "Hbb unreal engine editor tools."),
					FSlateIcon(FHbbEditorPluginsStyle::Get().GetStyleSetName(), "HbbEditorPlugins.PluginAction")
				);
				Section.AddEntry(AddContentEntry);
			}
		}
	}

	//右键资产菜单栏
	{
		FContentBrowserModule& ContentBrowserModule =FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		auto& menuExt = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		ContentBrowserFolderExtenderDelegate =
		FContentBrowserMenuExtender_SelectedAssets::CreateLambda([&,this](const TArray<FAssetData>& asset)
		{
			TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
			//在资产目录菜单组扩展菜单
			MenuExtender->AddMenuExtension(
			"GetAssetActions",
			EExtensionHook::After,
			nullptr,FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
			{
				//添加菜单
				MenuBuilder.AddMenuEntry(
				FText::FromString(TEXT("批量导出(默认格式)")),
				FText::FromString(TEXT("只支持Texture、StaticMesh、SkeletalMesh")),
				FSlateIcon(),
				//添加该菜单的点击回调事件
				FUIAction(FExecuteAction::CreateLambda([&]()
				{
					FContentBrowserModule& ContentBrowserModule =FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
					TArray<FAssetData> assets;
					TArray<FString> assetPaths;
					TArray<FString> assetPackagePaths;
					TArray<FString> assetName;
					FString savePath;
					ContentBrowserModule.Get().GetSelectedAssets(assets);
					FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
					assetPaths.Reserve(assets.Num());
					assetName.Reserve(assets.Num());
					for(auto& p: assets)
					{
						if(!p.AssetClass.IsEqual(UTexture2D::StaticClass()->GetFName())
							&& !p.AssetClass.IsEqual(UTextureCube::StaticClass()->GetFName())
							&& !p.AssetClass.IsEqual(UStaticMesh::StaticClass()->GetFName())
							&& !p.AssetClass.IsEqual(USkeletalMesh::StaticClass()->GetFName())
							&& !p.AssetClass.IsEqual(USkeleton::StaticClass()->GetFName())
							)
						assetPaths.Add(p.ObjectPath.ToString());
						assetName.Add(p.AssetName.ToString());
						assetPackagePaths.Add(p.PackagePath.ToString());
					}
					//AssetToolsModule.Get().ExportAssetsWithDialog(assetPaths, true);
					FString lastPath = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_EXPORT);
					IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

					FDesktopPlatformModule::Get()->OpenDirectoryDialog(nullptr,TEXT("批量导出,选择路径"),lastPath,savePath);
					auto batchExportCachePath = FPaths::ProjectSavedDir() + "/HbbPluginsBatchExport";
					if(!savePath.IsEmpty())
					{
						AssetToolsModule.Get().ExportAssets(assetPaths,batchExportCachePath);
						//路径提取
						for(int i = 0;  i< assetName.Num() ; i++)
						{
							IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
							
							auto oldPath = batchExportCachePath + assetPackagePaths[i];
							FileManager.CopyDirectoryTree(*savePath ,   *oldPath ,true);
							auto finalDelete  = batchExportCachePath;
							if(FileManager.DirectoryExists(*finalDelete))
							{
								FileManager.DeleteDirectoryRecursively(*finalDelete);
							}
						}
					}
				})
				));
			}));
			return MenuExtender.ToSharedRef();
		});
		menuExt.Add(ContentBrowserFolderExtenderDelegate);
		ContentBrowserFolderExtenderDelegateHandle = menuExt.Last().GetHandle();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHbbEditorPluginsModule, HbbEditorPlugins)