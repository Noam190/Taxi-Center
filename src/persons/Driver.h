//
// Created by mei on 29/11/16.
//
#include <string>
#include "Person.h"
#include "../Trip.h"
#include "../cabs/TaxiCab.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#ifndef EX2AP_DRIVER_H
#define EX2AP_DRIVER_H



class Driver : public Person{
private:
    friend boost::serialization::access;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        int check = archive & boost::serialization::base_object<Person>(*this);
        if (!check) {

        }
        archive & yearsOfExperience;
        archive & averageSatisfaction;
    }
    int yearsOfExperience;
    int averageSatisfaction;

public:

    // Constructor all the members
    Driver(Point location, int id, int age, int yearsOfExperience,
           int averageSatisfaction, STATUS status);

    //getters and setters
    int getYearsOfExperience();

    //getters and setters
    void setYearsOfExperience(int yearsOfExperience);

    //getters and setters
    int getAverageSatisfaction();

    //getters and setters
    void setAverageSatisfaction(int averageSatisfaction);
};


#endif //EX2AP_DRIVER_H