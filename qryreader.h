#ifndef QRYREADER_H
#define QRYREADER_H

// debug
#include <QDebug>

// file lib
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>

//sql lib
#include <QSqlDatabase>
#include <QSqlQuery>

class qryReader
{
public:
    qryReader();
    void execqry(); // executes the SQL query
private:

    QFile *file;        // file which is used "qry.txt"
    QStringList qrys;   // contains queries

    QSqlDatabase m_db;
    QSqlQuery *qry;
};

#endif // QRYREADER_H
