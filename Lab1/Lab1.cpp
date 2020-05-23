#include<Windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>
#include<vector>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // Дескриптор экземпляра приложения 
	HINSTANCE Prev, 	// В современных системах всегда 0
	LPTSTR cmd, 		// Командная строка
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
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// Заполнение окна белым цветом 
	wc.hbrBackground = CreateSolidBrush(RGB(153, 255, 153)); // Светло-салатовый цвет

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

RECT clientRect;
BOOL isLKM;
BOOL isPKM;
BOOL isFirst;
std::vector<int> v;
int arr[200];
int n;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		 // Обработчик сообщений
	{
	case WM_CREATE:
		n = 0;
		break;
	case WM_PAINT:
	{
			PAINTSTRUCT ps;
			GetClientRect(hWnd, &clientRect); // записать в clientRect координаты клиентской области окна, на которое указывает hWnd
			HDC hdc = BeginPaint(hWnd, &ps);
			LONG m = min(clientRect.right, clientRect.bottom);
			if ((!isLKM && !isPKM) || isPKM)
			{
				HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 0)));
				Ellipse(hdc, 0, 0, m, m);
				SelectObject(hdc, oldBrush);
				v.clear();
			}
			if (isLKM)
			{
				if (isFirst)
				{
					v.push_back(rand() % 256);
					v.push_back(rand() % 256);
					v.push_back(rand() % 256);
					v.push_back(rand());
					v.push_back(rand());
					v.push_back(rand());
					v.push_back(rand());
				}
				HBRUSH oldBrush = NULL;
				for (int i = 0; i < (v.size() / 7); i++)
				{
					if (i == 0)
					{
						oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 0)));
						Ellipse(hdc, 0, 0, m, m);
						SelectObject(hdc, CreateSolidBrush(RGB(v[0], v[1], v[2])));
					}
					else
						SelectObject(hdc, CreateSolidBrush(RGB(v[i * 7], v[i * 7 + 1], v[i * 7 + 2])));
					Pie(hdc, 0, 0, m, m, v[i * 7 + 3], v[i * 7 + 4], v[i * 7 + 5], v[i * 7 + 6]);
				}
				if (oldBrush) SelectObject(hdc, oldBrush);
				isFirst = FALSE;
			}
			EndPaint(hWnd, &ps);
			break;
	}
	case WM_LBUTTONDOWN:
		isPKM = FALSE;
		isLKM = TRUE;
		isFirst = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
		break;
	case WM_RBUTTONDOWN:
		isLKM = FALSE;
		isPKM = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}