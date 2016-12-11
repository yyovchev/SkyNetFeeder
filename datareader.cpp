#include "datareader.h"

dataReader::dataReader(QObject *parent) : QObject(parent)
{
    sec = 0;

    //create table if not exists new(a integer, b integer)
    m_db = QSqlDatabase::addDatabase("QSQLITE","conn2");

    /// AddDatebase()
    m_db.setDatabaseName(":memdb1?mode=memory&cache=shared");

    if( !m_db.open() )
    {
      qFatal( "Failed to connect. 02" );
    }

    qry = new QSqlQuery(m_db);
    qry_del = new QSqlQuery(m_db);

    qry_del->prepare("DELETE from flights where Last_seen < datetime('now', 'localtime', '-20 seconds')");
    qry->prepare("SELECT * FROM flights ");
}

dataReader::~dataReader()
{
    if(m_db.isOpen())
    {
        m_db.close();
    }
}

void dataReader::startsender()
{

}

Aircrafts dataReader::getFlights()
{
    Aircraft Ac;
    Aircrafts Acs;

    qDebug()<<"here";

    double mylat = settings.value("myLat").toDouble();
    double mylong = settings.value("myLong").toDouble();
    int range = settings.value("Radar_Range").toInt();

    if( !m_db.isOpen())
    {
      qFatal( "Failed to connect." );
    }
    qry_del->exec();
    if (qry->exec())
    {
        if (sec == 6){
            QSqlQuery sQry = *qry;
            sender.setqry(sQry);
            sec = 0;
            return previousValues;
        }
        while (qry->next())
        {

            Ac.icao  = qry->value("ICAO").toInt();
            Ac.Callsign  = qry->value("Callsign").toString();
            Ac.Longitude = qry->value("Longitude").toDouble();
            Ac.Latitude = qry->value("Latitude").toDouble();
            Ac.Altitude = qry->value("Altitude").toInt();
            Ac.Speed = qry->value("Speed").toInt();
            Ac.Heading = qry->value("Heading").toInt();
            Ac.Vertical_Rate = qry->value("Vr_speed").toInt();

            if (Ac.Latitude != 0 && Ac.Longitude != 0){
                ///settings value
                Ac.CountPos(mylat,mylong,438/2,range);
                Acs.insert(Ac.icao,Ac);
                qDebug()<<Ac.icao<<" added";
            }
        }

        sec++;

    }
    previousValues = Acs;
    return Acs;
}


