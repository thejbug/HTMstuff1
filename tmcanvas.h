#ifndef TMCANVAS_H
#define TMCANVAS_H

#include "qlabel.h"
#include "temporalmemory.h"
#include "canvas1d.h"

class TMCanvas: public QLabel
{
    Q_OBJECT

    TemporalMemory* tm;

    unsigned int cellWidth;
    unsigned int cellHeight;
    unsigned int top;
    unsigned int centerX;
    unsigned int bottom;
    unsigned int imageWidth;
    unsigned int imageHeight;

    QImage backgroundImage;
    QImage workingImage;
    float imageScale;
    Point center;

public:

    TMCanvas(QWidget* parent);

    void setup(TemporalMemory* t);

    void drawNetwork();
    void drawCells(unsigned int selectedCell, Point pos);
    Point cellPos(Point tmPos, unsigned int cellIndex);
    void drawDistal(Point pos);
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // TMCANVAS_H
