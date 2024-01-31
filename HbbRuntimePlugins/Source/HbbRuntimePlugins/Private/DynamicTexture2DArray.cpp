// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTexture2DArray.h"


UDynamicTexture2DArray::UDynamicTexture2DArray(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FTextureResource* UDynamicTexture2DArray::CreateResource(TArray<TSoftObjectPtr<UTexture2D>>&SourceTextures)
{
	TArray<TSoftObjectPtr<UTexture2D>>validTexs;
	validTexs.Reserve(SourceTextures.Num());
	for(auto& i : SourceTextures)
	{
		if(i)
		{
			validTexs.Add(i);
		}
	}
	
	if(validTexs.Num()>0&&validTexs[0])
	{
		if(GetResource() != nullptr)
		{
			auto currentRes = static_cast<FDynamicTexture2DArrayResource*>(GetResource());
			if(currentRes->GetSizeX() == validTexs[0]->GetSizeX() && currentRes->GetPixelFormat() == validTexs[0]->GetPixelFormat())
			{
				return GetResource();
			}
		}
		auto newResource = new FDynamicTexture2DArrayResource(validTexs[0]->GetSizeX(),validTexs[0]->GetPixelFormat(),validTexs[0]->GetNumMips(),validTexs.Num(),validTexs[0]->SRGB);
		SetResource(newResource);
		UpdateFromSourceTextures(validTexs);
		return newResource;
	}
	else
	{
		SetResource(nullptr);
		return nullptr;
	}
}

void UDynamicTexture2DArray::UpdateFromSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>&NewSourceTextures)
{
	//if(this->GetResource() == nullptr)
	if(this->GetResource() == nullptr || this->GetResource()->TextureRHI == nullptr)
	{
		return;
	}
	// if(this->GetResource()->TextureRHI == nullptr)
	// {
	// 	ENQUEUE_RENDER_COMMAND(UpdateDynamicTexture2DArray)(
	// 	[this,NewSourceTextures](FRHICommandListImmediate& RHICmdList)
	// 	{
	// 		auto res = static_cast<FDynamicTexture2DArrayResource*>(this->GetResource());
	// 		res->InitRHI(RHICmdList);
	// 	});
	// }
	ENQUEUE_RENDER_COMMAND(UpdateDynamicTexture2DArray)(
	[this,NewSourceTextures](FRHICommandListImmediate& RHICmdList)
	{
		auto res = static_cast<FDynamicTexture2DArrayResource*>(this->GetResource());
		UpdateFromSourceTextures_RenderThread(RHICmdList,res,NewSourceTextures);
	});
}

void UDynamicTexture2DArray::UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList,FDynamicTexture2DArrayResource* resource,TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures)
{
	//Copy the texture2D data in to the texture2DArray.
	//Copy the mip data in to the texture.
	for (uint32 MipIndex = 0; MipIndex < resource->GetNumMips(); MipIndex++)
	{		
		for (uint32 ArrayIndex = 0; ArrayIndex < resource->GetNumSlices(); ++ArrayIndex)
		{
			uint32 DestStride = 0;
			void* DestData = RHILockTexture2DArray(resource->TextureRHI, ArrayIndex, MipIndex, RLM_WriteOnly, DestStride, false);
			if (DestData)
			{
				int32 texSize = resource->GetSizeX();
					FRHICopyTextureInfo CopyInfo={};
					CopyInfo.DestMipIndex = MipIndex;
					CopyInfo.DestPosition = FIntVector(0, 0, 0);
					CopyInfo.DestSliceIndex = ArrayIndex;
					CopyInfo.NumMips = 1;
					CopyInfo.NumSlices = 1;
#if PLATFORM_IOS
					CopyInfo.Size = FIntVector(CachedSize.X >> Mip,	CachedSize.Y >> Mip, 1);
#else
					CopyInfo.Size = FIntVector(
						FMath::Max(texSize >> MipIndex, GPixelFormats[resource->GetPixelFormat()].BlockSizeX),
						FMath::Max(texSize >> MipIndex, GPixelFormats[resource->GetPixelFormat()].BlockSizeY),
						1);
#endif
					RHICmdList.CopyTexture(NewSourceTextures[ArrayIndex]->GetResource()->TextureRHI,resource->TextureRHI,CopyInfo);
				}
				RHIUnlockTexture2DArray(resource->TextureRHI, ArrayIndex, MipIndex, false);
				RHICmdList.Transition({FRHITransitionInfo(resource->TextureRHI,ERHIAccess::ReadableMask)});
		}
	}
}

FDynamicTexture2DArrayResource::FDynamicTexture2DArrayResource(uint32 InSizeXY, EPixelFormat InFormat, uint32 InNumMips,
	uint32 InNumSlices, bool InbSRGB)
{
	SizeXY = InSizeXY;
	Format = InFormat;
	NumMips = InNumMips;
	NumSlices = InNumSlices;
	Filter = ESamplerFilter::SF_Bilinear;
	SamplerAddress = ESamplerAddressMode::AM_Wrap;
	bSRGB = InbSRGB;
}

//
void FDynamicTexture2DArrayResource::InitRHI(FRHICommandListBase& RHICmdList)
{
	FTextureResource::InitRHI(RHICmdList);
	//
	FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2DArray(TEXT("FTexture2DArrayResource"), SizeXY, SizeXY, NumSlices, Format)
		.SetNumMips(NumMips);
	Desc.AddFlags( (bSRGB ? ETextureCreateFlags::SRGB : ETextureCreateFlags::None) |  ETextureCreateFlags::OfflineProcessed);
	TextureRHI = RHICreateTexture(Desc);

	//TextureSRV = RHICmdList.CreateShaderResourceView(TextureRHI, /*MipLevel = */0, static_cast<uint8>(NumMips), Format);
}

void FDynamicTexture2DArrayResource::ReleaseRHI()
{
	FTextureResource::ReleaseRHI();
	
}

// void FDynamicTexture2DArrayResource::GetData(int32 BaseRHIMipSizeX, int32 BaseRHIMipSizeY, uint32 ArrayIndex,
//                                              uint32 MipIndex, void* Dest, uint32 DestPitch) const
// {
	
// }
//
// void FDynamicTexture2DArrayResource::CreateTexture()
// {
	

	// Copy the mip data in to the texture.
	// for (int32 MipIndex = 0; MipIndex < NumMips; MipIndex++)
	// {		
	// 	for (uint32 ArrayIndex = 0; ArrayIndex < NumSlices; ++ArrayIndex)
	// 	{
	// 		uint32 DestStride = 0;
	// 		void* DestData = RHILockTexture2DArray(TextureRHI, ArrayIndex, MipIndex, RLM_WriteOnly, DestStride, false);
	// 		if (DestData)
	// 		{
	// 			GetData(SizeXY, SizeXY, ArrayIndex, MipIndex, DestData, DestStride);
	// 		}
	// 		else
	// 		{
	// 			UE_LOG(LogTexture, Warning, TEXT("Failed to lock texture 2d array mip/slice %d / %d (%s)"), MipIndex, ArrayIndex, *TextureName.ToString());
	// 		}
	// 		RHIUnlockTexture2DArray(TextureRHI, ArrayIndex, MipIndex, false);
	// 	}
	// }
// }
