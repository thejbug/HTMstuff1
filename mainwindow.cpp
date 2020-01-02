#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas1d.h"
#include "qpushbutton.h"
#include "qpainter.h"
#include "tmcanvas.h"
#include "sprite.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setGeometry(200,50, 900,700);


    BitArray input(50);
    SpatialPooler* sp = new SpatialPooler(50, input, 0.1f);
    TemporalMemory* tm = new TemporalMemory;
    tm->setup(sp, 8);

    TMCanvas* canvas = new TMCanvas(this);
    canvas->setGeometry(0,0,900,500);
    canvas->setup(tm);
    canvas->drawNetwork();

//    QLabel* lab = new QLabel(this);
//    Sprite beach("/Users/jaredemery/Desktop/beach.ssp");

//    QImage* frame1 = beach.frames[0];
//    lab->setGeometry(0,0, frame1->width(), frame1->height());
//    BitArray* bits = Sprite::toBinaryPixel(frame1);
//    QImage* bitImage = Sprite::fromBinaryPixel(bits, frame1->width(), frame1->height());
//    lab->setPixmap(QPixmap::fromImage(*bitImage));

}

MainWindow::~MainWindow()
{
    delete ui;
}



//    Canvas1D* canvas = new Canvas1D(this);

//    QPushButton* stepButton = new QPushButton(this);
//    stepButton->setGeometry(canvas->x(), canvas->y() - 50, 100, 35);
//    stepButton->setText("step");
//    connect(stepButton, &QPushButton::pressed, canvas, &Canvas1D::step);

//    canvas->step();
//    canvas->drawNetwork();

//    QImage image = QImage("/Users/jaredemery/Desktop/htmTestImages/paintBubbles.png").scaled(900, 500);
//    QImage copy = image;

//    QPainter painter(&image);
//    //image.fill(QColor(50,0,0));
//    //painter.setBrush(QColor(255,0,0));
//    //painter.drawRect(100,100,100,100);

//    int colors = 10;
//    int interval = 255 / colors;
//    for(int row = 0; row < image.height(); row ++)
//    {
//        for(int col = 0; col < image.width(); col ++)
//        {
//            auto color = image.pixelColor(col, row);
//            int r = color.red();
//            int g = color.green();
//            int b = color.blue();

//            //reduce color range
//            r = (r / interval) * interval;
//            g = (g / interval) * interval;
//            b = (b / interval) * interval;

////            //edge detection
////            int lighter = 1;
////            r *= lighter;
////            b *= lighter;
////            g *= lighter;

////            //r = g = b = (r + g + b) / 255;

////            int rad = 1;
////            int rsum = 0;
////            int gsum = 0;
////            int bsum = 0;
////            for(int y = -rad; y <= rad; y ++)
////            {
////                for(int x = -rad; x <= rad; x ++)
////                {
////                    int cx = col + x;
////                    int ry = row + y;
////                    if(cx >= 0 && cx < image.width() && ry >= 0 && ry < image.height())
////                    {
////                        int r2 = image.pixelColor(col + x, row + y).red() * lighter;
////                        int g2 = image.pixelColor(col + x, row + y).green() * lighter;
////                        int b2 = image.pixelColor(col + x, row + y).blue() * lighter;

////                        rsum += r2;
////                        gsum += g2;
////                        bsum += b2;
////                    }

////                }
////            }
////            rsum -= r;
////            gsum -= g;
////            bsum -= b;
////            int side = rad * 2 + 1;
////            int area = side*side - 1;
////            rsum /= area;
////            gsum /= area;
////            bsum /= area;
////            r = std::abs(rsum - r);
////            g = std::abs(gsum - g);
////            b = std::abs(bsum - b);

////            r = (std::abs(rsum - r) > 5 * lighter)? 255: 0;
////            g = (std::abs(gsum - g) > 5 * lighter)? 255: 0;
////            b = (std::abs(bsum - b) > 5 * lighter)? 255: 0;


//            copy.setPixelColor(col, row, QColor(r,g,b));
//        }
//    }

//    QLabel* lab = new QLabel(this);
//    lab->setGeometry(0, 0, 900, 500);
//    lab->setStyleSheet("background: white");

//    lab->setPixmap(QPixmap::fromImage(copy));
//    lab->setScaledContents(true);

