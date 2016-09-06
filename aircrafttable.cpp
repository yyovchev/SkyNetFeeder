#include "aircrafttable.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

aircraftTable::aircraftTable(QObject *parent) : QObject(parent)
{
    //create table if not exists new(a integer, b integer)
    m_db = QSqlDatabase::addDatabase("QSQLITE","conn2");

    /// AddDatebase()
    m_db.setDatabaseName("sources");

    if( !m_db.open() )
    {
      qFatal( "Failed to connect. 02" );
    }

    qry = new QSqlQuery(m_db);
    qry_del = new QSqlQuery(m_db);

    qry_del->prepare("DELETE from flights where Last_seen < datetime('now', 'localtime', '-20 seconds')");
    qry->prepare("SELECT * FROM flights ");
    // start timer, which print info
    timer = new QTimer();
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(PrintCount()));
}

aircraftTable::~aircraftTable()
{
    if(m_db.isOpen())
    {
        m_db.close();
    }
}

void aircraftTable::CountPos(QSqlQuery qry)
{
    int br =0;
    Aircraft Ac;
    Aircrafts Acs;

    double mylat = settings.value("myLat").toDouble();
    double mylong = settings.value("myLong").toDouble();
    int range = settings.value("Radar_Range").toInt();

    while (qry.next())
    {

        Ac.icao  = qry.value("ICAO").toInt();
        Ac.Callsign  = qry.value("Callsign").toString();
        Ac.Longitude = qry.value("Longitude").toDouble();
        Ac.Latitude = qry.value("Latitude").toDouble();
        Ac.Altitude = qry.value("Altitude").toInt();
        Ac.Speed = qry.value("Speed").toInt();
        Ac.Heading = qry.value("Heading").toInt();
        Ac.Vertical_Rate = qry.value("Vr_speed").toInt();

        if (Ac.Latitude != 0 && Ac.Longitude != 0){
            br++;
            ///settings value
            Ac.CountPos(mylat,mylong,438/2,range);

            Acs.insert(Ac.icao,Ac);
        }
    }
    qDebug()<<br;
    writeinFile(Acs);
}

void aircraftTable::writeinFile(Aircrafts Acs)
{
    QFile file("new.txt");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<<file.errorString();
        return;
    }
    QTextStream out(&file);

    for (AConstIterator iter = Acs.begin(), end = Acs.end(); iter != end; iter ++)
    {
        out<<iter.key()<<" ";
        out<<iter.value().Callsign<<" ";
        out<<iter.value().Longitude<<"";
        out<<iter.value().Latitude<<" ";
        out<<iter.value().Altitude<<" ";
        out<<iter.value().Heading<<" ";
        out<<iter.value().Speed<<" ";
        out<<iter.value().Vertical_Rate<<" ";
        out<<iter.value().r_x<<" ";
        out<<iter.value().r_y<<" ";
        out<<iter.value().Distance<<"\t";
    }

    file.close();
}

void aircraftTable::PrintCount()
{
    if( !m_db.isOpen())
    {
      qFatal( "Failed to connect." );
    }
    qry_del->exec();
    if (qry->exec())
    {
            CountPos(*qry);

    }
    else {
//        qDebug()<<qry->lastError().text();
    }
}


