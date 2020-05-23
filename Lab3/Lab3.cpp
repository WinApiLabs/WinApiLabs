#include<Windows.h>
#include<tchar.h>
#include<time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // ���������� /*UPD: ��������*/ ���������� ���������� 
	HINSTANCE Prev, 	// � ����������� �������� ������ 0 /*UPD: ���������� ����������� ���������� ����������*/
	LPTSTR cmd, 		// ��������� ������ /*UPD: ��������� �� ������ � ����������� ��������� ������ ��� ����� ���������*/
	int mode) 		// ����� ����������� ����
{
	HWND hWnd;		// ���������� �������� ���� ��������� 
	MSG msg; 		// ��������� ��� �������� ���������
	WNDCLASS wc; 	// ����� ���� /*UPD: C�������� ��� �������� ���������� ����*/
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// ���������� ���� ����� ������ 

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
	srand((unsigned)time(0));
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
HMENU menu;
HMENU colors;
RECT clientRect;
BOOL isRed;
BOOL isYellow;
BOOL isGreen;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		 // ���������� ���������
	{
	case WM_CREATE:
		menu = CreateMenu();
		colors = CreatePopupMenu();
		AppendMenu(menu, MF_POPUP, (UINT_PTR)colors, _T("�����"));
		AppendMenu(colors, MF_STRING, 1, _T("�������"));
		AppendMenu(colors, MF_STRING, 2, _T("������"));
		AppendMenu(colors, MF_STRING, 3, _T("�������"));
		AppendMenu(colors, MF_STRING, 4, _T("�������������� ����������"));
		SetMenu(hWnd, menu);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		GetClientRect(hWnd, &clientRect); // �������� � clientRect ���������� ���������� ������� ����, �� ������� ��������� hWnd
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
				InvalidateRect(hWnd, &clientRect, TRUE); // 1. ���������� �� �������
				//isRed = FALSE; isYellow = TRUE; isGreen = FALSE;
				SetTimer(hWnd, 2, /*5000*/ 250, NULL); // 2. 5 ������ ����� ������ �������
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
			InvalidateRect(hWnd, &clientRect, TRUE); // 5. ���������� �� �����
			SetTimer(hWnd, 3, /*2500*/ 250, NULL); // 6. 2 ������� ����� ������ ������
		}
		else if (wParam == 2)
		{
			KillTimer(hWnd, 2);
			//MessageBeep(MB_OK);
			isRed = FALSE; isYellow = FALSE; isGreen = TRUE;
			InvalidateRect(hWnd, &clientRect, TRUE); // 3. ���������� �� ������
			SetTimer(hWnd, 1, /*5000*/ 250, NULL); // 4. 5 ������ ����� ������ ������
		}
		else if (wParam == 3)
		{
			KillTimer(hWnd, 3);
			//MessageBeep(MB_OK);
			isRed = TRUE; isYellow = FALSE; isGreen = FALSE;
			InvalidateRect(hWnd, &clientRect, TRUE); // 7. ���������� �� �������
			SetTimer(hWnd, 2, /*5000*/ 250, NULL);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// ���������� ���������
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
