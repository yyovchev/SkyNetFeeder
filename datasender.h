#ifndef DATASENDER_H
#define DATASENDER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <iostream>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QEventLoop>
#include <QNetworkReply>
#include <QTimer>

class dataSender : public QObject
{
    Q_OBJECT
public:
    explicit dataSender(QObject *parent = 0, QString url = "http://softuni.sliven.org/test/upload.php");
    ~dataSender();

public slots:
    void sendJson();

private:
    void send(QByteArray data);

private:
    QSqlDatabase m_db;
    QSqlQuery *qry;
    QString Url_str;
    QTimer *timer;
};

#endif // DATASENDER_H
