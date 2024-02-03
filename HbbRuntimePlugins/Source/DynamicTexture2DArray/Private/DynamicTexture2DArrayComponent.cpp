// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTexture2DArrayComponent.h"

// Sets default values for this component's properties
UDynamicTexture2DArrayComponent::UDynamicTexture2DArrayComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UDynamicTexture2DArrayComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	if(SourceTextures.Num()>0 && TextureArray==nullptr)
	{
		TextureArray = UDynamicTexture2DArray::CreateDynamicTexture2DArray(this,{});
		
		//UpdateTextureArrayFromSourceTextures(); 
	}
}

// Called every frame
// void UDynamicTexture2DArrayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// }

void UDynamicTexture2DArrayComponent::UpdateSourceTextures(TArray<TSoftObjectPtr<UTexture2D>> NewSourceTextures)
{
	const auto newArraySize = NewSourceTextures.Num();
	if(newArraySize>0 && TextureArray)
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
			if(i>0)
			{
				if(tex == nullptr)
				{
					tex = NewSourceTextures[i].LoadSynchronous();
				}
				auto currentPlatformData = tex->GetPlatformData();
				//纹理格式不一致,不允许在一起
				if(firstPlatformData)
				{
					if(tex->GetMaterialType() != MCT_Texture2D )
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:需要添加到DynamicTexture2DArray的纹理类型不是一张Texture2D,请确认纹理类型无误,这张纹理是:%s")
							, *this->GetOwner()->GetActorNameOrLabel(),  *tex->GetPathName());
						continue;
					}
					else if(firstPlatformData->SizeX != currentPlatformData->SizeX ||	firstPlatformData->SizeY != currentPlatformData->SizeY)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的分辨率不一致!无法添加进DynamicTexture2DArray,指定纹理大小是%d,%d,但是当前纹理大小是%d,%d,这张纹理是:%s")
							, *this->GetOwner()->GetActorNameOrLabel(), firstPlatformData->SizeX , firstPlatformData->SizeY ,currentPlatformData->SizeX , currentPlatformData->SizeY,  *tex->GetPathName());
						continue;
					}
					else if(firstPlatformData->PixelFormat != currentPlatformData->PixelFormat)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的格式(PixelFormat)不一致!无法添加进DynamicTexture2DArray,指定格式是%s,但是当前纹理是%s,(请查看一下纹理的压缩格式或者是否勾选了Compress without alpha)这张纹理是:%s")
							, *this->GetOwner()->GetActorNameOrLabel() , GPixelFormats[firstPlatformData->PixelFormat].Name , GPixelFormats[currentPlatformData->PixelFormat].Name, *tex->GetPathName());
						continue;
					}
					else if(firstTemplate->GetNumMips() != tex->GetNumMips() )
					{
						UE_LOG(LogTemp, Warning, TEXT("%s:纹理的Mip层数不一致!添加到DynamicTexture2DArray失败,指定Mip层数是%d,但是当前Mip层数是%d,这张纹理是:%s")
							, *this->GetOwner()->GetActorNameOrLabel() , firstTemplate->GetNumMips() , tex->GetNumMips() , *tex->GetPathName());
						continue;
					}
				}
			}
			validTextures.Add(tex);
		}
		
		SourceTextures.Empty();
		SourceTextures = validTextures;
		TextureArray->SetSourceTextures(SourceTextures);
	}
}

void UDynamicTexture2DArrayComponent::UpdateSourceTexture(TSoftObjectPtr<UTexture2D> NewSourceTexture, int32 index )
{
	if(SourceTextures.Num()>0 && TextureArray)
	{
		if(SourceTextures.IsValidIndex(index))
		{
			SourceTextures[index] = NewSourceTexture;
			TextureArray->SetSourceTexture(SourceTextures[index],index);
		}
	}
}

void UDynamicTexture2DArrayComponent::UpdateTextureArrayFromSourceTextures()
{
	UpdateSourceTextures(SourceTextures);
	//TextureArray->UpdateFromSourceTextures(-1);
}
