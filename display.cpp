#include "Passport/Passport.hpp"
#include "Image/Image.hpp"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

using namespace satellite;

int main () {
  std::ifstream file("../2016-01-01.NOAA_19.pro", std::ios::in | std::ios::binary );
  passport::Proection pass;
  Image img;
  file >> pass;

  std::cout << pass << std::endl;

  std::cout << "Количество пикселей в строке: " << pass.getCountPixelsInLine() << '\n' << "Количество строк: " << pass.getCountLines() << '\n' << "Номер канала: " << pass.getChannelNum() << std::endl;

  img.read(pass.getCountPixelsInLine(), pass.getCountLines(), file);

  /*=========================================================================*/
    SDL_DisplayMode displayMode;
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window* window;
  /*=========================================================================*/
    //initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
      return 1;
    }
    std::cout << "SDL Init'ed" << std::endl;
  /*=========================================================================*/
    int request = SDL_GetDesktopDisplayMode(0, &displayMode);
  /*=========================================================================*/
    double coefficientPictureWH = img.width() / (double)(img.height());
    //Set height/width to Window
    int height = 606;//1830/3 = 610
    int width = 479;//1440/3 = 480
  /*=========================================================================*/
    window = SDL_CreateWindow("tmp", 0, 0, width, height, SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/);
    if (window == NULL) {
      std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
      return 1;
    }
    std::cout << "SDL window created" << std::endl;
  /*=========================================================================*/
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
      std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
      return 1;
    }
    std::cout << "SDL renderer init'ed" << std::endl;
  /*=========================================================================*/
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    int maxColor = 0;
    for (int i = 0; i < height; ++i)
      for (int j = 0; j < width; ++j) {
        int color;
        char l, k;
        if (3*i + 3 >= img.height() || 3*j + 3 >= img.width())
        color = 0;
        else {
          for (k = 0, color = 0; k < 3; ++k)
          for (l = 0; l < 3; ++l)
          color += img[3*i + k][3*j + l];
          color = (int)(color / k*l);
        }
        if (maxColor < color)
          maxColor = color;
      }
    int setedMax = maxColor, setedMin = 0;
  /*print*/
    while (true) {
      if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        break;
      for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j) {
          int color;
          char l, k;
          if (3*i + 3 >= img.height() || 3*j + 3 >= img.width())
          color = 0;
          else {
            for (k = 0, color = 0; k < 3; ++k)
            for (l = 0; l < 3; ++l)
            color += img[3*i + k][3*j + l];
            color = (int)(color / k*l);
          }
          color -= setedMin;
          if (color > setedMax)
          color = setedMax;
          if (color < 0)
          color = 0;
          color = color / ((double)(setedMax)) * 256;
          SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
          SDL_RenderDrawPoint(renderer, j, height - i);
        }
      SDL_RenderPresent(renderer);
    }
  /*=========================================================================*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  /*=========================================================================*/

  file.close();
  return 0;
}
