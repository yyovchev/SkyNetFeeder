#ifndef LOCALREADER_H
#define LOCALREADER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QTcpServer>
#include <QTimer>
#include "binaryparser.h"

#define STATE_WAIT 0
#define STATE_COLLECT 1
#define MAX_LINE_LENGTH 50



class localreader : public QObject
{
    Q_OBJECT
public:
    QString n_str;
    const char *charset;

private:
    QTcpSocket *socket;
    BinaryParser parser;
    int state;
    char line[MAX_LINE_LENGTH+1];
    unsigned char bin_data[MAX_LINE_LENGTH/2];
    size_t read_pos;
    QTimer *timer;

public:
    explicit localreader(QObject *parent = 0);
    int hex2bin(const char *input, unsigned int count, unsigned char *output);
    ~localreader();

private slots:
    void ReadData();
    void Disconnected();
    void TryToConnect();

signals:

public slots:


};

#endif // LOCALREADER_H
