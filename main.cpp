#include "mainwindow.h"
#include "iostream"
#include "bitarray.h"
#include "spatialpooler.h"
#include "chrono"
#include "temporalmemory.h"

#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int inputSize = 50;
    std::vector<BitArray*> inputSequence;
    int length = 8;
    for(int i = 0; i < length; i ++)
    {
        BitArray* input = new BitArray(inputSize);
        input->randomize();
        inputSequence.push_back(input);
    }


    SpatialPooler* sp = new SpatialPooler(35, *inputSequence[0], 0.1f);
    TemporalMemory tm;
    tm.setup(sp, 8);

    auto start = std::chrono::high_resolution_clock::now();
    int loops = 1;
    BitArray* ff = new BitArray(inputSize);
    for(int i = 0; i < loops; i ++)
    {

        BitArray* active = tm.active(-1);
        int note = i % length;
        cout << "timestep: " << i << endl;
        cout << "note: " << note << endl;
        if(note == 0)
        {
            active->clear();
        }

        tm.computePredictions(active);
        tm.print();

        tm.computeFeedForward(inputSequence[note]);
        //cout << "bursting: " << tm.burstingColumns->onBits().size() << "\n";
        tm.print();

        tm.fitTM(tm.active(-1), tm.winners(-1));
        tm.print();



        tm.step();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "\ntime: " << duration.count() / loops << " ms\n";

    return a.exec();
}
