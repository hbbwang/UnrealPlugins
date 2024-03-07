// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HbbRuntimeBPFunctionLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMaterilInsBlendMode
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Hbb Editor Function")
	bool bOverride;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Hbb Editor Function")
	TEnumAsByte<EBlendMode> Value;
};

USTRUCT(BlueprintType)
struct FMaterilInsStaticSwitchParameter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Hbb Editor Function")
	FName Name;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Hbb Editor Function")
	bool bOverride;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Hbb Editor Function")
	bool Value;
};

UCLASS()
class HBBRUNTIMEPLUGINS_API UHbbRuntimeBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable,Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static FMaterilInsStaticSwitchParameter GetMaterialInstanceStaticSwitchParameter(UObject* WorldContextObject,class UMaterialInstanceConstant* materialIns,FName StaticParameterName);
	
	UFUNCTION(BlueprintCallable,Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SetMaterialInstanceStaticSwitchParameter(UObject* WorldContextObject,class UMaterialInstanceConstant* materialIns,FName StaticParameterName,bool Value);

	UFUNCTION(BlueprintCallable,Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static FMaterilInsBlendMode GetMaterialInstanceBlendMode(UObject* WorldContextObject,class UMaterialInstanceConstant* materialIns);
	
	UFUNCTION(BlueprintCallable,Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SetMaterialInstanceBlendMode(UObject* WorldContextObject,class UMaterialInstanceConstant* materialIns,TEnumAsByte<EBlendMode> BlendMode);

	UFUNCTION(BlueprintCallable, Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void DrawMaterialtoRenderTargetWithAlpha (UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable, Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void ExportAssetFile (UObject* WorldContextObject, UObject* TextureRenderTarget, FString outputPath);

	UFUNCTION(BlueprintCallable, Category="Hbb Editor Function", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SetMaterialDomain (class UMaterialInstanceConstant* materialIns , TEnumAsByte<EMaterialDomain> Domain , bool bMarkDirt);

};
