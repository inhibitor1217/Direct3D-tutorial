#pragma once
#include "stdafx.h"
#include "UIModel.h"
#include "Font.h"
#define MAX_TEXT_LENGTH 1024

class UIText: public UIModel
{
public:
	enum ALIGN_MODE {
		LEFT,
		CENTER,
		RIGHT
	};

	void SetFont(Font *font);
	Font *GetFont();
	ID3D11ShaderResourceView *GetTexture() override;

	char *GetText();
	void SetText(char *text);
	XMFLOAT4 GetColor();
	void SetColor(float r, float g, float b, float a);
	float GetFontSize();
	void SetFontSize(float fontSize);
	float GetMaxLineWidth();
	void SetMaxLineWidth(int maxLineWidthInPixels);
	int GetAlignMode();
	void SetAlignMode(ALIGN_MODE mode);

private:
	struct Word {
		char characters[MAX_TEXT_LENGTH];
		float width = 0.0f;
		int length = 0;
	};
	struct Line {
		Word *words[MAX_TEXT_LENGTH];
		float width = 0.0f;
		int length = 0;
	};

	Font		*m_pFont = nullptr;
	char		m_text[MAX_TEXT_LENGTH];
	bool		m_updateFlag = false;

	XMFLOAT4	m_color;
	float		m_fontSize = 1.0f;
	float		m_maxLineWidth = 1.0f;
	ALIGN_MODE	m_alignMode = LEFT;

	bool InitBuffers(ID3D11Device *pDevice) override;
	bool UpdateBuffers(ID3D11DeviceContext *pDeviceContext, int posX, int posY) override;

	void GenerateTextMesh(int posX, int posY, VertexType *&vertices);
};

