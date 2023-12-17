// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "IndirectTexture.h"
#include "MatExprIndirTexSample.generated.h"

/**
 * 
 */
UCLASS()
class RENDERTEST_KIMGITAE_API UMatExprIndirTexSample : public UMaterialExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UIndirectTexture* _indirectTexture;

	UPROPERTY(EditAnywhere, Category = "Index Texture")
		UTexture2D* _indexTexture;

	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Index Texture"))
		FExpressionInput IndexTextureInput;

	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Tile Count"))
		FExpressionInput TileCntInput;

	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Texture Resolution"))
		FExpressionInput TexResolInput;

public:

	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;

	virtual UObject* GetReferencedTexture() const override
	{
		if (_indirectTexture != nullptr && _indirectTexture->_texture != nullptr)
		{
			return _indirectTexture->_texture;
		}
		return nullptr;
	}

	virtual bool CanReferenceTexture() const override { return true; }
};
