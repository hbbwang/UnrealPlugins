// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicTexture2DArray.h"
#include "Components/ActorComponent.h"
#include "DynamicTexture2DArrayComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HBBRUNTIMEPLUGINS_API UDynamicTexture2DArrayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicTexture2DArrayComponent();

	UFUNCTION(BlueprintCallable)
	UDynamicTexture2DArray* GetTextureArray()const{return TextureArray;}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DynamicTexture2DArray")
	int32 TextureSize;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DynamicTexture2DArray")
	TArray<TSoftObjectPtr<UTexture2D>> SourceTextures;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
    UFUNCTION(BlueprintCallable)
    void SetSourceTextures(TArray<UTexture2D*> NewSourceTextures);

	UFUNCTION(BlueprintCallable)
	void UpdateTextureArrayFromSourceTextures();
	
private:

	UPROPERTY()
	UDynamicTexture2DArray* TextureArray;
	
};
