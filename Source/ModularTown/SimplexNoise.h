/*
SimplexNoise 1.0.0
-----
DevDad - Afan Olovcic @ www.art-and-code.com - 08/12/2015

This algorithm was originally designed by Ken Perlin, but my code has been
adapted and extended from the implementation written by Stefan Gustavson (stegu@itn.liu.se)
and modified to fit to Unreal Engine 4.

This is a clean, fast, modern and free Perlin Simplex noise function.

Modified and simplified by Salaar Kohari

*/

#pragma once

#include "SimplexNoise.generated.h"


UCLASS()
class ARCHIPLEAGOALPHA_API USimplexNoise : public UObject
{
	GENERATED_UCLASS_BODY()
private:

	static unsigned char perm[];
	static float  grad(int hash, float x);
	static float  grad(int hash, float x, float y);
	static float  grad(int hash, float x, float y, float z);
	static float  grad(int hash, float x, float y, float z, float t);

public:

	static void setNoiseSeed(const int32& newSeed);
	static float SimplexNoise1D(float x);
	static float SimplexNoise2D(float x, float y);
	static float SimplexNoise3D(float x, float y, float z);
	static float SimplexNoise4D(float x, float y, float z, float w);

};

