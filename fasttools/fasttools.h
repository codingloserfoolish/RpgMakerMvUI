#pragma once
#include"config.h"
#include<cmath>
namespace hjdd
{
	constexpr float pi = 3.142f;

	extern "C" 
	{
		EXPORT float fast_atan(float k);
		EXPORT float fast_sin(float a);
		EXPORT float fast_cos(float a);
	}
}