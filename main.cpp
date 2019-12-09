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

    for(int i = 0; i < b.size(); i ++)
    {
        //b.clear();
        //b[i] = true;
        //b.print();
    }
    //cout << endl;

    //b.setRange(4, 20, false);
    //b.print();

    //vector<unsigned short> indices = {1, 4, 5};

    //b[1] = true;
    //b[4] = false;
    //b[5] = true;

    b.setRange(0, 25, true);
    b.setRange(4, 20, false);

    b.print();

    //auto on = b.onBits();

    //b[on].print();
    //BitArray on = b[indices];
    //on.print();
    cout << endl;

    //b[3] = true;
    //cout << b[3] << endl;



    //return a.exec();
}
