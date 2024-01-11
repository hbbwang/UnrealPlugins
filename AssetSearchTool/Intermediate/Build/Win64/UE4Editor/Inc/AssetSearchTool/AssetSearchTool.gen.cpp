// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetSearchTool/Public/AssetSearchTool.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAssetSearchTool() {}
// Cross Module References
	ASSETSEARCHTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult();
	UPackage* Z_Construct_UPackage__Script_AssetSearchTool();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
	ASSETSEARCHTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolResult();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
// End Cross Module References

static_assert(std::is_polymorphic<FAssetSearchToolMaterialInstanceResult>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FAssetSearchToolMaterialInstanceResult cannot be polymorphic unless super FTableRowBase is polymorphic");

class UScriptStruct* FAssetSearchToolMaterialInstanceResult::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ASSETSEARCHTOOL_API uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult, Z_Construct_UPackage__Script_AssetSearchTool(), TEXT("AssetSearchToolMaterialInstanceResult"), sizeof(FAssetSearchToolMaterialInstanceResult), Get_Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Hash());
	}
	return Singleton;
}
template<> ASSETSEARCHTOOL_API UScriptStruct* StaticStruct<FAssetSearchToolMaterialInstanceResult>()
{
	return FAssetSearchToolMaterialInstanceResult::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FAssetSearchToolMaterialInstanceResult(FAssetSearchToolMaterialInstanceResult::StaticStruct, TEXT("/Script/AssetSearchTool"), TEXT("AssetSearchToolMaterialInstanceResult"), false, nullptr, nullptr);
static struct FScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolMaterialInstanceResult
{
	FScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolMaterialInstanceResult()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("AssetSearchToolMaterialInstanceResult")),new UScriptStruct::TCppStructOps<FAssetSearchToolMaterialInstanceResult>);
	}
} ScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolMaterialInstanceResult;
	struct Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InstanceAssetName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_InstanceAssetName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InstanceAssetPath_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_InstanceAssetPath;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReferenceLevel_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_ReferenceLevel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParameterName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ParameterName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParameterValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ParameterValue;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_MetaData[];
#endif
		static void NewProp_bOverride_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParentAssetPath_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ParentAssetPath;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAssetSearchToolMaterialInstanceResult>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName = { "InstanceAssetName", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, InstanceAssetName), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath = { "InstanceAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, InstanceAssetPath), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "Comment", "//\xe6\xaf\x8d\xe6\x9d\x90\xe8\xb4\xa8\xe7\x90\x83\xe7\x9a\x84\xe7\xac\xac\xe5\x87\xa0\xe7\xba\xa7\xe5\xbc\x95\xe7\x94\xa8\n" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
		{ "ToolTip", "\xe6\xaf\x8d\xe6\x9d\x90\xe8\xb4\xa8\xe7\x90\x83\xe7\x9a\x84\xe7\xac\xac\xe5\x87\xa0\xe7\xba\xa7\xe5\xbc\x95\xe7\x94\xa8" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel = { "ReferenceLevel", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ReferenceLevel), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName = { "ParameterName", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParameterName), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue = { "ParameterValue", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParameterValue), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_SetBit(void* Obj)
	{
		((FAssetSearchToolMaterialInstanceResult*)Obj)->bOverride = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride = { "bOverride", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FAssetSearchToolMaterialInstanceResult), &Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath = { "ParentAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParentAssetPath), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_AssetSearchTool,
		Z_Construct_UScriptStruct_FTableRowBase,
		&NewStructOps,
		"AssetSearchToolMaterialInstanceResult",
		sizeof(FAssetSearchToolMaterialInstanceResult),
		alignof(FAssetSearchToolMaterialInstanceResult),
		Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_AssetSearchTool();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("AssetSearchToolMaterialInstanceResult"), sizeof(FAssetSearchToolMaterialInstanceResult), Get_Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Hash() { return 2278554022U; }

static_assert(std::is_polymorphic<FAssetSearchToolResult>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FAssetSearchToolResult cannot be polymorphic unless super FTableRowBase is polymorphic");

class UScriptStruct* FAssetSearchToolResult::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ASSETSEARCHTOOL_API uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolResult_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FAssetSearchToolResult, Z_Construct_UPackage__Script_AssetSearchTool(), TEXT("AssetSearchToolResult"), sizeof(FAssetSearchToolResult), Get_Z_Construct_UScriptStruct_FAssetSearchToolResult_Hash());
	}
	return Singleton;
}
template<> ASSETSEARCHTOOL_API UScriptStruct* StaticStruct<FAssetSearchToolResult>()
{
	return FAssetSearchToolResult::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FAssetSearchToolResult(FAssetSearchToolResult::StaticStruct, TEXT("/Script/AssetSearchTool"), TEXT("AssetSearchToolResult"), false, nullptr, nullptr);
static struct FScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolResult
{
	FScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolResult()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("AssetSearchToolResult")),new UScriptStruct::TCppStructOps<FAssetSearchToolResult>);
	}
} ScriptStruct_AssetSearchTool_StaticRegisterNativesFAssetSearchToolResult;
	struct Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_AssetName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetPath_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_AssetPath;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LorR_ReferenceLevel_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_LorR_ReferenceLevel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReferenceAssetPath_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReferenceAssetPath;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FirstMaterialInstanceReference_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_FirstMaterialInstanceReference;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "// typedef uint32_t AssetTypeFlags;\n// enum EAssetTypeBit : uint32_t\n// {\n// \x09None = 0,\n// \x09Particle = 1<<0,\n// \x09Material = 1<<1,\n// \x09MaterialInstance = 1<<2,\n// \x09MaterialFunction = 1<<3,\n// \x09StaticMesh = 1<<4,\n// \x09SkeletalMesh = 1<<5,\n// \x09Level = 1<<6\n// };\n" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
		{ "ToolTip", "typedef uint32_t AssetTypeFlags;\nenum EAssetTypeBit : uint32_t\n{\n      None = 0,\n      Particle = 1<<0,\n      Material = 1<<1,\n      MaterialInstance = 1<<2,\n      MaterialFunction = 1<<3,\n      StaticMesh = 1<<4,\n      SkeletalMesh = 1<<5,\n      Level = 1<<6\n};" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAssetSearchToolResult>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe8\xb5\x84\xe4\xba\xa7\xe5\x90\x8d" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName = { "AssetName", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolResult, AssetName), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe8\xb5\x84\xe4\xba\xa7\xe8\xb7\xaf\xe5\xbe\x84" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath = { "AssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolResult, AssetPath), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe5\xbc\x95\xe7\x94\xa8\xe7\xad\x89\xe7\xba\xa7" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel = { "LorR_ReferenceLevel", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolResult, LorR_ReferenceLevel), METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe5\xbc\x95\xe7\x94\xa8\xe5\xaf\xb9\xe8\xb1\xa1\xe8\xb7\xaf\xe5\xbe\x84" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath = { "ReferenceAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolResult, ReferenceAssetPath), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe4\xb8\x80\xe7\xba\xa7\xe6\x9d\x90\xe8\xb4\xa8\xe5\xae\x9e\xe4\xbe\x8b(\xe5\xa6\x82\xe6\x9e\x9c\xe6\x9c\x89\xe7\x9a\x84\xe8\xaf\x9d)" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference = { "FirstMaterialInstanceReference", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FAssetSearchToolResult, FirstMaterialInstanceReference), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_AssetSearchTool,
		Z_Construct_UScriptStruct_FTableRowBase,
		&NewStructOps,
		"AssetSearchToolResult",
		sizeof(FAssetSearchToolResult),
		alignof(FAssetSearchToolResult),
		Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolResult()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolResult_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_AssetSearchTool();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("AssetSearchToolResult"), sizeof(FAssetSearchToolResult), Get_Z_Construct_UScriptStruct_FAssetSearchToolResult_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FAssetSearchToolResult_Hash() { return 3309066611U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
