#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <string>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

TCHAR classname[] = TEXT("Lab4");
TCHAR title[] = TEXT("Lab4");

HANDLE shapes_thread;
DWORD WINAPI shapes_thread_proc(void*);

Font* font = 0;
HANDLE bench_thread;
DWORD WINAPI bench_thread_proc(void*);

HANDLE shapes_start_event, bench_start_event, bench_end_event;

HBITMAP window_bitmap = 0;
HDC window_memdc = 0;

int window_wid, window_hei;
bool alt = false;
DWORD t1 = 0, t2 = 0;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		shapes_start_event = CreateEvent(0, FALSE, FALSE, TEXT("ShapesStartEvent"));
		bench_start_event = CreateEvent(0, FALSE, FALSE, TEXT("BenchStartEvent"));
		bench_end_event = CreateEvent(0, FALSE, FALSE, TEXT("BenchEndEvent"));

		shapes_thread = CreateThread(0, 0, shapes_thread_proc, 0, 0, 0);
		bench_thread = CreateThread(0, 0, bench_thread_proc, 0, 0, 0);

		font = new Font(FontFamily::GenericMonospace(), 40, FontStyleRegular, UnitPixel);

		SetTimer(hWnd, 0, 4000, 0);
		break;
	}
	case WM_DESTROY:
	{
		TerminateThread(shapes_thread, 0);
		TerminateThread(bench_thread, 0);

		CloseHandle(shapes_start_event);
		CloseHandle(bench_start_event);
		CloseHandle(bench_end_event);

		if (window_memdc) DeleteDC(window_memdc);
		if (window_bitmap) DeleteObject(window_bitmap);

		if (font) delete font;

		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		window_wid = LOWORD(lParam);
		window_hei = HIWORD(lParam);

		if (window_memdc) DeleteDC(window_memdc);
		if (window_bitmap) DeleteObject(window_bitmap);

		HDC dc = GetDC(hWnd);

		window_memdc = CreateCompatibleDC(dc);
		window_bitmap = CreateCompatibleBitmap(dc, window_wid, window_hei);
		SelectObject(window_memdc, window_bitmap);

		ReleaseDC(hWnd, dc);
		break;
	}
	case WM_TIMER:
	{
		alt = !alt;
		InvalidateRect(hWnd, 0, FALSE);
		break;
	}
	case WM_PAINT:
	{
		SetEvent(shapes_start_event);
		WaitForSingleObject(bench_end_event, INFINITE);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, window_wid, window_hei, window_memdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return TRUE;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD WINAPI shapes_thread_proc(void*)
{
	for (;;)
	{
		WaitForSingleObject(shapes_start_event, INFINITE);
		Graphics graphics(window_memdc);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.Clear(Color(255, 255, 255));
		SolidBrush br(Color(alt ? 255 : 0, 0, alt ? 0 : 255));
		int min_x = window_wid / 2;
		int min_y = window_hei / 4;
		int dim = 2 * (window_wid < (window_hei / 2) ? window_wid : (window_hei / 2)) / 3;
		if (alt) graphics.FillRectangle(&br, Rect(min_x - dim / 2, min_y - dim / 2, dim, dim));
		else graphics.FillEllipse(&br, Rect(min_x - dim / 2, min_y - dim / 2, dim, dim));
		SetEvent(bench_start_event);
	}
}

DWORD WINAPI bench_thread_proc(void*)
{
	for (;;)
	{
		WaitForSingleObject(bench_start_event, INFINITE);
		t1 = timeGetTime();
		Graphics graphics(window_memdc);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		const std::wstring& s = std::to_wstring(t2 ? t1 - t2 : 0);

		RectF box{ 0, 0, 0, 0 };
		graphics.MeasureString(s.c_str(), s.length(), font,
			RectF(0, 0, window_wid, window_hei / 2), &box);

		SolidBrush text_brush(Color::Black);
		graphics.DrawString(s.c_str(), s.length(), font,
			RectF(window_wid / 2 - box.Width / 2, 3 * window_hei / 4 - box.Height / 2, box.Width, box.Height),
			StringFormat::GenericDefault(), &text_brush);
		SetEvent(bench_end_event);
		t2 = timeGetTime();
	}
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{

	GdiplusStartupInput gdiplusStartupInput;
	gdiplusStartupInput.GdiplusVersion = 1;

	ULONG_PTR gdiplusToken;
	if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, 0) != Ok) return 1;

	WNDCLASSEX wc{ };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = classname;
	wc.hIcon = 0;
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassEx(&wc))
	{
		GdiplusShutdown(gdiplusToken);
		return 1;
	}

	HWND hwnd = CreateWindowEx(0, classname, title,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 640, 0, 0, 0, 0);
	if (!hwnd)
	{
		GdiplusShutdown(gdiplusToken);
		UnregisterClass(classname, hInst);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	UnregisterClass(classname, hInst);
	return 0;
}