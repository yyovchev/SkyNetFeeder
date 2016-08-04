#ifndef AIRCRAFTSDB_H
#define AIRCRAFTSDB_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTimer>

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
    void UpdateValue(int icao, QString callsign); //Update Callsign only
    void UpdateValue(int icao, items item, double newValue); //Update all items
    void UpdatePositions (int icao, double lon, double lat); // Update Long and Lat
    void UpdateValues(int icao, double speed, double vrspeed, double heading);// Update Speed, Vrspeed, Heading

private:
    QSqlDatabase m_db;
    QSqlQuery *qry;
    QSqlQuery *qry_IcaoCheck;


signals:

private slots:
};

#endif // AIRCRAFTSDB_H
