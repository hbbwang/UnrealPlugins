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
	
private:

	void AddButton(FText ButtonName, FOnClicked onClickedFunc,FText toolTip = FText::GetEmpty());
	
	TSharedPtr<SVerticalBox> vBox;

	FContentBrowserMenuExtender_SelectedAssets ContentBrowserFolderExtenderDelegate;
	
	FDelegateHandle ContentBrowserFolderExtenderDelegateHandle;
	
	void RegisterMenus();
};
