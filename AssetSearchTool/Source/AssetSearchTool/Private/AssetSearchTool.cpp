// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetSearchTool.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "EditorAssetLibrary.h"
#include "LevelEditor.h"
#include "ObjectTools.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Editor/EditorWidgets/Public/SAssetDropTarget.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "IAssetTools.h"
#include "MaterialEditorUtilities.h"
#include "MaterialPropertyHelpers.h"
#include "Engine/Texture2DArray.h"
#include "Engine/TextureCube.h"
#include "MaterialEditor/DEditorScalarParameterValue.h"
#include "MaterialEditor/DEditorStaticComponentMaskParameterValue.h"
#include "MaterialEditor/DEditorStaticSwitchParameterValue.h"
#include "MaterialEditor/DEditorTextureParameterValue.h"
#include "MaterialEditor/DEditorVectorParameterValue.h"
#include "Widgets/Layout/SScrollBox.h"
#include "DetailWidgetRow.h"
#include "Async/Async.h"
#include "Misc/FileHelper.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "EngineUtils.h"
#include "IContentBrowserSingleton.h"
#include "Widgets/Text/SMultiLineEditableText.h"
static const FName AssetSearchToolTabName("AssetSearchTool");

#define LOCTEXT_NAMESPACE "FAssetSearchToolModule"

TMap<FString, TSharedPtr<CustomResultTable>>FAssetSearchToolModule::AllCustomResultTables;
TArray<FAssetSearchToolMaterialInstanceResult> FAssetSearchToolModule::MaterialInstanceResultData;

#define MakeFText(t) FText::FromString(TEXT(#t))
#define MakeFTextFromString(t) FText::FromString(t)

void FAssetSearchToolModule::StartupModule()
{
	FTickerDelegate TickDelegate = FTickerDelegate::CreateRaw(this,&FAssetSearchToolModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
	//
	// FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	// TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	// ToolbarExtender->AddToolBarExtension("Hbb Plugins", EExtensionHook::After, nullptr,
	// 	FToolBarExtensionDelegate::CreateRaw(this, &FAssetSearchToolModule::AddToolbarExtension));
	// LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}
//
// void FAssetSearchToolModule::AddToolbarExtension(FToolBarBuilder& Builder)
// {
// 	// Builder.BeginSection("Build");
// 	// Builder.AddToolBarButton(
// 	// 	FUIAction(
// 	// 		FExecuteAction::CreateLambda([this]()
// 	// 		{
// 	// 			
// 	// 		})
// 	// 	),
// 	// 	NAME_None,
// 	// 	FText::FromString(TEXT("A")),
// 	// 	FText::FromString(TEXT("Builds the database from all the animation sequences that reference this database through their codec.")),
// 	// 	FSlateIcon(FAppStyle::GetAppStyleSetName(), "Persona.ApplyCompression")
// 	// );
// 	// Builder.EndSection();
// 	//
// 	// Builder.AddComboButton(
// 	// 	FUIAction(),
// 	// 	FOnGetContent::CreateLambda([this]()
// 	// 	{
// 	// 		
// 	// 	}),
// 	// 	LOCTEXT("", "Compile Options"),
// 	// 	LOCTEXT("Compile_Options_Tooltip", "Change Compile Options"),
// 	// 	TAttribute<FSlateIcon>(),
// 	// 	true);
// }

void FAssetSearchToolModule::ShutdownModule()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	for(auto& i : AllCustomResultTables)
	{
		i.Value.Reset();
	}
	AllCustomResultTables.Empty();
	MaterialInstanceResultData.Empty();
	
	SearchType.Empty();
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AssetSearchToolTabName);
}

// void FAssetSearchToolModule::AddMaterialAssetUsageToolbarButton(FToolBarBuilder& Builder)
// {
// 	Builder.AddToolBarButton(
// 			FUIAction(FExecuteAction::CreateRaw(
// 				this,&FAssetSearchToolModule::ShowMaterialAssetUsageTable
// 			)),
// 			NAME_None,
// 			FText::FromString("Material Usage"),
// 			FText::FromString("Show Material Asset Usage Table."),
// 			FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Material")
// 		);
// }

void FAssetSearchToolModule::ReferenceSearchResultResultToMaterialInsSearchResult()
{
	// FAssetSearchToolModule::MaterialInstanceResultData.Empty();
	// Parent->currentMaterialInstanceResultDataFromWindowText->SetText(MakeFTextFromString(FString(TEXT("当前锁定:")) + parentMaterialName + " , " + FString::FromInt(ResultOutput.Num())));
	// Parent->currentMaterialInstanceResultDataFromWindowText->SetColorAndOpacity(FLinearColor(0,1,0,1));
	// localText = Parent->currentMaterialInstanceResultDataFromWindowText->GetText().ToString();
	// FAssetSearchToolModule::MaterialInstanceResultData = ResultOutput;
	if(refAssetPathResult.Num() >0)
	{
		FAssetSearchToolModule::MaterialInstanceResultData.Empty();
		FAssetSearchToolModule::MaterialInstanceResultData.Reserve(refAssetPathResult.Num());
		//FAssetSearchToolModule::MaterialInstanceResultData = ResultOutput;
		for(auto&i:refAssetPathResult)
		{
			FString FullAssetPathName = FString::Printf(TEXT("%s.%s"), *i, *FPackageName::GetLongPackageAssetName(i));
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			FAssetData asset;
			asset = AssetRegistryModule.Get().GetAssetByObjectPath(*FullAssetPathName);
			if(asset.AssetClass.IsEqual(UMaterialInstance::StaticClass()->GetFName()))
			{
				FAssetSearchToolMaterialInstanceResult newResult;
				newResult.AssetData = asset;
				newResult.InstanceAssetPath = asset.ObjectPath.ToString();
				newResult.bIsMaterialInsSearchResult = false;
				FAssetSearchToolModule::MaterialInstanceResultData.Add(newResult);
			}
		}
		this->currentMaterialInstanceResultDataFromWindowText->SetText(MakeFTextFromString(FString(TEXT("当前锁定:[引用搜索模式 Mat_ins] , ")) + FString::FromInt(MaterialInstanceResultData.Num()) ));
		this->currentMaterialInstanceResultDataFromWindowText->SetColorAndOpacity(FLinearColor(0,1,0,1));
	}
}

void FAssetSearchToolModule::ShowToolWindow()
{
	if(SearchMode.Num()<=0)
	{
		SearchMode.Add(MakeShareable(new FString(TEXT("引用搜索"))));
		SearchMode.Add(MakeShareable(new FString(TEXT("材质实例搜索"))));
		BeginSearchPath = TEXT("/Game/");
		BeginSearchExcludePath = TEXT("/Engine/;/Game/Editor/;/Game/NoPack/;/Game/Maps/;/Game/L10N/");
		
		MaterialInstanceBeginSearchPath =  MakeFText(/Game/);
		MaterialInstanceBeginSearchPathExclude=  MakeFTextFromString(TEXT("/Engine/;/Game/Editor/;/Game/NoPack/;/Game/Maps/;/Game/L10N/"));
	}
	CurrentSearchMode = SearchMode[0];
	if(ParentMode.Num()<=0)
	{
		ParentMode.Add(MakeShareable(new FString(TEXT("所有"))));
		ParentMode.Add(MakeShareable(new FString(TEXT("被依赖对象(左)"))));
		ParentMode.Add(MakeShareable(new FString(TEXT("依赖对象(右)"))));
		CurrentParentMode = ParentMode[0];	
	}
	if(SearchType.Num()<=0)
	{
		SearchType.Add(UParticleSystem::StaticClass());
		SearchType.Add(UMaterial::StaticClass());
		SearchType.Add(UMaterialInstanceConstant::StaticClass());
		SearchType.Add(UBlueprint::StaticClass());
		SearchType.Add(UTexture2D::StaticClass());
		SearchType.Add(UTextureCube::StaticClass());
		SearchType.Add(UStaticMesh::StaticClass());
		SearchType.Add(USkeletalMesh::StaticClass());
		SearchType.Add(UWorld::StaticClass());
		
		CurrentHeaderType = SearchType[0];
		CurrentEndType = SearchType[0];
	}
	if(MaterialInstanceSearchType.Num()<=0)
	{
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Scalar"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Vector"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("StaticSwitch"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Texture2D"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("TextureCube"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Texture2DArray"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("BlendMode"))));
		MaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("TwoSide"))));

		NewMaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Scalar"))));
		NewMaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("Vector"))));
		NewMaterialInstanceSearchType.Add(MakeShareable(new FString(TEXT("StaticSwitch"))));
		
		CurrentMaterialInstanceType = MaterialInstanceSearchType[0];
		NewCurrentMaterialInstanceType = NewMaterialInstanceSearchType[0];
		
		MaterialInstanceScalarOP.Add(MakeShareable(new FString(TEXT("等于"))));
		MaterialInstanceScalarOP.Add(MakeShareable(new FString(TEXT("大于"))));
		MaterialInstanceScalarOP.Add(MakeShareable(new FString(TEXT("小于"))));
		CurrentMaterialInstanceScalarOP = MaterialInstanceScalarOP[0];
		
		MaterialInstanceParentMaterial = TEXT("M_Base");
		
	}
	TSharedRef<SWindow> AssetSearchToolWindow = SNew(SWindow)
	.Title(FText::FromString(TEXT("资产检索工具")))
	.ClientSize(FVector2D(800,400))
	.SupportsMaximize(true)
	.SupportsMinimize(true)
	.HasCloseButton(true)
	.IsTopmostWindow(true)
	;
	AssetSearchToolWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda(
		[this](const TSharedRef<SWindow>& win)
	{
			UPackage* AssetPackage = FindPackage(nullptr, TEXT("/Game/AssetSearchResult"));
			if (AssetPackage)
			{
				// 确保包已完全加载
				AssetPackage->FullyLoad();
				// 将包标记为已加载
				AssetPackage->MarkAsFullyLoaded();
				// 删除包
				AssetPackage->ConditionalBeginDestroy();

				FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
				TArray<FString> pathArray = {ContentBrowserModule.Get().GetCurrentPath().GetInternalPathString()};
				ContentBrowserModule.Get().SyncBrowserToFolders(pathArray,true);
				
				// 输出删除成功的消息
				UE_LOG(LogTemp, Log, TEXT("Asset deleted successfully."));
			}
			//销毁搜索窗口
			for(auto& i : AllCustomResultTables)
			{
				i.Value.Reset();
			}
			AllCustomResultTables.Empty();
			SearchType.Empty();
	}));
	Box_ReferenceSearch = SNew(SBox);
	Box_MaterialInstanceSearch = SNew(SBox);
	
	auto mainVBox = SNew(SVerticalBox);
	mainVBox->AddSlot().AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("模式")))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(10)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&SearchMode)
			.InitiallySelectedItem(SearchMode[0])
			.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(*Item));
			})
			.OnSelectionChanged_Lambda([this](TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
			{
				CurrentSearchMode = Item;
				if(CurrentSearchMode.IsValid())
				{
					Box_ReferenceSearch->SetVisibility(CurrentSearchMode->Equals(*SearchMode[0])?EVisibility::Visible : EVisibility::Collapsed);
					Box_MaterialInstanceSearch->SetVisibility(CurrentSearchMode->Equals(*SearchMode[1])?EVisibility::Visible : EVisibility::Collapsed);
				}
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(*CurrentSearchMode);
				})
			]
		]
	];
	
	//引用模式
	{
		Box_ReferenceSearch->SetVisibility(EVisibility::Visible);
		auto beginHeaderText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
	        .ShadowOffset(FIntPoint(-1, 1))
	        .Text(FText::FromString(TEXT("从什么类型开始搜索")));
		auto beginHeaderCombbox = SNew(SComboBox<UClass*>)
			.OptionsSource(&SearchType)
			.InitiallySelectedItem(SearchType[0])
			.OnGenerateWidget_Lambda([this](UClass* Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(Item->GetName()));
			})
				.OnSelectionChanged_Lambda([this](UClass* Item, ESelectInfo::Type SelectInfo)
			{
				CurrentHeaderType = Item;	
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(CurrentHeaderType->GetName());
				})
			];
		
		auto beginSearchText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.Text(FText::FromString(TEXT("搜索路径")));
		auto beginSearchPathInput = SNew(SEditableTextBox)
			.Text(MakeFTextFromString(BeginSearchPath))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				BeginSearchPath = InText.ToString();
				BeginSearchPath.TrimStartAndEndInline();
			});

		auto BeginSearchExcludeText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.Text(FText::FromString(TEXT("排除路径")));
		auto BeginSearchExcludeInput = SNew(SEditableTextBox)
			.Text(MakeFTextFromString(BeginSearchExcludePath))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				BeginSearchExcludePath = InText.ToString();
				BeginSearchExcludePath.TrimStartAndEndInline();
			});
		
		auto endText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("需要搜索什么类型的资产")));
		auto endCombbox = SNew(SComboBox<UClass*>)
			.OptionsSource(&SearchType)
			.InitiallySelectedItem(SearchType[0])
			.OnGenerateWidget_Lambda([this](UClass* Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(Item->GetName()));
			})
				.OnSelectionChanged_Lambda([this](UClass* Item, ESelectInfo::Type SelectInfo)
			{
				CurrentEndType = Item;	
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(CurrentEndType->GetName());
				})
			];
		
		auto parentText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("引用关系")));
		auto parentCombbox = SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&ParentMode)
			.InitiallySelectedItem(ParentMode[0])
			.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(*Item));
			})
			.OnSelectionChanged_Lambda([this](TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
			{
				CurrentParentMode = Item;
				if(CurrentParentMode->Equals(*ParentMode[0]))
				{
					SearchRefIndex = 0 ;
				}
				else if(CurrentParentMode->Equals(*ParentMode[1]))
				{
					SearchRefIndex = 1;
				}
				else if(CurrentParentMode->Equals(*ParentMode[2]))
				{
					SearchRefIndex = 2;
				}
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(*CurrentParentMode);
				})
			];

		auto onlyResultText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFTextFromString(TEXT("只显示最终结果,不显示其他内容")))
			.Text(FText::FromString(TEXT("是否只要最终结果")));
		auto onlyResultCheckBox = SNew(SCheckBox)
			.ToolTipText(MakeFTextFromString(TEXT("只显示最终结果,不显示其他内容")))
			.IsChecked(false)
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bOnlyResult = CheckState == ECheckBoxState::Checked? true:false;
			});

		auto allReferenceLevelText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFTextFromString(TEXT("(建议能不用就不用,资产多的时候会搜上半天)一直搜索(同类型资产)到没有找到引用为止")))
			.Text(FText::FromString(TEXT("查找出所有引用等级")));
		auto allReferenceLevelCheckBox = SNew(SCheckBox)
			.IsChecked(false)
			.ToolTipText(MakeFTextFromString(TEXT("(建议能不用就不用,资产多的时候会搜上半天)一直搜索到没有找到引用为止")))
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bSearchAllReferenceLevel = CheckState == ECheckBoxState::Checked? true:false;
			});
		
		auto execButton = SNew(SButton)
		.Text(MakeFText(执行)).OnClicked_Lambda([this]()
		{
			if(DataTable)
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseAllEditorsForAsset(DataTable);
			SearchReference();
			
			return FReply::Handled();
		});

		ProgressBar_ReferenceSearch = SNew(SProgressBar)
		.Percent(0.0f)
		.BarFillType(EProgressBarFillType::LeftToRight)
		.FillColorAndOpacity(FLinearColor(0,1,0.2,1));

		auto vBox = SNew(SVerticalBox);
		auto hBox_beginHeader = SNew(SHorizontalBox);
		hBox_beginHeader->AddSlot().VAlign(VAlign_Center).AttachWidget(beginHeaderText);
		hBox_beginHeader->AddSlot().VAlign(VAlign_Center).Padding(5,0,0,0).AttachWidget(beginHeaderCombbox);
		vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(hBox_beginHeader);

		{
			auto aubHbox = SNew(SHorizontalBox);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(beginSearchText);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(beginSearchPathInput);
			vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(aubHbox);
		}

		{
			auto aubHbox = SNew(SHorizontalBox);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(BeginSearchExcludeText);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(BeginSearchExcludeInput);
			vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(aubHbox);
		}
		
		auto hBox_end = SNew(SHorizontalBox);
		hBox_end->AddSlot().VAlign(VAlign_Center).AttachWidget(endText);
		hBox_end->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(endCombbox);
		vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(hBox_end);

		{
			auto aubHbox = SNew(SHorizontalBox);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(parentText);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(parentCombbox);
			vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(aubHbox);
		}
		{
			auto aubHbox = SNew(SHorizontalBox);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(onlyResultText);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(onlyResultCheckBox);
			vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(aubHbox);
		}
		{
			auto aubHbox = SNew(SHorizontalBox);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(allReferenceLevelText);
			aubHbox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(allReferenceLevelCheckBox);
			vBox->AddSlot().VAlign(VAlign_Center).AttachWidget(aubHbox);
		}
		vBox->AddSlot().VAlign(VAlign_Center).Padding(0,30,0,0).AttachWidget(ProgressBar_ReferenceSearch.ToSharedRef());
		vBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(execButton);
		
		Box_ReferenceSearch->SetContent(vBox);
		mainVBox->AddSlot().AutoHeight().AttachWidget(Box_ReferenceSearch.ToSharedRef());
	}
	//材质实例搜索模式
	{
		Box_MaterialInstanceSearch->SetVisibility(EVisibility::Collapsed);
		materialInsSearchtipText = SNew(STextBlock).ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("")));
		
		auto beginSearchText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.Text(FText::FromString(TEXT("搜索路径")));
		auto beginSearchPathInput = SNew(SEditableTextBox)
			.Text(MaterialInstanceBeginSearchPath)
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				MaterialInstanceBeginSearchPath = InText;
			});

		auto beginSearchExculdeText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.Text(FText::FromString(TEXT("排除搜索路径(多选,用;区分)")));
		auto beginSearchExculdePathInput = SNew(SEditableTextBox)
			.Text(MaterialInstanceBeginSearchPathExclude)
			.ToolTipText(MakeFText(常见的:\n全局路径: All(或者什么都不填)\n引擎路径:/Engine/\n项目路径:/Game/\n其余的自行按照格式填写))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				MaterialInstanceBeginSearchPathExclude = InText;
			});


		TSharedPtr<SEditableTextBox> parentPathInput;
		auto parentPathInputAssetDrag = SNew(SAssetDropTarget)
		.OnIsAssetAcceptableForDrop_Lambda([](const UObject* InObject)
		{
			return InObject && (InObject->IsA<UMaterial>() || InObject->IsA<UMaterialInstanceConstant>());
		})
		.OnAssetDropped_Lambda([parentPathInput](const FAssetData& InAssetData)
		{
			parentPathInput->SetText(FText::FromString(InAssetData.ObjectPath.ToString()));
		})
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(0,2,0,0)
			[
				SNew(STextBlock)
				.Text(MakeFTextFromString(TEXT("母材质Path(强烈建议设置)")))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5,2,0,0)
			[
				SAssignNew(parentPathInput,SEditableTextBox)
				.Text(MakeFTextFromString(MaterialInstanceParentMaterial))
				.ToolTipText(MakeFTextFromString(TEXT("同时支持材质和材质实例资产,母材质球的资产路径,强烈建议设置,如果设置的母材质球,\n则会从母材质球的引用里进行查找,就不会进行全局查找,效率会很多。")))
				.OnTextChanged_Lambda([this](const FText& InText)
				{
					MaterialInstanceParentMaterial = InText.ToString();
					MaterialInstanceParentMaterial.TrimStartAndEndInline();
				})
			]
		];
		
		auto refCountText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("最高引用等级(需要指定母材质)")))
			.ToolTipText(MakeFTextFromString(TEXT("查找多少轮(等级)引用,范围是0-9999,一般也不会有这么高级...\n当没有设置母材质的时候,将会返回:\"?\"")));
		auto refCountInput = SNew(SEditableTextBox)
			.Text(MakeFTextFromString(FString::FromInt(MaterialInstanceRefCount)))
			.ToolTipText(MakeFTextFromString(TEXT("查找多少轮(等级)引用,范围是0-9999,一般也不会有这么高级...\n当没有设置母材质的时候,将会返回:\"?\"")))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				// 获取输入的字符串
				FString InputString = InText.ToString();

				// 检查输入的字符串是否仅包含数字
				bool bIsNumeric = InputString.IsNumeric();

				// 如果输入的字符串包含非数字字符，则删除这些字符
				if (!bIsNumeric)
				{
					FString FilteredString;
					for (const TCHAR& Char : InputString)
					{
						if (FChar::IsDigit(Char))
						{
							FilteredString.AppendChar(Char);
						}
					}
					InputString = FilteredString;
				}
				MaterialInstanceRefCount = FCString::Atoi(*InputString);
			});

		auto paramHBox = SNew(SHorizontalBox);
		paramHBox->SetVisibility(EVisibility::Visible);
		UpdateMaterialInstanceSearchType(paramHBox);
		auto materialParamTypeText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("搜索过滤参数类型")));	
		auto materialParamTypeComboBox = SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&MaterialInstanceSearchType)
			.InitiallySelectedItem(MaterialInstanceSearchType[0])
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(*Item));
			})
			.OnSelectionChanged_Lambda([=,this](TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
			{
				CurrentMaterialInstanceType = Item;
				UpdateMaterialInstanceSearchType(paramHBox);
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(*CurrentMaterialInstanceType);
				})
			];

		auto ignoreUnValidParamText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("忽略材质球里没有连接的参数")));
		auto ignoreUnValidParamCheckBox = SNew(SCheckBox)
			.IsChecked(true)
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bIgnoreUnValidParam = CheckState == ECheckBoxState::Checked? true:false;
			});
		
		auto paramOverrideText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("只需要重写过参数的材质实例")));
		auto paramOverrideCheckBox = SNew(SCheckBox)
			.IsChecked(true)
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bOveriddenOnly = CheckState == ECheckBoxState::Checked? true:false;
			});

		auto onlyOneParamText = SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("只搜索一个参数")));
		auto onlyOneParamCheckBox = SNew(SCheckBox)
			.IsChecked(true)
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bOnlyOneParam = CheckState == ECheckBoxState::Checked? true:false;
			});
		
		auto execButton = SNew(SButton)
			.Text(MakeFText(执行搜索(建议上面的参数好好设置一下能提高搜索效率))).OnClicked_Lambda([this]()
			{
				materialInsSearchtipText->SetText(MakeFText(执行中...可能要等一下));
				materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
				if(DataTable)
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseAllEditorsForAsset(DataTable);
				SearchMaterialInstances();
				//if(DataTable)
				//	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(DataTable);
				bool bNeedCreate = true;
				if(MaterialInstanceResultPath.Num()>0)
				{
					auto result = AllCustomResultTables.Find(MaterialInstanceParentMaterial);
					if(result)
					{
						FSlateApplication& SlateApplication = FSlateApplication::Get();
						if((*result)->bIsWindowClosed)
						{
							result->Reset();
							AllCustomResultTables.Remove(MaterialInstanceParentMaterial); 
						}
						else
						{
							bNeedCreate = false;
							(*result)->UpdateResult(MaterialInstanceResultPath);
						}
					}
					if(bNeedCreate)
					{
						AllCustomResultTables.Add(MaterialInstanceParentMaterial , MakeShareable(new CustomResultTable(this ,MaterialInstanceParentMaterial,MaterialInstanceResultPath, true)));
					}
				}
				
				return FReply::Handled();
			});

		auto openMatButton = SNew(SButton)
			.Text(MakeFText(打开所有材质实例编辑器(需要先搜索))).OnClicked_Lambda([this]()
			{
				// 获取AssetTools模块
				//IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				for(auto &i: MaterialInstanceResultPath)
				{
					auto assetData = AssetRegistryModule.Get().GetAssetByObjectPath(*i.InstanceAssetPath);
					FMaterialEditorUtilities::OnOpenMaterial(assetData);
				}
				return FReply::Handled();
			});
		

		//-------------------------------------------------------------
		
		//批量设置新参数
		auto materialNewValueTitleText = SNew(STextBlock).ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(TEXT("一键【批量修改】搜索到的所有材质实例参数\n(使用前请谨慎查看资产列表是否有误)")));

		currentMaterialInstanceResultDataFromWindowText = SNew(STextBlock).ShadowColorAndOpacity(FLinearColor::Black)
			.ToolTipText(MakeFTextFromString(TEXT("显示当前应该修改哪一个搜索结果,如果是未知,那就会修改失败。"))) 
			.ShadowOffset(FIntPoint(-1, 1))
			.Text(FText::FromString(FString(TEXT("当前锁定:")) + TEXT("未知")))
			.ColorAndOpacity(FLinearColor(1,0,0,1));
		
		auto newParamTypeText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("需要修改的参数类型")));	
		auto newParamTypeComboBox = SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&NewMaterialInstanceSearchType)
			.InitiallySelectedItem(NewMaterialInstanceSearchType[0])
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
			{
				return SNew(STextBlock).Text(MakeFTextFromString(*Item));
			})
			.OnSelectionChanged_Lambda([=,this](TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
			{
				NewCurrentMaterialInstanceType = Item;
			})
			[
				SNew(STextBlock).Text_Lambda([this]()
				{
					return FText::FromString(*NewCurrentMaterialInstanceType);
				})
			];

		auto newOverrideTypeText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("是否继承/(保留)重写开关")))
			.ToolTipText(MakeFTextFromString(FString(TEXT("此选项【默认打开】,参数的重写开关将会【原封不动】,\n")) + 
				TEXT("如果关闭了,则修改后的材质实例参数名字前面的勾会被【强制打开】\n【注意】,当搜索结果出现【同一个材质实例,搜索出了多个参数,且重写状态不一致】,将会随机选择!!!\n执行【一键批量修改选中材质实例】不受随机选择影响")));
		auto newOverrideTypeCheckBox = SNew(SCheckBox)
			.IsChecked(bNewMaterialInstanceOverrideInherit)
			.ToolTipText(MakeFTextFromString(FString(TEXT("此选项【默认打开】,参数的重写开关将会【原封不动】,\n")) + 
				TEXT("如果关闭了,则修改后的材质实例参数名字前面的勾会被【强制打开】\n【注意】,当搜索结果出现【同一个材质实例,搜索出了多个参数,且重写状态不一致】,将会随机选择!!!\n执行【一键批量修改选中材质实例】不受随机选择影响")))
			.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckState)
			{
				bNewMaterialInstanceOverrideInherit = CheckState == ECheckBoxState::Checked? true : false;
			});
		auto materialNewValueNameText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("参数名(需要完整填写)")));	
		auto materialNewValueName = SNew(SEditableTextBox)
			.Text(MakeFTextFromString(TEXT("")))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				NewMaterialInstanceParamName = InText.ToString();
			});
		auto materialNewValueText = SNew(STextBlock)
			.Text(MakeFTextFromString(TEXT("修改为")));	
		auto materialNewValue = SNew(SEditableTextBox)
			.Text(MakeFTextFromString(TEXT("")))
			.ToolTipText(MakeFTextFromString(TEXT("请注意值的格式,不符合转换规则的,将会被忽略(也就是修改失败)")))
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				NewMaterialInstanceParamValue = InText.ToString();
			});
		auto execNewValueButton = SNew(SButton)
			.Text(MakeFText(一键批量修改(此按钮只针对左边的搜索结果))).OnClicked_Lambda([this]()
			{
				if(MaterialInstanceResultData.Num() > 0)
				{
					if(!currentMaterialInstanceResultDataFromWindowText->GetText().ToString().Contains(TEXT("未知")))
					{
						SetMaterialInstancesParameterValue(MaterialInstanceResultData, bNewMaterialInstanceOverrideInherit);
					}
					else
					{
						materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("请锁定需要修改的搜索结果.")));
						materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1,1,0.0,1));
					}
				}
				return FReply::Handled();
			});

		auto execNewValueForSelectionMaterialButton = SNew(SButton)
			.Text(MakeFText(一键批量修改选中材质实例(针对内容浏览器))).OnClicked_Lambda([this]()
			{
				SetMaterialInstancesParameterValueFromSelection();
				return FReply::Handled();
			});

		
		TSharedPtr<SEditableTextBox> setNewParentPathInput;
		auto setNewParentPathInputAssetDrag = SNew(SAssetDropTarget)
		.OnIsAssetAcceptableForDrop_Lambda([](const UObject* InObject)
		{
			return InObject && (InObject->IsA<UMaterial>() || InObject->IsA<UMaterialInstanceConstant>());
		})
		.OnAssetDropped_Lambda([setNewParentPathInput](const FAssetData& InAssetData)
		{
			setNewParentPathInput->SetText(FText::FromString(InAssetData.ObjectPath.ToString()));
		})
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(0,2,0,0)
			[
				SNew(STextBlock)
				.Text(MakeFTextFromString(TEXT("设置新的Parent路径")))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(0,2,0,0)
			[
				SAssignNew(setNewParentPathInput,SEditableTextBox)
				.Text(MakeFTextFromString(TEXT("")))
				.ToolTipText(MakeFTextFromString(TEXT("为材质实例设置新的Parent,支持完整路径或者关键字(为了准确最好使用完整路径)")))
				.OnTextChanged_Lambda([this](const FText& InText)
				{
					SetNewParentForMaterialInstance = InText.ToString();
					SetNewParentForMaterialInstance.TrimStartAndEndInline();
				})
			]
		];
		
		auto setParentButton = SNew(SButton)
			//.IsEnabled(false)
			.Text(MakeFText(一键批量修改Parent(针对左边搜索结果))).OnClicked_Lambda([this]()
			{
				if(MaterialInstanceResultData.Num() > 0)
				{
					if(!currentMaterialInstanceResultDataFromWindowText->GetText().ToString().Contains(TEXT("未知")))
					{
						SetMaterialInstanceParent(MaterialInstanceResultData);
					}
					else
					{
						materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("请锁定需要修改的搜索结果.")));
						materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1,1,0.0,1));
					}
				}
				return FReply::Handled();
			});
		
		auto setParent2Button = SNew(SButton)
			//.IsEnabled(false)
			.Text(MakeFText(一键批量修改[选中的]Parent(针对内容浏览器))).OnClicked_Lambda([this]()
			{
				FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
				TArray<FAssetData> selections;
				ContentBrowserModule.Get().GetSelectedAssets(selections);
				TArray<FAssetSearchToolMaterialInstanceResult> selectionResults;
				selectionResults.SetNum(selections.Num());
				for(int i = 0 ; i < selectionResults.Num();i++)
				{
					FAssetSearchToolMaterialInstanceResult newResult;
					newResult.InstanceAssetPath = selections[i].ObjectPath.ToString();
					selectionResults.Add(newResult);
				}
				
				SetMaterialInstanceParent(selectionResults);
				return FReply::Handled();
			});

		
		auto tipHBox = SNew(SHorizontalBox);
		auto beginSearchHBox = SNew(SHorizontalBox);
		beginSearchHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(beginSearchText);
		beginSearchHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(beginSearchPathInput);
		auto beginSearchExcludeHBox = SNew(SHorizontalBox);
		beginSearchExcludeHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(beginSearchExculdeText);
		beginSearchExcludeHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(beginSearchExculdePathInput);
		
		auto parentHBox = SNew(SHorizontalBox);
		parentHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(parentPathInputAssetDrag);
		
		auto refCountHBox = SNew(SHorizontalBox);
		refCountHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(refCountText);
		refCountHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(refCountInput);
		auto typeHBox = SNew(SHorizontalBox);
		typeHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(materialParamTypeText);
		typeHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(materialParamTypeComboBox);
		auto ignoreUnValidParamHBox = SNew(SHorizontalBox);
		ignoreUnValidParamHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(ignoreUnValidParamText);
		ignoreUnValidParamHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(ignoreUnValidParamCheckBox);
		auto overrideHBox = SNew(SHorizontalBox);
		overrideHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(paramOverrideText);
		overrideHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(paramOverrideCheckBox);
		auto onlyOneParamHBox = SNew(SHorizontalBox);
		onlyOneParamHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(onlyOneParamText);
		onlyOneParamHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(onlyOneParamCheckBox);
		auto execButtonBox = SNew(SHorizontalBox);
		execButtonBox->AddSlot().AttachWidget(execButton);

		auto newOverrideTypeHBox = SNew(SHorizontalBox);
		newOverrideTypeHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(newOverrideTypeText);
		newOverrideTypeHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(newOverrideTypeCheckBox);
		
		auto setNewTypeHBox = SNew(SHorizontalBox);
		setNewTypeHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(newParamTypeText);
		setNewTypeHBox->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(newParamTypeComboBox);
		
		auto setNewValueHBox = SNew(SHorizontalBox);
		{
			auto SubHbox = SNew(SHorizontalBox);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(materialNewValueNameText);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(materialNewValueName);
			setNewValueHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(SubHbox);
		}
		{
			auto SubHbox = SNew(SHorizontalBox);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(materialNewValueText);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(materialNewValue);
			setNewValueHBox->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(SubHbox);
		}

		auto execNewValueButtonBox = SNew(SHorizontalBox);
		execNewValueButtonBox->AddSlot().AttachWidget(execNewValueButton);
		
		//分界线
		auto materialNewValueSplitter =SNew(SSplitter)
		.Orientation(EOrientation::Orient_Horizontal);
		
		auto mainvBox = SNew(SVerticalBox);
		mainvBox->AddSlot().AutoHeight().AttachWidget(materialInsSearchtipText.ToSharedRef());
		{
			auto vBox = SNew(SVerticalBox);
			auto scroll =  SNew(SScrollBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(beginSearchHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(beginSearchExcludeHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(parentHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(refCountHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(ignoreUnValidParamHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(overrideHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(onlyOneParamHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(typeHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(paramHBox);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(execButtonBox);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(openMatButton);
			
			scroll->AddSlot().AttachWidget(vBox);
			
			materialNewValueSplitter->AddSlot().AttachWidget(scroll);
		}
		{
			auto vBox = SNew(SVerticalBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(materialNewValueTitleText);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(currentMaterialInstanceResultDataFromWindowText.ToSharedRef());
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(newOverrideTypeHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(setNewTypeHBox);
			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(setNewValueHBox);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(execNewValueButtonBox);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(execNewValueForSelectionMaterialButton);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(setNewParentPathInputAssetDrag);

			vBox->AddSlot().AutoHeight().Padding(0,2,0,0).AttachWidget(setParentButton);
			vBox->AddSlot().AutoHeight().Padding(0,10,0,0).AttachWidget(setParent2Button);
			
			materialNewValueSplitter->AddSlot().AttachWidget(vBox);
		}
		mainvBox->AddSlot().AutoHeight().AttachWidget(materialNewValueSplitter);
		Box_MaterialInstanceSearch->SetContent(mainvBox);
		mainVBox->AddSlot().AutoHeight().AttachWidget(Box_MaterialInstanceSearch.ToSharedRef());
	}
	
	AssetSearchToolWindow->SetContent(mainVBox);
	
	// 获取Slate应用程序实例
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	SlateApplication.AddWindow(AssetSearchToolWindow,true);
	
	
}

bool FAssetSearchToolModule::Tick(float DeltaTime)
{
	if(ProgressBar_ReferenceSearch.IsValid())
	{
		ProgressBar_ReferenceSearch->SetPercent(RefSearchProgress);
	}
	
	return true;
}

int count = 0;
int refCount = 0;
int assetCount = 0;
const FAssetData* ReferenceFrom = NULL;
FString FirstReferenceObjectPath;
void FAssetSearchToolModule::SearchReference()
{
	//从什么类型开始搜索  CurrentHeaderType;
	//搜索结束类型  CurrentEndType;

	// 创建一个包以保存UDataTable   /Game/NoPack/hbb/
	FString PackageName = "/Game/NoPack/hbb/";
	PackageName += "AssetSearchResult";
	
	auto Package = CreatePackage(*PackageName);
	DataTable = NewObject<UDataTable>(Package,"AssetSearchResult", RF_Public | RF_Standalone);
	DataTable->RowStruct = FAssetSearchToolResult::StaticStruct();

	// 获取资产注册表模块
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	// 获取所有资产的数据
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(CurrentEndType->GetFName(),AssetData);//预先加载一下
	AssetData.Empty();
	AssetRegistryModule.Get().GetAssetsByClass(CurrentHeaderType->GetFName(),AssetData);
	count = 0;
	// 遍历所有资产
	TArray<FAssetData> RealAssetData;
	RealAssetData.Reserve(AssetData.Num());
	for (const FAssetData& Asset : AssetData)
	{
		auto assetPath = Asset.ObjectPath.ToString();
		auto assetPackagePath = Asset.PackagePath.ToString();

		if(!BeginSearchExcludePath.IsEmpty())
		{
			bool bFound = false;
			TArray<FString> allExculdePath;
			BeginSearchExcludePath.ParseIntoArray(allExculdePath,TEXT(";"));
			for(auto ep : allExculdePath)
			{
				if(Asset.ObjectPath.ToString().Contains(ep))
				{
					bFound = true;
					break;
				}
			}
			if(bFound)
			{
				continue;
			}
		}
		if (BeginSearchPath.IsEmpty() || BeginSearchPath.Equals(TEXT("All")) || assetPath.Contains(BeginSearchPath))
		{
			if(!Asset.AssetClass.IsEqual(UPackage::StaticClass()->GetFName()))
			{
				RealAssetData.Add(Asset);
			}
		}
	}
	AssetData.Empty();
	ProgressBar_ReferenceSearch->SetPercent(0.0f);

	RefSearchResults.Empty();
	
	
	{
		Async(EAsyncExecution::ThreadPool, [=,this]()
		{
			assetCount = 0;
			for (auto & Asset : RealAssetData)
			{
				if(!bOnlyResult)
				{
					FAssetSearchToolResult title;
					title.AssetName = Asset.AssetName.ToString();
					title.AssetPath = Asset.ObjectPath.ToString();
					title.FirstMaterialInstanceReference = "-";
					title.ReferenceAssetPath = "-";
					title.LorR_ReferenceLevel = "-";
					DataTable->AddRow(*((FString::FromInt(assetCount) + "--")),title);
					count++;
				}
				assetCount++;
				refAssetPathResult.Empty();
				refAssetPathResult.Reserve(RealAssetData.Num());
				refCount = 0;
				
				{
					SearchAllReference(SearchRefIndex , Asset , CurrentEndType, 0,bSearchAllReferenceLevel,false);
				}
				
				RefSearchProgress = ((float)assetCount) / ((float)RealAssetData.Num());
			}
			
			//FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
			//ContentBrowserModule.Get().SyncBrowserToAssets(TArray<UObject*>({ DataTable }), true);
			Async(EAsyncExecution::TaskGraphMainThread, [=,this]()
			{
				Package->MarkPackageDirty();
				if(DataTable)
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(DataTable);
				ReferenceFrom = NULL;
			});
		});
	}
}

struct RefPaths
{
	FString Path;
	FString LorR;
	int RefType;
};

TArray<FAssetSearchToolResult>  FAssetSearchToolModule::SearchAllReference(int& refType ,const FAssetData& asset ,UClass* endType, int referenceLevel, bool bSearchAllRef, bool bMatSearch)
{
	if(referenceLevel == 0 )
	{
		ReferenceFrom = &asset;
	}

	TArray<FAssetSearchToolResult> refSearchResults;
	static FAssetRegistryModule* AssetRegistryModule;
	if(!AssetRegistryModule)
	{
		AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	}

	TArray<RefPaths> Paths;

	if(refType == 2 )
	{
		TArray<FName>beDependentObjectPath;
		AssetRegistryModule->GetDependencies(asset.PackageName,beDependentObjectPath);

		Paths.Reserve(beDependentObjectPath.Num());

		for(auto& i :beDependentObjectPath)
		{
			RefPaths newPath;
			newPath.Path = i.ToString();
			newPath.LorR = "R:";
			newPath.RefType = refType;
			Paths.Add(newPath);
		}
	}
	else if(refType  == 1 )
	{
		TArray<FName> ReferenceObjectPath ;
		AssetRegistryModule->Get().GetReferencers(asset.PackageName,ReferenceObjectPath);
		
		Paths.Reserve(ReferenceObjectPath.Num());

		for(auto& i :ReferenceObjectPath)
		{
	 		RefPaths newPath;
	 		newPath.Path = i.ToString();
	 		newPath.LorR = "L:";
	 		newPath.RefType = refType;
	 		Paths.Add(newPath);
		}
	}
	 else if(refType  == 0 )
	 {
	 	TArray<FName> ReferenceObjectPath ;
	 	AssetRegistryModule->Get().GetReferencers(asset.PackageName,ReferenceObjectPath);
		TArray<FName>beDependentObjectPath;
		AssetRegistryModule->GetDependencies(asset.PackageName,beDependentObjectPath);

		Paths.Reserve(ReferenceObjectPath.Num() + beDependentObjectPath.Num());

	 	for(auto& i :ReferenceObjectPath)
	 	{
	 		RefPaths newPath;
	 		newPath.Path = i.ToString();
	 		newPath.LorR = "L:";
	 		newPath.RefType = 1;
	 		Paths.Add(newPath);
	 	}
	 	for(auto& i :beDependentObjectPath)
	 	{
	 		RefPaths newPath;
	 		newPath.Path = i.ToString();
	 		newPath.LorR = "R:";
	 		newPath.RefType = 2;
	 		Paths.Add(newPath);
	 	}
	}
	//
	for(auto& i :Paths)
	{
		FAssetSearchToolResult newResult;
		newResult.AssetName = TEXT("-");
		newResult.AssetPath = ReferenceFrom->ObjectPath.ToString();
		newResult.LorR_ReferenceLevel = i.LorR + FString::FromInt(referenceLevel);
		FString FullAssetPathName = FString::Printf(TEXT("%s.%s"), *i.Path, *FPackageName::GetLongPackageAssetName(i.Path));
		newResult.ReferenceAssetPath = FullAssetPathName;
		if(referenceLevel == 0 )
		{
			FirstReferenceObjectPath = newResult.ReferenceAssetPath.GetAssetPathString();
		}
		newResult.FirstMaterialInstanceReference = FirstReferenceObjectPath;
		
		//防止搜到相同的引用进入死循环了
		if(refAssetPathResult.Find(i.Path) == INDEX_NONE
			//&& !FullAssetPathName.Equals(asset.ObjectPath.ToString())
		)
		{
			refAssetPathResult.Add(i.Path);
		}
		else
		{
			continue;
		}

		FAssetData AssetData = AssetRegistryModule->Get().GetAssetByObjectPath(*FullAssetPathName,true);
		FName targetClassName = endType->GetFName();

		if(bOnlyResult)
		{
			newResult.AssetPath = "-";
			newResult.AssetName = FPackageName::GetLongPackageAssetName(i.Path);
			newResult.ReferenceAssetPath = "-";
			newResult.LorR_ReferenceLevel = "-";
			newResult.FirstMaterialInstanceReference = "-";
		}
		
		
		{
			if(AssetData.AssetClass.IsEqual(targetClassName))
			{
				if(!bMatSearch)
				{
					if(!bOnlyResult)
					{
						DataTable->AddRow(*(FString::FromInt(assetCount-1) + "--" + *FString::FromInt(refCount)),newResult);
					}
					else
					{
						DataTable->AddRow(*(FString::FromInt(assetCount-1)),newResult);
					}
					refSearchResults.Add(newResult);
					refCount++;
					count++;
				}
				else
				{
					MaterialReferenceAssetPaths.Add(AssetData);
					MaterialReferenceAssetReferenceLevel.Add(referenceLevel);
				}
				if(bSearchAllRef)
				{
					refSearchResults.Insert(SearchAllReference(i.RefType,AssetData, endType,referenceLevel+1, bSearchAllRef,bMatSearch),0);
				}
			}
			else
			{
				if(!bMatSearch)
				{
					refSearchResults.Insert(SearchAllReference(i.RefType,AssetData, endType,referenceLevel+1, bSearchAllRef,bMatSearch),0);
				}
			}	
		}
	}
	return refSearchResults;
}

void FAssetSearchToolModule::SearchMaterialInstances()
{
	// 创建一个包以保存UDataTable
	FString PackageName = "/Game/NoPack/hbb/";
	PackageName += "AssetSearchResult";
	
	auto Package = CreatePackage(*PackageName);
	DataTable = NewObject<UDataTable>(Package,"AssetSearchResult", RF_Public | RF_Standalone);
	DataTable->RowStruct = FAssetSearchToolMaterialInstanceResult::StaticStruct();
	
	// 获取资产注册表模块
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	// 获取所有资产的数据
	TArray<FAssetData> AssetData;
	bool bSucceed = false;
	if(MaterialInstanceParentMaterial.Len()>0)
	{
		AssetRegistryModule.Get().GetAssetsByClass(UMaterial::StaticClass()->GetFName(),AssetData);
		auto parentMat = AssetData.FindByPredicate([this](FAssetData& asset)
		{
			return asset.ObjectPath.ToString().Contains(MaterialInstanceParentMaterial);
		});
		if(!parentMat)
		{
			AssetRegistryModule.Get().GetAssetsByClass(UMaterialInstanceConstant::StaticClass()->GetFName(),AssetData);
			parentMat = AssetData.FindByPredicate([this](FAssetData& asset)
			{
				return asset.ObjectPath.ToString().Contains(MaterialInstanceParentMaterial);
			});
		}
		if(parentMat && AssetData.Num()>0)
		{
			refAssetPathResult.Empty();
			MaterialReferenceAssetPaths.Empty();
			MaterialReferenceAssetReferenceLevel.Empty();
			
			refAssetPathResult.Reserve(AssetData.Num());
			MaterialReferenceAssetPaths.Reserve(AssetData.Num());
			MaterialReferenceAssetReferenceLevel.Reserve(AssetData.Num());
			
			int refType = 1;
			SearchAllReference(refType,*parentMat,UMaterialInstanceConstant::StaticClass(),0,true,true);
			AssetData.Empty();
			AssetData = MaterialReferenceAssetPaths;
			bSucceed = true;
		}
		if(!bSucceed)
		{
			materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("没有找到这个母材质球")));
			materialInsSearchtipText ->SetColorAndOpacity(FLinearColor(1.0,1.0,0.0,1.0));
			return;
		}
	}
	else
	{
		AssetRegistryModule.Get().GetAssetsByClass(UMaterialInstanceConstant::StaticClass()->GetFName(),AssetData);
	}
	MaterialInstanceResultPath.Empty();
	MaterialInstanceResultPath.Reserve(AssetData.Num());
	count = 0;
	GetMaterialInstanceResultByAssetData(AssetData,MaterialReferenceAssetReferenceLevel,MaterialInstanceBeginSearchPath,MaterialInstanceBeginSearchPathExclude);
	
	materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("搜索到结果:")+FString::FromInt(count)));
	materialInsSearchtipText ->SetColorAndOpacity(FLinearColor(0.1,1.0,0.2,1.0));
	
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	//ContentBrowserModule.Get().SyncBrowserToAssets(TArray<UObject*>({ DataTable }),true,false);
	
	Package->MarkPackageDirty();
	
}
TArray<FName> MaterialInstanceResultByAssetDataCaches;
void FAssetSearchToolModule::GetMaterialInstanceResultByAssetData(TArray<FAssetData>& AssetData,TArray<int>& RefLevel,FText& InBeginSearchPath,FText& InBeginSearchPathExclude)
{
	MaterialInstanceResultByAssetDataCaches.Empty();
	MaterialInstanceResultByAssetDataCaches.Reserve(AssetData.Num());
	auto assetIndex = -1 ;
	auto MaterialEditorInstance = NewObject<UMaterialEditorInstanceConstant>(GetTransientPackage(), NAME_None, RF_Transactional);
	for (const FAssetData& Asset : AssetData)
	{
		assetIndex++;
		
		FAssetSearchToolMaterialInstanceResult reuslt;
		reuslt.ReferenceLevel = -1;
		if(RefLevel.Num()>0)
		{
			reuslt.ReferenceLevel = RefLevel[assetIndex];
			if(reuslt.ReferenceLevel > MaterialInstanceRefCount)
			{
				continue;
			}
		}

		if(MaterialInstanceResultByAssetDataCaches.FindByPredicate([=](FName& objectPath)
		{
			return Asset.ObjectPath.IsEqual(objectPath);
		}))
		{
			continue;
		}
		else
		{
			MaterialInstanceResultByAssetDataCaches.Add(Asset.ObjectPath);
		}
		
		reuslt.InstanceAssetPath = Asset.ObjectPath.ToString();
		reuslt.InstanceAssetName = Asset.AssetName.ToString();
		reuslt.AssetData = Asset;
		if(!InBeginSearchPathExclude.IsEmpty())
		{
			bool bFound = false;
			TArray<FString> allExculdePath;
			InBeginSearchPathExclude.ToString().ParseIntoArray(allExculdePath,TEXT(";"));
			for(auto ep : allExculdePath)
			{
				if(Asset.ObjectPath.ToString().Contains(ep))
				{
					bFound = true;
					break;
				}
			}
			if(bFound)
			{
				continue;
			}
		}
		if (InBeginSearchPath.IsEmpty() || InBeginSearchPath.EqualTo(MakeFText(All)) || Asset.ObjectPath.ToString().Contains(InBeginSearchPath.ToString()))
		{
			UMaterialInstanceConstant* MaterialInstance =  LoadObject<UMaterialInstanceConstant>(nullptr,*(Asset.ObjectPath.ToString()));
			if(MaterialInstance!=nullptr)
			{
				reuslt.ParentAssetPath = MaterialInstance->Parent->GetPathName();
				MaterialEditorInstance->SetSourceInstance(MaterialInstance);
				//auto materialName = MaterialInstance->GetMaterial()->GetName();
				// if( MaterialInstanceParentMaterial.Len()>1 && !materialName.Contains(MaterialInstanceParentMaterial))
				// {
				// 	continue;
				// }
				if(CurrentMaterialInstanceType->Equals(TEXT("Scalar")))
				{
					TArray<UDEditorScalarParameterValue*> scalarParams;
					auto parameterGroups = MaterialEditorInstance->ParameterGroups;
					for(auto&g:parameterGroups)
					{
						bool bFound = false;
						for(auto& p : g.Parameters)
						{
							auto scalar = Cast<UDEditorScalarParameterValue>(p);
							if(scalar && 
							(!bIgnoreUnValidParam || (bIgnoreUnValidParam && MaterialEditorInstance->VisibleExpressions.FindByPredicate([scalar](FMaterialParameterInfo& info)
							{
								return info.Name.IsEqual(scalar->ParameterInfo.Name);//去除没有连接的动态参数节点...
							})))
							)
							{
								if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
								{
									scalarParams.Add(scalar);
									if(bOnlyOneParam)
									{
										bFound = true;
										break;
									}
								}
							}
						}
						if(bFound)
							break;
					}
					for(auto& p :scalarParams)
					{
						if(bOveriddenOnly == false || bOveriddenOnly == p->bOverride)
						{
							bool bFound = false;
							if(MaterialInstanceParamName.Len()<=0 && scalarParams.Num()>0)
							{
								bFound = true;
							}
							reuslt.ParameterName = "(Scalar) " + p->ParameterInfo.Name.ToString();
							reuslt.ParameterValue = FString::SanitizeFloat(p->ParameterValue,8);
							reuslt.bOverride = p->bOverride;
							//if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
							{
								if(MaterialInstanceParamValue.Len()>0)
								{
									auto floatValue = FCString::Atof(*MaterialInstanceParamValue);
									if( ((CurrentMaterialInstanceScalarOP)->Equals(TEXT("等于")) && FMath::IsNearlyEqual(p->ParameterValue,floatValue,0.00001f))
										|| (CurrentMaterialInstanceScalarOP->Equals(TEXT("大于")) && p->ParameterValue > floatValue)
										||( CurrentMaterialInstanceScalarOP->Equals(TEXT("小于")) && p->ParameterValue < floatValue)
									)
									{
										bFound = true;
									}
								}
								else
								{
									bFound = true;
								}
							}
							if(bFound)
							{	
								DataTable->AddRow(*FString::FromInt(count),reuslt);
								MaterialInstanceResultPath.Add(reuslt);
								//MaterialInstanceResultData.AddUnique(Asset);
								count++;
							}
						}
					}
				}
				else if(CurrentMaterialInstanceType->Equals(TEXT("Vector")))
				{
					TArray<UDEditorVectorParameterValue*> Params;
					auto parameterGroups = MaterialEditorInstance->ParameterGroups;
					for(auto&g:parameterGroups)
					{
						bool bFound = false;
						for(auto& p : g.Parameters)
						{
							auto edParam = Cast<UDEditorVectorParameterValue>(p);
							if(edParam && 
							(!bIgnoreUnValidParam || (bIgnoreUnValidParam && MaterialEditorInstance->VisibleExpressions.FindByPredicate([edParam](FMaterialParameterInfo& info)
							{
								return info.Name.IsEqual(edParam->ParameterInfo.Name);//去除没有连接的动态参数节点...
							})))
							)
							{
								if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
								{
									Params.Add(edParam);
									if(bOnlyOneParam)
									{
										bFound = true;
										break;
									}
								}
							}
						}
						if(bFound)
							break;
					}
					for(auto& p : Params)
					{
						if(bOveriddenOnly == false || bOveriddenOnly == p->bOverride)
						{
							bool bFound = false;
							if(MaterialInstanceParamName.Len()<=0 && Params.Num()>0)
							{
								bFound = true;
							}
							reuslt.ParameterName = "(Vector) " + p->ParameterInfo.Name.ToString();
							reuslt.ParameterValue = p->ParameterValue.ToString();
							reuslt.bOverride = p->bOverride;
							//if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
							{
								if(MaterialInstanceParamValue.Len()>4)
								{
									FLinearColor color ;
									color.InitFromString(MaterialInstanceParamValue);
									if(color.Equals(p->ParameterValue,0.00001f))
									{
										bFound = true;
									}
								}
								else
								{
									bFound = true;
								}
							}
							if(bFound)
							{
								DataTable->AddRow(*FString::FromInt(count),reuslt);
								MaterialInstanceResultPath.Add(reuslt);
								//MaterialInstanceResultData.AddUnique(Asset);
								count++;
							}
						}
					}
				}
				else if(CurrentMaterialInstanceType->Equals(TEXT("StaticSwitch")))
				{
					TArray<UDEditorStaticSwitchParameterValue*> Params;
					auto parameterGroups = MaterialEditorInstance->ParameterGroups;
					for(auto&g:parameterGroups)
					{
						bool bFound = false;
						for(auto& p : g.Parameters)
						{
							auto edParam = Cast<UDEditorStaticSwitchParameterValue>(p);
							if(edParam && 
							(!bIgnoreUnValidParam || (bIgnoreUnValidParam && MaterialEditorInstance->VisibleExpressions.FindByPredicate([edParam](FMaterialParameterInfo& info)
							{
								return info.Name.IsEqual(edParam->ParameterInfo.Name);//去除没有连接的动态参数节点...
							})))
							)
							{
								if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
								{
									Params.Add(edParam);
									if(bOnlyOneParam)
									{
										bFound = true;
										break;
									}
								}
							}
						}
						if(bFound)
							break;
					}
					for(auto& p : Params)
					{
						if(bOveriddenOnly == false || bOveriddenOnly == p->bOverride)
						{
							bool bFound = false;
							if(MaterialInstanceParamName.Len()<=0 && Params.Num()>0)
							{
								bFound = true;
							}
							reuslt.ParameterName = "(StaticSwitch) " + p->ParameterInfo.Name.ToString();
							reuslt.ParameterValue = p->ParameterValue > 0 ? "1":"0";
							reuslt.bOverride = p->bOverride;
							//if( p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
							{
								if(MaterialInstanceParamValue.Len()>0)
								{
									int switchVal;
									switchVal = FCString::Atoi(*MaterialInstanceParamValue);
									if(switchVal == (p->ParameterValue > 0?1:0))
									{
										bFound = true;
									}
								}
								else
								{
									bFound = true;
								}
							}
							if(bFound)
							{
								DataTable->AddRow(*FString::FromInt(count),reuslt);
								MaterialInstanceResultPath.Add(reuslt);
								//MaterialInstanceResultData.AddUnique(Asset);
								count++;
							}
						}
					}
				}
				else if(CurrentMaterialInstanceType->Contains(TEXT("Texture")))
				{
					TArray<UDEditorTextureParameterValue*> Params;
					auto parameterGroups = MaterialEditorInstance->ParameterGroups;
					for(auto&g:parameterGroups)
					{
						bool bFound = false;
						for(auto& p : g.Parameters)
						{
							auto edParam = Cast<UDEditorTextureParameterValue>(p);
							if(edParam && 
							(!bIgnoreUnValidParam || (bIgnoreUnValidParam && MaterialEditorInstance->VisibleExpressions.FindByPredicate([edParam](FMaterialParameterInfo& info)
							{
								return info.Name.IsEqual(edParam->ParameterInfo.Name);//去除没有连接的动态参数节点...
							})))
							)
							{
								if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
								{
									Params.Add(edParam);
									if(bOnlyOneParam)
									{
										bFound = true;
										break;
									}
								}
							}
						}
						if(bFound)
							break;
					}
					int typeIndex = 0;//0:Texture2D,1:TextureCube,2:Texture2DArray
					if(CurrentMaterialInstanceType->Equals(TEXT("TextureCube")))
						typeIndex = 1;
					else if(CurrentMaterialInstanceType->Equals(TEXT("Texture2DArray")))
						typeIndex = 2;
					for(auto& p : Params)
					{
						if(bOveriddenOnly == false || bOveriddenOnly == p->bOverride)
						{
							bool bFound = false;
							if(MaterialInstanceParamName.Len()<=0 && Params.Num()>0)
							{
								bFound = true;
							}
							if(typeIndex == 0)
								reuslt.ParameterName = "(Texture2D) " ;
							else if(typeIndex == 1)
								reuslt.ParameterName = "(TextureCube) " ;
							else if(typeIndex == 2)
								reuslt.ParameterName = "(Texture2DArray) " ;
							reuslt.ParameterName += p->ParameterInfo.Name.ToString();
							reuslt.bOverride = p->bOverride>0 ? true:false;
							if(p->ParameterValue)
								reuslt.ParameterValue = p->ParameterValue->GetPathName();
							else
							{
								reuslt.ParameterValue = TEXT("NULL(警告,存在空纹理引用,无法检测纹理类型,建议先解决该问题...)");
							}
							//if(p->ParameterInfo.Name.ToString().Contains(*MaterialInstanceParamName,ESearchCase::CaseSensitive))
							{
								if(p->ParameterValue)
								{
									if(MaterialInstanceParamValue.Len()>0)
									{
										if(reuslt.ParameterValue.Contains(*MaterialInstanceParamValue))
										{
											bFound = true;
										}
									}
									else
									{
										bFound = true;
									}
								}
							}
							if(bFound)
							{
								if((p->ParameterValue && (
										(typeIndex == 0 && p->ParameterValue->GetClass()->GetFName().IsEqual(UTexture2D::StaticClass()->GetFName()))
									||	(typeIndex == 1 && p->ParameterValue->GetClass()->GetFName().IsEqual(UTextureCube::StaticClass()->GetFName()))
									||	(typeIndex == 2 && p->ParameterValue->GetClass()->GetFName().IsEqual(UTexture2DArray::StaticClass()->GetFName()))
									)) || p->ParameterValue == NULL
									)
								{
									DataTable->AddRow(*FString::FromInt(count),reuslt);
									MaterialInstanceResultPath.Add(reuslt);
									//MaterialInstanceResultData.AddUnique(Asset);
									count++;
								}
							}
						}
					}
				}
				else if(CurrentMaterialInstanceType->Equals(TEXT("BlendMode")))
				{
					auto materialPropertyOverrides = MaterialEditorInstance->BasePropertyOverrides;
					bool originOverride = materialPropertyOverrides.bOverride_BlendMode>0 ? true:false;
					auto blendModel = materialPropertyOverrides.BlendMode;
					UEnum* BlendModeEnum = StaticEnum<EBlendMode>();
					if(bOveriddenOnly == false || bOveriddenOnly == originOverride)
					{
						bool bFound = false;
						reuslt.ParameterName = "(BlendMode) ";
						reuslt.ParameterValue = BlendModeEnum->GetNameStringByIndex(static_cast<int32>(blendModel));
						reuslt.bOverride = originOverride;
						if(MaterialInstanceParamValue.Len()>0)
						{
							if(MaterialInstanceParamValue.Contains(reuslt.ParameterValue,ESearchCase::CaseSensitive))
							{
								bFound = true;
							}
						}
						else
						{
							bFound = true;
						}
						if(bFound)
						{
							DataTable->AddRow(*FString::FromInt(count),reuslt);
							MaterialInstanceResultPath.Add(reuslt);
							//MaterialInstanceResultData.AddUnique(Asset);
							count++;
						}
					}
				}
				else if(CurrentMaterialInstanceType->Equals(TEXT("TwoSide")))
				{
					auto materialPropertyOverrides = MaterialEditorInstance->BasePropertyOverrides;
					bool originOverride = materialPropertyOverrides.bOverride_TwoSided>0 ? true:false;
					auto TwoSide = materialPropertyOverrides.TwoSided;
					if(bOveriddenOnly == false || bOveriddenOnly == originOverride)
					{
						bool bFound = false;
						reuslt.ParameterName = "(TwoSide) ";
						reuslt.ParameterValue = TwoSide>0? "true":"false";
						reuslt.bOverride = originOverride;
						if(MaterialInstanceParamValue.Len()>0)
						{
							int i = FCString::Atoi(*MaterialInstanceParamValue);
							if((i>0?true:false) == (TwoSide>0?true:false))
							{
								bFound = true;
							}
						}
						else
						{
							bFound = true;
						}
						if(bFound)
						{
							DataTable->AddRow(*FString::FromInt(count),reuslt);
							MaterialInstanceResultPath.Add(reuslt);
							//MaterialInstanceResultData.AddUnique(Asset);
							count++;
						}
					}
				}
			}
		}
	}
}

void FAssetSearchToolModule::UpdateMaterialInstanceSearchType(TSharedRef<SHorizontalBox> box)
{
	box->ClearChildren();
	MaterialInstanceResultPath.Empty();
	//MaterialInstanceResultData.Empty();
	materialInsSearchtipText->SetText(MakeFTextFromString(""));
	MaterialInstanceParamName="";
	MaterialInstanceParamValue="";
	MaterialInstanceParamName.TrimStartAndEndInline();
	MaterialInstanceParamValue.TrimStartAndEndInline();
	{
		if(!CurrentMaterialInstanceType->Equals(TEXT("BlendMode")) 
		&& !CurrentMaterialInstanceType->Equals(TEXT("TwoSide"))
		)
		{
			auto idText = SNew(STextBlock)
				.Text(MakeFTextFromString(TEXT("参数名")));	
			auto newParamName = SNew(SEditableTextBox)
				   .Text(MakeFTextFromString(TEXT("")))
				   .ToolTipText(MakeFTextFromString(TEXT("参数名")))
				   .OnTextChanged_Lambda([this](const FText& InText)
				   {
						MaterialInstanceParamName = InText.ToString();
						   MaterialInstanceParamName.TrimStartAndEndInline();
				   });
			{
				auto SubHbox = SNew(SHorizontalBox);
				SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(idText);
				SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(newParamName);
				box->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(SubHbox);
			}
		}
		
		auto vaText = SNew(STextBlock)
			.ToolTipText(MakeFTextFromString(TEXT("参数值注意:\n【颜色】的参数形式为:R=1,G=2,B=3\nStaticSwitch为0或者1\nTexture为资产的路径(允许不完整,只接受关键字)")))
			.Text(MakeFTextFromString(TEXT("参数值")));	
		auto newParamValue = SNew(SEditableTextBox)
		   .Text(MakeFTextFromString(TEXT("")))
		   .ToolTipText(MakeFTextFromString(TEXT("参数值注意:\n【颜色】的参数形式为:R=1,G=2,B=3\nStaticSwitch为0或者1\nTexture为资产的路径(允许不完整,只接受关键字)")))
		   .OnTextChanged_Lambda([this](const FText& InText)
		   {
				MaterialInstanceParamValue = InText.ToString();
			   	MaterialInstanceParamValue.TrimStartAndEndInline();
		   });
		
		{
			auto SubHbox = SNew(SHorizontalBox);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(vaText);
			SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(newParamValue);
			box->AddSlot().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(SubHbox);
		}

		if(CurrentMaterialInstanceType->Equals(TEXT("Scalar")))
		{
			auto minText = SNew(STextBlock)
				.Text(MakeFTextFromString(TEXT("判断方式")));	
			auto minParamValue = SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&MaterialInstanceScalarOP)
				.InitiallySelectedItem(MaterialInstanceScalarOP[0])
				.ToolTipText(MakeFTextFromString(TEXT("OP = 等于,大于,小于\n计算方式 [ 材质参数 OP 输入的参数值 ] ")))
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
				{
					return SNew(STextBlock).Text(MakeFTextFromString(*Item));
				})
				.OnSelectionChanged_Lambda([=,this](TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
				{
					CurrentMaterialInstanceScalarOP = Item;
				})
				[
					SNew(STextBlock).Text_Lambda([this]()
					{
						return FText::FromString(*CurrentMaterialInstanceScalarOP);
					})
				];
			{
				auto SubHbox = SNew(SHorizontalBox);
				SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(0,2,0,0).AttachWidget(minText);
				SubHbox->AddSlot().AutoWidth().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(minParamValue);
				box->AddSlot().VAlign(VAlign_Center).Padding(5,2,0,0).AttachWidget(SubHbox);
			}
		}
	}
}

void FAssetSearchToolModule::SetMaterialInstancesParameterValue(TArray<FAssetSearchToolMaterialInstanceResult>& AssetData, bool newMaterialInstanceOverrideInherit)
{
	int successfulCount = 0;
	if(NewMaterialInstanceParamValue.Len()>0)
	{
		auto MaterialEditorInstance = NewObject<UMaterialEditorInstanceConstant>(GetTransientPackage(), NAME_None, RF_Transactional);
		for(auto&i:AssetData)
		{
			UMaterialInstanceConstant* MaterialInstance = LoadObject<UMaterialInstanceConstant>(nullptr,*(i.InstanceAssetPath));
			if(MaterialInstance)
			{
				MaterialEditorInstance->SetSourceInstance(MaterialInstance);
				if(NewCurrentMaterialInstanceType->Equals(TEXT("Scalar")))
				{
					bool bFound = false; 
					for(auto& pg: MaterialEditorInstance->ParameterGroups)
					{
						for(auto& ep:pg.Parameters)
						{
							if(ep->ParameterInfo.Name.IsEqual(*NewMaterialInstanceParamName))
							{
								auto val = Cast<UDEditorScalarParameterValue>(ep);
								if(val)
								{
									val->ParameterValue = FCString::Atof(*NewMaterialInstanceParamValue);
									if(newMaterialInstanceOverrideInherit && i.bIsMaterialInsSearchResult)
										ep->bOverride = i.bOverride;
									else
										ep->bOverride = true;
									bFound = true;
									MaterialEditorInstance->PostEditChange();
									MaterialEditorInstance->MarkPackageDirty();
									successfulCount++;
								}
								break;
							}
						}
						if(bFound)
							break;
					}
				}
				else if(NewCurrentMaterialInstanceType->Equals(TEXT("Vector")))
				{
					bool bFound = false; 
					for(auto& pg: MaterialEditorInstance->ParameterGroups)
					{
						for(auto& ep:pg.Parameters)
						{
							if(ep->ParameterInfo.Name.IsEqual(*NewMaterialInstanceParamName))
							{
								auto val = Cast<UDEditorVectorParameterValue>(ep);
								if(val)
								{
									FLinearColor color ; color.InitFromString(NewMaterialInstanceParamValue);
									val->ParameterValue = color;
									if(newMaterialInstanceOverrideInherit && i.bIsMaterialInsSearchResult)
										ep->bOverride = i.bOverride;
									else
										ep->bOverride = true;
									bFound = true;
									MaterialEditorInstance->PostEditChange();
									MaterialEditorInstance->MarkPackageDirty();
									successfulCount++;
								}
								break;
							}
						}
						if(bFound)
							break;
					}
				}
				else if(NewCurrentMaterialInstanceType->Equals(TEXT("StaticSwitch")))
				{
					TArray<FMaterialParameterInfo> ParameterInfos;
					TArray<FGuid> OutParameterIds;
					MaterialInstance->GetAllStaticSwitchParameterInfo(ParameterInfos,OutParameterIds);
					if(auto info = ParameterInfos.FindByPredicate([this](FMaterialParameterInfo& value)
					{
						return value.Name.ToString().Equals( *NewMaterialInstanceParamName , ESearchCase::CaseSensitive);
					}))
					{
						int bEnable = FCString::Atoi(*NewMaterialInstanceParamValue);
						FStaticParameterSet NewStaticParameterSet;
						FStaticSwitchParameter SwitchParameter;
						SwitchParameter.ParameterInfo=*info;
						SwitchParameter.Value = bEnable>0? true:false;
						if(newMaterialInstanceOverrideInherit && i.bIsMaterialInsSearchResult)
							SwitchParameter.bOverride = i.bOverride;
						else
							SwitchParameter.bOverride = true;
						NewStaticParameterSet.StaticSwitchParameters.Add(SwitchParameter);
						MaterialInstance->UpdateStaticPermutation(NewStaticParameterSet);
						MaterialInstance->InitStaticPermutation();
						MaterialInstance->PostEditChange();
						MaterialInstance->MarkPackageDirty();
						successfulCount++;
					}
				}
			}
		}
	}

	if(successfulCount >= AssetData.Num())
	{
		materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("批量修改成功,本工具不会强行保存资产,请确认无误后,自行手动保存资产.")));
		materialInsSearchtipText->SetColorAndOpacity(FLinearColor(0.0,1.0,0.0,1));
	}
	else if(successfulCount >0 && successfulCount < AssetData.Num())
	{
		materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("批量修改成功,【但是有部分材质修改失败】,本工具不会强行保存资产,请确认无误后,自行手动保存资产.")));
		materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1.0,1.0,0.0,1));
	}
	else
	{
		materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("批量修改失败,一个都没改成...")));
		materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1.0,0.0,0.0,1));
	}
}

void FAssetSearchToolModule::SetMaterialInstancesParameterValueFromSelection()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> selections;
	ContentBrowserModule.Get().GetSelectedAssets(selections);
	TArray<FAssetSearchToolMaterialInstanceResult> selectionMaterialInstances;
	auto MaterialEditorInstance = NewObject<UMaterialEditorInstanceConstant>(GetTransientPackage(), NAME_None, RF_Transactional);
	for(auto i: selections)
	{
		if(i.AssetClass.IsEqual(UMaterialInstanceConstant::StaticClass()->GetFName()))
		{
			FAssetSearchToolMaterialInstanceResult newResult;
			newResult.InstanceAssetName = i.AssetName.ToString();
			newResult.InstanceAssetPath = i.ObjectPath.ToString();
			newResult.AssetData = i;
			UMaterialInstanceConstant* MaterialInstance = LoadObject<UMaterialInstanceConstant>(nullptr,*(i.ObjectPath.ToString()));
			if(MaterialInstance)
			{
				MaterialEditorInstance->SetSourceInstance(MaterialInstance);
				bool bFound =false;
				for(auto& pg: MaterialEditorInstance->ParameterGroups)
				{
					for(auto& ep:pg.Parameters)
					{
						if(ep->ParameterInfo.Name.IsEqual(*NewMaterialInstanceParamName))
						{
							newResult.ParameterName = NewMaterialInstanceParamName;
							newResult.bOverride = ep->bOverride;
							bFound = true;
							break;
						}
					}
					if(bFound)
						break;
				}
				selectionMaterialInstances.Add(newResult);
			}
		}
	}
	SetMaterialInstancesParameterValue(selectionMaterialInstances,bNewMaterialInstanceOverrideInherit);
}

void FAssetSearchToolModule::SetMaterialInstanceParent(TArray<FAssetSearchToolMaterialInstanceResult>& InstanceAssetData) 
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	if(!NewMaterialInstaceParent.ObjectPath.ToString().Contains(SetNewParentForMaterialInstance))
	{
		TArray<FAssetData> ParentAssetData;
		AssetRegistryModule.Get().GetAssetsByClass(UMaterial::StaticClass()->GetFName(),ParentAssetData);
		auto parentMat = ParentAssetData.FindByPredicate([this](FAssetData& asset)
		{
			return asset.ObjectPath.ToString().Contains(SetNewParentForMaterialInstance);
		});
		if(!parentMat)
		{
			AssetRegistryModule.Get().GetAssetsByClass(UMaterialInstanceConstant::StaticClass()->GetFName(),ParentAssetData);
			parentMat = ParentAssetData.FindByPredicate([this](FAssetData& asset)
			{
				return asset.ObjectPath.ToString().Contains(SetNewParentForMaterialInstance);
			});
		}
		if(parentMat)
		{
			NewMaterialInstaceParent = *parentMat;
		}
	}
	
	UMaterialInterface* parentInterface;
	auto parent0 = LoadObject<UMaterial>(nullptr, *(NewMaterialInstaceParent.ObjectPath.ToString()) );
	parentInterface = parent0;
	if(!parent0)
	{
		auto parent1 = LoadObject<UMaterialInstanceConstant>(nullptr, *(NewMaterialInstaceParent.ObjectPath.ToString()) );
		parentInterface = parent1;
	}
	if(parentInterface)
	{
		auto MaterialEditorInstance = NewObject<UMaterialEditorInstanceConstant>(GetTransientPackage(), NAME_None, RF_Transactional);
		FMaterialUpdateContext Context;
		
		for(auto p : InstanceAssetData)
		{
			TArray<FScalarParameterValue> scalars;
			TArray<FVectorParameterValue> vectors;
			TArray<FTextureParameterValue> texs;
			FStaticParameterSet staticParameters;
			auto instance = LoadObject<UMaterialInstanceConstant>(nullptr,*p.InstanceAssetPath);
			if(instance)
			{
				UMaterialInterface* targetParent = parentInterface;
				MaterialEditorInstance->SetSourceInstance(instance);
				//切换Parent之前确保参数同步!因为有些材质实例是继承了上一个材质实例的参数的,参数前面的重写开关是关闭状态,
				//如果直接切换Parent,可能会导致继承了错误的参数。
				for(auto& pg : MaterialEditorInstance->ParameterGroups)
				{
					for(auto& i : pg.Parameters)
					{
						auto scalar = Cast<UDEditorScalarParameterValue>(i);
						auto vector = Cast<UDEditorVectorParameterValue>(i);
						auto tex = Cast<UDEditorTextureParameterValue>(i);
						auto swi = Cast<UDEditorStaticSwitchParameterValue>(i);
						auto mask_swi = Cast<UDEditorStaticComponentMaskParameterValue>(i);
						if(scalar)
						{
							float value;
							if(instance->GetBaseMaterial()->GetScalarParameterValue(scalar->ParameterInfo,value))
							if(scalar->ParameterValue != value)
							{
								FScalarParameterValue v;
								v.ParameterInfo = scalar->ParameterInfo;
								v.ParameterValue = scalar->ParameterValue;
								scalars.Add(v);
							}
						}
						else if(vector)
						{
							FLinearColor value ;
							if(instance->GetBaseMaterial()->GetVectorParameterValue(vector->ParameterInfo,value))
							if(!value.Equals(vector->ParameterValue))
							{
								FVectorParameterValue v;
								v.ParameterInfo = vector->ParameterInfo;
								v.ParameterValue = vector->ParameterValue;
								vectors.Add(v);
							}
						}
						else if(tex)
						{
							UTexture* value = NULL;
							if(instance->GetBaseMaterial()->GetTextureParameterValue(tex->ParameterInfo,value))
							if(value && tex->ParameterValue)
							{
								if(!value->GetPathName().Equals(tex->ParameterValue->GetPathName()))
								{
									FTextureParameterValue v;
									v.ParameterInfo = tex->ParameterInfo;
									v.ParameterValue = tex->ParameterValue;
									texs.Add(v);
								}
							}
						}
						else if(swi)
						{
							bool value ;FGuid guid;
							if(instance->GetBaseMaterial()->GetStaticSwitchParameterValue(swi->ParameterInfo,value, guid))
							if(value != swi->ParameterValue)
							{
								FStaticSwitchParameter v;
								v.ParameterInfo = swi->ParameterInfo;
								v.bOverride = true;
								v.Value = swi->ParameterValue > 0 ? true :false;
								staticParameters.StaticSwitchParameters.Add(v);
							}
						}
						else if(mask_swi)
						{
							bool R,G,B,A;FGuid guid;
							if(instance->GetBaseMaterial()->GetStaticComponentMaskParameterValue(mask_swi->ParameterInfo,R,G,B,A,guid))
							if(R != mask_swi->ParameterValue.R || G != mask_swi->ParameterValue.G || B != mask_swi->ParameterValue.B || A != mask_swi->ParameterValue.A )
							{
								FStaticComponentMaskParameter v;
								v.ParameterInfo = mask_swi->ParameterInfo;
								v.bOverride = true;
								v.R = mask_swi->ParameterValue.R > 0 ? true : false;
								v.G = mask_swi->ParameterValue.G > 0 ? true : false;
								v.B = mask_swi->ParameterValue.B > 0 ? true : false;
								v.A = mask_swi->ParameterValue.A > 0 ? true : false;
								staticParameters.EditorOnly.StaticComponentMaskParameters.Add(v);
							}
						}
					}
				}

				if(targetParent)
				{
					instance->PreEditChange(nullptr);
					instance->SetParentEditorOnly(targetParent);
					instance->PostEditChange();
					Context.AddMaterialInstance(instance);
					instance->MarkPackageDirty();
					//延迟执行修改
					FGraphEventRef SetParentTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
					[this , instance,scalars,vectors,texs,staticParameters]()
					{
						for(auto &i:scalars)
						{
							instance->SetScalarParameterValueEditorOnly(i.ParameterInfo,i.ParameterValue);
						}
						for(auto &i:vectors)
						{
							instance->SetVectorParameterValueEditorOnly(i.ParameterInfo,i.ParameterValue);
						}
						for(auto &i:texs)
						{
							instance->SetTextureParameterValueEditorOnly(i.ParameterInfo,i.ParameterValue);
						}
						instance->UpdateStaticPermutation(staticParameters);
						instance->InitStaticPermutation();
						instance->PostEditChange();
						instance->MarkPackageDirty();
            					
					}, TStatId(), nullptr, ENamedThreads::GameThread);
				}
			}
		}
		
		materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("批量替换Parent成功.")));
		materialInsSearchtipText->SetColorAndOpacity(FLinearColor(0.0,1.0,0.0,1));
	}
	else
	{
		materialInsSearchtipText->SetText(MakeFTextFromString(TEXT("批量替换Parent失败,未找到Parent.")));
		materialInsSearchtipText->SetColorAndOpacity(FLinearColor(1.0,0.5,0.0,1));
	}
}

CustomResultTable::CustomResultTable(FAssetSearchToolModule* sParent ,FString parentMaterialNameInput ,TArray<FAssetSearchToolMaterialInstanceResult>& input, bool isMaterialInsSearchType)
{
	bIsMaterialInsSearchType = isMaterialInsSearchType;
	Parent = sParent;
	parentMaterialName = parentMaterialNameInput;
	AssetSearchToolResultWindow = SNew(SWindow)
		.Title(FText::FromString(FString(TEXT("资产检索工具 -")) + parentMaterialNameInput + TEXT("- 搜索结果显示") ))
		.ClientSize(FVector2D(400,200))
		.SupportsMaximize(true)
		.SupportsMinimize(true)
		.HasCloseButton(true)
		.IsTopmostWindow(true)
		;
	AssetSearchToolResultWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda(
	[this](const TSharedRef<SWindow>& win)
	{
		this->bIsWindowClosed = true;
		if(localText.Equals(Parent->currentMaterialInstanceResultDataFromWindowText->GetText().ToString()))
		{
			FAssetSearchToolModule::MaterialInstanceResultData.Empty();
			Parent->currentMaterialInstanceResultDataFromWindowText->SetText(MakeFTextFromString(FString(TEXT("当前锁定:")) + TEXT("未知")));
			Parent->currentMaterialInstanceResultDataFromWindowText->SetColorAndOpacity(FLinearColor(1,0,0,1));
			Parent->CurrentMaterialInsSearchLockPanel = "";
		}
	}));

	MainVBox = SNew(SVerticalBox);
	SAssignNew(MainScrollBox,SScrollBox)
	+SScrollBox::Slot()
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.MaxWidth(120)
			[
				SNew(SButton)
				.Text(MakeFTextFromString(TEXT("把数据输出到CSV")))
				.OnClicked_Lambda(
				[this]()
				{
					SaveToCSV();
					return FReply::Handled();
				})
			]
			+SHorizontalBox::Slot()
			.Padding(10,0,0,0)
			.MaxWidth(100)
			[
				SNew(SButton)
				.Text(MakeFTextFromString(TEXT("锁定搜索结果")))
				.Visibility(bIsMaterialInsSearchType ? EVisibility::Visible :  EVisibility::Collapsed)
				.OnClicked_Lambda(
				[this]()
				{
					GetAssetData();
					return FReply::Handled();
				})
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0,5,0,0)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(MakeFTextFromString(TEXT("搜索")))
			]
			+SHorizontalBox::Slot().Padding(4,0,0,0).VAlign(VAlign_Center)
			[
				SAssignNew(SearchText,SEditableText)
				.Text(MakeFTextFromString(TEXT("")))
				.OnTextCommitted_Lambda(
				[this](const FText& InText, ETextCommit::Type CommitType)
				{
					CurrentSearchText = InText;
					UpdateContent(CurrentSearchText.ToString());
					if(localText.Equals(Parent->currentMaterialInstanceResultDataFromWindowText->GetText().ToString()))
					{
						GetAssetData();
					}				
				})
			]
		]
	]
	;
	MainScrollBox->AddSlot().AttachWidget(MainVBox.ToSharedRef());

	
	AssetSearchToolResultWindow->SetContent(MainScrollBox.ToSharedRef());
	// 获取Slate应用程序实例
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	SlateApplication.AddWindow(AssetSearchToolResultWindow.ToSharedRef(),true);
	
	//加载
	UpdateResult(input);

	//自动获取当前窗口的数据?
	//GetAssetData();
}

CustomResultTable::~CustomResultTable()
{
	SearchText.Reset();
	MainScrollBox.Reset();
	MainVBox.Reset();
	ResultInput.Empty();
	ResultOutput.Empty();
	//关闭窗户
	FSlateApplication::Get().DestroyWindowImmediately(AssetSearchToolResultWindow.ToSharedRef());
	AssetSearchToolResultWindow.Reset();
	
}

void CustomResultTable::UpdateContent(FString newSearchText)
{
	ResultOutput.Empty();
	ResultOutput.Reserve(ResultInput.Num());
	//
	newSearchText.TrimStartAndEndInline();
	//
	TArray<FString>searchTextFilterArray;
	newSearchText.ParseIntoArray(searchTextFilterArray,TEXT(","));
	
	FString refFilter;
	FString assetNameFilter;
	FString assetPathFilter;
	FString paramNameFilter;
	FString paramValueFilter;
	FString overrideFilter;
	FString parentFilter;

	for(auto& f : searchTextFilterArray)
	{
		FString header;
		FString searchText;
		f.Split("=",&header,&searchText);
		header.TrimStartAndEndInline();
		searchText.TrimStartAndEndInline();
		if(header.Equals(TEXT("引用")))
		{
			refFilter = searchText;
		}
		else if(header.Equals(TEXT("资产名")))
		{
			assetNameFilter = searchText;
		}
		else if(header.Equals(TEXT("资产路径")))
		{
			assetPathFilter = searchText;
		}
		else if(header.Equals(TEXT("参数名")))
		{
			paramNameFilter = searchText;
		}
		else if(header.Equals(TEXT("参数值")))
		{
			paramValueFilter = searchText;
		}
		else if(header.Equals(TEXT("重写")))
		{
			overrideFilter = searchText;
		}
		else if(header.Equals(TEXT("Parent")))
		{
			parentFilter = searchText;
		}
	}
	//
	MainVBox->ClearChildren();
	//
	{
		auto IndexesVBox = SNew(SVerticalBox);
		auto ButtonsVBox = SNew(SVerticalBox);
		auto AssetNamesVBox = SNew(SVerticalBox);
		auto AssetPathsVBox = SNew(SVerticalBox);
		auto RefLevelsVBox = SNew(SVerticalBox);
		auto ParamNamesVBox = SNew(SVerticalBox);
		auto ParamValuesVBox = SNew(SVerticalBox);
		auto OverridedsVBox = SNew(SVerticalBox);
		auto ParentPathsVBox = SNew(SVerticalBox);
		//Title
		{
			//Index
			TSharedPtr<SEditableText> index = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(""));
			IndexesVBox->AddSlot().AutoHeight().AttachWidget(index.ToSharedRef());
			//AssetName
			TSharedPtr<SEditableText> assetName = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("资产名")));
			AssetNamesVBox->AddSlot().AutoHeight().AttachWidget(assetName.ToSharedRef());
			//AssetPath
			TSharedPtr<SEditableText> assetPath = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("资产路径")));
			AssetPathsVBox->AddSlot().AutoHeight().AttachWidget(assetPath.ToSharedRef());
			//Index
			TSharedPtr<SEditableText> button = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("打开")));
			ButtonsVBox->AddSlot().AutoHeight().AttachWidget(button.ToSharedRef());
			//Ref level
			TSharedPtr<SEditableText> refLevel = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("引用")))
				.ToolTipText(MakeFTextFromString(TEXT("引用等级,从指定的母材质球开始,第一级是0,以此论推")));
			RefLevelsVBox->AddSlot().AutoHeight().AttachWidget(refLevel.ToSharedRef());
			//Param name
			TSharedPtr<SEditableText> paramName = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("参数名")));
			ParamNamesVBox->AddSlot().AutoHeight().AttachWidget(paramName.ToSharedRef());
			//Param name
			TSharedPtr<SEditableText> paramValue = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("参数值")));
			ParamValuesVBox->AddSlot().AutoHeight().AttachWidget(paramValue.ToSharedRef());
			//Override
			TSharedPtr<SEditableText> overrde = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(TEXT("重写")));
			OverridedsVBox->AddSlot().AutoHeight().AttachWidget(overrde.ToSharedRef());
			//Parent path
			TSharedPtr<SEditableText> parentPath = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString("Parent"));
			ParentPathsVBox->AddSlot().AutoHeight().AttachWidget(parentPath.ToSharedRef());
		}
		int searchIndex = -1;
		for(auto & i : ResultInput)
		{
			if(i.InstanceAssetPath.Len() < 2)
			{
				continue;
			}
	
			if(newSearchText.Len() > 0 )
			{
				bool bOK = true;
				// FString refFilter;
				// FString assetNameFilter;
				// FString assetPathFilter;
				// FString paramNameFilter;
				// FString paramValueFilter;
				// FString overrideFilter;
				// FString parentFilter;
				if(refFilter.Len()>0 || assetNameFilter.Len()>0 || assetPathFilter.Len()>0 || paramNameFilter.Len()>0 || paramValueFilter.Len()>0 || overrideFilter.Len()>0 || parentFilter.Len()>0)
				{
					if(i.ReferenceLevel != FCString::Atoi(*refFilter))
					{
						bOK = false;
					}
					if(!i.InstanceAssetName.Equals(assetNameFilter))
					{
						bOK = false;
					}
					if(!i.InstanceAssetPath.Equals(assetPathFilter))
					{
						bOK = false;
					}
					if(!i.ParameterName.Equals(paramNameFilter))
					{
						bOK = false;
					}
					if(!i.ParameterValue.Equals(paramValueFilter))
					{
						bOK = false;
					}
					if(i.bOverride != overrideFilter.Contains("true") )
					{
						bOK = false;
					}
					if(!i.ParentAssetPath.Equals(parentFilter))
					{
						bOK = false;
					}
				}
				else if(!i.InstanceAssetPath.Contains(newSearchText))
				{
					bOK = false;
				}
	
				if(!bOK)
				{
					continue;
				}
			}
			
			searchIndex++;
			ResultOutput.Add(i);
			//Index
			TSharedPtr<SEditableText> indexText = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.5,5,0.5,1.0))
				.Text(MakeFTextFromString(FString::FromInt(searchIndex)));
			IndexesVBox->AddSlot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillHeight(12).AttachWidget(indexText.ToSharedRef());
			//AssetName
			TSharedPtr<SEditableText> assetName = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.InstanceAssetName));
			AssetNamesVBox->AddSlot().AutoHeight().AttachWidget(assetName.ToSharedRef());
			//AssetPath
			TSharedPtr<SEditableText> assetPath = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.InstanceAssetPath));
			AssetPathsVBox->AddSlot().VAlign(VAlign_Center).FillHeight(12).AttachWidget(assetPath.ToSharedRef());
			//Button
			auto assetPathString = i.InstanceAssetPath;
			TSharedPtr<SButton> button = SNew(SButton)
				.Text(MakeFTextFromString(TEXT("..")))
				.DesiredSizeScale(FVector2D(0.9,0.6))
				.OnClicked_Lambda([assetPathString]()
				{
					FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
					auto assetData = AssetRegistryModule.Get().GetAssetByObjectPath(*assetPathString);
					//FMaterialEditorUtilities::OnOpenMaterial(assetData);
					FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
					ContentBrowserModule.Get().SyncBrowserToAssets({assetData});
					return FReply::Handled();
				})
			;
			ButtonsVBox->AddSlot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillHeight(12).AttachWidget(button.ToSharedRef());
			
			//Ref level
			TSharedPtr<SEditableText> refLevel = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(FString::FromInt(i.ReferenceLevel)))
				.ToolTipText(MakeFTextFromString(TEXT("引用等级,从指定的母材质球开始,第一级是0,以此论推")));;
			RefLevelsVBox->AddSlot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillHeight(12).AttachWidget(refLevel.ToSharedRef());
			//Param name
			TSharedPtr<SEditableText> paramName = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.ParameterName));
			ParamNamesVBox->AddSlot().VAlign(VAlign_Center).FillHeight(12).AttachWidget(paramName.ToSharedRef());
			//Param name
			TSharedPtr<SEditableText> paramValue = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.ParameterValue));
			ParamValuesVBox->AddSlot().VAlign(VAlign_Center).FillHeight(12).AttachWidget(paramValue.ToSharedRef());
			//Override
			TSharedPtr<SEditableText> overrde = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.bOverride?"true":"false"));
			OverridedsVBox->AddSlot().VAlign(VAlign_Center).FillHeight(12).AttachWidget(overrde.ToSharedRef());
			//Parent path
			TSharedPtr<SEditableText> parentPath = SNew(SEditableText)
				.IsReadOnly(true)
				.ColorAndOpacity(FLinearColor(0.95,1,1,1.0))
				.Text(MakeFTextFromString(i.ParentAssetPath));
			ParentPathsVBox->AddSlot().VAlign(VAlign_Center).FillHeight(12).AttachWidget(parentPath.ToSharedRef());
		}
		
		auto Split = SNew(SSplitter);
		Split->SetOrientation(EOrientation::Orient_Horizontal);
		Split->AddSlot().SizeRule(SSplitter::ESizeRule::SizeToContent).AttachWidget(IndexesVBox);
		Split->AddSlot().SizeRule(SSplitter::ESizeRule::SizeToContent).AttachWidget(RefLevelsVBox);
		Split->AddSlot().SizeRule(SSplitter::ESizeRule::SizeToContent).AttachWidget(ButtonsVBox);
		Split->AddSlot().AttachWidget(AssetNamesVBox);
		Split->AddSlot().AttachWidget(AssetPathsVBox);
		Split->AddSlot().AttachWidget(ParamNamesVBox);
		Split->AddSlot().AttachWidget(ParamValuesVBox);
		Split->AddSlot().AttachWidget(OverridedsVBox);
		Split->AddSlot().AttachWidget(ParentPathsVBox);
		
		MainVBox->AddSlot().Padding(0,5,0,0).AttachWidget(Split);
	}

	
}

void CustomResultTable::UpdateResult(TArray<FAssetSearchToolMaterialInstanceResult>& input)
{
	SearchText->SetText(FText::FromString(TEXT("")));
	ResultInput = input;
	UpdateContent("");
}

FString CustomResultTable::lastSavePath="";

void CustomResultTable::SaveToCSV()
{
	TArray<FString> OutFilenames;
	FDesktopPlatformModule::Get()->SaveFileDialog(
		nullptr,
		TEXT("文件保存"),
		lastSavePath,
		parentMaterialName  + TEXT("_MatIns_Search_Result.csv"),
		TEXT("Curve Table CSV (*.csv)|*.csv"),
		EFileDialogFlags::None,
		OutFilenames
		);
	if(OutFilenames.Num()>0)
	{
		lastSavePath = OutFilenames[0];
		SaveToCSV(lastSavePath);
	}
}

void CustomResultTable::SaveToCSV(FString path)
{
	// 创建一个数组来存储CSV数据
	TArray<FString> CsvData;

	CsvData.Add(TEXT("Index,引用等级,资产名,资产路径,参数名,参数值,重写,Parent路径")); // 添加表头
	int resultIndex = 0 ;
	// 添加数据行
	for(auto &i: ResultOutput)
	{
		FString data =
			FString::FromInt(resultIndex) + TEXT(",")
			+ FString::FromInt(i.ReferenceLevel) + TEXT(",")
			+ i.InstanceAssetName + TEXT(",")
			+ i.InstanceAssetPath + TEXT(",")
			+ i.ParameterName + TEXT(",")
			+ i.ParameterValue + TEXT(",")
			+ (i.bOverride?TEXT("true"):TEXT("false"))+ TEXT(",")
			+ i.ParentAssetPath;
		CsvData.Add(data);
		resultIndex++;
	}
	// = FString::Join(CsvData, TEXT("\n"));
	FString CsvString;
	for(auto&s : CsvData)
	{
		CsvString += s;
		CsvString += LINE_TERMINATOR;
	}
	FFileHelper::SaveStringToFile(CsvString , *path , FFileHelper::EEncodingOptions::ForceUTF8);
}

void CustomResultTable::GetAssetData()
{
	FAssetSearchToolModule::MaterialInstanceResultData.Empty();
	Parent->currentMaterialInstanceResultDataFromWindowText->SetText(MakeFTextFromString(FString(TEXT("当前锁定:")) + parentMaterialName + " , " + FString::FromInt(ResultOutput.Num())));
	Parent->currentMaterialInstanceResultDataFromWindowText->SetColorAndOpacity(FLinearColor(0,1,0,1));
	localText = Parent->currentMaterialInstanceResultDataFromWindowText->GetText().ToString();
	FAssetSearchToolModule::MaterialInstanceResultData = ResultOutput;
	Parent->CurrentMaterialInsSearchLockPanel = parentMaterialName;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetSearchToolModule, AssetSearchTool)