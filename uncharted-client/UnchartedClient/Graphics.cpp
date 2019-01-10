#include "Graphics.h"



Graphics::Graphics()
{
}


Graphics::Graphics(const Graphics &other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Init(INT screenWidth, INT screenHeight, HWND hwnd)
{
	m_pDirect3D = (D3D *)malloc(sizeof(D3D));
	if (!m_pDirect3D)
		return false;

	if (!m_pDirect3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	m_pCamera = new Camera();
	if (!m_pCamera)
		return false;
	m_pCamera->SetPosition(0.0f, 3.0f, -10.0f);

	// Load Fonts.
	char *fontNames[] = {
		"Calibri"
	};
	for (auto fontName : fontNames) {
		Font *font = new Font();
		font->Init(fontName, m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext(), screenWidth, screenHeight);
		m_fonts.push_back(font);
	}

	// Load Textures.
	char *textureFiles[] = {
		"./Assets/Textures/char.bmp"
	};
	for (auto filename : textureFiles) {
		Texture *texture = new Texture(filename);
		texture->Init(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext());
		m_textures.push_back(texture);
	}

	// Load Meshes.
	char *meshFiles[] = {
		"./Assets/Models/char.obj"
	};
	for (auto filename : meshFiles) {
		Mesh *mesh = new Mesh();
		mesh->LoadOBJ(filename);
		m_meshes.push_back(mesh);
	}

	// Load 3D Models.
	m_pModel = new TextureModel(m_meshes[0], m_textures[0]);
	if (!m_pModel)
		return false;
	if (!m_pModel->Init(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext())) {
		MessageBox(hwnd, "Could not initialize the model object", "Error", MB_OK);
		return false;
	}

	// Load UI Models.
	m_pUIText = new UIText();
	if (!m_pUIText)
		return false;
	if (!m_pUIText->Init(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext(),
		screenWidth, screenHeight, 1080, 720)) {
		MessageBox(hwnd, "Could not initialize the UI object", "Error", MB_OK);
		return false;
	}
	m_pUIText->SetFont(m_fonts[0]);
	m_pUIText->SetText("Hello, world!\n\nIn C++ there is a concept of constructor's intialization list, which is where you can and should call the base class' constructor.");
	m_pUIText->SetFontSize(4.0f);
	m_pUIText->SetColor(0.0f, 1.0f, 1.0f, 0.3f);

	// Load Shaders.
	m_pTextureShader = new TextureShader();
	if (!m_pTextureShader)
		return false;
	if (!m_pTextureShader->Init(m_pDirect3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not initialize the TextureShader object", "Error", MB_OK);
		return false;
	}

	m_pUIShader = new UIShader();
	if (!m_pUIShader)
		return false;
	if (!m_pUIShader->Init(m_pDirect3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not initialize the UIShader object", "Error", MB_OK);
		return false;
	}

	m_pTextShader = new TextShader();
	if (!m_pTextShader)
		return false;
	if (!m_pTextShader->Init(m_pDirect3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not initialize the TextShader object", "Error", MB_OK);
		return false;
	}
	
	// Allocate memory for shader uniform variables.
	if (!GeneralShader::CreateVSUniformVariable(m_pTextureVSShaderVariables))
		return false;
	if (!GeneralShader::CreateVSUniformVariable(m_pTextVSShaderVariables))
		return false;
	if (!TextShader::CreatePSUniformVariable(m_pTextPSShaderVariables))
		return false;

	return true;
}


void Graphics::Shutdown()
{
	if (m_pTextureShader)
		m_pTextureShader->Shutdown();

	if (m_pUIShader)
		m_pUIShader->Shutdown();

	if (m_pTextShader)
		m_pTextShader->Shutdown();

	if (m_pModel)
		m_pModel->Shutdown();

	if (m_pUIText)
		m_pUIText->Shutdown();

	if (m_pDirect3D)
		m_pDirect3D->Shutdown();

	Memory::SafeDelete(m_pTextureShader);
	Memory::SafeDelete(m_pUIShader);
	Memory::SafeDelete(m_pTextShader);
	Memory::SafeDelete(m_pModel);
	Memory::SafeDelete(m_pUIText);
	Memory::SafeDelete(m_pCamera);
	Memory::SafeDelete(m_pDirect3D);

	for (auto mesh : m_meshes) {
		Memory::SafeDelete(mesh);
	}
	
	for (auto texture : m_textures) {
		texture->Shutdown();
		Memory::SafeDelete(texture);
	}

	for (auto font : m_fonts) {
		font->Shutdown();
		Memory::SafeDelete(font);
	}

	Memory::SafeDelete(m_pTextureVSShaderVariables);
	Memory::SafeDelete(m_pTextVSShaderVariables);
	Memory::SafeDelete(m_pTextPSShaderVariables);
}


bool Graphics::Frame()
{
	return Render();
}


bool Graphics::Render()
{
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate viewMatrix from camera component.
	m_pCamera->Render();

	// Retrieve world, view, projection, orthographic matrices.
	XMMATRIX world, view, projection, ortho;
	m_pDirect3D->GetWorldMatrix(world);
	m_pCamera->GetViewMatrix(view);
	m_pDirect3D->GetProjectionMatrix(projection);
	m_pDirect3D->GetOrthoMatrix(ortho);

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);
	ortho = XMMatrixTranspose(ortho);

	// Generate shader uniform variable wrappers.
	GeneralShader::SetVSUniformVariable(m_pTextureVSShaderVariables, world, view, projection);
	GeneralShader::SetVSUniformVariable(m_pTextVSShaderVariables, world, view, ortho);

	// Render Scene.
	m_pModel->Render(m_pDirect3D->GetDeviceContext());

	if (!m_pTextureShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), m_pTextureVSShaderVariables, NULL,
		reinterpret_cast<TextureModel *>(m_pModel)->GetTexture()))
		return false;

	m_pDirect3D->UseZBuffer(false);

	// Render UIs with disabled depth buffer.

	m_pDirect3D->UseAlphaBlending(true);

	// Render Text with enabled alpha blending.

	TextShader::SetPSUniformVariable(m_pTextPSShaderVariables, m_pUIText->GetColor());
	if (!m_pUIText->Render(m_pDirect3D->GetDeviceContext(), 0, 0))
		return false;
	if (!m_pTextShader->Render(m_pDirect3D->GetDeviceContext(), m_pUIText->GetIndexCount(), m_pTextVSShaderVariables, m_pTextPSShaderVariables, m_pUIText->GetTexture()))
	 	return false;

	m_pDirect3D->UseAlphaBlending(false);

	m_pDirect3D->UseZBuffer(true);

	m_pDirect3D->EndScene();

	return true;
}