#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#pragma data_seg("Shared")
volatile int shared_var = 0;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // ���������� �������� ���������� 
	HINSTANCE Prev, 	// � ����������� �������� ������ 0 
	LPTSTR cmd, 		// ��������� ������ 
	int mode) 		// ����� ����������� ����
{
	HWND hWnd;		// ���������� �������� ���� ��������� 
	MSG msg; 		// ��������� ��� �������� ��������� 
	WNDCLASS wc; 	// ����� ����
	// ����������� ������ ���� 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 				// ��� ������ ���� 
	wc.lpfnWndProc = WndProc; 					// ������� ���� 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// ����� ���� 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// ����������� ������ 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// ����������� ������ 
	wc.lpszMenuName = NULL; 					// ��� ���� 
	wc.cbClsExtra = 0; 						// ��� �������������� ������ ������ 
	wc.cbWndExtra = 0; 						// ��� �������������� ������ ���� 
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// ���������� ���� ����� ������ 


		// ����������� ������ ����
	if (!RegisterClass(&wc)) return 0;

	// �������� ���� 
	hWnd = CreateWindow(WinName,			// ��� ������ ���� 
		_T("������ Windows-����������"), 		// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ���� 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 ������� ���� 
		CW_USEDEFAULT, 				// width 
		CW_USEDEFAULT, 				// Height 
		HWND_DESKTOP, 				// ���������� ������������� ���� 
		NULL, 						// ��� ���� 
		This, 						// ���������� ���������� 
		NULL); 					// �������������� ���������� ��� 

	ShowWindow(hWnd, mode); 				// �������� ����

	// ���� ��������� ��������� 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// ������� ���������� ����� ������� ������� 
		DispatchMessage(&msg); 		// �������� ��������� ������� WndProc() 
	}

	return 0;
}

// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
  	TCHAR str[256], name[256];
	int i;
	int x, y;

	switch (message)		 // ���������� ���������
	{
	case WM_CREATE:
	{
		shared_var++; 
		i = 1;

		if (shared_var <= 2)
		{
			SendMessage(hWnd, WM_GETTEXT, 256, (LPARAM)name);
			_stprintf_s(str, _T("%s - ���������: %d"), name, shared_var);
			SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)str);


		}
		else if (shared_var > 2)
		{
			i = MessageBox(hWnd, _T("��������� ��������� ��������� ����������?"),
				_T("��������� ����������"), MB_YESNO | MB_ICONQUESTION);
			if (i == 7)
			{
				shared_var--;
				
				PostQuitMessage(0);
			}
			else if (i == 6)
			{

				SendMessage(hWnd, WM_GETTEXT, 256, (LPARAM)name);
				_stprintf_s(str, _T("%s - ���������: %d"), name, shared_var);
				SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)str);
	    	}
		}
	}
		break;
	case WM_PAINT:
	{ 	
		
			hdc = BeginPaint(hWnd, &ps);

			RECT rect;
			GetClientRect(hWnd, &rect);
			SetBkMode(hdc, TRANSPARENT);
			x = rect.right / 2;
		    y = rect.bottom / 2;
			WCHAR  num[100];
			wsprintf(num, TEXT("%d"), shared_var);
			TextOut(hdc, x, y, num, lstrlen(num));

			EndPaint(hWnd, &ps);

	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

