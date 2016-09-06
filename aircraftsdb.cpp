
#include "aircraftsdb.h"
#include <QDebug>
#include <QSqlError>
#include <QProcess>

AircraftsDB::AircraftsDB(QObject *parent) : QObject(parent)
{
    //create table if not exists new(a integer, b integer)
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    /// AddDatebase()
    m_db.setDatabaseName("sources");


    if( !m_db.open() )
    {
      qFatal( "Failed to connect. 01" );
    }
    qDebug( "Connected to database! ");
    qDebug()<<"Database name: "+m_db.databaseName();
    qDebug()<<"Database driver: "+m_db.driverName();
    qry = new QSqlQuery(m_db);

    // Create table ( ICAO, Callsign, Long, Lat, Alt, Speed, Vr, Heading)
   qry->prepare( "create table if not exists flights(ICAO INTEGER UNIQUE PRIMARY KEY, "
                  "Callsign TEXT,"
                  "Longitude REAL,"
                  "Latitude REAL,"
                  "Altitude REAL,"
                  "Speed REAL,"
                  "Vr_speed REAL,"
                  "Heading REAL,"
                  "Last_seen datetime)" );
    if( !qry->exec() )
    {
       qDebug() << "Error while adding table";
       QSqlError err = qry->lastError();
       qDebug()<<err;
    }
    else
    {
       qDebug() << "Table created: "+m_db.tables().first();
    }

    qry_IcaoCheck = new QSqlQuery(m_db);
    qry_IcaoCheck->prepare("insert OR IGNORE into flights(ICAO, Callsign, Longitude, Latitude, Altitude, Speed, Vr_speed, "
                           "Heading, Last_seen) values(:icao, '?', 0, 0, 0, 0, 0,"
                           "0, datetime('now','localtime'))");

}

AircraftsDB::~AircraftsDB()
{
    if (!m_db.isOpen())
        m_db.close();
}

// Only for Callsign
// Update last_seen
void AircraftsDB::UpdateValue(int icao, QString callsign)
{

    if( !m_db.isOpen())
    {
      qFatal( "Failed to connect." );
    }

    QSqlDatabase::database().transaction();

    qry_IcaoCheck->bindValue(":icao", icao);
    if (!qry_IcaoCheck->exec())
    {
        QSqlError err = qry->lastError();
        qDebug()<<err;
    }

    if(!qry->exec("UPDATE flights SET Callsign='"+callsign+
                  "' , Last_seen=datetime('now','localtime') "+
                  " WHERE ICAO="+QString::number(icao)))
    {
        QSqlError err = qry->lastError();
        qDebug()<<err;
    }
    QSqlDatabase::database().commit();
}

// Add new values for long, lat, alt , spee, vr, heading
// Update lastseen every time
void AircraftsDB::UpdateValue(int icao, AircraftsDB::items item, double newValue)
{
    if( !m_db.isOpen() )
    {
      qFatal( "Failed to connect." );
    }

    QSqlDatabase::database().transaction();

    qry_IcaoCheck->bindValue(":icao", icao);
    if (!qry_IcaoCheck->exec())
    {
        QSqlError err = qry->lastError();
        qDebug()<<err;
    }

    switch (item) {
       case Altitude:{
        if(!qry->exec("UPDATE flights "
                      "SET Altitude = " + QString::number(newValue)+
                      " , Last_seen=datetime('now','localtime') "+
                      " WHERE ICAO = "+QString::number(icao) ))
        {
            qDebug()<<"error while adding alt";
            QSqlError err = qry->lastError();
            qDebug()<<err;
        }
    }break;
    default:
        qDebug()<<"Error (UpdateValue(icao, item, newValue) 'Unknow item')";
        return;
        break;
    }

    QSqlDatabase::database().commit();
}


// Updata positions
void AircraftsDB::UpdatePositions(int icao, double lon, double lat)
{

    if( !m_db.isOpen() )
    {
      qFatal( "Failed to connect." );
    }

    QSqlDatabase::database().transaction();

    qry_IcaoCheck->bindValue(":icao", icao);
    if (!qry_IcaoCheck->exec())
    {
        QSqlError err = qry->lastError();
        qDebug()<<err;
    }
        if(!qry->exec("UPDATE flights"
                      " SET Longitude = " + QString::number(lon)+
                      " , Last_seen=datetime('now','localtime') "+
                      " , Latitude = " + QString::number(lat)+
                      " WHERE ICAO = "+QString::number(icao) ))
        {
            qDebug()<<"error while adding long, lat, alt";
            QSqlError err = qry->lastError();
            qDebug()<<err;
        }

        QSqlDatabase::database().commit();
}

// Update Speed, Vrspeed, Heading
void AircraftsDB::UpdateValues(int icao, double speed, double vrspeed, double heading)
{
    if( !m_db.isOpen() )
    {
      qFatal( "Failed to connect." );
    }

    QSqlDatabase::database().transaction();

    qry_IcaoCheck->bindValue(":icao", icao);
    if (!qry_IcaoCheck->exec())
    {
        QSqlError err = qry->lastError();
        qDebug()<<err;
    }
        if(!qry->exec("UPDATE flights"
                      " SET Speed = " + QString::number(speed)+
                      " , Last_seen=datetime('now','localtime') "+
                      " , Vr_speed = " + QString::number(vrspeed)+
                      " , Heading = " + QString::number(heading)+
                      " WHERE ICAO = "+QString::number(icao) ))
        {
            qDebug()<<"error while adding long, lat, alt";
            QSqlError err = qry->lastError();
            qDebug()<<err;
        }

        QSqlDatabase::database().commit();
}




