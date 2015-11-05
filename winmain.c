#include <Windows.h>
#include <tchar.h>

const TCHAR g_szClassName[] = _T("SkeletonAppWindowClass");

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
ATOM RegisterWCEX(HINSTANCE hInstance);
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd);

ATOM RegisterWCEX(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = 
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = g_szClassName;

	return RegisterClassEx(&wcex);
}

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, 0L);
	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;

	switch (Msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(_T("BUTTON"), _T("Say Hi"), WS_VISIBLE | WS_CHILD, 130, 230, 70, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);
		break;
	case WM_COMMAND:
		if ((HWND)lParam == hButton)
		{
			MessageBox(0, _T("Hello World!"), _T("Information"), MB_OK | MB_ICONINFORMATION);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	MSG Msg;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	RegisterWCEX(hInstance);

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_szClassName, _T("Simple Window"), WS_VISIBLE | WS_SYSMENU, 100, 100, 350, 370, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	EnumChildWindows(hWnd, EnumChildProc, 0L);
	UpdateWindow(hWnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
