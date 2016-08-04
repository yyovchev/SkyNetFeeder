#ifndef OEVALUES_H
#define OEVALUES_H

#include <QTime>
#include <QHash>
#include <QHashIterator>

struct MidValue
{
    int Long_even;
    int Long_odd;
    int Lat_even;
    int Lat_odd;
    QTime FrPart;

public:
    MidValue():
        Long_even(0),
        Long_odd(0),
        Lat_even(0),
        Lat_odd(0),
        FrPart(QTime::fromString("0","s"))
        {   }
};

typedef QHash<int,MidValue> MidValues;
typedef MidValues::iterator MVIterator;

class OEValues : public MidValues
{
private:
    int icao;

public:
    OEValues();
    void AddIcao(int icao);
    void SetOddValues(int Long, int Lat);
    void SetEvenValues(int Long, int Lat);
    int SecFromFRmsg();   // true when sec < 20;
    void SetTime(QTime time = QTime::currentTime());
    void GetOdd(int &lat, int &lon);
    void GetEven(int &lat, int &lon);
    bool IsTimeNull();

};

#endif // OEVALUES_H
