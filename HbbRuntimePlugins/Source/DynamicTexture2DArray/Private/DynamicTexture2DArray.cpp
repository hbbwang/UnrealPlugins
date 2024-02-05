// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTexture2DArray.h"
#include "DynamicTexture2DArrayComponent.h"
#include "EngineUtils.h"
#include "RenderGraphBuilder.h"

#define LOCTEXT_NAMESPACE "FDynamicTexture2DArrayModule"
void FDynamicTexture2DArrayModule::StartupModule()
{
}
void FDynamicTexture2DArrayModule::ShutdownModule() 
{
}
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FDynamicTexture2DArrayModule, DynamicTexture2DArray)


static TArray<UDynamicTexture2DArray*> DynamicTexture2DArrayMap;

//UDynamicTexture2DArray UObject:
#include UE_INLINE_GENERATED_CPP_BY_NAME(DynamicTexture2DArray)
#define LOCTEXT_NAMESPACE "UDynamicTexture2DArray"
UDynamicTexture2DArray::UDynamicTexture2DArray(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TextureSize = 2;   
	PixelFormat = PF_DXT1; 
	NumMips = 1; 
	NumSlices = 1;
	SRGB = true;
	bForceUpdate = false;
}

void UDynamicTexture2DArray::SetSourceTextures(TArray<TSoftObjectPtr<UTexture2D>>NewSourceTextures)
{
	if(NewSourceTextures.Num()>0) 
	{
		SourceTextures.Empty();
		SourceTextures.Reserve(NewSourceTextures.Num());
		int32 index = 0;
		for(auto& i : NewSourceTextures)
		{
			UTexture2D* texObject = nullptr;
			if(!i || !i.IsValid())
			{
				texObject = i.LoadSynchronous();
			}
			else
			{
				texObject = i.Get();
			}
			if(index == 0)
			{
				TextureSize = texObject->GetPlatformData()->SizeX; 
				PixelFormat = texObject->GetPlatformData()->PixelFormat;
				NumMips = texObject->GetPlatformData()->Mips.Num();
				SRGB = texObject->SRGB;
			}
			else
			{ 
				if(TextureSize != texObject->GetSizeX() 
				|| PixelFormat != texObject->GetPixelFormat()
				|| NumMips != texObject->GetNumMips()
				|| SRGB != texObject->SRGB
				)
				{
					continue;
				}
			}
			if(texObject->NeverStream == 0 )
			{ 
				texObject->NeverStream = 1;
				texObject->UpdateResource();
			}
			SourceTextures.Add(FDT2DArraySource{texObject,index});
			index ++;
		}
		NumSlices = SourceTextures.Num();
		UpdateResource();
	}
}

void UDynamicTexture2DArray::SetSourceTexture(TSoftObjectPtr<UTexture2D> NewSourceTexture, int32 index )
{
	if(NewSourceTexture)
	{
		if(SourceTextures.Num()>0 && SourceTextures.IsValidIndex(index) )
		{
			UTexture2D* texObject = NewSourceTexture.Get();
			if(!texObject)
			{
				texObject =NewSourceTexture.LoadSynchronous();
			}
			if(TextureSize != texObject->GetSizeX() 
				|| PixelFormat != texObject->GetPixelFormat()
				|| NumMips != texObject->GetNumMips()
				|| SRGB != texObject->SRGB
				)
			{
				return;
			}
			if(texObject->NeverStream == 0)
			{
				texObject->NeverStream = 1;
				texObject->UpdateResource();
			}
			SourceTextures.Add(FDT2DArraySource{NewSourceTexture,index});
			UpdateResource(); 
		}
	}
}

TArray<TSoftObjectPtr<UTexture2D>> UDynamicTexture2DArray::GetValidTexture(
	TArray<TSoftObjectPtr<UTexture2D>>& NewSourceTextures)
{
	const auto newArraySize = NewSourceTextures.Num();
	if(newArraySize>0)
	{
		TArray<TSoftObjectPtr<UTexture2D>> validTextures;
		validTextures.Reserve(newArraySize);
		auto firstTemplate = NewSourceTextures[0].Get();
		if(firstTemplate == nullptr)
		{
			firstTemplate = NewSourceTextures[0].LoadSynchronous();
		}
		auto firstPlatformData = firstTemplate->GetPlatformData();
		for (int32  i = 0 ; i<newArraySize;i++ )
		{
			UTexture2D * tex = NewSourceTextures[i].Get();
			if(tex == nullptr)
			{
				tex = NewSourceTextures[i].LoadSynchronous();
			}
			if(i>0)
			{
				auto currentPlatformData = tex->GetPlatformData();
				//纹理格式不一致,不允许在一起
				if(firstPlatformData)
				{
					if(tex->GetMaterialType() != MCT_Texture2D )
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:需要添加到DynamicTexture2DArray的纹理类型不是一张Texture2D,请确认纹理类型无误,这张纹理是:%s")
							, *tex->GetName(),  *tex->GetPathName());
						continue;
					}
					else if(firstPlatformData->SizeX != currentPlatformData->SizeX ||	firstPlatformData->SizeY != currentPlatformData->SizeY)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的分辨率不一致!无法添加进DynamicTexture2DArray,指定纹理大小是%d,%d,但是当前纹理大小是%d,%d,这张纹理是:%s")
							, *tex->GetName() , firstPlatformData->SizeX , firstPlatformData->SizeY ,currentPlatformData->SizeX , currentPlatformData->SizeY,  *tex->GetPathName());
						continue;
					}
					else if(firstPlatformData->PixelFormat != currentPlatformData->PixelFormat)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的格式(PixelFormat)不一致!无法添加进DynamicTexture2DArray,指定格式是%s,但是当前纹理是%s,(请查看一下纹理的压缩格式或者是否勾选了Compress without alpha)这张纹理是:%s")
							, *tex->GetName() , GPixelFormats[firstPlatformData->PixelFormat].Name , GPixelFormats[currentPlatformData->PixelFormat].Name, *tex->GetPathName());
						continue;
					}
					else if(firstTemplate->GetNumMips() != tex->GetNumMips() )
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的Mip层数不一致!添加到DynamicTexture2DArray失败,指定Mip层数是%d,但是当前Mip层数是%d,这张纹理是:%s")
							, *tex->GetName() , firstTemplate->GetNumMips() , tex->GetNumMips() , *tex->GetPathName());
						continue;
					}
				}
			}
			if(tex->NeverStream == 0)
			{
				tex->NeverStream = 1;
				tex->UpdateResource();
			}
			validTextures.Add(tex);
		}
		return validTextures;
	}
	return TArray<TSoftObjectPtr<UTexture2D>>();
}

UDynamicTexture2DArray* UDynamicTexture2DArray::CreateDynamicTexture2DArray(UObject* WorldContextObject,
                                                                            TArray<TSoftObjectPtr<UTexture2D>> Texture2Ds)
{
	if(Texture2Ds.Num()>0)
	{
		UDynamicTexture2DArray* newDT2DA = NULL;
		if(!WorldContextObject)
		{
			UE_LOG(LogTemp, Error, TEXT("[CreateDynamicTexture2DArray] 错误！ Owner 所有者为空! 可能会导致UDynamicTexture2DArray无法被卸载，常驻内存中。"));
			WorldContextObject = GetTransientPackage();
		}
		auto validTexs = GetValidTexture(Texture2Ds);
		if(validTexs.Num()>0)
		{
			newDT2DA = NewObject<UDynamicTexture2DArray>(WorldContextObject);
			newDT2DA->SetSourceTextures(validTexs);
			return newDT2DA;
		}
		else
		{
			return CreateDynamicTexture2DArrayDefault(WorldContextObject);
		}
	}
	else
	{
		return CreateDynamicTexture2DArrayDefault(WorldContextObject);
	}
}

UDynamicTexture2DArray* UDynamicTexture2DArray::CreateDynamicTexture2DArrayDefault(UObject* WorldContextObject)
{
	UDynamicTexture2DArray* newDT2DA = NULL;
	if(!WorldContextObject)
	{
		UE_LOG(LogTemp, Error, TEXT("[CreateDynamicTexture2DArray] 错误！ Owner 所有者为空! 可能会导致UDynamicTexture2DArray无法被卸载，常驻内存中。"));
		WorldContextObject = GetTransientPackage();
	}
	newDT2DA = NewObject<UDynamicTexture2DArray>(WorldContextObject);
	auto systemTexRHIDesc = GWhiteTexture->GetTextureRHI()->GetDesc();
	newDT2DA->SRGB = GWhiteTexture->bSRGB;
	newDT2DA->TextureSize = systemTexRHIDesc.GetSize().X;
	newDT2DA->PixelFormat = systemTexRHIDesc.Format;
	newDT2DA->NumMips = systemTexRHIDesc.NumMips;
	newDT2DA->NumSlices = systemTexRHIDesc.ArraySize; 
	newDT2DA->UpdateResource();

	// Make sure any render commands are executed, in particular things like InitRHI, or asset updates on the render thread.
	FlushRenderingCommands();
	
	ENQUEUE_RENDER_COMMAND(CreateDynamicTexture2DArrayDefault)(
	[newDT2DA](FRHICommandListImmediate& RHICmdList)
	{
		newDT2DA->UpdateFromSourceTextures_RenderThread(RHICmdList,{GWhiteTexture});
	});
	return newDT2DA;
}

TArray<UDynamicTexture2DArray*> UDynamicTexture2DArray::GetDynamicTexture2DArrayMap()
{
	return DynamicTexture2DArrayMap;
}

FTextureResource* UDynamicTexture2DArray::CreateResource()
{
	if(GetResource() != nullptr)
	{
		auto currentRes = static_cast<FDynamicTexture2DArrayResource*>(GetResource());
		if(currentRes->GetSizeX() == TextureSize
			&& currentRes->GetPixelFormat() == PixelFormat
			&& currentRes->GetNumMips() == NumMips
			&& currentRes->GetNumSlices() == NumSlices
			&& currentRes->bSRGB == SRGB
			)
		{
			return GetResource();
		}
	}
	ReleaseResource();
	auto newResource = new FDynamicTexture2DArrayResource(this,TextureSize,PixelFormat,NumMips,NumSlices,SRGB);
	SetResource(newResource);
	return newResource;
}

void UDynamicTexture2DArray::ResetToNull()
{
	auto systemTexRHIDesc = GBlackTexture->GetTextureRHI()->GetDesc();
	SRGB = GBlackTexture->bSRGB;
	TextureSize = systemTexRHIDesc.GetSize().X;
	PixelFormat = systemTexRHIDesc.Format;
	NumMips = systemTexRHIDesc.NumMips;
	NumSlices = systemTexRHIDesc.ArraySize;
	SourceTextures.Empty();
	UpdateResource();
	ENQUEUE_RENDER_COMMAND(CreateDynamicTexture2DArrayDefault)(
	[this](FRHICommandListImmediate& RHICmdList)
	{
		UpdateFromSourceTextures_RenderThread(RHICmdList,{GBlackTexture});
	});
	
	for(auto& i : DynamicTexture2DArrayMap)
	{
		if(i == this)
		{
			DynamicTexture2DArrayMap.Remove(i);
			break;
		}
	}
}

void UDynamicTexture2DArray::UpdateResource()
{
	auto currentRes = static_cast<FDynamicTexture2DArrayResource*>(GetResource());
	if(GetResource() == nullptr
		|| PixelFormat	!= currentRes->GetPixelFormat()
		|| TextureSize	!= currentRes->GetSizeX()
		|| NumSlices	!= currentRes->GetNumSlices()
		|| NumMips		!= currentRes->GetNumMips()
		|| SRGB		!= currentRes->bSRGB
		|| bForceUpdate)
	{
		bForceUpdate = false;
		UTexture::UpdateResource();
		UpdateFromSourceTextures();
	}
}

void UDynamicTexture2DArray::ForceUpdateResource()
{
	bForceUpdate = true;
	UpdateResource();  
}

void UDynamicTexture2DArray::UpdateFromSourceTextures()
{
	// Make sure any render commands are executed, in particular things like InitRHI, or asset updates on the render thread.
	FlushRenderingCommands();
	
	ENQUEUE_RENDER_COMMAND(UpdateFromSourceTextures)(
[this](FRHICommandListImmediate& RHICmdList)
		{
			UpdateFromSourceTextures_RenderThread(RHICmdList);
		});
}

void UDynamicTexture2DArray::UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList)
{
	check(IsInRenderingThread());
	if(this->GetResource() == nullptr
	|| this->GetResource()->TextureRHI == nullptr
	|| SourceTextures.Num() <= 0)
	{
		return;
	}
	
	auto resource = static_cast<FDynamicTexture2DArrayResource*>(this->GetResource());
	auto DestinationTextureArrayRHI = resource->TextureRHI;
	{
		for (int32 ArrayIndex = 0; ArrayIndex < SourceTextures.Num(); ArrayIndex++)
		{
			auto& newSourceTexture = SourceTextures[ArrayIndex];
			for (uint32 MipIndex = 0; MipIndex < resource->GetNumMips(); MipIndex++)
			{	
				if(!newSourceTexture.Texture.IsValid())
				{
					continue;
				}
				{
					UTexture2D* srcTex = newSourceTexture.Texture.Get();
					auto SourceTextureRHI = srcTex->GetResource()->TextureRHI;
					if(!SourceTextureRHI.IsValid() || !DestinationTextureArrayRHI.IsValid())
					{
						continue;
					}
					FRHICopyTextureInfo CopyInfo={};
					CopyInfo.SourceMipIndex = MipIndex;
					//
					CopyInfo.DestMipIndex = MipIndex;
					//CopyInfo.DestSliceIndex = ArrayIndex;
					CopyInfo.DestSliceIndex = newSourceTexture.Index;
					//
					auto CopySize = SourceTextureRHI->GetMipDimensions(MipIndex);
					CopyInfo.Size = CopySize;
				
					TransitionAndCopyTexture(
						RHICmdList,
						SourceTextureRHI,
						DestinationTextureArrayRHI,
						CopyInfo);
					
					newSourceTexture.bExec = true;
				}
			}
		}
	}
	Async(EAsyncExecution::TaskGraphMainThread, [this]()
	{
		for(int32 i = 0 ; i < SourceTextures.Num() ; i++)
		{
			//重置,开启原图的Texture Streaming
			if(SourceTextures[i].Texture && SourceTextures[i].bExec)
			{
				SourceTextures[i].Texture->NeverStream = 0;
				SourceTextures[i].Texture->UpdateResource();
				SourceTextures.RemoveAt(i);
				i = i - 1;
			}
		}
	});
}

void UDynamicTexture2DArray::UpdateFromSourceTextures_RenderThread(FRHICommandListImmediate& RHICmdList,TArray<FTexture*> FTextures)
{
	check(IsInRenderingThread());
	
	if(this->GetResource() == nullptr
	|| this->GetResource()->TextureRHI == nullptr
	|| FTextures.Num() <= 0)
	{
		return;
	}
	auto resource = static_cast<FDynamicTexture2DArrayResource*>(this->GetResource());
	auto DestinationTextureArrayRHI = resource->TextureRHI;
	for (int32 ArrayIndex = 0; ArrayIndex < FTextures.Num(); ArrayIndex++)
	{
		for (uint32 MipIndex = 0; MipIndex < resource->GetNumMips(); MipIndex++)
		{	
			if(!FTextures[ArrayIndex] && !FTextures[ArrayIndex]->GetTextureRHI().IsValid())
			{
				continue;
			}
			{
				auto SourceTextureRHI = FTextures[ArrayIndex]->GetTextureRHI();
				
				FRHICopyTextureInfo CopyInfo={};
				CopyInfo.SourceMipIndex = MipIndex;
				//
				CopyInfo.DestMipIndex = MipIndex;
				CopyInfo.DestSliceIndex = ArrayIndex;
				//
				auto CopySize = SourceTextureRHI->GetMipDimensions(MipIndex);
				CopyInfo.Size = CopySize;
				
				TransitionAndCopyTexture(
					RHICmdList,
					SourceTextureRHI,
					DestinationTextureArrayRHI,
					CopyInfo);
			}
		}
	}
	Async(EAsyncExecution::TaskGraphMainThread, [this]()
	{
		for(int32 i = 0 ; i < SourceTextures.Num() ; i++)
		{
			//重置,开启原图的Texture Streaming
			if(SourceTextures[i].Texture && SourceTextures[i].bExec)
			{
				SourceTextures[i].Texture->NeverStream = 0;
				SourceTextures[i].Texture->UpdateResource();
				SourceTextures[i].Texture = nullptr;//删除引用
				SourceTextures.RemoveAt(i);
				i = FMath::Max(0, i - 1);
			}
		}
	});
}

FDynamicTexture2DArrayResource::FDynamicTexture2DArrayResource(UDynamicTexture2DArray* InOwner , uint32 InSizeXY, EPixelFormat InFormat, uint32 InNumMips,
                                                               uint32 InNumSlices, bool InbSRGB)
{
	SizeXY = InSizeXY;
	Format = InFormat;
	NumMips = InNumMips;
	NumSlices = InNumSlices;
	Filter = ESamplerFilter::SF_Bilinear;
	SamplerAddress = ESamplerAddressMode::AM_Wrap;
	bSRGB = InbSRGB;
    Owner = InOwner;
}

//
void FDynamicTexture2DArrayResource::InitRHI(FRHICommandListBase& RHICmdList)
{
	FTextureResource::InitRHI(RHICmdList);
	//
	//
	FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2DArray(TEXT("FDynamicTexture2DArrayResource"),SizeXY,SizeXY,NumSlices,Format)
	.SetNumMips(NumMips)
	.SetFlags((bSRGB ? ETextureCreateFlags::SRGB : ETextureCreateFlags::None)  | ETextureCreateFlags::OfflineProcessed | ETextureCreateFlags::ShaderResource);
	
	TextureRHI = RHICreateTexture(Desc);

	if (Owner)
	{
		RHIBindDebugLabelName(TextureRHI, *Owner->GetName());
		RHIUpdateTextureReference(Owner->TextureReference.TextureReferenceRHI, TextureRHI);
	}
	
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

#undef LOCTEXT_NAMESPACE