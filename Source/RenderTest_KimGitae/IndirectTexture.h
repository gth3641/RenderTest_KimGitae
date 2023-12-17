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
	//�ؽ�ó ������
	UPROPERTY(EditAnywhere, Category = "Tile Texture")
		UTexture2D* _texture;

	//Ÿ�� ũ��
	UPROPERTY(EditAnywhere, Category = "Tile Count")
		FIntPoint _tileCount;

	//�ؽ�ó ũ��
	UPROPERTY(EditAnywhere, Category = "Texture Rsolution")
		FIntPoint _textureResolution;

	//����ȭ �õ�
	UPROPERTY(EditAnywhere, Category = "RandomSeed")
		float _salt;

};
