#include <iostream>
using namespace std;
#include <SDL.h>
int app_main()
{
    SDL_Surface* screen;
    SDL_Window* window;
    SDL_Surface* image;

    SDL_Init(SDL_INIT_VIDEO); // 初始化视频子系统
    // 创建一个窗体
    window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    // 获得一个与窗体关联的surface，赋值给screen
    screen = SDL_GetWindowSurface(window);
    image = SDL_LoadBMP("box.bmp"); // 加载一个bmp图片文件，并把surface赋值给image
    SDL_BlitSurface(image, NULL, screen, NULL); // 将image中的的数据拷贝到screen中，相当于直接显示
    SDL_FreeSurface(image); // image数据以经拷贝出去，已经失去价值
    SDL_UpdateWindowSurface(window); // 刷新窗体，让与窗体关联的screen中的数据能够显示出来。
    // show image for 2 seconds
    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}