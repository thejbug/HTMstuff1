#ifndef SPRITE_H
#define SPRITE_H

#include "vector"
#include "qimage.h"
#include "fstream"
#include "json.hpp"
#include "bitarray.h"

class Sprite
{

public:
    std::vector<QImage*> frames;

    Sprite();

    Sprite(QString filename);

    static BitArray* toBinaryPixel(QImage* image);
    static QImage* fromBinaryPixel(BitArray* image,  int width, int height);
};

#endif // SPRITE_H
