#include<Windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame1");

int WINAPI _tWinMain(HINSTANCE This,		 // Дескриптор /*UPD: текущего*/ экземпляра приложения 
	HINSTANCE Prev, 	// В современных системах всегда 0 /*UPD: Дескриптор предыдущего экземпляра приложения*/
	LPTSTR cmd, 		// Командная строка /*UPD: Указатель на строку с параметрами командной строки без имени программы*/
	int mode) 		// Режим отображения окна
{
	HWND hWnd;		// Дескриптор главного окна программы 
	MSG msg; 		// Структура для хранения сообщения
	WNDCLASS wc; 	// Класс окна /*UPD: Cтруктура для хранения параметров окна*/
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// Заполнение окна белым цветом 

	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("Каркас Windows-приложения1"), 		// Заголовок окна 
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
HWND hWnd2;
BOOL isFinished;
RECT clientRect;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPCSTR szMessage;
	LPTSTR psz = new TCHAR[9];
	LPTSTR pStop;
	switch (message)		 // Обработчик сообщений
	{
	case WM_CREATE:
		isFinished = TRUE;
		break;
	case WM_LBUTTONDOWN:
		if (isFinished)
			if (hWnd2 = FindWindow(_T("MainFrame2"), _T("Каркас Windows-приложения2")))
			{
				InvalidateRect(hWnd, &clientRect, TRUE);
				isFinished = FALSE;
				MessageBox(hWnd, _T("Второе окно открыто. Сейчас будет передан дескриптор."), _T("Статус 2-го окна"), MB_OK);
				if (MessageBox(hWnd, _T("Дескриптор передан. Хотите, чтобы окна автоматически закрылись?"), _T("Статус передачи"), MB_YESNO) == IDYES)
				{
					SendMessage(hWnd2, WM_USER + 2, (WPARAM)hWnd, NULL);
				}
				else
				{
					SendMessage(hWnd2, WM_USER + 1, (WPARAM)hWnd, NULL);
				}
			}
			else
				MessageBox(hWnd, _T("Второе окно закрыто."), _T("Статус 2-го окна"), MB_OK);
		break;
	case WM_USER + 1:
		SetTimer(hWnd, NULL, 15000, NULL);
		break;
	case WM_USER + 2:
		isFinished = TRUE;
		break;
	case WM_TIMER:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		//szMessage = /*(LPCWSTR) hWnd2*/ _T("Text");
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		_stprintf(psz, _T("%p"), hWnd2);
		//hWnd = (HWND)_tcstol(psz, &pStop, 16);
		//DrawText(hdc, _T("Text"), _tcslen(_T("Text")), &clientRect, DT_CENTER);
		DrawText(hdc, psz, _tcslen(psz), &clientRect, DT_CENTER);
		delete[]psz;
		//TextOutA(hdc, 70, 50, szMessage, strlen((const char*)szMessage));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



