// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "Engine/DataTable.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "AssetSearchTool.generated.h"

class FToolBarBuilder;
class FMenuBuilder;

// typedef uint32_t AssetTypeFlags;
// enum EAssetTypeBit : uint32_t
// {
// 	None = 0,
// 	Particle = 1<<0,
// 	Material = 1<<1,
// 	MaterialInstance = 1<<2,
// 	MaterialFunction = 1<<3,
// 	StaticMesh = 1<<4,
// 	SkeletalMesh = 1<<5,
// 	Level = 1<<6
// };

USTRUCT(BlueprintType)
struct ASSETSEARCHTOOL_API FAssetSearchToolResult :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch, meta = (DisplayName = "资产名"))
	FString AssetName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch, meta = (DisplayName = "资产路径"))
	FSoftObjectPath AssetPath;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch, meta = (DisplayName = "引用等级"))
	FString LorR_ReferenceLevel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch, meta = (DisplayName = "引用对象路径"))
	FSoftObjectPath ReferenceAssetPath;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch, meta = (DisplayName = "一级材质实例(如果有的话)"))
	FSoftObjectPath FirstMaterialInstanceReference;
};

USTRUCT(BlueprintType)
struct ASSETSEARCHTOOL_API FAssetSearchToolMaterialInstanceResult :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	FString InstanceAssetName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	FString InstanceAssetPath;

	//母材质球的第几级引用
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	int ReferenceLevel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	FString ParameterName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	FString ParameterValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	bool bOverride;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AssetSearch)
	FString ParentAssetPath;

	FAssetData AssetData;

	bool bIsMaterialInsSearchResult = true;
};

class CustomResultTable
{
	friend class FAssetSearchToolModule;
public:
	
	CustomResultTable(class FAssetSearchToolModule* parent ,FString parentMaterialNameInput ,TArray<FAssetSearchToolMaterialInstanceResult>& input , bool bIsMaterialInsSearchType = false);

	~CustomResultTable();
	
	void UpdateContent(FString newSearchText);

	void UpdateResult(TArray<FAssetSearchToolMaterialInstanceResult>& input);

	TSharedPtr<SWindow> AssetSearchToolResultWindow;

	bool bIsWindowClosed =false ;

	void SaveToCSV();
	
	void SaveToCSV(FString path);

	void GetAssetData();

	FText CurrentSearchText;
	
private:
	class FAssetSearchToolModule* Parent;
	
	FString parentMaterialName;

	FString localText;

	static FString lastSavePath;
	
	TSharedPtr<SEditableText> SearchText;
	
	TSharedPtr<SScrollBox> MainScrollBox;

	TSharedPtr<SVerticalBox> MainVBox;
	
	TArray<FAssetSearchToolMaterialInstanceResult> ResultInput;

	TArray<FAssetSearchToolMaterialInstanceResult> ResultOutput;

	bool bIsMaterialInsSearchType ;
};


class ASSETSEARCHTOOL_API FAssetSearchToolModule : public IModuleInterface
{
	friend class CustomResultTable;
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void ShowToolWindow();

	/**
	 * @brief  This function multithreading is not supported.
	 * @param  refType :0:all, 1:left, 2:right
	 * @param  asset :FAssetData asset object
	 * @param  endType :Search result class type
	 * @param  bSearchAllReferenceLevel :Is search all? default false.
	 * @param  referenceLevel : Reference count
	 * @param  bMatSearch :false
	 * @return Result array
	 */
	TArray<FAssetSearchToolResult> SearchAllReference(int& refType ,const FAssetData& asset ,UClass* endType, int referenceLevel , bool bSearchAllReferenceLevel = false, bool bMatSearch = false);
	
private:
	
	TArray<FAssetSearchToolResult> RefSearchResults;

	bool Tick(float DeltaTime);

	FTSTicker::FDelegateHandle TickDelegateHandle;
	
	float RefSearchProgress = 0;
	
	TArray<FAssetSearchToolMaterialInstanceResult> MaterialInstanceResultPath;

	//用来储存一键修改搜索结果
	static TArray<FAssetSearchToolMaterialInstanceResult> MaterialInstanceResultData;
	
	TSharedPtr<STextBlock> currentMaterialInstanceResultDataFromWindowText;

	FString CurrentMaterialInsSearchLockPanel;
	
	void ReferenceSearchResultResultToMaterialInsSearchResult();

	//搜索模式
	TArray<TSharedPtr<FString>> SearchMode;
	TSharedPtr<FString> CurrentSearchMode;
	TArray<UClass*> SearchType;

	TArray<FString> refAssetPathResult;
	
	//从什么类型开始搜索
	UClass* CurrentHeaderType;
	FString BeginSearchPath;
	FString BeginSearchExcludePath;

	//搜索结束类型
	UClass* CurrentEndType;
	//FString EndSearchPath;

	int SearchRefIndex = 0;//0 所有, 1 左 , 2 右
	
	//父子关系
	TArray<TSharedPtr<FString>> ParentMode;
	TSharedPtr<FString> CurrentParentMode;
	 
	//忽略相同资产,只求搜索的最终结果(输出的资产将不会进行排序)
	bool bOnlyResult = false;

	//搜索所有引用等级
	bool bSearchAllReferenceLevel = false;
	
	//引用查找模块
	TSharedPtr<SBox> Box_ReferenceSearch;

	TSharedPtr<SProgressBar> ProgressBar_ReferenceSearch; 
	
	//材质实例搜索模块
	TSharedPtr<SBox> Box_MaterialInstanceSearch;

	TSharedPtr<STextBlock> materialInsSearchtipText;
	
	TArray<TSharedPtr<FString>> MaterialInstanceSearchType;
	TSharedPtr<FString> CurrentMaterialInstanceType;
	TArray<TSharedPtr<FString>> NewMaterialInstanceSearchType;
	TSharedPtr<FString> NewCurrentMaterialInstanceType;
	
	//材质实例搜索 过滤参数
	FString MaterialInstanceParentMaterial;
	FString MaterialInstanceParamName;
	FString MaterialInstanceParamValue;
	FString NewMaterialInstanceParamName;
	FString NewMaterialInstanceParamValue;
	int MaterialInstanceRefCount = 9999;
	
	TArray<TSharedPtr<FString>> MaterialInstanceScalarOP;
	TSharedPtr<FString> CurrentMaterialInstanceScalarOP;

	//一键修改参数的是否继承重写开关
	bool bNewMaterialInstanceOverrideInherit = true;

	//材质实例搜索 路径
	FText MaterialInstanceBeginSearchPath;

	//材质实例搜索 排除路径
	FText MaterialInstanceBeginSearchPathExclude;

	TArray<FAssetData> MaterialReferenceAssetPaths;
	TArray<int> MaterialReferenceAssetReferenceLevel;
	//批量设置新Parent
	FString SetNewParentForMaterialInstance;
	//FString SetOldParentForMaterialInstance;
	//FString CurrentNewParentScalar;
	//FString CurrentNewParentValue;
	//FString CurrentNewParentOP;

	//是否不包含材质参数覆盖
	bool bOveriddenOnly = true;

	//只找一个参数
	bool bOnlyOneParam = true;

	//只找一个参数
	bool bIgnoreUnValidParam = true;
	
	FAssetData NewMaterialInstaceParent;
	//FAssetData OldMaterialInstaceParent;
	//表格
	class UDataTable* DataTable = nullptr;
	
	static TMap<FString, TSharedPtr<CustomResultTable>> AllCustomResultTables;
	
	void SearchReference();

	void SearchMaterialInstances();
	void UpdateMaterialInstanceSearchType(TSharedRef<SHorizontalBox> box);
	void SetMaterialInstancesParameterValue(TArray<FAssetSearchToolMaterialInstanceResult>& AssetData, bool newMaterialInstanceOverrideInherit);
	void SetMaterialInstancesParameterValueFromSelection();
	void SetMaterialInstanceParent(TArray<FAssetSearchToolMaterialInstanceResult>& AssetData);
	void GetMaterialInstanceResultByAssetData(TArray<FAssetData>& AssetData,TArray<int>& RefLevel,FText& BeginSearchPath, FText& BeginSearchPathExclude);
	
};
