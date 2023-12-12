// Fill out your copyright notice in the Description page of Project Settings.


#include "MatExprIndirTexSample.h"
#include "MaterialCompiler.h"

/// <summary>
/// HLSL �ڵ��Դϴ�.
/// �����κ��� �ִ��� ����ȭ���Ѽ� ��������ϴ�.
/// </summary>
/// <param name="Compiler"></param>
/// <param name="OutputIndex"></param>
/// <returns></returns>
int32 UMatExprIndirTexSample::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	//UV
	int32 UVcoord = Compiler->TextureCoordinate(0, false, false);

	//�ؽ�ó ��Ƽ���� �߰�
	int32 texureRef;
	int32 texure = Compiler->Texture(
		_indirectTexture->_texture,
		texureRef,
		EMaterialSamplerType::SAMPLERTYPE_Color
	);

	//���� �ؽ�ó�� ������
	int32 TexResolX = Compiler->Constant(_indirectTexture->TextureResolution.X);
	int32 TexResolY = Compiler->Constant(_indirectTexture->TextureResolution.Y);
	int32 TexResol = Compiler->Mul(TexResolX, TexResolY);

	//Ÿ�� �ؽ�ó�� ������
	int32 tileCountX = Compiler->Constant(_indirectTexture->_tileCount.X);
	int32 tileCountY = Compiler->Constant(_indirectTexture->_tileCount.Y);
	int32 tileCount = Compiler->Mul(tileCountX, tileCountY);

	//UV��ǥ �ε���ȭ
	int32 CoordX = Compiler->Mul(Compiler->ComponentMask(UVcoord, true, false, false, false), TexResolX);
	CoordX = Compiler->Floor(CoordX);
	int32 CoordY = Compiler->Mul(Compiler->ComponentMask(UVcoord, false, true, false, false), TexResolY);
	CoordY = Compiler->Floor(CoordY);

	//����� ���� ����ȭ -> (_indirectTexture->salt)�� �õ� ������ ���
	//���� ������ �ִ��� ������ ����
	int32 IndexMap = Compiler->Mul(CoordY, TexResolX);
	IndexMap = Compiler->Add(IndexMap, CoordX);
	IndexMap = Compiler->Mul(IndexMap, IndexMap);									//�ε��� ����
	IndexMap = Compiler->Add(IndexMap, Compiler->Constant(20));
	IndexMap = Compiler->Fmod(IndexMap, Compiler->Constant(_indirectTexture->salt));//����ȭ �õ� ��
	IndexMap = Compiler->Frac(IndexMap);											//Frac �Լ� ������� ����ȭ
	IndexMap = Compiler->Mul(IndexMap, Compiler->Constant(1000));
	IndexMap = Compiler->Fmod(IndexMap, tileCount);

	//UV��ǥ
	int32 crdX = Compiler->ComponentMask(UVcoord, true, false, false, false);
	int32 crdY = Compiler->ComponentMask(UVcoord, false, true, false, false);

	//���� �ʰ� UV ��ǥ ����
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

	//���
	int32 textureSample = Compiler->TextureSample(
		texure,
		result,
		EMaterialSamplerType::SAMPLERTYPE_Color
	);

	return textureSample;
}
