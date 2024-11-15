#include"fasttools.h"
namespace hjdd
{
	float fast_atan(float k)
	{
		float real_v = k;
		if (k > 1)
		{
			real_v = 1 / k;
			return pi / 2 - (pi / 4) * real_v - 0.285 * real_v * (1 - abs(real_v));
		}
		else if (k < -1)
		{
			real_v = 1 / k;
			return -pi / 2 - (pi / 4) * real_v - 0.285 * real_v * (1 - abs(real_v));
		}
		else
			return (pi / 4) * real_v + 0.285 * real_v * (1 - abs(real_v));
	}
	float fast_sin(float a)
	{
		if (a == 0)return 0;
		float real_v = abs(a);
		float real_v_2 = real_v * real_v;
		float ret = (50.27 * real_v - 16 * real_v_2) / (49.36-12.57*real_v+4* real_v_2);
		if (a < 0)return -ret;
		return ret;
	}
	float fast_cos(float a)
	{
		if (a == 0)return 1;
		if ((pi / 2 - a) >= pi)return fast_sin(pi/2+a);
		return fast_sin(pi / 2 - a);
	}
}