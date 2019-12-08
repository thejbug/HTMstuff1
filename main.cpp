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


    BitArray b(10);
    //cout << b.size() << endl;

    for(int i = 0; i < b.size(); i ++)
    {
        b.clear();
        b.set(i, true);
        //b.print();
    }
    //b.set(1, true);
    //b.print();
    BitArray b2(b);
    //b2.print();

    //cout << b[9];


    b.clear();
    b[5] = true;
    //b.set(5, true);
    bool b5 = b[5];
    std::cout << b5 << std::endl;

    for(int i = 0; i < b.size(); i ++)
    {
        b.clear();
        b[i] = true;
        b.print();
    }

    //cout << b.at(8);

    unsigned char bt = 1;
    bt <<= 7;
    //cout << static_cast<unsigned int>(static_cast<unsigned char>(bt))  << endl;
    //cout << static_cast<bool>(bt) << endl;


    int p;
    cin >> p;

    //return a.exec();
}
