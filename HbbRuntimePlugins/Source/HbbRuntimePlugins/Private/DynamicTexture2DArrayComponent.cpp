// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTexture2DArrayComponent.h"

// Sets default values for this component's properties
UDynamicTexture2DArrayComponent::UDynamicTexture2DArrayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	TextureSize = 1024;
}

// Called when the game starts
void UDynamicTexture2DArrayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(TextureArray==nullptr)
		TextureArray = NewObject<UDynamicTexture2DArray>(this,NAME_None, RF_Public);
	TextureArray->CreateResource(SourceTextures);
}


// Called every frame
void UDynamicTexture2DArrayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDynamicTexture2DArrayComponent::SetSourceTextures(TArray<UTexture2D*> NewSourceTextures)
{
	const auto newArraySize = NewSourceTextures.Num();
	if(newArraySize>0 && NewSourceTextures[0])
	{
		SourceTextures.Reserve(newArraySize);
		for(int i = 0 ;i < newArraySize;i++)
		{
			if(NewSourceTextures[i] && SourceTextures.IsValidIndex(i))
			{
				SourceTextures[i] = NewSourceTextures[i];
			}
		}
		TextureArray->UpdateFromSourceTextures(SourceTextures);
	}
}
