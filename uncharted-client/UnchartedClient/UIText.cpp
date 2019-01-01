#include "UIText.h"



void UIText::SetFont(Font *font)
{
	m_pFont = font;
}


Font *UIText::GetFont()
{
	return m_pFont;
}


ID3D11ShaderResourceView *UIText::GetTexture()
{
	return m_pFont->GetTexture()->GetTexture();
}


char *UIText::GetText()
{
	return m_text;
}


void UIText::SetText(char *text)
{
	if (strcmp(text, m_text) != 0)
		m_updateFlag = true;

	strncpy_s(m_text, text, MAX_TEXT_LENGTH);
}


XMFLOAT4 UIText::GetColor()
{
	return m_color;
}


void UIText::SetColor(float r, float g, float b, float a)
{
	m_color = { r, g, b, a };
}


float UIText::GetFontSize()
{
	return m_fontSize;
}


void UIText::SetFontSize(float fontSize)
{
	m_fontSize = fontSize;
}


float UIText::GetMaxLineWidth()
{
	return m_maxLineWidth;
}


void UIText::SetMaxLineWidth(int maxLineWidthInPixels)
{
	m_maxLineWidth = static_cast<float>(maxLineWidthInPixels) / static_cast<float>(m_screenWidth);
}


bool UIText::GetCentering()
{
	return m_centering;
}


void UIText::SetCentering(bool centering)
{
	m_centering = centering;
}


bool UIText::InitBuffers(ID3D11Device *pDevice)
{
	m_vertexCount = 0;
	m_indexCount = 0;

	VertexType *vertices = new VertexType[MAX_TEXT_LENGTH * 4];
	unsigned long *indices = new unsigned long[MAX_TEXT_LENGTH * 6];

	for (int i = 0; i < MAX_TEXT_LENGTH; i++) {
		indices[6 * i + 0] = 4 * i + 0;
		indices[6 * i + 1] = 4 * i + 1;
		indices[6 * i + 2] = 4 * i + 2;
		indices[6 * i + 3] = 4 * i + 0;
		indices[6 * i + 4] = 4 * i + 3;
		indices[6 * i + 5] = 4 * i + 1;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * MAX_TEXT_LENGTH * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer))) {
		Memory::SafeDeleteArray(vertices);
		Memory::SafeDeleteArray(indices);
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * MAX_TEXT_LENGTH * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer))) {
		Memory::SafeDeleteArray(vertices);
		Memory::SafeDeleteArray(indices);
		return false;
	}

	Memory::SafeDeleteArray(vertices);
	Memory::SafeDeleteArray(indices);

	return true;
}


bool UIText::UpdateBuffers(ID3D11DeviceContext *pDeviceContext, int posX, int posY)
{
	if (!m_updateFlag && posX == m_prevPosX && posY == m_prevPosY)
		return true;

	m_updateFlag = false;
	m_prevPosX = posX;
	m_prevPosY = posY;

	VertexType *vertices;

	GenerateTextMesh(posX, posY, vertices);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		Memory::SafeDeleteArray(vertices);
		return false;
	}

	VertexType *pVertices = reinterpret_cast<VertexType *>(mappedResource.pData);
	memcpy(pVertices, reinterpret_cast<void *>(vertices), sizeof(VertexType) * m_vertexCount);

	pDeviceContext->Unmap(m_pVertexBuffer, 0);
	pDeviceContext->Unmap(m_pIndexBuffer, 0);

	Memory::SafeDeleteArray(vertices);

	return true;
}


void UIText::GenerateTextMesh(int posX, int posY, VertexType *&vertices)
{
	Line *lines[MAX_TEXT_LENGTH];
	int numLines = 0, numChars = 0;

	Line *currentLine = reinterpret_cast<Line *>(malloc(sizeof(Line)));
	Word *currentWord = reinterpret_cast<Word *>(malloc(sizeof(Word)));
	float addWidth = 0.0f;

	ZeroMemory(currentLine, sizeof(Line));
	ZeroMemory(currentWord, sizeof(Word));

	for (int i = 0; m_text[i]; i++) {
		int characterKey = static_cast<int>(m_text[i]);
		switch (characterKey) {
		case ' ':
			addWidth = currentWord->width + (currentWord->length > 0 ? m_pFont->GetSpaceWidth() : 0.0f);
			if ((currentLine->width + addWidth) * m_fontSize <= m_maxLineWidth) {
				currentLine->words[currentLine->length++] = currentWord;
				currentLine->width += addWidth;
				numChars++;
			}
			else {
				lines[numLines++] = currentLine;
				currentLine = reinterpret_cast<Line *>(malloc(sizeof(Line)));
				ZeroMemory(currentLine, sizeof(Line));
				currentLine->words[currentLine->length++] = currentWord;
				currentLine->width += addWidth;
			}
			currentWord = reinterpret_cast<Word *>(malloc(sizeof(Word)));
			ZeroMemory(currentWord, sizeof(Word));
			break;
		case '\n':
			addWidth = currentWord->width + (currentWord->length > 0 ? m_pFont->GetSpaceWidth() : 0.0f);
			if ((currentLine->width + addWidth) * m_fontSize <= m_maxLineWidth) {
				currentLine->words[currentLine->length++] = currentWord;
				currentLine->width += addWidth;
			}
			else {
				lines[numLines++] = currentLine;
				currentLine = reinterpret_cast<Line *>(malloc(sizeof(Line)));
				ZeroMemory(currentLine, sizeof(Line));
				currentLine->words[currentLine->length++] = currentWord;
				currentLine->width += addWidth;
			}
			lines[numLines++] = currentLine;
			currentLine = reinterpret_cast<Line *>(malloc(sizeof(Line)));
			ZeroMemory(currentLine, sizeof(Line));
			currentWord = reinterpret_cast<Word *>(malloc(sizeof(Word)));
			ZeroMemory(currentWord, sizeof(Word));
			break;
		default:
			currentWord->characters[currentWord->length++] = characterKey;
			currentWord->width += m_pFont->GetCharacter(characterKey)->xAdvance;
			numChars++;
			break;
		}
	}

	addWidth = currentWord->width + (currentWord->length > 0 ? m_pFont->GetSpaceWidth() : 0.0f);
	if ((currentLine->width + addWidth) * m_fontSize <= m_maxLineWidth) {
		currentLine->words[currentLine->length++] = currentWord;
		currentLine->width += addWidth;
	}
	else {
		lines[numLines++] = currentLine;
		currentLine = reinterpret_cast<Line *>(malloc(sizeof(Line)));
		ZeroMemory(currentLine, sizeof(Line));
		currentLine->words[currentLine->length++] = currentWord;
		currentLine->width += addWidth;
	}
	lines[numLines++] = currentLine;


	float cursorX = 0.0f, cursorY = 0.0f;
	int idx = 0;
	
	m_vertexCount = 4 * numChars;
	m_indexCount = 6 * numChars;
	vertices = new VertexType[m_vertexCount];

	for (int i = 0; i < numLines; i++) {
		Line *line = lines[i];
		if (m_centering)
			cursorX = (m_maxLineWidth - line->width) * 0.5f;

		for (int j = 0; j < line->length; j++) {
			Word *word = line->words[j];

			for (int k = 0; k < word->length; k++) {
				int characterKey = word->characters[k];
				Character *data = m_pFont->GetCharacter(characterKey);

				float x = (cursorX + (data->xOffset * m_fontSize)) * m_screenWidth;
				float y = (cursorY + (data->yOffset * m_fontSize)) * m_screenHeight;
				float maxX = x + (data->sizeX * m_fontSize) * m_screenWidth;
				float maxY = y + (data->sizeY * m_fontSize) * m_screenHeight;
				float properX = (posX + x) - 0.5f * m_screenWidth;
				float properY = 0.5f * m_screenHeight - (posY + y);
				float properMaxX = (posX + maxX) - 0.5f * m_screenWidth;
				float properMaxY = 0.5f * m_screenHeight - (posY + maxY);

				vertices[idx++] = { XMFLOAT3(properX,    properY,    0.0f), XMFLOAT2(data->xTextureCoord,    data->yTextureCoord) };
				vertices[idx++] = { XMFLOAT3(properMaxX, properMaxY, 0.0f), XMFLOAT2(data->xMaxTextureCoord, data->yMaxTextureCoord) };
				vertices[idx++] = { XMFLOAT3(properX,    properMaxY, 0.0f), XMFLOAT2(data->xTextureCoord,    data->yMaxTextureCoord) };
				vertices[idx++] = { XMFLOAT3(properMaxX, properY,    0.0f), XMFLOAT2(data->xMaxTextureCoord, data->yTextureCoord) };

				cursorX += data->xAdvance * m_fontSize;
			}

			cursorX += m_pFont->GetSpaceWidth() * m_fontSize;
		}

		cursorX = 0;
		cursorY += m_pFont->GetLineHeight() * m_fontSize;
	}

	for (int i = 0; i < numLines; i++) {
		Line *line = lines[i];
		for (int j = 0; j < line->length; j++) {
			Memory::SafeDelete(line->words[j]);
		}
		Memory::SafeDelete(line);
	}

}