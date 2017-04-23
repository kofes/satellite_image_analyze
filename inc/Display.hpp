#pragma once

#include <SFML/Graphics.hpp>
#include "Image.hpp"

namespace satellite {

void display ( const Image& img, unsigned short width, unsigned short height, unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy, unsigned short minColor = 0, unsigned short maxColor = 300 );

};
