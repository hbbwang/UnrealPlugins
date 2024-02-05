// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Texture.h"
#include "DynamicTexture2DArray.generated.h"

class FDynamicTexture2DArrayModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class FDynamicTexture2DArrayResource;
/**
 * 
 */

struct FDT2DArraySource
{
	//新纹理对象
	TSoftObjectPtr<UTexture2D> Texture;
	//需要更新到哪个Index
	int32 Index;
	//
	bool bExec = false;
};

UCLASS(HideCategories = Object, MinimalAPI, BlueprintType)
//UCLASS(Abstract, BlueprintType) 
class UDynamicTexture2DArray : public UTexture
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API virtual void SetSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures);
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API virtual void SetSourceTexture(TSoftObjectPtr<UTexture2D>NewSourceTexture , int32 index );
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray") 
	DYNAMICTEXTURE2DARRAY_API virtual void UpdateResource();
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API virtual void ForceUpdateResource();
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API void UpdateFromSourceTextures();

	DYNAMICTEXTURE2DARRAY_API virtual EMaterialValueType GetMaterialType() const override { return MCT_Texture2DArray; }
	DYNAMICTEXTURE2DARRAY_API const EPixelFormat GetPixelFormat() const {return PixelFormat;}
	
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API const int32 GetSizeX() const {return TextureSize;}
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API const int32 GetNumMips() const {return (int32)NumMips;}
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	DYNAMICTEXTURE2DARRAY_API const int32 GetNumSlices() const {return (int32)NumSlices;}
	//重新设置为单张黑色1x1纹理
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray", meta=(ToolTip="Reset to black texture."))
	virtual void ResetToNull();
	
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	static TArray<TSoftObjectPtr<UTexture2D>> GetValidTexture(TArray<TSoftObjectPtr<UTexture2D>>& NewSourceTextures);

	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray", meta=( WorldContext="WorldContextObject"))
	static class UDynamicTexture2DArray* CreateDynamicTexture2DArray(UObject* WorldContextObject ,TArray<TSoftObjectPtr<UTexture2D>>Texture2Ds);
	
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray", meta=( WorldContext="WorldContextObject",ToolTip="Create a DynamicTexture2DArray and add a white texture."))
	static class UDynamicTexture2DArray* CreateDynamicTexture2DArrayDefault(UObject* WorldContextObject);

	//本来应该存在FScene里跟随场景储存Texture2DArray,但是咱们走插件流程就没办法去改引擎了,就在这里存静态数组...
	UFUNCTION(BlueprintCallable, Category="DynamicTexture2DArray")
	static TArray<UDynamicTexture2DArray*>GetDynamicTexture2DArrayMap();
	
protected:
	virtual FTextureResource* CreateResource();
	void UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList);
	void UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList, TArray<FTexture*> FTextures);

private:
	bool bForceUpdate ;
	EPixelFormat PixelFormat;
	TArray<FDT2DArraySource>SourceTextures;
	int32 TextureSize;
	uint32 NumMips;
	uint32 NumSlices;
};


class FDynamicTexture2DArrayResource : public FTextureResource
{
	friend class UDynamicTexture2DArray;
public:
	FDynamicTexture2DArrayResource(UDynamicTexture2DArray* InOwner , uint32 InSizeXY, EPixelFormat InFormat, uint32 InNumMips,uint32 InNumSlices,bool InbSRGB);
	
	virtual uint32 GetSizeX() const override { return SizeXY; }
	virtual uint32 GetSizeY() const override { return SizeXY;}

	/** Called when the resource is initialized. This is only called by the rendering thread. */
	virtual void InitRHI(FRHICommandListBase& RHICmdList) override;
	virtual void ReleaseRHI() override;
	
	//void GetData(int32 BaseRHIMipSizeX, int32 BaseRHIMipSizeY, uint32 ArrayIndex, uint32 MipIndex, void* Dest, uint32 DestPitch) const;
	//void CreateTexture();
	
	EPixelFormat GetPixelFormat() const
	{
		return Format;
	}

	uint32 GetNumMips() const
	{
		return NumMips;
	}
	
	uint32 GetNumSlices() const
	{
		return NumSlices;
	}
	
private:
	UDynamicTexture2DArray* Owner;
	uint32 SizeXY;
	EPixelFormat Format;
	uint32 NumMips;
	uint32 NumSlices;
	ESamplerFilter Filter;
	ESamplerAddressMode SamplerAddress;
	bool bSRGB;
};

