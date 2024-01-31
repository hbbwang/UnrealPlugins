// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture.h"
#include "DynamicTexture2DArray.generated.h"

class FDynamicTexture2DArrayResource;
/**
 * 
 */
#define MAX_ARRAY_SLICES 512

UCLASS(HideCategories = Object, MinimalAPI, BlueprintType)
class UDynamicTexture2DArray : public UTexture
{
	GENERATED_UCLASS_BODY()
	friend class UDynamicTexture2DArrayComponent;
	
public:
	virtual void SetSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures);
	virtual FTextureResource* CreateResource()override;
	virtual void UpdateResource() override;
	void UpdateFromSourceTextures();
	virtual EMaterialValueType GetMaterialType() const override { return MCT_Texture2DArray; }
	
private:
	
	void UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList,FDynamicTexture2DArrayResource* resource);

	TArray<TSoftObjectPtr<UTexture2D>>SourceTextures;
};

class FDynamicTexture2DArrayResource : public FTextureResource
{
public:
	FDynamicTexture2DArrayResource(uint32 InSizeXY, EPixelFormat InFormat, uint32 InNumMips,uint32 InNumSlices,bool InbSRGB);
	
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

	FShaderResourceViewRHIRef GetSRV()
	{
		return TextureSRV;
	}
	
private:
	uint32 SizeXY;
	EPixelFormat Format;
	uint32 NumMips;
	uint32 NumSlices;
	ESamplerFilter Filter;
	ESamplerAddressMode SamplerAddress;
	bool bSRGB;
	FShaderResourceViewRHIRef TextureSRV;
};