#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

///================================================================================================
/// This class create structure Aircraft, in which is saved all temporary information about flights
/// Aircraft CountPos count the coordinates of the plane on the desktop (r_x and r_y)
/// Bearing and Distance are count in the CountPos method, they depend on the plane's location
/// Radios is the radios of the biggest circle of radar (const)
/// center_lat and certer_lon are the coordinates of the user, there are taken from the settings
/// range is the range of the radar (km)
///================================================================================================

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
    QTime Last_seen;        // time of the last msg from the plane
    double Distance;        // count in CountPos
    double Bearing;         // count in CountPos
    int Vertical_Rate;
    int r_x;                // position on the screen
    int r_y;                // position on the screen
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
