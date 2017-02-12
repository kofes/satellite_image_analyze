#include "Image.hpp"

satellite::Image::Image ( short width, short height, const std::string& fileName ) {
  std::ifstream file;

  if (width < 0 || height < 0) return;

  file.open(fileName);
  if (!file.is_open()) return;

  pImage = new unsigned short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (int i = 0; i < height; ++i) {
    pImage[i] = new unsigned short[width];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      file.close();
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (int i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);

  file.close();
};

satellite::Image::~Image () {
  if (pImage != nullptr) {
    for (int i = 0; i < iHeight; ++i)
      delete[] pImage[i];
    delete[] pImage;
  }
  iWidth = iHeight = 0;
}

void satellite::Image::read ( short width, short height, std::ifstream& file ) {
  if (width < 0 || height < 0) return;

  if (pImage != nullptr) {
    for (int i = 0; i < iHeight; ++i)
      delete[] pImage[i];
    delete[] pImage;
  }
  iWidth = iHeight = 0;

  pImage = new unsigned short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (int i = 0; i < height; ++i) {
    pImage[i] = new unsigned short[width];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (int i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);
}

void satellite::Image::display ( unsigned short width, unsigned short height, unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy ) {
  SDL_DisplayMode displayMode;
  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window* window;
  short coefficient;

  if (!width || !height || !dx || !dy) return;

  coefficient = std::max(dy/(double)height, dx/(double)width);
  if (coefficient < 1 + 1e-5)
    coefficient = 1;

  if (SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    return;
  }

  window = SDL_CreateWindow("picture", 0, 0, width, height, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);

  int maxColor = 0;

  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j) {
      int color;
      color = 0;
      if (coefficient*i + coefficient < dy && coefficient*j + coefficient < dx) {
        for (char k = 0; k < coefficient; ++k)
        for (char l = 0; l < coefficient; ++l)
          color += pImage[y0 + coefficient*i + k][x0 + coefficient*j + l];
        color /= coefficient*coefficient;
      }
      if (maxColor < color)
        maxColor = color;
    }

  int setedMax = maxColor, setedMin = 0;

  while (true) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;

    for (int i = 0; i < height; ++i)
      for (int j = 0; j < width; ++j) {
        int color;
        color = 0;
        if (coefficient*i + coefficient < dy && coefficient*j + coefficient < dx) {
          for (char k = 0; k < coefficient; ++k)
          for (char l = 0; l < coefficient; ++l)
            color += pImage[y0 + coefficient*i + k][x0 + coefficient*j + l];
          color /= coefficient*coefficient;
        }
        color -= setedMin;
        if (color > setedMax)
          color = setedMax;
        if (color < 0)
          color = 0;
        color = color / ((double)setedMax) * 256;
        SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
        SDL_RenderDrawPoint(renderer, j, height - i);
      }
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
