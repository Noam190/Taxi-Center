#include "sockets/Udp.h"
#include "core/Matrix.h"
#include "TaxiCenter.h"
#include "Menu.h"
#include "sockets/Tcp.h"
#include <pthread.h>

void *printTry( void *ptr )
{
    int message = *((int*) ptr);
    cout << "test seoond thread" << message << "\n";
}


int main(int argc, char *argv[]) {


    int portNum = atoi(argv[1]);
    TcpServer* tcp = new TcpServer(portNum);
    //udp.initialize();

    Clock* clock = new Clock();
    TaxiCenter* taxiCenter = new TaxiCenter( clock);
    unsigned long width, height;

    //grid initialize grid and add obstacles if there are any.
    std::cin >> width >> height;
    Matrix* grid = new Matrix(width, height);
    ThreadPool* threadPool = new ThreadPool(0);
    Menu menu = Menu(threadPool,taxiCenter, grid, clock, tcp);

//    //thread
//    pthread_t t1;
//    int * data = new int;
//    *data = 2;
//    int status = pthread_create(&t1, NULL, printTry, (void*) data);
//    if (status)
//    {
//        std::cout<<" error";
//    }


    //run all the other inputs
    menu.run();

    delete grid;
    delete tcp;
    delete clock;
    delete taxiCenter;
    return 0;
}

//Node* n  = grid->getNode(4, 34);
//std::list<Node*> l = n->getNeighbors();
//while (!l.empty()){
//std::cout << *(l.front()) << std::endl;
//l.pop_front();
//}