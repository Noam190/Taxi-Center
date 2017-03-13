//
//  Created by noam on 16/12/16.
//

#include "Menu.h"
#include "creators/ThreadCreator.h"
#include "InputParser.h"
#include <pthread.h>

//  run the input to the program
void Menu::run() {
    //string input;
    long readBytes;
    char buffer[8192];
    std::fill_n(buffer, 8192, 0);
    int option = 0;

    do {
//         getline(cin, input);
//         trim(input);
        readBytes = tcp->receiveData(buffer, 8192, guiPort);
        if(readBytes > 0 && this->inputParser->checkInput("manu options", buffer)) {
            option = atoi(buffer);
            runOption(option);
            if (option != 9) {
                tcp->sendData("valid", guiPort);
            }
        } else {
            std::cout << "-1" << endl;
            tcp->sendData("error", guiPort);
        }
    } while (option != 7);
}

void Menu::runOption(int option) {
    switch (option) {
        case 1: //insert drivers
            expectingDriver();
            break;
        case 2: //insert trip
            insertTrip();
            break;
        case 3: //insert taxi
            insertTaxi();
            break;
        case 4: //get the drivers location
            getDriverLocation();
            break;
        case 9:// move the drivers to the next point.
            moveOneStep();
            break;
        default:
            return;
    }
}

void Menu::moveOneStep() {
    this->taxiCenter->moveAllRidesOneStep();
    string guiDriverLocationString =  this->taxiCenter->getGuiDriverLocationString();
    tcp->sendData(guiDriverLocationString, guiPort);
}

//insert a new taxi from the input arguments
void Menu::insertTaxi() {
    string input;
    int id, taxiType;
    char manufacturer, color;

    getline(cin, input);
    trim(input);
    if(this->inputParser->checkInput("taxi cab", input)) {
        vector<string> temp;
        boost::split(temp, input, boost::is_any_of(","));
        id = stoi(temp[0]);
        taxiType = stoi(temp[1]);
        manufacturer = temp[2][0];
        color = temp[3][0];
        //create the taxi
        TaxiCab *cab = createTaxi(id, taxiType, manufacturer, color);
        this->taxiCenter->addTaxiCab(cab);
        return;
    }

    std::cout << "-1" << endl;
}

//expecting a new driver from the client
void Menu::expectingDriver() {
    string input;
    int numOfDrivers;
    getline(cin, input);
    trim(input);
    if(this->inputParser->checkInput(regex("\\d+"), input)) {
        numOfDrivers = stoi(input);
        for (int i = 0; i < numOfDrivers; ++i) {
            taxiCenter->addClient();
        }
        return;
    }
    std::cout << "-1" << endl;
}

//insert a new trip from the input arguments
void Menu::insertTrip() {
    string input;
    int id;
    long xStart, yStart, xEnd, yEnd;
    int numOfPass;
    double tariff;
    int timeOfStart;

    getline(cin, input);
    trim(input);
    if(this->inputParser->checkInput("trip", input)) {
        vector<string> temp;
        boost::split(temp, input, boost::is_any_of(","));
        id = stoi(temp[0]);
        xStart = stol(temp[1]);
        yStart = stol(temp[2]);
        xEnd = stol(temp[3]);
        yEnd = stol(temp[4]);
        timeOfStart = stoi(temp[7]);
        if (checkPoint(xStart, yStart) && checkPoint(xEnd, yEnd)) {
            if (xStart == xEnd && yStart == yEnd) {
                return;
            }
            numOfPass = stoi(temp[5]);
            tariff = stoi(temp[6]);

            //create the trip
            Trip* newTrip = createTrip(grid, id, xStart, yStart, xEnd, yEnd, numOfPass, tariff, timeOfStart,
                                       tripThreadPool, taxiCenter);

            taxiCenter->insertTrip(newTrip);
            return;
        }
    }
    std::cout << "-1" << endl;
}

//get the driver location from the input arguments
void Menu::getDriverLocation() {
    string input;
    int idDriver;
    getline(cin, input);
    trim(input);
    if(this->inputParser->checkInput(regex("\\d+"), input)) {
        idDriver = stoi(input);
        try {
            std::cout << this->taxiCenter->getDriverLocation(idDriver);
        } catch (...) {
            std::cout << -1 << std::endl;
        }
        return;
    }
    std::cout << "-1" << endl;
}

//constructor to a new
Menu::Menu(TaxiCenter *taxiCenter, Matrix *grid, InputParser *inputParser, ThreadPool* tripThreadPool,
           int guiPort, TcpServer* tcp)
        : grid(grid), taxiCenter(taxiCenter), inputParser(inputParser), tripThreadPool(tripThreadPool){
    this->tcp = tcp;
    this->guiPort = guiPort;
    inputParser->addRegex("taxi cab", "\\d+,[12],[FHTS],[RBGWP]");
    inputParser->addRegex("trip", "\\d+,\\d+,\\d+,\\d+,\\d+,\\d+,(?:\\d*\\.)?\\d+,[1-9]\\d*");
    inputParser->addRegex("manu options", "[123479]");

}

bool Menu::checkPoint(long x, long y){
    if(x >= 0 && x < this->grid->getWidth() && y >= 0 && y < this->grid->getHeight()){
        return !this->grid->isObstacle(x, y);
    }
    return false;
}




