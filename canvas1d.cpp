#include "canvas1d.h"
#include "qpainter.h"
#include "iostream"

using namespace std;

Canvas1D::Canvas1D(QWidget* parent):
    QLabel(parent)
{
    int labw = 800;
    int labh = 600;
    setGeometry(50,50, labw, labh);
    imageScale = 2.0f;
    cellWidth = 15 * imageScale;

    center = Point(labw * imageScale / 2, labh * imageScale / 2);

    BitArray input(25);
    //THE BIG BUG
    //spatialPooler = new SpatialPooler(50, input, 0.15f);
    spatialPooler = nullptr;
}

void Canvas1D::drawNetwork()
{
    float imageScale = 2.0f;
    int w = width() * imageScale;
    int h = height() * imageScale;
    workingImage = QImage(w, h, QImage::Format_RGB32);
    workingImage.fill(QColor(220, 235, 252));

    BitArray* input = new BitArray(50);
    input->randomize();


    if(!spatialPooler)
    {
        spatialPooler = new SpatialPooler(50, *input, 0.15f);
    }
    spatialPooler->computeOverlap(input);
    spatialPooler->computeKWinners();
    spatialPooler->fit(input);

    drawCells(spatialPooler->activity(), Point(center.x, 200));

    drawCells(input, Point(center.x, 750));


    setPixmap(QPixmap::fromImage(workingImage));
    setScaledContents(true);
}

void Canvas1D::drawCells(BitArray* cells, Point pos)
{
    QPainter painter(&workingImage);
    painter.setPen(QColor(0,0,0));

    int numCells = cells->size();
    int totalWidth = numCells * cellWidth;
    int startX = pos.x - totalWidth / 2;
    for(int i = 0; i < numCells; i ++)
    {
        int offset = i * cellWidth;
        if(cells->at(i))
        {
            painter.setBrush(QColor(255,150,0));
        }
        else
        {
            painter.setBrush(QColor(255,255,255));
        }
        painter.drawRect(startX + offset, pos.y, cellWidth, cellWidth);
    }
}

void Canvas1D::mouseMoveEvent(QMouseEvent *ev)
{
    drawNetwork();
}

void Canvas1D::mousePressEvent(QMouseEvent *ev)
{

}

void Canvas1D::mouseReleaseEvent(QMouseEvent *ev)
{

}
