#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QFile>
#include <QString>

class fileWriter : public QObject
{
    Q_OBJECT
public:
    explicit fileWriter(QObject *parent = 0);

private:
    QString filename;
    QFile file;

signals:

public slots:
};

#endif // FILEWRITER_H
