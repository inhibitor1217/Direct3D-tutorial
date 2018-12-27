#include "stdafx.h"
#include "SystemClass.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstace, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	SystemClass *p_system = new SystemClass(hInstance);
	
	if (!p_system)
		return -1;

	if (p_system->Init()) {
		p_system->Run();
	}

	p_system->Shutdown();
	Memory::SafeDelete(p_system);

	return 0;
}