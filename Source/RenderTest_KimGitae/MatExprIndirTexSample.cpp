// Fill out your copyright notice in the Description page of Project Settings.


#include "MatExprIndirTexSample.h"
#include "MaterialCompiler.h"

/// <summary>
/// HLSL 코드입니다.
/// 랜덤부분은 최대한 간소화시켜서 만들었습니다.
/// </summary>
/// <param name="Compiler"></param>
/// <param name="OutputIndex"></param>
/// <returns></returns>
int32 UMatExprIndirTexSample::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	//UV
	int32 UVcoord = Compiler->TextureCoordinate(0, false, false);

	//텍스처 머티리얼에 추가
	int32 texureRef;
	int32 texure = Compiler->Texture(
		_indirectTexture->_texture,
		texureRef,
		EMaterialSamplerType::SAMPLERTYPE_Color
	);

	//가상 텍스처의 사이즈
	int32 TexResolX = Compiler->Constant(_indirectTexture->TextureResolution.X);
	int32 TexResolY = Compiler->Constant(_indirectTexture->TextureResolution.Y);
	int32 TexResol = Compiler->Mul(TexResolX, TexResolY);

	//타일 텍스처의 사이즈
	int32 tileCountX = Compiler->Constant(_indirectTexture->_tileCount.X);
	int32 tileCountY = Compiler->Constant(_indirectTexture->_tileCount.Y);
	int32 tileCount = Compiler->Mul(tileCountX, tileCountY);

	//UV좌표 인덱스화
	int32 CoordX = Compiler->Mul(Compiler->ComponentMask(UVcoord, true, false, false, false), TexResolX);
	CoordX = Compiler->Floor(CoordX);
	int32 CoordY = Compiler->Mul(Compiler->ComponentMask(UVcoord, false, true, false, false), TexResolY);
	CoordY = Compiler->Floor(CoordY);

	//사용할 맵의 랜덤화 -> (_indirectTexture->salt)를 시드 값으로 사용
	//랜덤 수준은 최대한 가볍게 진행
	int32 IndexMap = Compiler->Mul(CoordY, TexResolX);
	IndexMap = Compiler->Add(IndexMap, CoordX);
	IndexMap = Compiler->Mul(IndexMap, IndexMap);									//인덱스 제곱
	IndexMap = Compiler->Add(IndexMap, Compiler->Constant(20));
	IndexMap = Compiler->Fmod(IndexMap, Compiler->Constant(_indirectTexture->salt));//랜덤화 시드 값
	IndexMap = Compiler->Frac(IndexMap);											//Frac 함수 사용으로 난수화
	IndexMap = Compiler->Mul(IndexMap, Compiler->Constant(1000));
	IndexMap = Compiler->Fmod(IndexMap, tileCount);

	//UV좌표
	int32 crdX = Compiler->ComponentMask(UVcoord, true, false, false, false);
	int32 crdY = Compiler->ComponentMask(UVcoord, false, true, false, false);

	//랜덤 맵과 UV 좌표 결합
	int32 mapX = Compiler->Floor(Compiler->Fmod(IndexMap, tileCountX));
	mapX = Compiler->Div(mapX, TexResolX);

	crdX = Compiler->Fmod(crdX, Compiler->Div(Compiler->Constant(1.0f), TexResolX));
	crdX = Compiler->Add(crdX, mapX);
	crdX = Compiler->Mul(crdX, Compiler->Div(TexResolX, tileCountX));

	int32 mapY = Compiler->Floor(Compiler->Div(IndexMap, tileCountX));
	mapY = Compiler->Div(mapY, TexResolY);

	crdY = Compiler->Fmod(crdY, Compiler->Div(Compiler->Constant(1.0f), TexResolY));
	crdY = Compiler->Add(crdY, mapY);
	crdY = Compiler->Mul(crdY, Compiler->Div(TexResolY, tileCountY));

	int32 result = Compiler->AppendVector(crdX, crdY);

	//출력
	int32 textureSample = Compiler->TextureSample(
		texure,
		result,
		EMaterialSamplerType::SAMPLERTYPE_Color
	);

	return textureSample;
}
