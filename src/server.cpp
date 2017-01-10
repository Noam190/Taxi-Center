#include "sockets/Udp.h"
#include "core/Matrix.h"
#include "TaxiCenter.h"
#include "Menu.h"
#include <pthread.h>

void *printTry( void *ptr )
{
    int message = *((int*) ptr);
    cout << "test seoond thread" << message << "\n";
}


int main(int argc, char *argv[]) {


    int portNum = atoi(argv[1]);
    Udp* udp = new Udp(true, portNum);
    //udp.initialize();

    Clock* clock = new Clock();
    TaxiCenter* taxiCenter = new TaxiCenter( clock);
    int width, height;

    //grid initialize grid and add obstacles if there are any.
    std::cin >> width >> height;
    Matrix* grid = new Matrix(width, height);

    Menu menu = Menu(taxiCenter, grid, clock, udp);

    //thread
    pthread_t t1;
    int * data = new int;
    *data = 2;
    int status = pthread_create(&t1, NULL, printTry, (void*) data);
    if (status)
    {
        std::cout<<" error";
    }


    //run all the other inputs
    menu.run();

    delete grid;
    delete udp;
    delete clock;
    delete taxiCenter;
    return 0;
}

