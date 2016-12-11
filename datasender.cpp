#include "datasender.h"

dataSender::dataSender(QObject *parent, QString url) : QObject(parent)
{
    Url_str = "http://skynet.sliven.org/skynet.php";

    m_db = QSqlDatabase::addDatabase("QSQLITE","read");
    m_db.setDatabaseName(":memdb1?mode=memory&cache=shared");

    qry = new QSqlQuery(m_db);

    autoSend = false;
    msec = 2500;

    timer = new QTimer();
    if (autoSend){

        if( !m_db.open() )
        {
          qDebug( "Failed to connect. 01" );
        }

        qry->prepare( "select * from flights" );

        timer->start(msec);
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(sendJson()));
    }

}

dataSender::~dataSender()
{
    delete qry;
    delete timer;
}

void dataSender::setqry(QSqlQuery qry)
{
    QJsonObject fl, inf;
    QJsonArray arr;

    while (qry.next())
    {
        inf.insert(QString::fromLatin1("icao"),QString::number(qry.value("ICAO").toInt(),16).toUpper());
        inf.insert(QString::fromLatin1("callsign"), qry.value("Callsign").toString());
        inf.insert(QString::fromLatin1("lon"), qry.value("Longitude").toString());
        inf.insert(QString::fromLatin1("lat"), qry.value("Latitude").toString());
        inf.insert(QString::fromLatin1("altitude"), qry.value("Altitude").toString());
        inf.insert(QString::fromLatin1("vr_speed"), qry.value("Vr_speed").toString());
        inf.insert(QString::fromLatin1("speed"), QString::number(qry.value("Speed").toInt()));
        inf.insert(QString::fromLatin1("heading"),QString::number(qry.value("Heading").toInt()));

//        fl.insert("icao",qry.value("icao").toString());
//        fl.insert("",inf);
        arr.append(inf);
       // qDebug()<<arr;
    }

    QJsonDocument doc = QJsonDocument::fromVariant(arr.toVariantList());
 //   qDebug()<<doc.toJson();
    send(doc.toJson());
}

void dataSender::send(QByteArray data)
{
    QUrl url = QUrl(Url_str);
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Name","SomeValue");
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QEventLoop loop;
    QNetworkReply *reply = manager.post(request,data);
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();

}

void dataSender::sendJson()
{
    QJsonObject fl, inf;
    QJsonArray arr;
    if( !qry->exec() )
    {
       QSqlError err = qry->lastError();
       qDebug()<<err;
    }

    //icao ,callsign , lon , lat , alt , vr_speed , speed , heading

    while (qry->next())
    {
        inf.insert(QString::fromLatin1("callsign"), qry->value("callsign").toString());
        inf.insert(QString::fromLatin1("lon"), qry->value("lon").toString());
        inf.insert(QString::fromLatin1("lat"), qry->value("lat").toString());
        inf.insert(QString::fromLatin1("alt"), qry->value("alt").toString());
        inf.insert(QString::fromLatin1("vr_speed"), qry->value("vr_speed").toString());
        inf.insert(QString::fromLatin1("speed"), qry->value("speed").toString());
        inf.insert(QString::fromLatin1("heading"),qry->value("heading").toString());

        fl.insert("icao",qry->value("icao").toString());
        fl.insert("info",inf);
        arr.append(fl);
    }

    QJsonDocument doc = QJsonDocument::fromVariant(arr.toVariantList());
    qDebug()<<doc.toJson();
    send(doc.toJson());
}
