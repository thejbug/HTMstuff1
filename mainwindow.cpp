#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas1d.h"
#include "qpushbutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setGeometry(200,50, 900,700);
    Canvas1D* canvas = new Canvas1D(this);

    QPushButton* stepButton = new QPushButton(this);
    stepButton->setGeometry(canvas->x(), canvas->y() - 50, 100, 35);
    stepButton->setText("step");
    connect(stepButton, &QPushButton::pressed, canvas, &Canvas1D::step);

    canvas->step();
    canvas->drawNetwork();
}

MainWindow::~MainWindow()
{
    delete ui;
}

