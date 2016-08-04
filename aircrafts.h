#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

#include <QString>
#include <QTime>
#include <math.h>
#include <QPointF>
#include <QHash>

#include "defines.h"

class Aircraft {
public:
    int icao;
    double Latitude;
    double Longitude;
    double Heading;
    double Altitude;
    double Speed;
    QString Callsign;
    QTime Last_seen;
    double Distance;
    double Bearing;
    int Vertical_Rate;
    int r_x;
    int r_y;
public:
    Aircraft() {}
    Aircraft(double lat, double lon, double heading) : Latitude(lat), Longitude(lon), Heading(heading) {}
    void CountPos(double center_lat, double center_lon, int radius, int range);
};


typedef QHash<int, Aircraft> AircraftHashMap;
typedef AircraftHashMap::iterator AIterator;
typedef AircraftHashMap::const_iterator AConstIterator;
typedef QHashIterator<
    AircraftHashMap::key_type,
    AircraftHashMap::mapped_type
> AHashIterator;
typedef AircraftHashMap Aircrafts;

#endif // AIRCRAFTS_H
