// Fill out your copyright notice in the Description page of Project Settings.

#include "HbbRuntimeBPFunctionLibrary.h"

#include "AssetExportTask.h"
#include "AssetToolsModule.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "MaterialDomain.h"
#include "ObjectTools.h"
#include "PixelShaderUtils.h"
#include "RenderGraphBuilder.h"
#include "ScreenPass.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Exporters/Exporter.h"
#include "MaterialEditor/MaterialEditorInstanceConstant.h"
#include "Materials/MaterialInstanceConstant.h"
#include "UObject/GCObjectScopeGuard.h"
#define LOCTEXT_NAMESPACE "HbbRuntimeBPFunctionLibrary"

FMaterilInsStaticSwitchParameter UHbbRuntimeBPFunctionLibrary::GetMaterialInstanceStaticSwitchParameter(UObject* WorldContextObject,UMaterialInstanceConstant* materialIns,
	FName StaticParameterName)
{
	FMaterilInsStaticSwitchParameter result={};
#if WITH_EDITOR
	if(materialIns == nullptr)
	{
		return result;
	}
	FStaticParameterSet staticParameters = materialIns->GetStaticParameters();
	for(int i = 0; i < staticParameters.StaticSwitchParameters.Num();i++)
	{
		if(staticParameters.StaticSwitchParameters[i].ParameterInfo.Name.IsEqual(StaticParameterName))
		{
			result.bOverride = staticParameters.StaticSwitchParameters[i].bOverride;
			result.Value = staticParameters.StaticSwitchParameters[i].Value;
			result.Name = staticParameters.StaticSwitchParameters[i].ParameterInfo.Name;
			break;
		}
	}
#endif
	return result;
}

void UHbbRuntimeBPFunctionLibrary::SetMaterialInstanceStaticSwitchParameter(UObject* WorldContextObject,UMaterialInstanceConstant* materialIns,
																		   FName StaticParameterName, bool Value)
{
#if WITH_EDITOR
	if(materialIns == nullptr)
	{
		return;
	}
	FStaticParameterSet staticParameters = materialIns->GetStaticParameters();
	for(int i = 0; i < staticParameters.StaticSwitchParameters.Num();i++)
	{
		if(staticParameters.StaticSwitchParameters[i].ParameterInfo.Name.IsEqual(StaticParameterName))
		{
			staticParameters.StaticSwitchParameters[i].Value = Value;
			break;
		}
	}
	materialIns->UpdateStaticPermutation(staticParameters);
	materialIns->InitStaticPermutation();
	materialIns->PostEditChange();
	materialIns->MarkPackageDirty();
#endif
}

FMaterilInsBlendMode UHbbRuntimeBPFunctionLibrary::GetMaterialInstanceBlendMode(UObject* WorldContextObject,
	UMaterialInstanceConstant* materialIns)
{
	FMaterilInsBlendMode result = {};
#if WITH_EDITOR
	if(materialIns == nullptr)
	{
		return result;
	}
	result.bOverride = materialIns->BasePropertyOverrides.bOverride_BlendMode;
	result.Value = materialIns->BasePropertyOverrides.BlendMode;
#endif
	return result;
}

void UHbbRuntimeBPFunctionLibrary::SetMaterialInstanceBlendMode(UObject* WorldContextObject,UMaterialInstanceConstant* materialIns,
															   TEnumAsByte<EBlendMode> BlendMode)
{
#if WITH_EDITOR
	if(materialIns == nullptr)
	{
		return;
	}
	materialIns->BasePropertyOverrides.bOverride_BlendMode = true;
	materialIns->BasePropertyOverrides.BlendMode = BlendMode;
	materialIns->UpdateOverridableBaseProperties();
	materialIns->PostEditChange();
	materialIns->MarkPackageDirty();
#endif
}


void UHbbRuntimeBPFunctionLibrary::DrawMaterialtoRenderTargetWithAlpha(UObject* WorldContextObject,
	UTextureRenderTarget2D* TextureRenderTarget, UMaterialInterface* Material)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(DrawMaterialToRenderTarget);

	//把母材质暂时切换成Postprocess
	auto oldDomian = Material->GetMaterial()->MaterialDomain;
	auto oldOutputAlpha = Material->GetMaterial()->BlendableOutputAlpha;
	
	Material->GetMaterial()->MaterialDomain = EMaterialDomain::MD_PostProcess;
	Material->GetMaterial()->BlendableOutputAlpha = 1;
	Material->GetMaterial()->PostEditChange();
	//Material->GetMaterial()->MarkPackageDirty();
	
	Material->GetMaterial()->EnsureIsComplete();
	Material->EnsureIsComplete();
	
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!World)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("DrawMaterialToRenderTarget_InvalidWorldContextObject", "DrawMaterialToRenderTarget: WorldContextObject is not valid."));
	}
	else if (!Material)
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("DrawMaterialToRenderTarget_InvalidMaterial", "DrawMaterialToRenderTarget[{0}]: Material must be non-null."), FText::FromString(GetPathNameSafe(WorldContextObject))));
	}
	else if (!TextureRenderTarget)
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("DrawMaterialToRenderTarget_InvalidTextureRenderTarget", "DrawMaterialToRenderTarget[{0}]: TextureRenderTarget must be non-null."), FText::FromString(GetPathNameSafe(WorldContextObject))));
	}
	else if (!TextureRenderTarget->GetResource())
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("DrawMaterialToRenderTarget_ReleasedTextureRenderTarget", "DrawMaterialToRenderTarget[{0}]: render target has been released."), FText::FromString(GetPathNameSafe(WorldContextObject))));
	}
	else
	{
		Material->EnsureIsComplete();
		
		World->FlushDeferredParameterCollectionInstanceUpdates();

		FTextureRenderTargetResource* RenderTargetResource = TextureRenderTarget->GameThread_GetRenderTargetResource();

		UCanvas* Canvas = World->GetCanvasForDrawMaterialToRenderTarget();

		FCanvas RenderCanvas(
			RenderTargetResource, 
			nullptr, 
			World,
			World->GetFeatureLevel());

		Canvas->Init(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, nullptr, &RenderCanvas);

		{
			SCOPED_DRAW_EVENTF_GAMETHREAD(DrawMaterialToRenderTarget, *TextureRenderTarget->GetFName().ToString());

			ENQUEUE_RENDER_COMMAND(FlushDeferredResourceUpdateCommand)(
				[RenderTargetResource](FRHICommandListImmediate& RHICmdList)
				{
					RenderTargetResource->FlushDeferredResourceUpdate(RHICmdList);
				});
			auto screenSize = FVector2D(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY);
			
			if (Material 
			&& TextureRenderTarget->SizeX > 0.0f 
			&& TextureRenderTarget->SizeY > 0.0f 
			// Canvas can be NULL if the user tried to draw after EndDrawCanvasToRenderTarget
			&& Canvas)
				{
					// This is a user-facing function, so we'd rather make sure that shaders are ready by the time we render, in order to ensure we don't draw with a fallback material :
					Material->EnsureIsComplete();
					FCanvasTileItem TileItem(FVector2D(0, 0), Material->GetRenderProxy(), screenSize, FVector2D(0, 0), FVector2D(0, 0) + FVector2D::UnitVector);
					TileItem.Rotation = FRotator(0, 0.f, 0);
					TileItem.PivotPoint = FVector2D(0.5f,0.5f);
					TileItem.SetColor(Canvas->DrawColor);
					Canvas->DrawItem(TileItem);
				}

			
			RenderCanvas.Flush_GameThread();
			Canvas->Canvas = nullptr;

			//UpdateResourceImmediate must be called here to ensure mips are generated.
			TextureRenderTarget->UpdateResourceImmediate(false);

			ENQUEUE_RENDER_COMMAND(ResetSceneTextureExtentHistory)(
				[RenderTargetResource](FRHICommandListImmediate& RHICmdList)
				{
					RenderTargetResource->ResetSceneTextureExtentsHistory();
				});
		}
		Material->GetMaterial()->BlendableOutputAlpha = oldOutputAlpha;
		Material->GetMaterial()->EnsureIsComplete();
		Material->EnsureIsComplete();
		Material->GetMaterial()->MaterialDomain = oldDomian;
		Material->GetMaterial()->PostEditChange();
		//Material->GetMaterial()->MarkPackageDirty();
		
	}
}

void UHbbRuntimeBPFunctionLibrary::ExportAssetFile(UObject* WorldContextObject, UObject* object, FString outputPath)
{
#if WITH_EDITOR
	if(object == nullptr)
	{
		return ;
	}
	{
		//获取所有导出器
		TArray<UExporter*> Exporters;
		ObjectTools::AssembleListOfExporters(Exporters);
		//批处理导出器记录
		UExporter* BatchExporters = nullptr;
		for (int32 ExporterIndex = Exporters.Num() - 1; ExporterIndex >= 0; --ExporterIndex)
		{
			UExporter* Exporter = Exporters[ExporterIndex];
			if (Exporter->SupportedClass)
			{
				const bool bObjectIsSupported = Exporter->SupportsObject(object);
				if (bObjectIsSupported)
				{
					check(Exporter->FormatExtension.Num() == Exporter->FormatDescription.Num());
					check(Exporter->FormatExtension.IsValidIndex(Exporter->PreferredFormatIndex));
					//看看是不是我们需要的格式的导出器
					for (int32 FormatIndex = 0; FormatIndex < Exporter->FormatExtension.Num(); ++FormatIndex)
					{
						const FString& FormatExtension = Exporter->FormatExtension[FormatIndex];
						if (FormatExtension.Equals(FPaths::GetExtension(outputPath),ESearchCase::IgnoreCase))
						{
							BatchExporters = Exporter;
							break;
						}
					}
				}
			}
		}

		if(BatchExporters != nullptr && BatchExporters->SupportsObject(object))
		{
			UAssetExportTask* ExportTask = NewObject<UAssetExportTask>();
			FGCObjectScopeGuard ExportTaskGuard(ExportTask);
			ExportTask->Object = object;
			ExportTask->Exporter = BatchExporters;
			ExportTask->Filename = outputPath;
			ExportTask->bSelected = false;
			ExportTask->bReplaceIdentical = true;
			ExportTask->bPrompt = false;
			ExportTask->bUseFileArchive = ExportTask->IsA(UPackage::StaticClass());
			ExportTask->bWriteEmptyFiles = false;
			UExporter::RunAssetExportTask(ExportTask);
		}
		
	}


#endif
}

void UHbbRuntimeBPFunctionLibrary::SetMaterialDomain(UMaterialInstanceConstant* materialIns,
	TEnumAsByte<EMaterialDomain> Domain, bool bMarkDirt)
{
#if WITH_EDITOR
	materialIns->GetMaterial()->EnsureIsComplete();
	materialIns->EnsureIsComplete();
	materialIns->GetMaterial()->MaterialDomain = Domain;
	materialIns->GetMaterial()->PostEditChange();
	if(bMarkDirt)
	{
		materialIns->GetMaterial()->MarkPackageDirty();
	}
#endif
}

#undef LOCTEXT_NAMESPACE

