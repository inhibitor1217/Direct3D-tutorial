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