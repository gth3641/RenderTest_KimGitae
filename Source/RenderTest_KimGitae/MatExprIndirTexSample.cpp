// Fill out your copyright notice in the Description page of Project Settings.


#include "MatExprIndirTexSample.h"
#include "MaterialCompiler.h"

/// <summary>
/// Dynamic Texture와 Tile Count, Texture resolution 정보를 받아
/// 최종 UV좌표를 반환하는 표현식입니다.
/// </summary>
/// <param name="Compiler"></param>
/// <param name="OutputIndex"></param>
/// <returns></returns>
int32 UMatExprIndirTexSample::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	int32 InputColor = IndexTextureInput.Compile(Compiler);
	if (InputColor == INDEX_NONE) return Compiler->Errorf(TEXT("Unlink texture input"));

	int32 TileCnt = TileCntInput.Compile(Compiler);
	if (TileCnt == INDEX_NONE) return Compiler->Errorf(TEXT("Unlink tile count input"));

	int32 TexResol = TexResolInput.Compile(Compiler);
	if (TexResol == INDEX_NONE) return Compiler->Errorf(TEXT("Unlink texture resolution input"));

	//UV coordinate
	int32 UVcoord = Compiler->TextureCoordinate(0, false, false);
	int32 UVcoordX = Compiler->ComponentMask(UVcoord, true, false, false, false);
	int32 UVcoordY = Compiler->ComponentMask(UVcoord, false, true, false, false);

	//Tile Count
	int32 TileCntX = Compiler->ComponentMask(TileCnt, true, false, false, false);
	int32 TileCntY = Compiler->ComponentMask(TileCnt, false, true, false, false);

	//Indirect Texture resolution
	int32 TexResolX = Compiler->ComponentMask(TexResol, true, false, false, false);
	int32 TexResolY = Compiler->ComponentMask(TexResol, false, true, false, false);

	//Indirect Texture color data
	int32 colorX = Compiler->ComponentMask(InputColor, true, false, false, false);
	int32 colorY = Compiler->ComponentMask(InputColor, false, true, false, false);

	//UV coordinate Slicing
	UVcoordX = Compiler->Fmod(UVcoordX, Compiler->Div(Compiler->Constant(1), TexResolX));
	UVcoordY = Compiler->Fmod(UVcoordY, Compiler->Div(Compiler->Constant(1), TexResolY));

	//UV + Texture
	colorX = Compiler->Mul(colorX, Compiler->Constant(255));
	colorX = Compiler->Div(colorX, TexResolX);
	colorX = Compiler->Add(colorX, UVcoordX);
	colorX = Compiler->Mul(colorX, Compiler->Div(TexResolX, TileCntX));

	colorY = Compiler->Mul(colorY, Compiler->Constant(255));
	colorY = Compiler->Div(colorY, TexResolY);
	colorY = Compiler->Add(colorY, UVcoordY);
	colorY = Compiler->Mul(colorY, Compiler->Div(TexResolY, TileCntY));

	int32 resultUV = Compiler->AppendVector(colorX, colorY);
	
	return resultUV;
}
