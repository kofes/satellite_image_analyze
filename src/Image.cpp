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

satellite::Image::Image ( const satellite::Image& img ) {
  if ( img.pImage == nullptr || !img.iWidth || !img.iHeight ) {
    pImage = nullptr;
    iWidth = iHeight = 0;
    return;
  }

  iWidth = img.iWidth;
  iHeight = img.iHeight;

  pImage = new unsigned short*[iHeight];
  if (pImage == nullptr) {
    iWidth = iHeight = 0;
    return;
  }
  for (int i = 0; i < iHeight; ++i) {
    pImage[i] = new unsigned short[iWidth];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      iWidth = iHeight = 0;
      return;
    }
  }

  for (unsigned short i = 0; i < iHeight; ++i)
    for (unsigned short j = 0; j < iWidth; ++j)
      pImage[i][j] = img.pImage[i][j];
}

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

void satellite::Image::display ( unsigned short width, unsigned short height, unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy, unsigned short minColor, unsigned short maxColor ) {
  sf::RenderWindow window;
  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  sf::Image imgBuffer;
  sf::Texture imgTexture;
  sf::Sprite imgSprite;
  sf::View view;
  float coefficient;
  unsigned short MAX_COLOR;

  if ( pImage == nullptr || !dx || !dy) return;
  if (y0 + dy > iHeight) dy = iHeight - y0;
  if (x0 + dx > iWidth) dx = iWidth - x0;

  if (!height || height > desktopMode.height) height = desktopMode.height;
  if (!width || width > desktopMode.width) width = desktopMode.width;

  coefficient = std::max(dy/(double)height, dx/(double)width);
  if (coefficient < 1) coefficient = 1;

  std::cout << "coef: " << coefficient << std::endl;

  window.create(sf::VideoMode(width, height), "Image");
  window.setVerticalSyncEnabled(false);

  imgBuffer.create(dx, dy, sf::Color::Black);
  imgTexture.create(dx, dy);

  if (minColor >= maxColor)
    minColor = maxColor - 1 > 0 ? maxColor - 1 : 0;

  for (int i = 0; i < dy; ++i)
    for (int j = 0; j < dx; ++j)
      if (MAX_COLOR < pImage[i][j])
        MAX_COLOR = pImage[i][j];
  if (!maxColor)
    maxColor = MAX_COLOR;


  for (int i = 0; i < dy; ++i)
    for (int j = 0; j < dx; ++j) {
      unsigned short color;
      color = pImage[y0 + (dy - i - 1)][x0 + j];

      if (color < minColor) color = minColor;
      if (color > maxColor) color = maxColor;

      color = color / ((double)maxColor) * 255;
      imgBuffer.setPixel(j, i, sf::Color(color, color, color));
    }


  imgTexture.update(imgBuffer);
  imgSprite.setTexture(imgTexture);
  view.reset(sf::FloatRect(0, 0, width, height));
  char zoom = 0, maxZoom = 20, minZoom = -5;
  imgSprite.setOrigin(imgTexture.getSize().x/2, imgTexture.getSize().y/2);
  imgSprite.setScale(1/coefficient, 1/coefficient);
  imgSprite.setPosition(width/2, height/2);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      switch (event.type) {
        case (sf::Event::Closed) :
          window.close();
        break;
        case (sf::Event::Resized) :
          view.setSize(event.size.width,event.size.height);
          view.setCenter(event.size.width/2,event.size.height/2);
          imgSprite.setPosition(event.size.width/2,event.size.height/2);
        break;
        case (sf::Event::KeyPressed) :
          switch (event.key.code) {
            case (sf::Keyboard::Add) :
              if (zoom < maxZoom) {
                ++zoom;
                imgSprite.setScale((1 + zoom*0.1)/coefficient, (1 + zoom*0.1)/coefficient);
              }
            break;
            case (sf::Keyboard::Subtract) :
              if (zoom > minZoom) {
                --zoom;
                imgSprite.setScale((1 + zoom*0.1)/coefficient, (1 + zoom*0.1)/coefficient);
              }
            break;
            case (sf::Keyboard::Left) :
              if (view.getSize().x < imgSprite.getGlobalBounds().width)
                view.move(-20, 0);
              std::cout << "View width: " << view.getSize().x << ", View height: " << view.getSize().y << ", View center: " << view.getCenter().x << ":" << view.getCenter().y << std::endl;
            break;
            case (sf::Keyboard::Numpad9) :
              maxColor += 2;
              if (maxColor > MAX_COLOR)
                maxColor = MAX_COLOR;
            break;
            case (sf::Keyboard::Numpad3) :
              maxColor -= 2;
              if (maxColor < minColor || !maxColor || !(maxColor + 1))
                maxColor = minColor;
            break;
            case (sf::Keyboard::Numpad7) :
              minColor += 2;
              if (minColor > maxColor)
                minColor = maxColor;
            break;
            case (sf::Keyboard::Numpad1) :
              minColor -= 2;
              if (minColor > maxColor)
                minColor = 0;
            break;
            case (sf::Keyboard::Right) :
              if (view.getSize().x < imgSprite.getGlobalBounds().width)
                view.move(20, 0);
              std::cout << "View width: " << view.getSize().x << ", View height: " << view.getSize().y << ", View center: " << view.getCenter().x << ":" << view.getCenter().y << std::endl;
            break;
            case (sf::Keyboard::Up) :
              if (view.getSize().y < imgSprite.getGlobalBounds().height)
                view.move(0, -20);
              std::cout << "View width: " << view.getSize().x << ", View height: " << view.getSize().y << ", View center: " << view.getCenter().x << ":" << view.getCenter().y << std::endl;
            break;
            case (sf::Keyboard::Down) :
              if (view.getSize().y < imgSprite.getGlobalBounds().height)
                view.move(0, 20);
              std::cout << "View width: " << view.getSize().x << ", View height: " << view.getSize().y << ", View center: " << view.getCenter().x << ":" << view.getCenter().y << std::endl;
            break;
            case (sf::Keyboard::P) :
              std::cout << mousePosition.x - imgSprite.getGlobalBounds().left << ":" << mousePosition.y - imgSprite.getGlobalBounds().top/imgSprite.getScale().y << std::endl;
            break;
            case (sf::Keyboard::F) :
              view.setCenter(event.size.width/2,event.size.height/2);
              imgSprite.setPosition(event.size.width/2,event.size.height/2);
            break;
          }
        break;
      }
    }

    for (int i = 0; i < dy; ++i)
      for (int j = 0; j < dx; ++j) {
        unsigned short color;
        color = pImage[y0 + (dy - i - 1)][x0 + j];

        if (color < minColor) color = minColor;
        if (color > maxColor) color = maxColor;

        color = color / ((double)maxColor) * 255;
        imgBuffer.setPixel(j, i, sf::Color(color, color, color));
      }


    imgTexture.update(imgBuffer);
    imgSprite.setTexture(imgTexture);

    window.setView(view);
    window.clear(sf::Color::Black);
    window.draw(imgSprite);
    window.display();
  }
};

void satellite::Image::changeMaxMin ( unsigned short minColor, unsigned short maxColor ) {
  if ( pImage == nullptr || !iWidth || !iHeight )
    return;

  for (int i = 0; i < iHeight; ++i)
    for (int j = 0; j < iWidth; ++j) {
      if ( pImage[i][j] > maxColor )
        pImage[i][j] = maxColor;
      if ( pImage[i][j] < minColor )
        pImage[i][j] = minColor;
    }
};

void satellite::Image::binary ( unsigned short border ) {
  if ( pImage == nullptr || !iWidth || !iHeight )
    return;

  for (int i = 0; i < iHeight; ++i)
    for (int j = 0; j < iWidth; ++j) {
      if ( pImage[i][j] >= border )
        pImage[i][j] = 255;
      else
        pImage[i][j] = 0;
    }
}

void satellite::Image::copy ( unsigned short width, unsigned short height, unsigned short** src )  {
  if ( !width || !height || src == nullptr || *src == nullptr )
    return;

  for (unsigned short i = 0; i < iHeight; ++i)
    delete[] pImage[i];
  delete[] pImage;

  iWidth = width;
  iHeight = height;

  pImage = new unsigned short*[iHeight];
  if (pImage == nullptr) {
    iWidth = iHeight = 0;
    return;
  }
  for (int i = 0; i < iHeight; ++i) {
    pImage[i] = new unsigned short[iWidth];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      iWidth = iHeight = 0;
      return;
    }
  }

  for (int i = 0; i < iHeight; ++i)
    for (int j = 0; j < iWidth; ++j)
      pImage[i][j] = src[i][j];
};
