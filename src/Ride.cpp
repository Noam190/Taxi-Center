//
// Created by noam on 17/12/16.
//

#include "Ride.h"

//move the ride to the end
void Ride::moveToTheEnd() {
    if (clock->getCurrentTime() >= trip->getTimeOfStart()) {
        this->trip->updateLocation(this->end);
        this->driver->updateLocation(this->end);
    }
}

//move the ride one step
void Ride::moveOneStep() {
    if (clock->getCurrentTime() >= trip->getTimeOfStart()) {
        this->trip->moveOneStep();
        this->driver->updateLocation(this->trip->getCurrentPoint());
    }
}

//constructor
Ride::Ride(Trip *trip, Driver *driver, Clock *clock, int taxiType)
        : start(trip->getStartPoint()), end(trip->getEndPoint()) ,taxiType(0){
    this->driver = driver;
    this->trip = trip;
    this->clock = clock;
    this->taxiType=taxiType;
}

//getters and setters
Point Ride::getEndPoint() {
    return this->end;
}

//getters and setters
Point Ride::getStartPoint() {
    return this->start;
}

//getters and setters
Trip *Ride::getTrip() {
    return this->trip;
}

//getters and setters
Driver *Ride::getDriver() {
    return this->driver;
}

bool Ride::isDone() {
    return this->end == this->trip->getCurrentPoint();
}

//get type of taxi - 1  standart, 2 luxury
int Ride::getTaxiType(){
    return this->taxiType;
}

