#include "Font.h"



Font::Font()
{
	ZeroMemory(m_metaData, sizeof(m_metaData));
}


Font::Font(const Font &other)
{
}


Font::~Font()
{
}


bool Font::Init(char *fontName, ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, int screenWidth, int screenHeight)
{
	char stringBuffer[256], fntFilename[256], pngFilename[256];

	strncpy_s(stringBuffer, FONT_PATH, sizeof(stringBuffer));
	strncat_s(stringBuffer, fontName, sizeof(stringBuffer));
	strncpy_s(fntFilename, stringBuffer, sizeof(fntFilename));
	strncpy_s(pngFilename, stringBuffer, sizeof(pngFilename));
	strncat_s(fntFilename, FONT_FNT, sizeof(fntFilename));
	strncat_s(pngFilename, FONT_PNG, sizeof(pngFilename));

	if (!LoadFNTFile(fntFilename, screenWidth, screenHeight))
		return false;

	m_pTexture = new Texture(pngFilename);
	return m_pTexture->Init(pDevice, pDeviceContext);
}


void Font::Shutdown()
{
	if (m_pTexture)
		m_pTexture->Shutdown();

	Memory::SafeDelete(m_pTexture);
}


float Font::GetSpaceWidth()
{
	return m_spaceWidth;
}


float Font::GetLineHeight()
{
	return LINE_HEIGHT;
}


Character *Font::GetCharacter(int key)
{
	if (key < 0 || key >= MAX_ASCII)
		return NULL;
	return &m_metaData[key];
}


Texture *Font::GetTexture()
{
	return m_pTexture;
}


bool Font::LoadFNTFile(char *filename, int screenWidth, int screenHeight)
{
	FILE *file;
	char buffer[1024], *p_buffer, varBuffer[128], *p_var;
	int integerBuffer;
	unsigned int bufferStringLength;

	float	verticalPerPixelSize = 0.0f;
	float	horizontalPerPixelSize = 0.0f;
	int		padding[4];
	int		paddingWidth = 0;
	int		paddingHeight = 0;

	float imageSize;
	int characterKey;
	FNTMetaData metaData;

	// File format validation
	bool initPaddingFlag = false, initLineHeight = false, initImageWidth = false;

	if (fopen_s(&file, filename, "r") != 0)
		return false;

	fgets(buffer, sizeof(buffer), file);
	bufferStringLength = strlen(buffer);
	sscanf_s(buffer, "%s", varBuffer, sizeof(varBuffer));
	if (strcmp(varBuffer, "info") == 0) {
		p_buffer = buffer + strlen(varBuffer) + 1;
		
		while (p_buffer < buffer + bufferStringLength
			&& sscanf_s(p_buffer, "%s", varBuffer, sizeof(varBuffer)) > 0) {
			p_buffer += strlen(varBuffer) + 1;
			p_var = varBuffer;
			while (*p_var != '=') p_var++;
			*p_var++ = '\0';

			if (strcmp(varBuffer, "padding") == 0) {
				initPaddingFlag = true;
				if (sscanf_s(p_var, "%d,%d,%d,%d", padding, padding + 1, padding + 2, padding + 3) != 4)
					return false;
				paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
				paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
			}
		}
	}
	else return false;

	if (!initPaddingFlag)
		return false;

	fgets(buffer, sizeof(buffer), file);
	bufferStringLength = strlen(buffer);
	sscanf_s(buffer, "%s", varBuffer, sizeof(varBuffer));
	if (strcmp(varBuffer, "common") == 0) {
		p_buffer = buffer + strlen(varBuffer) + 1;
		
		while (p_buffer < buffer + bufferStringLength
			&& sscanf_s(p_buffer, "%s", varBuffer, sizeof(varBuffer)) > 0) {
			p_buffer += strlen(varBuffer) + 1;
			p_var = varBuffer;
			while (*p_var != '=') p_var++;
			*p_var++ = '\0';

			if (sscanf_s(p_var, "%d", &integerBuffer) != 1)
				return false;

			if (strcmp(varBuffer, "lineHeight") == 0) {
				initLineHeight = true;
				int lineHeightPixels = integerBuffer - paddingHeight;
				verticalPerPixelSize = LINE_HEIGHT / static_cast<float>(lineHeightPixels);
				horizontalPerPixelSize = verticalPerPixelSize / (static_cast<float>(screenWidth) / static_cast<float>(screenHeight));
			}
			else if (strcmp(varBuffer, "scaleW") == 0) {
				initImageWidth = true;
				imageSize = static_cast<float>(integerBuffer);
			}
		}
	}
	else return false;

	if (!initLineHeight)
		return false;
	if (!initImageWidth)
		return false;

	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		sscanf_s(buffer, "%s", varBuffer, sizeof(varBuffer));
		bufferStringLength = strlen(buffer);
		p_buffer = buffer + strlen(varBuffer) + 1;
		if (strcmp(varBuffer, "char") == 0) {
			characterKey = -1;

			while (p_buffer < buffer + bufferStringLength
				&& sscanf_s(p_buffer, "%s", varBuffer, sizeof(varBuffer)) > 0) {
				p_buffer += strlen(varBuffer) + 1;
				while (*p_buffer == ' ') p_buffer++;
				p_var = varBuffer;
				while (*p_var != '=') p_var++;
				*p_var++ = '\0';

				if (sscanf_s(p_var, "%d", &integerBuffer) != 1)
					return false;

				if (strcmp(varBuffer, "id") == 0) {
					if (integerBuffer < 0 || integerBuffer >= MAX_ASCII)
						return false;
					characterKey = integerBuffer;
					metaData.id = integerBuffer;
				}
				else if (strcmp(varBuffer, "x") == 0)
					metaData.x = integerBuffer;
				else if (strcmp(varBuffer, "y") == 0)
					metaData.y = integerBuffer;
				else if (strcmp(varBuffer, "width") == 0)
					metaData.width = integerBuffer;
				else if (strcmp(varBuffer, "height") == 0)
					metaData.height = integerBuffer;
				else if (strcmp(varBuffer, "xoffset") == 0)
					metaData.xOffset = integerBuffer;
				else if (strcmp(varBuffer, "yoffset") == 0)
					metaData.yOffset = integerBuffer;
				else if (strcmp(varBuffer, "xadvance") == 0)
					metaData.xAdvance = integerBuffer;
			}

			float xTextureCoord = static_cast<float>(metaData.x + padding[PAD_LEFT] - DESIRED_PADDING) / imageSize;
			float yTextureCoord = static_cast<float>(metaData.y + padding[PAD_TOP] - DESIRED_PADDING) / imageSize;
			int width = metaData.width - (paddingWidth - 2 * DESIRED_PADDING);
			int height = metaData.height - (paddingHeight - 2 * DESIRED_PADDING);
			float quadWidth = static_cast<float>(width) * horizontalPerPixelSize;
			float quadHeight = static_cast<float>(height) * verticalPerPixelSize;
			float xTextureSize = static_cast<float>(width) / imageSize;
			float yTextureSize = static_cast<float>(height) / imageSize;
			float xOffset = static_cast<float>(metaData.xOffset + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
			float yOffset = static_cast<float>(metaData.yOffset + padding[PAD_TOP] - DESIRED_PADDING) * verticalPerPixelSize;
			float xAdvance = static_cast<float>(metaData.xAdvance - paddingWidth) * horizontalPerPixelSize;

			m_metaData[characterKey] = {
				metaData.id,
				xTextureCoord,
				yTextureCoord,
				xTextureCoord + xTextureSize,
				yTextureCoord + yTextureSize,
				xOffset,
				yOffset,
				quadWidth,
				quadHeight,
				xAdvance
			};

			if (characterKey == ' ') {
				m_spaceWidth = xAdvance;
			}
		}
	}

	if (fclose(file) != 0)
		return false;

	return true;
}