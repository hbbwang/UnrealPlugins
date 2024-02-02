// Fill out your copyright notice in the Description page of Project Settings.


#include "HbbRuntimeBPFunctionLibrary.h"
#include "DynamicTexture2DArray.h"

UDynamicTexture2DArray* UHbbRuntimeBPFunctionLibrary::CreateDynamicTexture2DArray(UObject* Owner)
{
	if(!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("[CreateDynamicTexture2DArray] 错误！ Owner 所有者为空! 可能会导致UDynamicTexture2DArray无法被卸载，常驻内存中。"));
		Owner = GetTransientPackage();
	}
	auto newDT2DA = NewObject<UDynamicTexture2DArray>(Owner,NAME_None, RF_Public|RF_Transient);
	auto systemTexRHIDesc = GWhiteTexture->GetTextureRHI()->GetDesc();
	newDT2DA->SRGB = GWhiteTexture->bSRGB;
	newDT2DA->TextureSize = systemTexRHIDesc.GetSize().X;
	newDT2DA->PixelFormat = systemTexRHIDesc.Format;
	newDT2DA->NumMips = systemTexRHIDesc.NumMips;
	newDT2DA->NumSlices = systemTexRHIDesc.ArraySize; 

	newDT2DA->UpdateResource();

	ENQUEUE_RENDER_COMMAND(UpdateDynamicTexture2DArray)(
	[newDT2DA](FRHICommandListImmediate& RHICmdList)
	{
		newDT2DA->UpdateFromSourceTextures_RenderThread(RHICmdList,{GWhiteTexture});
	});
	
	return newDT2DA;
}
