// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture.h"
#include "DynamicTexture2DArray.generated.h"

class FDynamicTexture2DArrayResource;
/**
 * 
 */

UCLASS(HideCategories = Object, MinimalAPI, BlueprintType)
class UDynamicTexture2DArray : public UTexture
{
	GENERATED_UCLASS_BODY()
	friend class UDynamicTexture2DArrayFactory;
	friend class UDynamicTexture2DArrayComponent;
	friend class UHbbRuntimeBPFunctionLibrary;
public:
	HBBRUNTIMEPLUGINS_API virtual void SetSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures);
	HBBRUNTIMEPLUGINS_API virtual void SetSourceTexture(TSoftObjectPtr<UTexture2D>NewSourceTexture , int32 index );
	HBBRUNTIMEPLUGINS_API virtual void UpdateResource() override;
	HBBRUNTIMEPLUGINS_API virtual void UpdateResource(int32 updateTextureIndex);
	HBBRUNTIMEPLUGINS_API virtual void ForceUpdateResource();
	HBBRUNTIMEPLUGINS_API void UpdateFromSourceTextures(int32 index);
	HBBRUNTIMEPLUGINS_API virtual EMaterialValueType GetMaterialType() const override { return MCT_Texture2DArray; }
protected:
	virtual FTextureResource* CreateResource()override;
	void UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList, int32 index);
	void UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList, TArray<FTexture*> FTextures);
private:
	TArray<TSoftObjectPtr<UTexture2D>>SourceTextures;
	UDynamicTexture2DArrayComponent* Comp;
	bool bForceUpdate ;
	//
	int32 TextureSize;
	EPixelFormat PixelFormat;
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