#include "aircrafts.h"

void Aircraft::CountPos(double center_lat, double center_lon, int radius, int range)
{
    double lat1 = D2R(center_lat); 			// long1, lat1 my coordinates
    double lon1 = D2R(center_lon);			//
    double lat2 = D2R(Latitude);            //
    double lon2 = D2R(Longitude);           //

    double DLon = lon2 - lon1;
    double DLat = lat2 - lat1;

    double a = sin(DLat/2.)*sin(DLat/2.)+
               cos(lat1)*cos(lat2)*
               sin(DLon/2)*sin(DLon/2);
    double c = 2*atan2(sqrt(a),sqrt(1-a));
    Distance = EARTH_RADIUS * c;

    double y = sin(DLon)*cos(lat2);
    double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(DLon);

    Bearing = atan2(y,x); 			//
    double cos_b = cos(Bearing);	// cos_b, sin_b used in updateXY
    double sin_b = sin(Bearing);	// ...
    Bearing = R2D(Bearing);
    if (Bearing<0) Bearing+=360;	// [0:360]

    r_y = radius*(1-(Distance/range)*cos_b);
    r_x = radius*(1+(Distance/range)*sin_b);
}
