// Made by RequestFX#1541

#include <Windows.h>
#include <iostream>

HHOOK hook; // Mouse event hook

LRESULT WINAPI LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam; // Contains information about a low-level mouse input event.

	if (nCode >= 0) {
		switch (wParam) {

		case WM_MOUSEMOVE: {
			POINT cursorPos;
			GetCursorPos(&cursorPos); // get current Mouse Position

			// calc the next Mouse Position and reverse it
			int deltaX = pMouseStruct->pt.x - cursorPos.x;
			int deltaY = pMouseStruct->pt.y - cursorPos.y;
			SetCursorPos(pMouseStruct->pt.x - deltaX * 2, pMouseStruct->pt.y - deltaY * 2);
			return -1; // return here to cancel the passed mouse movement
		}
		}
	}
	/* If nCode is less than zero, the hook procedure must pass the message to the CallNextHookEx function
	and should return the value returned by CallNextHookEx. */
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE); // dont show Console

	hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0); // initial hook
	if (!hook) {
		std::cout << "Failed to initialize Hook!\n";
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1); // im not sure if I should put this sleep in

		// SHIFT+CTRL+ALT+DEL_KEY to exit
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000 &&
			GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000) break;
	}

	UnhookWindowsHookEx(hook); // clear our hook instance
	return 0;
}