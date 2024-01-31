// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTexture2DArray.h"

#include "RenderGraphBuilder.h"


UDynamicTexture2DArray::UDynamicTexture2DArray(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDynamicTexture2DArray::SetSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures)
{
	SourceTextures = NewSourceTextures;
}

FTextureResource* UDynamicTexture2DArray::CreateResource()
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
		ReleaseResource();
		auto newResource = new FDynamicTexture2DArrayResource(validTexs[0]->GetSizeX(),validTexs[0]->GetPixelFormat(),validTexs[0]->GetNumMips(),validTexs.Num(),validTexs[0]->SRGB);
		SetResource(newResource);
		//UpdateFromSourceTextures(validTexs);
		return newResource;
	}
	else
	{
		ReleaseResource();
		SetResource(nullptr);
		return nullptr;
	}
}

void UDynamicTexture2DArray::UpdateResource()
{
	Super::UpdateResource();
}

UE_DISABLE_OPTIMIZATION

void UDynamicTexture2DArray::UpdateFromSourceTextures()
{
	// if(this->GetResource() == nullptr)
	// //if(this->GetResource() == nullptr || this->GetResource()->TextureRHI == nullptr)
	// {
	// 	//return;
	// 	CreateResource();
	// }
	UpdateResource();
	
	ENQUEUE_RENDER_COMMAND(UpdateDynamicTexture2DArray)(
	[this](FRHICommandListImmediate& RHICmdList)
	{
		auto res = static_cast<FDynamicTexture2DArrayResource*>(this->GetResource());
		UpdateFromSourceTextures_RenderThread(RHICmdList,res);
	});
}

void UDynamicTexture2DArray::UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList,FDynamicTexture2DArrayResource* resource)
{
	// resource->ReleaseRHI();
	// this->CreateResource();
	// resource->InitRHI(RHICmdList);
	
	//Copy the texture2D data in to the texture2DArray.
	//Copy the mip data in to the texture.
	for (uint32 MipIndex = 0; MipIndex < resource->GetNumMips(); MipIndex++)
	{	
		for (uint32 ArrayIndex = 0; ArrayIndex <1; ArrayIndex++)
		{
			uint32 DestStride = 0;
			void* DestData = RHILockTexture2DArray(resource->GetTexture2DArrayRHI(),ArrayIndex,MipIndex,RLM_WriteOnly,DestStride,false);
			int32 texSize = resource->GetSizeX();
			// else if (DestData && srcData)
			//if (DestData && srcData)
			{
 				FRHICopyTextureInfo CopyInfo={};
				
				CopyInfo.SourceMipIndex = MipIndex;
				CopyInfo.SourcePosition = FIntVector(0, 0, 0);
				CopyInfo.SourceSliceIndex = 0;
				//
 				CopyInfo.NumMips = 1;
 				CopyInfo.NumSlices = 1;
				//
				CopyInfo.DestMipIndex = MipIndex;
				CopyInfo.DestPosition = FIntVector(0, 0, 0);
				CopyInfo.DestSliceIndex = ArrayIndex;
				//
				// auto nextSizeX =  FMath::Max(texSize >> (MipIndex), GPixelFormats[resource->GetPixelFormat()].BlockSizeX);
				// auto nextSizeY =  FMath::Max(texSize >> (MipIndex), GPixelFormats[resource->GetPixelFormat()].BlockSizeY);
				// auto nextSizeX =  texSize >> (MipIndex) / GPixelFormats[resource->GetPixelFormat()].BlockSizeX;
				// auto nextSizeY =  texSize >> (MipIndex), GPixelFormats[resource->GetPixelFormat()].BlockSizeY);
				
				auto nextSizeX =  texSize  >> (MipIndex) ;
				auto nextSizeY =  texSize >> (MipIndex) ;
				CopyInfo.Size = FIntVector(
					nextSizeX,
					nextSizeY,
					1);
				
				//RHICmdList.CopyTexture(SourceTextures[ArrayIndex]->GetResource()->TextureRHI,resource->TextureRHI,CopyInfo);
				TransitionAndCopyTexture(
					RHICmdList,
					SourceTextures[ArrayIndex]->GetResource()->TextureRHI,
					resource->TextureRHI,
					CopyInfo);
				 // auto nextSizeX =  FMath::Max(texSize >> MipIndex, GPixelFormats[resource->GetPixelFormat()].BlockSizeX);
				 // auto nextSizeY =  FMath::Max(texSize >> MipIndex, GPixelFormats[resource->GetPixelFormat()].BlockSizeY);
				//CopyTextureData2D(srcData,DestData,nextSizeY,resource->GetPixelFormat(),SrcStride,DestStride);
			}
			RHIUnlockTexture2DArray(resource->GetTexture2DArrayRHI(), ArrayIndex, MipIndex, false);
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
		FRHITextureCreateDesc::Create2DArray(TEXT("FDynamicTexture2DArrayResource"))
	.SetNumMips(NumMips)
	.SetNumSamples(1)
	.SetInitialState(ERHIAccess::SRVMask)
	.SetArraySize(NumSlices)
	.SetExtent(SizeXY, SizeXY)
	.SetFormat(Format)
	.SetFlags((bSRGB ? ETextureCreateFlags::SRGB : ETextureCreateFlags::None)  | ETextureCreateFlags::ShaderResource);
	
	TextureRHI = RHICreateTexture(Desc);

	// Create the sampler state RHI resource.
	FSamplerStateInitializerRHI SamplerStateInitializer
	(
		Filter,
		SamplerAddress,
		SamplerAddress,
		SamplerAddress
	);
	SamplerStateRHI = GetOrCreateSamplerState(SamplerStateInitializer);
	DeferredPassSamplerStateRHI = SamplerStateRHI;
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
UE_ENABLE_OPTIMIZATION