#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QPoint>
#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QObject>
#include <QSslConfiguration>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QNetworkConfigurationManager>

struct CoordinatePoint {
    double x;
    double y;
};



class Locations
{
private:
    QSqlDatabase TownLoacations;
    QSqlQuery *qry;

public:
    Locations();
    ~Locations();
    CoordinatePoint getTownLocation(QString name);
    CoordinatePoint getCoordinatesOnline();

private:
    CoordinatePoint coordinates;

};

#endif // LOCATIONS_H
