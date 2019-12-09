#include "mainwindow.h"
#include "iostream"
#include "bitarray.h"

#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    BitArray b(25, 1, performanceHint::SPARSE);
    BitArray c(25, 1, performanceHint::DENSE);
    BitArray d(25);

    b[1] = true;
    c[2] = true;
    d[3] = true;
    b.print();
    c.print();
    d.print();
    cout << endl;

    (b | c | d).print();

    for(int i = 0; i < b.size(); i ++)
    {
    }

    b.setRange(0, 25, true);
    b.setRange(4, 20, false);

    //return a.exec();
}
