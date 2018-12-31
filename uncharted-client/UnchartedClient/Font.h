#pragma once
#include "stdafx.h"
#define MAX_ASCII 128

struct Character {
	int id;
	float xTextureCoord;
	float yTextureCoord;
	float xMaxTextureCoord;
	float yMaxTextureCoord;
	float xOffset;
	float yOffset;
	float sizeX;
	float sizeY;
	float xAdvance;
};

class Font
{
public:
	Font();
	Font(const Font &other);
	~Font();

	bool Init(char *filename, int screenWidth, int screenHeight);
	void Shutdown();

	float GetSpaceWidth();
	Character *GetCharacter(int key);

private:
	struct FNTMetaData {
		int id;
		int x;
		int y;
		int width;
		int height;
		int xOffset;
		int yOffset;
		int xAdvance;
	};

	const int			PAD_TOP = 0;
	const int			PAD_LEFT = 1;
	const int			PAD_BOTTOM = 2;
	const int			PAD_RIGHT = 3;
	const int			DESIRED_PADDING = 3;
	const float			LINE_HEIGHT = 0.03f;
	const LPCSTR		SPLITTER = " ";
	const LPCSTR		NUMBER_SEPARATOR = ",";

	float m_spaceWidth = 0.0f;

	Character m_metaData[MAX_ASCII];

	bool LoadFNTFile(char *filename, int screenWidth, int screenHeight);
};

