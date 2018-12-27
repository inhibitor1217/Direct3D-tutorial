#pragma once
#include "stdafx.h"

namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
		if (t) {
			delete t;
			t = nullptr;
		}
	}

	template <class T> void SafeDeleteArray(T& t)
	{
		if (t) {
			delete[] t;
			t = nullptr;
		}
	}

	template <class T> void SafeRelease(T& t)
	{
		if (t) {
			t->Release();
			t = nullptr;
		}
	}
}

namespace Math
{
	float Deg2Radians(float deg);
}

namespace Debug
{
	char *PrintXMMATRIX(XMMATRIX matrix);
}