#include<Windows.h>
#include<tchar.h>
#include<time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

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
	srand((unsigned)time(0));
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
HMENU menu;
HMENU colors;
RECT clientRect;
BOOL isRed;
BOOL isYellow;
BOOL isGreen;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		 // Обработчик сообщений
	{
	case WM_CREATE:
		menu = CreateMenu();
		colors = CreatePopupMenu();
		AppendMenu(menu, MF_POPUP, (UINT_PTR)colors, _T("Цвета"));
		AppendMenu(colors, MF_STRING, 1, _T("Красный"));
		AppendMenu(colors, MF_STRING, 2, _T("Желтый"));
		AppendMenu(colors, MF_STRING, 3, _T("Зеленый"));
		AppendMenu(colors, MF_STRING, 4, _T("Автоматическое управление"));
		SetMenu(hWnd, menu);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		GetClientRect(hWnd, &clientRect); // записать в clientRect координаты клиентской области окна, на которое указывает hWnd
		clientRect.right = 500;
		clientRect.bottom = 500;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (isRed)
		{
			//MessageBeep(MB_OK);
			//HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
			Ellipse(hdc, clientRect.right / 4, 0, (3 * clientRect.right) / 4, clientRect.bottom / 3); // 1
			SelectObject(hdc, oldBrush);
			Ellipse(hdc, clientRect.right / 4, clientRect.bottom / 3, (3 * clientRect.right) / 4, (2 * clientRect.bottom) / 3); // 2
			Ellipse(hdc, clientRect.right / 4, (2 * clientRect.bottom) / 3, (3 * clientRect.right) / 4, clientRect.bottom); // 3
		}
		else
		{
			Ellipse(hdc, clientRect.right / 4, 0, (3 * clientRect.right) / 4, clientRect.bottom / 3); // 1
			if (isYellow)
			{
				//MessageBeep(MB_OK);
				//HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 0)));
				HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
				Ellipse(hdc, clientRect.right / 4, clientRect.bottom / 3, (3 * clientRect.right) / 4, (2 * clientRect.bottom) / 3); // 2
				SelectObject(hdc, oldBrush);
				Ellipse(hdc, clientRect.right / 4, (2 * clientRect.bottom) / 3, (3 * clientRect.right) / 4, clientRect.bottom); // 3
			}
			else
			{
				Ellipse(hdc, clientRect.right / 4, clientRect.bottom / 3, (3 * clientRect.right) / 4, (2 * clientRect.bottom) / 3); // 2
				if (isGreen)
				{
					//HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
					HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
					Ellipse(hdc, clientRect.right / 4, (2 * clientRect.bottom) / 3, (3 * clientRect.right) / 4, clientRect.bottom); // 3
					SelectObject(hdc, oldBrush);
				}
				else
					Ellipse(hdc, clientRect.right / 4, (2 * clientRect.bottom) / 3, (3 * clientRect.right) / 4, clientRect.bottom); // 3
			}
		}
		//SelectObject(hdc, oldBrush);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_COMMAND:
		if (wParam == 1)
		{
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			isRed = TRUE; isYellow = FALSE; isGreen = FALSE;
			InvalidateRect(hWnd, &clientRect, TRUE);
		}
		else if (wParam == 2)
		{
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			isRed = FALSE; isYellow = TRUE; isGreen = FALSE;
			InvalidateRect(hWnd, &clientRect, TRUE);
		}
		else if (wParam == 3)
		{
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			isRed = FALSE; isYellow = FALSE; isGreen = TRUE;
			InvalidateRect(hWnd, &clientRect, TRUE);
		}
		else if (wParam == 4)
		{
			//MessageBeep(MB_OK);
			//while (true)//isRed == TRUE || isYellow == TRUE || isGreen == TRUE)
			//{
				isRed = TRUE; isYellow = FALSE; isGreen = FALSE;
				InvalidateRect(hWnd, &clientRect, TRUE); // 1. поменяется на красный
				//isRed = FALSE; isYellow = TRUE; isGreen = FALSE;
				SetTimer(hWnd, 2, /*5000*/ 250, NULL); // 2. 5 секунд будет гореть красный
				//InvalidateRect(hWnd, &clientRect, FALSE);
				//isRed = FALSE; isYellow = FALSE; isGreen = TRUE;
				//SetTimer(hWnd, 2, 2000, NULL);
				//SetTimer(hWnd, 3, 5000, NULL);
				//MessageBeep(MB_OK);
			//}
		}
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			KillTimer(hWnd, 1);
			//MessageBeep(MB_OK);
			isRed = FALSE; isYellow = TRUE; isGreen = FALSE;
			InvalidateRect(hWnd, &clientRect, TRUE); // 5. поменяется на жёлтый
			SetTimer(hWnd, 3, /*2500*/ 250, NULL); // 6. 2 секунды будет гореть желтый
		}
		else if (wParam == 2)
		{
			KillTimer(hWnd, 2);
			//MessageBeep(MB_OK);
			isRed = FALSE; isYellow = FALSE; isGreen = TRUE;
			InvalidateRect(hWnd, &clientRect, TRUE); // 3. поменяется на зелёный
			SetTimer(hWnd, 1, /*5000*/ 250, NULL); // 4. 5 секунд будет гореть зелёный
		}
		else if (wParam == 3)
		{
			KillTimer(hWnd, 3);
			//MessageBeep(MB_OK);
			isRed = TRUE; isYellow = FALSE; isGreen = FALSE;
			InvalidateRect(hWnd, &clientRect, TRUE); // 7. поменяется на красный
			SetTimer(hWnd, 2, /*5000*/ 250, NULL);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
