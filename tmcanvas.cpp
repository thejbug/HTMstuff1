#include "tmcanvas.h"
#include "qpainter.h"

TMCanvas::TMCanvas(QWidget* parent):
    QLabel(parent)
{
    setScaledContents(true);
}

void TMCanvas::setup(TemporalMemory *t)
{
    tm = t;


    top = 100;
    centerX = 25 + (width() - 50) / 2;
    bottom = top + (tm->height() * cellWidth);

    imageScale = 2.0f;
    imageWidth = width() * imageScale;
    imageHeight = height() * imageScale;

    cellWidth = (width() - 150) / tm->width();
    cellWidth = cellWidth * imageScale;
    cellHeight = 15 * imageScale;

    center = Point(imageWidth / 2, imageHeight / 2);

    backgroundImage = QImage("/Users/jaredemery/Desktop/QTprojects/darkGarage.png").scaled(imageWidth,imageHeight);
}

void TMCanvas::drawNetwork()
{
    drawCells(0, center);
    setPixmap(QPixmap::fromImage(workingImage));
}

Point TMCanvas::cellPos(Point tmPos, unsigned int cellIndex)
{
    int col = cellIndex / tm->height();
    int cell = cellIndex % tm->height();

    int numCells = tm->width();
    int margin = 2;
    int cellSpace = cellWidth + 2 * margin;
    int totalWidth = numCells * cellSpace;
    int startX = tmPos.x - totalWidth / 2;

    int colHeight = cellWidth * tm->height();
    int startY = tmPos.y - colHeight;

    int x = startX + col * cellSpace + margin + cellWidth / 2;
    int y = startY + cell * cellWidth + cellWidth / 2;

    return Point(x,y);
}

void TMCanvas::drawCells(unsigned int selectedCell, Point pos)
{
    workingImage = backgroundImage;

    drawDistal(pos);

    QPainter painter(&workingImage);
    QPen pen(QColor(180,180,180));
    pen.setWidth(2);
    painter.setPen(pen);

    int numCells = tm->width();
    int margin = 2;
    int cellSpace = cellWidth + 2 * margin;
    int totalWidth = numCells * cellSpace;
    int startX = pos.x - totalWidth / 2;

    int colHeight = cellWidth * tm->height();
    int startY = pos.y - colHeight;

    BitArray* activeCols = tm->sp_->activity();
    BitArray* active = tm->active();
    BitArray* prediced = tm->prediced();
    BitArray* winners = tm->winners();

    for(unsigned int col = 0; col < tm->width(); col ++)
    {
        int xOffset = col * cellSpace;
        if(activeCols->at(col))
        {
            pen.setColor(QColor(250, 150, 0));
            pen.setWidth(4);
            painter.setPen(pen);
            painter.setBrush(QColor(25,25,25));
            painter.drawRoundedRect(startX + xOffset, startY - 2, cellSpace, colHeight + 4, cellSpace / 2, cellSpace / 2);
        }

        xOffset += margin;
        for(unsigned int cell = 0; cell < tm->height(); cell ++)
        {
            int yOffset = cellWidth * cell;
            unsigned int cellIndex = col * tm->height() + cell;
            bool a = active->at(cellIndex);
            bool p = prediced->at(cellIndex);
            if(a | p)
            {
                pen.setWidth(4);
                pen.setColor(QColor(255,150,0));
            }
            else
            {
                pen.setWidth(2);
                pen.setColor(QColor(180,180,180));
            }
            if(a)
            {
                painter.setBrush(QColor(255,150,0));
            }
            else
            {
                painter.setBrush(QColor(25,25,25));
            }
            painter.setPen(pen);
            painter.drawEllipse(startX + xOffset, startY + yOffset, cellWidth, cellWidth);

            //painter.setBrush(QColor(0,0,255));
            //Point cellpos = cellPos(pos, cellIndex);
            //painter.drawEllipse(cellpos.x, cellpos.y, cellWidth, cellWidth);

            if(winners->at(cellIndex))
            {
                //painter.setBrush(Qt::color0);
                //painter.drawEllipse()
            }

        }
    }
}

void TMCanvas::drawDistal(Point pos)
{
    QPainter painter(&workingImage);
    painter.setPen(QColor(180,180,180));

    for(unsigned int cell = 0; cell < tm->height() * tm->width(); cell ++)
    {
        Neuron* n = tm->neurons[cell];
        for(unsigned int d = 0; d < n->distal->size(); d ++)
        {
            Dendrite* dend = n->distal->at(d);
            if(dend->active_)
            {
                painter.setPen(QColor(255,150,0));
            }
            else
            {
                painter.setPen(QColor(255,255,255));
            }
            for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
            {
                unsigned int inputIndx = dend->potentialIndices->at(s);

                Point p1 = cellPos(pos, cell);
                Point p2 = cellPos(pos, inputIndx);

                painter.drawLine(p1.x, p1.y, p2.x, p2.y);
            }
        }
    }
}

void TMCanvas::mouseMoveEvent(QMouseEvent *ev)
{
    BitArray* input= new BitArray(50);
    input->randomize();

    tm->step();
    tm->computePredictions(tm->active(-1));
    tm->computeFeedForward(input);
    tm->fitTM(tm->active(-1), tm->winners(-1));

    std::cout << "synapses: " << tm->countSynapses() << std::endl;

    drawCells(rand() % tm->width(), center);
    setPixmap(QPixmap::fromImage(workingImage));
}

void TMCanvas::mousePressEvent(QMouseEvent *ev)
{

}
