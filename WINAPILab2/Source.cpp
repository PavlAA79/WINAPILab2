#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#pragma data_seg("Shared")
volatile int shared_var = 0;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // Дескриптор текущего приложения 
	HINSTANCE Prev, 	// В современных системах всегда 0 
	LPTSTR cmd, 		// Командная строка 
	int mode) 		// Режим отображения окна
{
	HWND hWnd;		// Дескриптор главного окна программы 
	MSG msg; 		// Структура для хранения сообщения 
	WNDCLASS wc; 	// Класс окна
	// Определение класса окна 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 				// Имя класса окна 
	wc.lpfnWndProc = WndProc; 					// Функция окна 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// Стиль окна 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// Стандартная иконка 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// Стандартный курсор 
	wc.lpszMenuName = NULL; 					// Нет меню 
	wc.cbClsExtra = 0; 						// Нет дополнительных данных класса 
	wc.cbWndExtra = 0; 						// Нет дополнительных данных окна 
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// Заполнение окна белым цветом 


		// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("Каркас Windows-приложения"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		CW_USEDEFAULT, 				// width 
		CW_USEDEFAULT, 				// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 				// Показать окно

	// Цикл обработки сообщений 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// Функция трансляции кодов нажатой клавиши 
		DispatchMessage(&msg); 		// Посылает сообщение функции WndProc() 
	}

	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
  	TCHAR str[256], name[256];
	int i;
	int x, y;

	switch (message)		 // Обработчик сообщений
	{
	case WM_CREATE:
	{
		shared_var++; 
		i = 1;

		if (shared_var <= 2)
		{
			SendMessage(hWnd, WM_GETTEXT, 256, (LPARAM)name);
			_stprintf_s(str, _T("%s - Экземпляр: %d"), name, shared_var);
			SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)str);


		}
		else if (shared_var > 2)
		{
			i = MessageBox(hWnd, _T("Запустить следующий экземпляр приложения?"),
				_T("Экземпляр приложения"), MB_YESNO | MB_ICONQUESTION);
			if (i == 7)
			{
				shared_var--;
				
				PostQuitMessage(0);
			}
			else if (i == 6)
			{

				SendMessage(hWnd, WM_GETTEXT, 256, (LPARAM)name);
				_stprintf_s(str, _T("%s - Экземпляр: %d"), name, shared_var);
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

