#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas1d.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setGeometry(200,50, 900,700);
    Canvas1D* canvas = new Canvas1D(this);

    canvas->drawNetwork();
}

MainWindow::~MainWindow()
{
    delete ui;
}

