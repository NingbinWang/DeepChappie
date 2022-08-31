#include <iostream>
using namespace std;
#include <SDL.h>
int app_main()
{
    SDL_Surface* screen;
    SDL_Window* window;
    SDL_Surface* image;

    SDL_Init(SDL_INIT_VIDEO); // ��ʼ����Ƶ��ϵͳ
    // ����һ������
    window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    // ���һ���봰�������surface����ֵ��screen
    screen = SDL_GetWindowSurface(window);
    image = SDL_LoadBMP("box.bmp"); // ����һ��bmpͼƬ�ļ�������surface��ֵ��image
    SDL_BlitSurface(image, NULL, screen, NULL); // ��image�еĵ����ݿ�����screen�У��൱��ֱ����ʾ
    SDL_FreeSurface(image); // image�����Ծ�������ȥ���Ѿ�ʧȥ��ֵ
    SDL_UpdateWindowSurface(window); // ˢ�´��壬���봰�������screen�е������ܹ���ʾ������
    // show image for 2 seconds
    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}