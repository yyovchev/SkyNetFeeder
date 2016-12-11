#include "locations.h"
#include <QMessageBox>
Locations::Locations()
{
    // coordinates which we will return to the main function
    // x: longitude
    // y: latitude
    coordinates.x = 0;
    coordinates.y = 0;


    //SQLite database with names(en, bg) and locations of every town in Bulgaria
    TownLoacations = QSqlDatabase::addDatabase("QSQLITE","townconnection");
    TownLoacations.setDatabaseName("/home/yordan/sqlite/loc");

    if( !TownLoacations.open() )
    {
      qDebug( "Failed to connect loc database" );
    }

    qry = new QSqlQuery(TownLoacations);
    //qry->prepare("select loc from location where ekatte = (select ekatte from names where name_en = ':name')");
}

Locations::~Locations()
{
    TownLoacations.close();
    delete qry;
}

CoordinatePoint Locations::getTownLocation(QString name)
{
    if(qry->exec("select count(ekatte) from names where name_en ='" +name+"'")){
        qry->next();
        int count = qry->value(0).toInt();
        if(count == 0)
        {
            QMessageBox messageBox;
            messageBox.warning(0,"Error","Town is not in the database !");
            coordinates.x = 0; coordinates.y = 0;
            return coordinates;
        }
    }
    if (qry->exec("select loc from location where ekatte = (select ekatte from names where name_en = '"+name+"')"))
    {
        qry->next();
        QString loc;
        loc = qry->value("loc").toString();
        QStringList locs = loc.split(",");
        loc = locs.at(0);
        coordinates.y = loc.toDouble();
        loc = locs.at(1);
        coordinates.x = loc.toDouble();
    }
    return coordinates;
}

CoordinatePoint Locations::getCoordinatesOnline()
{
    QNetworkConfigurationManager cmgr;
    if (!cmgr.isOnline())
    {
        QMessageBox messageBox;
        messageBox.warning(0,"Error","You are offline !");
        coordinates.x = 0; coordinates.y = 0;
        return coordinates;
    }
    QUrl url = QUrl("http://ipinfo.io/json");
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Name","SomeValue");
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsdata = doc.object();

    QString loc;
    loc = jsdata["loc"].toString();
    QStringList locs = loc.split(",");
    loc = locs.at(0);
    coordinates.x = loc.toDouble();
    loc = locs.at(1);
    coordinates.y = loc.toDouble();

    reply->deleteLater();

    return coordinates;
}
