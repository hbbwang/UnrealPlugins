// Copyright Epic Games, Inc. All Rights Reserved.

#include "HbbEditorPlugins.h"

#include "AssetExportTask.h"
#include "AssetSearchTool.h"
#include "AssetToolsModule.h"
#include "BusyCursor.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "HbbEditorPluginsStyle.h"
#include "IContentBrowserSingleton.h"
#include "LevelEditor.h"
#include "ObjectTools.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Editor/LevelEditor/Private/SLevelEditor.h"
#include "Engine/TextureCube.h"
#include "Exporters/Exporter.h"
#include "UObject/GCObjectScopeGuard.h"

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
		.OnClicked(onClickedFunc);
	vBox->AddSlot().AutoHeight().AttachWidget(button);
}
// UE_DISABLE_OPTIMIZATION
void FHbbEditorPluginsModule::AssetsBatchExport()
{
	FContentBrowserModule& ContentBrowserModule =FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FAssetData> assets;
	TArray<FString> assetPaths; 
	TArray<FString> assetPackagePaths;
	TArray<FString> assetName;
	TArray<UObject*> assetObjects;
	TArray<FString> assetClasses;
	FString savePath;
	ContentBrowserModule.Get().GetSelectedAssets(assets);
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	assetPaths.Reserve(assets.Num());
	assetName.Reserve(assets.Num());
	for(auto& p: assets)
	{
		//排除类型
		if(!p.AssetClassPath.GetAssetName().IsEqual(UTexture2D::StaticClass()->GetFName())
			&& !p.AssetClassPath.GetAssetName().IsEqual(UTextureCube::StaticClass()->GetFName())
			&& !p.AssetClassPath.GetAssetName().IsEqual(UStaticMesh::StaticClass()->GetFName())
			&& !p.AssetClassPath.GetAssetName().IsEqual(USkeletalMesh::StaticClass()->GetFName())
			&& !p.AssetClassPath.GetAssetName().IsEqual(USkeleton::StaticClass()->GetFName())
			)
		{
			continue;
		}
		assetPaths.Add(p.ObjectPath.ToString());
		assetName.Add(p.AssetName.ToString());
		assetPackagePaths.Add(p.PackagePath.ToString());
		assetObjects.Add(p.GetAsset());
		assetClasses.Add(p.AssetClassPath.GetAssetName().ToString());
	}
	if(assetPaths.Num() <= 0 )
	{
		FMessageDialog::Open(EAppMsgType::Ok,LOCTEXT("HbbPlugins_AssetsBatchExport_Nothing" , "没有能支持批量导出的文件" ));
		return;
	}
	
	FString lastPath = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_EXPORT);
	FDesktopPlatformModule::Get()->OpenDirectoryDialog(nullptr,TEXT("批量导出,选择路径"),lastPath,savePath);
	struct OutputStruct
	{
		FString Path;
		UObject* Object;
	};
	TArray<OutputStruct> outputPaths;
	
	if(!savePath.IsEmpty())
	{
		FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_EXPORT, savePath);
		//获取所有导出器
		TArray<UExporter*> Exporters;
		ObjectTools::AssembleListOfExporters(Exporters);
		//批处理导出器记录
		TArray<UExporter*> BatchExporters;
		//获取支持的导出格式
		for(auto &o : assetObjects)
		{
			for (int32 ExporterIndex = Exporters.Num() - 1; ExporterIndex >= 0; --ExporterIndex)
			{
				UExporter* Exporter = Exporters[ExporterIndex];
				if (Exporter->SupportedClass)
				{
					const bool bObjectIsSupported = Exporter->SupportsObject(o);
					if (bObjectIsSupported)
					{
						check(Exporter->FormatExtension.Num() == Exporter->FormatDescription.Num());
						check(Exporter->FormatExtension.IsValidIndex(Exporter->PreferredFormatIndex));
						FString outputPath = savePath + "/" + o->GetName();
						
						//完善默认后缀
						if(Exporter->SupportedClass->GetFName().IsEqual(UStaticMesh::StaticClass()->GetFName())
							||Exporter->SupportedClass->GetFName().IsEqual(USkeletalMesh::StaticClass()->GetFName())
							||Exporter->SupportedClass->GetFName().IsEqual(USkeleton::StaticClass()->GetFName())
							)
						{
							outputPath += ".FBX";
						}
						else if(Exporter->SupportedClass->GetFName().IsEqual(UTexture2D::StaticClass()->GetFName()))
						{
							outputPath += ".TGA";
						}
						else if(Exporter->SupportedClass->GetFName().IsEqual(UTextureCube::StaticClass()->GetFName()))
						{
							outputPath += ".HDR";
						}
						
						//看看是不是我们需要的格式的导出器
						bool bFoundExporter = false;
						for (int32 FormatIndex = 0; FormatIndex < Exporter->FormatExtension.Num(); ++FormatIndex)
						{
							const FString& FormatExtension = Exporter->FormatExtension[FormatIndex];
							//if (FCString::Stricmp(*FormatExtension, *FPaths::GetExtension(outputPath)) == 0)
							if (FormatExtension.Equals(FPaths::GetExtension(outputPath),ESearchCase::IgnoreCase))
							{
								bFoundExporter = true;
								break;
							}
						}
						
						if(bFoundExporter && outputPath.Len() > 1)
						{
							//收集导出路径,和Object
							outputPaths.Add({outputPath,o});
							
							if(!BatchExporters.Contains(Exporter))
							{
								BatchExporters.Add(Exporter);
							}
						}
					}
				}
			}
		}
		TArray<UExporter*> AddBatchExporters;
		
		for(auto& p: outputPaths)
		{
			for(auto& e : BatchExporters)
			{	
				if(e->SupportsObject(p.Object))
				{
					const FScopedBusyCursor BusyCursor;
					
					if(!AddBatchExporters.Contains(e))
					{
						e->SetBatchMode(true);
						e->SetCancelBatch(false);
						e->SetShowExportOption(true);
						e->AddToRoot();
						AddBatchExporters.Add(e);
					}
					
					UAssetExportTask* ExportTask = NewObject<UAssetExportTask>();
					FGCObjectScopeGuard ExportTaskGuard(ExportTask);
					ExportTask->Object = p.Object;
					ExportTask->Exporter = e;
					ExportTask->Filename = p.Path;
					ExportTask->bSelected = false;
					ExportTask->bReplaceIdentical = true;
					ExportTask->bPrompt = false;
					ExportTask->bUseFileArchive = ExportTask->IsA(UPackage::StaticClass());
					ExportTask->bWriteEmptyFiles = false;
					
					UExporter::RunAssetExportTask(ExportTask);
					break;
				}
			}
		}
		for(auto& e : BatchExporters)
		{
			e->SetBatchMode(false);
			e->SetCancelBatch(false);
			e->SetShowExportOption(true);
			e->RemoveFromRoot();
		}
		BatchExporters.Empty();
		outputPaths.Empty();
	}
}
// UE_ENABLE_OPTIMIZATION

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

						//资产批量导出
						AddButton(LOCTEXT("AssetBatchExportButtonName", "Asset Batch Export") , FOnClicked::CreateLambda(
					[this]()
							{
								AssetsBatchExport();
								return FReply::Handled(); 
							}),
							FText::FromString(TEXT("(ContentBrowser选中的资产)批量导出"
							"\n该功能主要为了避免每一次都要选择保存目录，过于繁琐\n目前只支持Texture2D、TextureCube、StaticMesh、SkeletalMesh、Skeleton."
							"\nTexture2D的格式是TGA\nTextureCube的格式是HDR\nStaticMesh、SkeletalMesh、Skeleton的格式是FBX"
							))
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
			nullptr,FMenuExtensionDelegate::CreateLambda([this](FMenuBuilder& MenuBuilder)
			{
				//添加菜单
				MenuBuilder.AddMenuEntry(
				FText::FromString(TEXT("批量导出")),
				FText::FromString(TEXT("该功能主要为了避免每一次都要选择保存目录，过于繁琐\n目前只支持Texture2D、TextureCube、StaticMesh、SkeletalMesh、Skeleton."
					"\nTexture2D的格式是TGA\nTextureCube的格式是HDR\nStaticMesh、SkeletalMesh、Skeleton的格式是FBX")),
				FSlateIcon(),
				//添加该菜单的点击回调事件
				FUIAction(FExecuteAction::CreateLambda([this]()
				{
					AssetsBatchExport();
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