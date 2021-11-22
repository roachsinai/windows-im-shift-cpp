// https://stackoverflow.com/a/68483943/6074780
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <iostream>
#include <map>
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

    std::map<int, int> m;

    for (int j = starty; j < endy; ++j)
    {
        for (int i = startx; i < endx; ++i)
        {
            ++m[image[i + width * j]];
        }
    }
    delete[] image;

    if (argc == 5)
        std::cout << m.size() << std::endl;
    else if (m.size() <= std::stoi(argv[5]))
        std::cout << 2 << std::endl;
    else
        std::cout << 1 << std::endl;

    return 0;
}
