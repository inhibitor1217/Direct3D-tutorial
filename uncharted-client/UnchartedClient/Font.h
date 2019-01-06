#pragma once
#include "stdafx.h"
#include "Texture.h"
#define MAX_ASCII 128

const char FONT_PATH[] = "./Assets/Fonts/";
const char FONT_PNG[] = ".png";
const char FONT_FNT[] = ".fnt";

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

	bool Init(char *filename, ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, int screenWidth, int screenHeight);
	void Shutdown();

	float GetSpaceWidth();
	float GetLineHeight();
	Character *GetCharacter(int key);
	Texture *GetTexture();

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
	const int			DESIRED_PADDING = 8;
	const float			LINE_HEIGHT = 0.03f;
	const LPCSTR		SPLITTER = " ";
	const LPCSTR		NUMBER_SEPARATOR = ",";

	float m_spaceWidth = 0.0f;

	Character m_metaData[MAX_ASCII];

	Texture *m_pTexture = nullptr;

	bool LoadFNTFile(char *filename, int screenWidth, int screenHeight);
};

