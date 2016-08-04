#ifndef BINARYPARSER_H
#define BINARYPARSER_H
#include <QString>
#include <math.h>
#include <QHash>
#include "aircraftsdb.h"
#include "oevalues.h"

enum DataTypes {
    TYPE_UNKOWN,
    TYPE_DF17_TC4,
    TYPE_DF17_TC17,
    TYPE_DF17_AllPos,
};

class BinaryParser
{
public:
    const char *charset;
    int ICAO;

    QString Callsign;

    double Speed;
    double Heading;
    double Vr_speed;

    double Longitude;
    double Latitude;
    int Altitude;
    int LAT_CPR_EVEN;
    int LON_CPR_EVEN;
    int LAT_CPR_ODD;
    int LON_CPR_ODD;

    AircraftsDB *sources;
    OEValues *OddEvenValues;

public:
    BinaryParser();
    ~BinaryParser();
    DataTypes parseData(const unsigned char *);
    void GetICAO (const unsigned char*);
    void IDDecode (const unsigned char*);
    void GetSpeed(const unsigned char*);
    void GetPosition(const unsigned char*);
    int getNL(double);
    int Modulo(int , int);
};

#endif // BINARYPARSER_H
