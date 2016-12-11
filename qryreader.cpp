#include "qryreader.h"

qryReader::qryReader()
{
    file = new QFile("qry.txt");
    file->open(QIODevice::ReadOnly);

    QTextStream out(file);


    QString qrystr =out.readAll();
//    qDebug()<<qrystr;

    qrys = qrystr.split("\n");

    // tab (a text,  text);

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    /// AddDatebase()
    m_db.setDatabaseName("test.sql");
    if( !m_db.open() )
    {
      qFatal( "Failed to connect. 01" );
    }

    qry = new QSqlQuery(m_db);

    execqry();
}

void qryReader::execqry()
{

    QString str;
    foreach (str, qrys) {
        qry->exec(str);
        qDebug()<<str;

    }
}
