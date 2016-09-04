#include "locations.h"

Locations::Locations()
{
    // coordinates which we will return to the main function
    // x: longitude
    // y: latitude
    coordinates = QPoint(0,0);
}

QPoint Locations::getCoordinates()
{
    int w;
    std::cin>>w;

    if (w==1)
        getTownLocation();
    else if (w==2)
        getCoordinatesOnline();
    else if (w == 3)
        writeCoordinates();


    return coordinates;
}

void Locations::getTownLocation()
{
    std::string town;
    std::cin>>town;
    QString townName = QString::fromStdString(town);

    //SQLite database with names(en, bg) and locations of every town in Bulgaria
    QSqlDatabase TownLoacations = QSqlDatabase::addDatabase("QSQLITE");
    TownLoacations.setDatabaseName("/home/yordan/sqlite/loc");

    if( !TownLoacations.open() )
    {
      qDebug( "Failed to connect loc database" );
    }

    QSqlQuery qry(TownLoacations);
    qry.prepare("select loc from location where ekatte = (select ekatte from names where name_en = '" +townName+"')");
    if (!qry.exec())
    {
        qDebug("Town not found !");
    }
    else {
        qry.next();
        qDebug()<<qry.value("loc").toString();
    }
}

void Locations::getCoordinatesOnline()
{
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

    qDebug()<<jsdata["loc"].toString();

    reply->deleteLater();
}

void Locations::writeCoordinates()
{
    int test;
    std::cout<<"long: ";
    std::cin>>test;
    std::cout<<"lat: ";
    std::cin>>test;
}















