// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "IndirectTexture.h"
#include "VirtualTextureActor.generated.h"

UCLASS()
class RENDERTEST_KIMGITAE_API AVirtualTextureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVirtualTextureActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		UIndirectTexture* _indirectTexture;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	UMaterialInterface* _baseMaterial;

private:
	void createIndirectTexture();
	UTexture2D* _dynamicTexture;
};
