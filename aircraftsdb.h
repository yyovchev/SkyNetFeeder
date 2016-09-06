#ifndef AIRCRAFTSDB_H
#define AIRCRAFTSDB_H

///======================================================================
/// This class update sqlite database in which is stored all information
/// about recent planes. The information is saved no more then 20 second
/// from the last message from the plane.
///======================================================================


#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class AircraftsDB : public QObject
{
    Q_OBJECT

public:
    enum items {
        Callsign,
        Longitude,
        Latitude,
        Altitude,
        Speed,
        Vr_speed,
        Heading

    };

public:
    explicit AircraftsDB(QObject *parent = 0);
    ~AircraftsDB();
    void UpdateValue(int icao, QString callsign);                               // Update Callsign only
    void UpdateValue(int icao, items item, double newValue);                    // Update Altitude
    void UpdatePositions (int icao, double lon, double lat);                    // Update Long and Lat
    void UpdateValues(int icao, double speed, double vrspeed, double heading);  // Update Speed, Vrspeed, Heading

private:
    QSqlDatabase m_db;          // Sqlite database
    QSqlQuery *qry;             // Update with the new information
    QSqlQuery *qry_IcaoCheck;   // Check if the plene is in the database


signals:

private slots:
};

#endif // AIRCRAFTSDB_H
