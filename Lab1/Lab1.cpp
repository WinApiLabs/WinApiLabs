#include<Windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>
#include<vector>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // ���������� ���������� ���������� 
	HINSTANCE Prev, 	// � ����������� �������� ������ 0
	LPTSTR cmd, 		// ��������� ������
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
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// ���������� ���� ����� ������ 
	wc.hbrBackground = CreateSolidBrush(RGB(153, 255, 153)); // ������-��������� ����

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

RECT clientRect;
BOOL isLKM;
BOOL isPKM;
BOOL isFirst;
std::vector<int> v;
int arr[200];
int n;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		 // ���������� ���������
	{
	case WM_CREATE:
		n = 0;
		break;
	case WM_PAINT:
	{
			PAINTSTRUCT ps;
			GetClientRect(hWnd, &clientRect); // �������� � clientRect ���������� ���������� ������� ����, �� ������� ��������� hWnd
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
		break; 			// ���������� ���������
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}