#include "canvas1d.h"
#include "qpainter.h"
#include "iostream"
#include "qtimer.h"

using namespace std;

Canvas1D::Canvas1D(QWidget* parent):
    QLabel(parent)
{
    int labw = 800;
    int labh = 550;
    setGeometry(50,100, labw, labh);
    imageScale = 2.0f;
    cellWidth = 5 * imageScale;

    center = Point(labw * imageScale / 2, labh * imageScale / 2);

    //THE BIG BUG
    //spatialPooler = new SpatialPooler(50, input, 0.15f);
    spatialPooler = nullptr;
    input = nullptr;
}

void Canvas1D::drawNetwork()
{
    float imageScale = 2.0f;
    int w = width() * imageScale;
    int h = height() * imageScale;
    workingImage = QImage(w, h, QImage::Format_RGB32);
    workingImage.fill(QColor(220, 235, 252));

    Point inputPos(center.x, 750);
    Point spPos(center.x, 200);

    drawSynapses(spPos, inputPos);

    drawCells(spatialPooler->activity(), spPos);

    drawCells(input, inputPos);


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

void Canvas1D::drawSynapses(Point spPos, Point inPos)
{
    QPainter painter(&workingImage);
    QPen pen;
    pen.setColor(QColor(0,0,0));

    BitArray* output = spatialPooler->activity();

    int spWidth = spatialPooler->size() * cellWidth;
    int spStartX = spPos.x - spWidth / 2 + cellWidth / 2;

    int inWidth = input->size() * cellWidth;
    int inStartX = inPos.x - inWidth / 2 + cellWidth / 2;
    for(int n = 0; n < spatialPooler->size(); n ++)
    {
        int spOffset = n * cellWidth;
        Dendrite* dend = spatialPooler->neurons()[n]->getProximalDendrite();
        auto perms = dend->getPermanences();
        BitArray cons = dend->getConnections();

        for(int p = 0; p < perms.size(); p ++)
        {
            if(rand() % 100 > 15)
            {
                continue;
            }
            int inOffset = p * cellWidth;
            if(output->at(n) && input->at(p))
            {
                pen.setColor(QColor(255, 150, 0));
            }
            else
            {
                pen.setColor(QColor(0,0,0));
            }
            if(cons[p])// && output->at(n))
            {
                int penw = perms[p] / 75;
                pen.setWidth(penw);
                painter.setPen(pen);
                painter.drawLine(spStartX + spOffset, spPos.y + cellWidth,
                                 inStartX + inOffset, inPos.y);
            }

        }
    }
}

void Canvas1D::animateFeedForward(Point spPos, Point inPos)
{


    BitArray* output = spatialPooler->activity();

    int spWidth = spatialPooler->size() * cellWidth;
    int spStartX = spPos.x - spWidth / 2 + cellWidth / 2;

    int inWidth = input->size() * cellWidth;
    int inStartX = inPos.x - inWidth / 2 + cellWidth / 2;

    int spY = spPos.y + cellWidth;
    int inY = inPos.y;



    QTimer timer;
    int steps = 10;
    int totalTime = 150;
    int msPerStep = totalTime / steps;

    BitArray show(input->size());
    show.randomize(0.75f);

    for(int t = 0; t < steps + 1; t ++)
    {
        float progress = (t+1) * (1.0f / steps);
        timer.singleShot(t * msPerStep, [=]()
        {
            //std::cout << progress << std::endl;
            //QImage background = workingImage;
            QPainter painter(&workingImage);
            QPen pen;
            pen.setWidth(5);
            pen.setColor(QColor(255, 150, 0));
            painter.setPen(pen);
            for(int n = 0; n < spatialPooler->neurons().size(); n ++)
            {
                if(output->at(n))
                {
                    pen.setWidth(2);
                    pen.setColor(QColor(255, 150, 0));
                }
                else
                {
                    continue;
                    pen.setWidth(2);
                    pen.setColor(QColor(255 / 2, 150 / 2, 0));
                }
                painter.setPen(pen);
                int spX = spStartX + n * cellWidth;
                Dendrite* proximal = spatialPooler->neurons().at(n)->getProximalDendrite();
                BitArray cons = proximal->getConnections();
                for(int s = 0; s < cons.size(); s ++)
                {
                    if(!show[s])
                    {
                        continue;
                    }
                    if(cons[s] && input->at(s))
                    {
                        int inX = inStartX + s * cellWidth;

                        float prog = std::min(1.0f, progress);

                        int midX = inX + prog*(spX - inX);
                        int midY = inY + prog*(spY - inY);

                        float progTail = std::max(0.0f, progress - 1.0f / steps);

                        int startX = inX + progTail*(spX - inX);
                        int startY = inY + progTail*(spY - inY);

                        painter.drawLine(startX, startY, midX, midY);
                    }
                }
//                if(output->at(n))
//                {
//                    int spX = spStartX + n * cellWidth;
//                    Dendrite* proximal = spatialPooler->neurons().at(n)->getProximalDendrite();
//                    BitArray cons = proximal->getConnections();
//                    for(int s = 0; s < cons.size(); s ++)
//                    {
//                        if(cons[s] && input->at(s))
//                        {
//                            int inX = inStartX + s * cellWidth;

//                            int midX = inX + progress*(spX - inX);
//                            int midY = inY + progress*(spY - inY);

//                            painter.drawLine(inX, inY, midX, midY);
//                        }
//                    }
//                }
            }
            setPixmap(QPixmap::fromImage(workingImage));
        });
    }



}

void Canvas1D::drawLearning(float prog)
{

}

void Canvas1D::computeTimeStep()
{
    if(!input)
    {
        input = new BitArray(150);
    }
    input->randomize();
    t ++;
    int start = t % input->size();
    int end = std::min(static_cast<int>(input->size()) , start + 5);
    //input->clear();
    //input->setRange(start, end, true);


    if(!spatialPooler)
    {
        spatialPooler = new SpatialPooler(150, *input, 0.05f);
        spatialPooler->setSynapseIncrement(0.01f);
        spatialPooler->setSynapseDecrement(0.01f);
    }
    spatialPooler->computeOverlap(input);
    spatialPooler->computeKWinners();
    spatialPooler->fit(input);
}

void Canvas1D::step()
{
    Point inputPos(center.x, 750);
    Point spPos(center.x, 200);

    computeTimeStep();
    drawNetwork();
    animateFeedForward(spPos, inputPos);
}


void Canvas1D::mouseMoveEvent(QMouseEvent *ev)
{
    step();
}

void Canvas1D::mousePressEvent(QMouseEvent *ev)
{

}

void Canvas1D::mouseReleaseEvent(QMouseEvent *ev)
{

}
