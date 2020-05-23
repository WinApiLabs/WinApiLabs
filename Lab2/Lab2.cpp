#include<Windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame1");

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
		_T("������ Windows-����������1"), 		// ��������� ���� 
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
	switch (message)		 // ���������� ���������
	{
	case WM_CREATE:
		isFinished = TRUE;
		break;
	case WM_LBUTTONDOWN:
		if (isFinished)
			if (hWnd2 = FindWindow(_T("MainFrame2"), _T("������ Windows-����������2")))
			{
				InvalidateRect(hWnd, &clientRect, TRUE);
				isFinished = FALSE;
				MessageBox(hWnd, _T("������ ���� �������. ������ ����� ������� ����������."), _T("������ 2-�� ����"), MB_OK);
				if (MessageBox(hWnd, _T("���������� �������. ������, ����� ���� ������������� ���������?"), _T("������ ��������"), MB_YESNO) == IDYES)
				{
					SendMessage(hWnd2, WM_USER + 2, (WPARAM)hWnd, NULL);
				}
				else
				{
					SendMessage(hWnd2, WM_USER + 1, (WPARAM)hWnd, NULL);
				}
			}
			else
				MessageBox(hWnd, _T("������ ���� �������."), _T("������ 2-�� ����"), MB_OK);
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
		break; 			// ���������� ���������
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



