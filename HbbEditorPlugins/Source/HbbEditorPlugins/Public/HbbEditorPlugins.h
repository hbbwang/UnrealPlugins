// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FHbbEditorPluginsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//资产批量导出,与引擎自带导出不一样的地方是它不会每一次都询问保存路径。
	void AssetsBatchExport(TArray<FAssetData> assets);
	
private:

	//给工具菜单添加按钮
	void AddButton(FText ButtonName, FOnClicked onClickedFunc,FText toolTip = FText::GetEmpty());
	
	void RegisterMenus();

	TSharedPtr<SVerticalBox> vBox;

	FContentBrowserMenuExtender_SelectedAssets ContentBrowserFolderExtenderDelegate;
	
	FDelegateHandle ContentBrowserFolderExtenderDelegateHandle;
};
