#include "windowhelper.h"

#include <iostream>
#include <windows.h>
#include <QDebug>
#include <vector>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

QString set_focus_pattern = "";

// static
void WindowHelper::SetFocus(const QString &pattern)
{
    set_focus_pattern = pattern;
    EnumWindows(EnumWindowsProc, NULL);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM)
{
    const int size = 256;
    wchar_t buffer[size];

    GetWindowText(hwnd, buffer, size);

    QString title = QString::fromWCharArray(buffer, GetWindowTextLength(hwnd));

    if (title.contains(set_focus_pattern)) {
        bool res = SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        qDebug() << "Set focus to" << set_focus_pattern << ", res:" << res;
        return false;
    }

    return true;
}
