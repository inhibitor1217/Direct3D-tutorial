#include "Utils.h"

float Math::Deg2Radians(float deg)
{
	return deg * 0.0174532925f;
}


char *Debug::PrintXMMATRIX(XMMATRIX matrix) 
{
	char *debugString = (char *)malloc(1024);

	XMFLOAT4X4 matrixT;
	XMStoreFloat4x4(&matrixT, matrix);

	sprintf_s(debugString, sizeof(debugString), "[\n%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f %.3f\n]\n",
		matrixT._11, matrixT._12, matrixT._13, matrixT._14, matrixT._21, matrixT._22, matrixT._23, matrixT._24,
		matrixT._31, matrixT._32, matrixT._33, matrixT._34, matrixT._41, matrixT._42, matrixT._43, matrixT._43);

	return debugString;
}