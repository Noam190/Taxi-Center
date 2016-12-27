//
// Created by mei on 27/12/16.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace std;
using namespace boost::archive;
std::stringstream ss;

class Point {
    int x, y;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & x;
        ar & y;
    }
public:
    Point(int x, int y) : x(x), y(y) {}

    Point() : x(0), y(0) {}
    int getX() {return this->x;}
    int getY() {return this->y;}

};

class GridPoint {
    Point *p;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & p;
    }

public:
    GridPoint(Point *p) : p(p) {};
    GridPoint() : p(NULL) {}
    virtual ~GridPoint() {
        delete p;
    }
    friend std::ostream& operator<< (std::ostream &os, const GridPoint &p);
};

std::ostream& operator<< (std::ostream &os, const GridPoint &p) {
    os << '(' << p.p->getX() << ',' << p.p->getY() << ')';
    return os;
}

class D3GridPoint : public GridPoint {
    int z;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<GridPoint>(*this);
        ar & z;
    }
public:
    D3GridPoint(Point *p, int z) : GridPoint(p), z(z) {};
};



int main() {


    GridPoint *gp = new GridPoint(new Point(1,5));

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << gp;
    s.flush();

    cout << serial_str << endl;

    GridPoint *gp2;
    boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> gp2;

    cout << *gp2;

    delete gp;
    delete gp2;
    return 0;
}
