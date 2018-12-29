#include "stdafx.h"
#include "System.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstace, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	System *p_system = new System(hInstance);
	
	if (!p_system)
		return -1;

	if (p_system->Init()) {
		p_system->Run();
	}

	p_system->Shutdown();
	Memory::SafeDelete(p_system);

	return 0;
}