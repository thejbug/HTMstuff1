#include "sprite.h"

Sprite::Sprite()
{

}

using json = nlohmann::json;

Sprite::Sprite(QString filename)
{
    std::ifstream filestream;
    json j;

    filestream.open(filename.toStdString());
    if(filestream.fail())
        return;
    filestream >> j;
    filestream.close();


    //loop through the frames
    for(int frameCounter = 0; frameCounter < j["numberOfFrames"].get<int>(); frameCounter++)
    {
        QImage* frame = new QImage(j["width"].get<ushort>(), j["height"].get<ushort>(), QImage::Format_RGB32);
        for(int row = 0; row < frame->height(); row++)
        {
          for(int col = 0; col < frame->width(); col++)
          {
              json color = j["frames"]["frame"+std::to_string(frameCounter)][row][col];
              QColor c(color[0], color[1], color[2]);
              frame->setPixelColor(col, row, c);
          }
        }

        frames.push_back(frame);
    }
}

BitArray* Sprite::toBinaryPixel(QImage *image)
{
    BitArray* encoding = new BitArray(image->width() * image->height());
    for(int row = 0; row < image->height(); row ++)
    {
        for(int col = 0; col < image->width(); col ++)
        {
            unsigned int index = row * image->width() + col;

            if(image->pixelColor(col, row).red() > 150)
            {
                encoding->set(index, true);
            }
        }
    }
    return encoding;
}

QImage* Sprite::fromBinaryPixel(BitArray *image, int width, int height)
{
    QImage* img = new QImage(width, height, QImage::Format_RGB32);
    for(int row = 0; row < height; row ++)
    {
        for(int col = 0; col < width; col ++)
        {
            unsigned int index = row * width + col;

            if(image->at(index))
            {
                img->setPixelColor(col, row, QColor(255,255,255));
            }
            else
            {
                img->setPixelColor(col, row, QColor(0,0,0));
            }
        }
    }
    return img;
}

