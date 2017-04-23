#include "../inc/Display.hpp"

void satellite::display ( const Image& img, unsigned short width, unsigned short height, unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy, unsigned short minColor, unsigned short maxColor ) {
  sf::RenderWindow window;
  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  sf::Image imgBuffer;
  sf::Texture imgTexture;
  sf::Sprite imgSprite;
  sf::View view;
  float coefficient;
  unsigned short MAX_COLOR;

  if ( img.width() == 0 || !dx || !dy) return;
  if (y0 + dy > img.height()) dy = img.height() - y0;
  if (x0 + dx > img.width()) dx = img.width() - x0;

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
      if (MAX_COLOR < img[i][j])
        MAX_COLOR = img[i][j];
  if (!maxColor)
    maxColor = MAX_COLOR;


  for (int i = 0; i < dy; ++i)
    for (int j = 0; j < dx; ++j) {
      short color;
      color = img[y0 + (dy - i - 1)][x0 + j];
      if (color >= 0) {
        if (color < minColor) color = minColor;
        if (color > maxColor) color = maxColor;
        color = color / ((double)maxColor) * 255;
        imgBuffer.setPixel(j, i, sf::Color(color, color, color));
      } else
        imgBuffer.setPixel(j, i, sf::Color(0, 0, 255));
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
            default: break;
          }
        break;
      }
    }

    for (int i = 0; i < dy; ++i)
      for (int j = 0; j < dx; ++j) {
        short color;
        color = img[y0 + (dy - i - 1)][x0 + j];
        if (color >= 0) {
          if (color < minColor) color = minColor;
          if (color > maxColor) color = maxColor;
          color = color / ((double)maxColor) * 255;
          imgBuffer.setPixel(j, i, sf::Color(color, color, color));
        } else
          imgBuffer.setPixel(j, i, sf::Color(0, 0, 255));
      }


    imgTexture.update(imgBuffer);
    imgSprite.setTexture(imgTexture);

    window.setView(view);
    window.clear(sf::Color::Black);
    window.draw(imgSprite);
    window.display();
  }
};
