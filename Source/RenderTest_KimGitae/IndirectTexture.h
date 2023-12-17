// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IndirectTexture.generated.h"

/**
 * 
 */
UCLASS()
class RENDERTEST_KIMGITAE_API UIndirectTexture : public UDataAsset
{
	GENERATED_BODY()
public:
	//텍스처 참조값
	UPROPERTY(EditAnywhere, Category = "Tile Texture")
		UTexture2D* _texture;

	//타일 크기
	UPROPERTY(EditAnywhere, Category = "Tile Count")
		FIntPoint _tileCount;

	//텍스처 크기
	UPROPERTY(EditAnywhere, Category = "Texture Rsolution")
		FIntPoint _textureResolution;

	//랜덤화 시드
	UPROPERTY(EditAnywhere, Category = "RandomSeed")
		float _salt;

};
