// https://stackoverflow.com/a/68483943/6074780
// https://stackoverflow.com/a/22419083/6074780
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <iostream>
#include <string>
#include <Windows.h>


int* screen_shot(int& width, int& height) {
    HDC hdc = GetDC(NULL); // get the desktop device context
    HDC cdc = CreateCompatibleDC(hdc); // create a device context to use yourself
    height = (int)GetSystemMetrics(SM_CYVIRTUALSCREEN); // get the width and height of the screen
    width  = 16*height/9; // only capture left monitor for dual screen setups, for both screens use (int)GetSystemMetrics(SM_CXVIRTUALSCREEN);
    HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height); // create a bitmap
    SelectObject(cdc, hbitmap); // use the previously created device context with the bitmap
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = width;
    bmi.biHeight = -height; // flip image upright
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 3*width*height;
    BitBlt(cdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY); // copy from desktop device context to bitmap device context
    ReleaseDC(NULL, hdc);
    int* image = new int[width*height];
    GetDIBits(cdc, hbitmap, 0, height, image, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    DeleteObject(hbitmap);
    DeleteDC(cdc);
    return image;
}


void send_shift()
{
    INPUT ip;

    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "shift" key
    ip.ki.wVk = 0x10; // virtual-key code for the "shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "shift" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}


int main(int argc, char* argv[])
{
    if (argc != 5 && argc != 6)
    {
        std::cout << "Must give 4 or 5 parameters." << std::endl;
        return -1;
    }
    int width=0, height=0;
    int* image = screen_shot(width, height);

    int startx = std::stoi(argv[1]);
    int starty = std::stoi(argv[2]);
    int endx = std::stoi(argv[3]);
    int endy = std::stoi(argv[4]);

    long long int count = 0;

    for (int j = starty; j < endy; ++j)
    {
        for (int i = startx; i < endx; ++i)
        {
            count += image[i + width * j];
        }
    }
    delete[] image;

    if (argc == 5)
        std::cout << count << std::endl;
    else if (count == std::stoi(argv[5]))
    {
        send_shift();
        std::cout << 2 << std::endl;
    }
    else
        std::cout << 1 << std::endl;

    return 0;
}
