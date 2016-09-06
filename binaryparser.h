#ifndef BINARYPARSER_H
#define BINARYPARSER_H

///========================================================
/// This class decode ADS-B type DF17
///========================================================

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
    const char *charset;    // string with all possible character for the callsign
    int ICAO;               // unique value of the plane (base 16)

    QString Callsign;

    double Speed;
    double Heading;
    double Vr_speed;

    double Longitude;
    double Latitude;
    int Altitude;

    int LAT_CPR_EVEN;       // Mid value used for decoding Latitude
    int LON_CPR_EVEN;       // Mid value used for decoding Longitude
    int LAT_CPR_ODD;        // Mid value used for decoding Latitude
    int LON_CPR_ODD;        // Mid value used for decoding Longitude

    AircraftsDB *sources;       // Store all information abaut the planes
    OEValues *OddEvenValues;    // Store mid vallues of ecery plane

public:
    BinaryParser();
    ~BinaryParser();
    DataTypes parseData(const unsigned char *);     // 'main' method

private:
    void GetICAO (const unsigned char*);            // Get icao from the msg (first 8 bytes)
    void IDDecode (const unsigned char*);           // Decodo the char set
    void GetSpeed(const unsigned char*);            // Decode the speed, heading and vertical speed
    void GetPosition(const unsigned char*);         // Decode altitude, longitude and latitude (long. and lat. come in two parts)
    int getNL(double);
    int Modulo(int , int);
};

#endif // BINARYPARSER_H
