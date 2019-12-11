#include "mainwindow.h"
#include "iostream"
#include "bitarray.h"
#include "spatialpooler.h"
#include "chrono"

#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    BitArray input(50);
    //input.clear();
    //input.setRange(10,15, true);
    input.randomize();
    cout << "input:";
    input.print();
    cout << endl << endl;

    SpatialPooler sp(25, input, 0.1f);
    sp.setSynapseIncrement(0.05f);
    sp.setSynapseDecrement(0.05f);

    auto start = std::chrono::high_resolution_clock::now();
    int loops = 100;
    BitArray cons;
    for(int i = 0; i < loops; i ++)
    {
        input.randomize();
        sp.computeOverlap(&input);
        sp.computeKWinners();
        sp.fit(&input);

        BitArray* act = sp.activity();
        unsigned int firstOn = act->onBits()[0];

        cons = sp.neurons()[firstOn]->getProximalConnections();

        //cout << "\nstep " << i << ":";
        //cons.print();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    cons.print();

    cout << "\ntime: " << duration.count() / loops << " ms\n";



    //return a.exec();
}
