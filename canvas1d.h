#ifndef CANVAS1D_H
#define CANVAS1D_H

#include "qlabel.h"
#include "spatialpooler.h"

struct Point
{
public:
    int x, y;
    Point(): x(0), y(0) {}
    Point(int a, int b): x(a), y(b) {}
};


class Canvas1D: public QLabel
{
    Q_OBJECT

    QImage workingImage;
    SpatialPooler* spatialPooler;

    Point center;
    float imageScale;
    int cellWidth;


public:
    Canvas1D(QWidget* parent = nullptr);

    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

    void drawNetwork();
    void drawCells(BitArray* cells, Point pos);
};

#endif // CANVAS1D_H
