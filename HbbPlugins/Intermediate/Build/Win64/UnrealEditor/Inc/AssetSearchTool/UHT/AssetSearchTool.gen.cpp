// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetSearchTool/Public/AssetSearchTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAssetSearchTool() {}
// Cross Module References
	ASSETSEARCHTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult();
	ASSETSEARCHTOOL_API UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolResult();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
	UPackage* Z_Construct_UPackage__Script_AssetSearchTool();
// End Cross Module References

static_assert(std::is_polymorphic<FAssetSearchToolResult>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FAssetSearchToolResult cannot be polymorphic unless super FTableRowBase is polymorphic");

	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_AssetSearchToolResult;
class UScriptStruct* FAssetSearchToolResult::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_AssetSearchToolResult.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_AssetSearchToolResult.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAssetSearchToolResult, (UObject*)Z_Construct_UPackage__Script_AssetSearchTool(), TEXT("AssetSearchToolResult"));
	}
	return Z_Registration_Info_UScriptStruct_AssetSearchToolResult.OuterSingleton;
}
template<> ASSETSEARCHTOOL_API UScriptStruct* StaticStruct<FAssetSearchToolResult>()
{
	return FAssetSearchToolResult::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AssetName_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_AssetName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AssetPath_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AssetPath;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LorR_ReferenceLevel_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_LorR_ReferenceLevel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceAssetPath_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReferenceAssetPath;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FirstMaterialInstanceReference_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_FirstMaterialInstanceReference;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// typedef uint32_t AssetTypeFlags;\n// enum EAssetTypeBit : uint32_t\n// {\n// \x09None = 0,\n// \x09Particle = 1<<0,\n// \x09Material = 1<<1,\n// \x09MaterialInstance = 1<<2,\n// \x09MaterialFunction = 1<<3,\n// \x09StaticMesh = 1<<4,\n// \x09SkeletalMesh = 1<<5,\n// \x09Level = 1<<6\n// };\n" },
#endif
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "typedef uint32_t AssetTypeFlags;\nenum EAssetTypeBit : uint32_t\n{\n      None = 0,\n      Particle = 1<<0,\n      Material = 1<<1,\n      MaterialInstance = 1<<2,\n      MaterialFunction = 1<<3,\n      StaticMesh = 1<<4,\n      SkeletalMesh = 1<<5,\n      Level = 1<<6\n};" },
#endif
	};
#endif
	void* Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAssetSearchToolResult>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe8\xb5\x84\xe4\xba\xa7\xe5\x90\x8d" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName = { "AssetName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolResult, AssetName), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe8\xb5\x84\xe4\xba\xa7\xe8\xb7\xaf\xe5\xbe\x84" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath = { "AssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolResult, AssetPath), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe5\xbc\x95\xe7\x94\xa8\xe7\xad\x89\xe7\xba\xa7" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel = { "LorR_ReferenceLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolResult, LorR_ReferenceLevel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe5\xbc\x95\xe7\x94\xa8\xe5\xaf\xb9\xe8\xb1\xa1\xe8\xb7\xaf\xe5\xbe\x84" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath = { "ReferenceAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolResult, ReferenceAssetPath), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "DisplayName", "\xe4\xb8\x80\xe7\xba\xa7\xe6\x9d\x90\xe8\xb4\xa8\xe5\xae\x9e\xe4\xbe\x8b(\xe5\xa6\x82\xe6\x9e\x9c\xe6\x9c\x89\xe7\x9a\x84\xe8\xaf\x9d)" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference = { "FirstMaterialInstanceReference", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolResult, FirstMaterialInstanceReference), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_AssetPath,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_LorR_ReferenceLevel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_ReferenceAssetPath,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewProp_FirstMaterialInstanceReference,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_AssetSearchTool,
		Z_Construct_UScriptStruct_FTableRowBase,
		&NewStructOps,
		"AssetSearchToolResult",
		Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers),
		sizeof(FAssetSearchToolResult),
		alignof(FAssetSearchToolResult),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolResult()
	{
		if (!Z_Registration_Info_UScriptStruct_AssetSearchToolResult.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_AssetSearchToolResult.InnerSingleton, Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_AssetSearchToolResult.InnerSingleton;
	}

static_assert(std::is_polymorphic<FAssetSearchToolMaterialInstanceResult>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FAssetSearchToolMaterialInstanceResult cannot be polymorphic unless super FTableRowBase is polymorphic");

	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult;
class UScriptStruct* FAssetSearchToolMaterialInstanceResult::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult, (UObject*)Z_Construct_UPackage__Script_AssetSearchTool(), TEXT("AssetSearchToolMaterialInstanceResult"));
	}
	return Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.OuterSingleton;
}
template<> ASSETSEARCHTOOL_API UScriptStruct* StaticStruct<FAssetSearchToolMaterialInstanceResult>()
{
	return FAssetSearchToolMaterialInstanceResult::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InstanceAssetName_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_InstanceAssetName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InstanceAssetPath_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_InstanceAssetPath;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceLevel_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ReferenceLevel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParameterName_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ParameterName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParameterValue_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ParameterValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bOverride_MetaData[];
#endif
		static void NewProp_bOverride_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bOverride;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParentAssetPath_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ParentAssetPath;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAssetSearchToolMaterialInstanceResult>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName = { "InstanceAssetName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, InstanceAssetName), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath = { "InstanceAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, InstanceAssetPath), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData[] = {
		{ "Category", "AssetSearch" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe6\xaf\x8d\xe6\x9d\x90\xe8\xb4\xa8\xe7\x90\x83\xe7\x9a\x84\xe7\xac\xac\xe5\x87\xa0\xe7\xba\xa7\xe5\xbc\x95\xe7\x94\xa8\n" },
#endif
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xaf\x8d\xe6\x9d\x90\xe8\xb4\xa8\xe7\x90\x83\xe7\x9a\x84\xe7\xac\xac\xe5\x87\xa0\xe7\xba\xa7\xe5\xbc\x95\xe7\x94\xa8" },
#endif
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel = { "ReferenceLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ReferenceLevel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName = { "ParameterName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParameterName), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue = { "ParameterValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParameterValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_SetBit(void* Obj)
	{
		((FAssetSearchToolMaterialInstanceResult*)Obj)->bOverride = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride = { "bOverride", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAssetSearchToolMaterialInstanceResult), &Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData[] = {
		{ "Category", "AssetSearch" },
		{ "ModuleRelativePath", "Public/AssetSearchTool.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath = { "ParentAssetPath", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetSearchToolMaterialInstanceResult, ParentAssetPath), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_InstanceAssetPath,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ReferenceLevel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParameterValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_bOverride,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewProp_ParentAssetPath,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_AssetSearchTool,
		Z_Construct_UScriptStruct_FTableRowBase,
		&NewStructOps,
		"AssetSearchToolMaterialInstanceResult",
		Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers),
		sizeof(FAssetSearchToolMaterialInstanceResult),
		alignof(FAssetSearchToolMaterialInstanceResult),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult()
	{
		if (!Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.InnerSingleton, Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_TechnologyTest_Plugins_HbbPlugins_Source_AssetSearchTool_Public_AssetSearchTool_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_TechnologyTest_Plugins_HbbPlugins_Source_AssetSearchTool_Public_AssetSearchTool_h_Statics::ScriptStructInfo[] = {
		{ FAssetSearchToolResult::StaticStruct, Z_Construct_UScriptStruct_FAssetSearchToolResult_Statics::NewStructOps, TEXT("AssetSearchToolResult"), &Z_Registration_Info_UScriptStruct_AssetSearchToolResult, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAssetSearchToolResult), 831677033U) },
		{ FAssetSearchToolMaterialInstanceResult::StaticStruct, Z_Construct_UScriptStruct_FAssetSearchToolMaterialInstanceResult_Statics::NewStructOps, TEXT("AssetSearchToolMaterialInstanceResult"), &Z_Registration_Info_UScriptStruct_AssetSearchToolMaterialInstanceResult, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAssetSearchToolMaterialInstanceResult), 4112624286U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_TechnologyTest_Plugins_HbbPlugins_Source_AssetSearchTool_Public_AssetSearchTool_h_1319921335(TEXT("/Script/AssetSearchTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_TechnologyTest_Plugins_HbbPlugins_Source_AssetSearchTool_Public_AssetSearchTool_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_TechnologyTest_Plugins_HbbPlugins_Source_AssetSearchTool_Public_AssetSearchTool_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
