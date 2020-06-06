#pragma once
#include "GDX11Renderer/GDxPreInclude.h"

class GGiEngineUtil
{
public:
	static const float PI;

	static void NormalizeFloat3(float& x, float& y, float& z)
	{
		float l = (float)pow(x * x + y * y + z * z, 0.5);
		x /= l;
		y /= l;
		z /= l;
	}

	static std::vector<float> GetNormalizedFloat3(float* x)
	{
		std::vector<float> y(3);
		float l = (float)pow(x[0] * x[0] + x[1] * x[1] + x[2] * x[2], 0.5);
		y[0] = x[0] / l;
		y[1] = x[1] / l;
		y[2] = x[2] / l;
		return y;
	}

	static void NormalizeFloat3(float* x)
	{
		float l = (float)pow(x[0] * x[0] + x[1] * x[1] + x[2] * x[2], 0.5);
		x[0] /= l;
		x[1] /= l;
		x[2] /= l;
	}
};

