#include <Windows.h>
#include "WinUser.h"
#include <iostream>
#include <time.h>

#include "keyboard.h"

using namespace std;

// 某款机械键盘，频繁出现按一次，响应多次的情况。
// 这里不提名
// 我的键盘因为超出了包退期，也超出了包换期，要自己寄过去修，需要多花钱
// 所以我试着写了这个程序
// 程序大部分都是从其他地方抄来的
//
// 原理：
// 如果是类似于长按导致的重复触发，可以通过设置重复速率来避免。
// 但该款键盘，应该是电路或其他的地方的问题，按一次后，触发多次。
// 本来 win 自带筛选键功能里有误触发筛选功能，但最短过滤时长竟然是 0.5 秒！
// 除非我击键小于 2下/秒，这样设置才不会对我的正常使用产生影响。
// 所以，我写了这个过滤工具，你可以设置自己的击键。
// 本程序运行后，会帮你屏蔽超过该击键的重复触发。

// ============== 下面是程序部分 ===============

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;
clock_t start, finish;
double during;
DWORD last_code = 0;

double delay_time = 100.0;


// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

void ReleaseHook() {
    UnhookWindowsHookEx(_hook);
}

// This is the callback function. Consider it the event that is raised when, in this case,
// a key is pressed.
//LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)

{
    if (nCode >= 0) {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN) {
            // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            // a key (non-system) is pressed.
            //cout << kbdStruct.vkCode << endl;
            finish  = clock();
            during = double(finish - start);
            if (during < delay_time && kbdStruct.vkCode == last_code) {
                cout << "Repeat key : 0x" << hex << last_code << "! Deny! keyboard double click in:" << during << "ms" << endl;
                return 1;
            }
            start = clock();
            last_code = kbdStruct.vkCode;
        }
    }


    // call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
    // Set the hook and set it to use the callback function above
    // WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
    // The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
    // function that sets and releases the hook. If you create a hack you will not need the callback function
    // in another place then your own code file anyway. Read more about it at MSDN.
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0))) {
        MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
    }
}

void UpdateKeyPressRate(int pressRate) {
    if (pressRate <= 0) {
        return;
    }

    delay_time = double(1000 / (double)pressRate);
}


int Setup() {
    start = clock();

    cout << "start monitor" << endl;

    // Set the hook
    SetHook();

    // Don't mind this, it is a meaningless loop to keep a console application running.
    // I used this to test the keyboard hook functionality. If you want to test it, keep it in ;)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {

    }
    return 0;
    //while (true)
    //{
    //}
}
