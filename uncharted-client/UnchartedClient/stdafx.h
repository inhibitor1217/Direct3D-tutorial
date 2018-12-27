#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "DxDefine.h"

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