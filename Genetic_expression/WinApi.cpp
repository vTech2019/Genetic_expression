#include "Header.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WinApi::~WinApi()
{
}

WinApi::WinApi(TCHAR* name_window, Parameters* data)
{
	window_class = { 0 };
	window_class.lpszClassName = name_window;
	window_class.lpfnWndProc = WndProc;
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	//window_class.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	//window_class.lpszClassName = MAKEINTRESOURCE(IDD_FORMVIEW);;
	if (!RegisterClass(&window_class)) {
		DWORD code_error = GetLastError();
		TCHAR* info;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, code_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)& info, 0, NULL);
		MessageBox(NULL, info, TEXT("RegisterClass message!"), MB_OK);
		LocalFree(info);
		return;
	}
	main_window = CreateWindow(name_window, name_window, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), (LPVOID)data);
	ShowWindow(main_window, SW_SHOWDEFAULT);
	UpdateWindow(main_window);
}
HRESULT WinApi::UpdateWindow(HWND hwnd)
{
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(hwnd);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE: {
		Parameters* data = (Parameters*)((CREATESTRUCT*)lParam)->lpCreateParams;
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAIN_DIALOG), hwnd, DlgProc, (LPARAM)data);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	case WM_ERASEBKGND:
		return 1;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Parameters* pParam = NULL;

	switch (uMsg)
	{
	case WM_INITDIALOG: {
		pParam = (Parameters*)lParam;
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			HWND edit = GetDlgItem(hwnd, IDC_EDIT_FUNCTION);
			HWND edit_symbols = GetDlgItem(hwnd, IDC_EDIT_SYMBOLS);
			size_t length = Edit_GetTextLength(edit);
			size_t length_symbols = Edit_GetTextLength(edit_symbols);
			HLOCAL local_memory = LocalAlloc(LMEM_MOVEABLE | LMEM_DISCARDABLE, length);
			HLOCAL local_memory_symbols = LocalAlloc(LMEM_MOVEABLE | LMEM_DISCARDABLE, length_symbols);
			if (local_memory && local_memory_symbols) {
				TCHAR* memory = (TCHAR*)LocalLock(local_memory);
				TCHAR* memory_symbols = (TCHAR*)LocalLock(local_memory_symbols);
				Edit_GetText(edit, memory, length);
				Edit_GetText(edit_symbols, memory_symbols, length_symbols);

				LocalUnlock(local_memory);
				LocalUnlock(local_memory_symbols);
				local_memory = LocalFree(local_memory);
				local_memory_symbols = LocalFree(local_memory_symbols);
				EndDialog(hwnd, LOWORD(wParam));
			}
			else			{

			}
			return TRUE;
		}

		case IDCANCEL:
			EndDialog(hwnd, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}