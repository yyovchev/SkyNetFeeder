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


class Locations
{
public:
    Locations();
    QPoint getCoordinates();

private:
    void getTownLocation();
    void getCoordinatesOnline();
    void writeCoordinates();

private:
    QPoint coordinates;

};

#endif // LOCATIONS_H
