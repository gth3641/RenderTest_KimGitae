// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualTextureActor.h"

// Sets default values
AVirtualTextureActor::AVirtualTextureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshComponent"));
    RootComponent = StaticMeshComponent;

    StaticMeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube")));

    _baseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/IndirectMat"));
    

}

// Called when the game starts or when spawned
void AVirtualTextureActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//For check on Editor
void AVirtualTextureActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (_baseMaterial)
    {
        if (!MaterialInstance)
        {
            MaterialInstance = UMaterialInstanceDynamic::Create(_baseMaterial, this);
            StaticMeshComponent->SetMaterial(0, MaterialInstance);
        }

        createIndirectTexture();

        if (_dynamicTexture)
        {
            MaterialInstance->SetTextureParameterValue(FName("IndexTexture"), _dynamicTexture);

            MaterialInstance->SetVectorParameterValue(FName("TileCount"),
                FLinearColor(_indirectTexture->_tileCount.X, _indirectTexture->_tileCount.Y, 0, 0));

            MaterialInstance->SetVectorParameterValue(FName("TextureResolution"),
                FLinearColor(_indirectTexture->_textureResolution.X, _indirectTexture->_textureResolution.Y, 0, 0));
            
            MaterialInstance->SetTextureParameterValue(FName("TileTexture"), _indirectTexture->_texture);
            
            MaterialInstance->PostEditChange();
        }
    }
}

// Called every frame
void AVirtualTextureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// 간접 텍스처를 만듭니다.
/// UTexture2D* _dynamicTexture에 결과를 저장하고 이미 있다면 할당 해제 후 재할당합니다.
/// </summary>
void AVirtualTextureActor::createIndirectTexture()
{
    if (!_indirectTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("createIndirectTexture: _indirectTexture is nullptr"));
        return;
    }
    if (!_indirectTexture->_texture) 
    {
        UE_LOG(LogTemp, Warning, TEXT("createIndirectTexture: _indirectTexture's _texture is nullptr"));
        return;
    }

    if (_dynamicTexture) _dynamicTexture->ReleaseResource();

    //get Indirect Dummy Texture
    UTexture* baseMatOrgTexture = nullptr;
    _baseMaterial->GetTextureParameterValue(FName("IndexTexture"), baseMatOrgTexture);

    if (!baseMatOrgTexture) 
    {
        UE_LOG(LogTemp, Warning, TEXT("createIndirectTexture: Dummy texture not found"));
        return;
    }

    //Convert UTexture to UTexture2D
    UTexture2D* OrgTexture2D = Cast<UTexture2D>(baseMatOrgTexture);

    if (!OrgTexture2D)
    {
        UE_LOG(LogTemp, Warning, TEXT("createIndirectTexture: Texture casting failed"));
        return;
    }

    int32 texSizeX = _indirectTexture->_textureResolution.X;
    int32 texSizeY = _indirectTexture->_textureResolution.Y;

    int32 tileCntX = _indirectTexture->_tileCount.X;
    int32 tileCntY = _indirectTexture->_tileCount.Y;


    //Create dynamic texture
    _dynamicTexture = UTexture2D::CreateTransient(
        texSizeX,
        texSizeY,
        OrgTexture2D->GetPixelFormat()
    );

    //Set indices
    FTexture2DMipMap& mipmap = _dynamicTexture->PlatformData->Mips[0];
    void* data = mipmap.BulkData.Lock(LOCK_READ_WRITE);
    FColor* colorData = static_cast<FColor*>(data);

    for (int y = 0; y < texSizeY; y++) {
        for (int x = 0; x < texSizeX; x++) {
            int getRand = FMath::RandRange(0, tileCntX * tileCntY - 1);

            int idxX = getRand % tileCntX;
            int idxY = getRand / tileCntX;
            colorData[y * texSizeX + x] = FColor(idxX, idxY, 0, 2);
        }
    }
    mipmap.BulkData.Unlock();

    _dynamicTexture->Filter = TF_Nearest;
    _dynamicTexture->SRGB = OrgTexture2D->SRGB;
    _dynamicTexture->UpdateResource();

}

