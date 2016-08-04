#include "oevalues.h"

OEValues::OEValues()
{

}

void OEValues::AddIcao(int icao)
{
    if (!contains(icao))
    {
       MidValue newicao_value;
       insert(icao,newicao_value);
    }
    this->icao = icao;
}

void OEValues::SetOddValues(int Long, int Lat)
{
    MVIterator iter = find(icao);

    if (iter != end())
    {
        MidValue *a = &iter.value();
        a->Lat_odd = Lat;
        a->Long_odd = Long;
    }
}

void OEValues::SetEvenValues(int Long, int Lat)
{
    MVIterator iter = find(icao);

    if (iter != end())
    {
        MidValue *a = &iter.value();
        a->Lat_even = Lat;
        a->Long_even = Long;
    }
}

int OEValues::SecFromFRmsg()
{
    MVIterator iter = find(icao);

    if (iter != end())
    {
        MidValue *a = &iter.value();

        return a->FrPart.secsTo(QTime::currentTime());
    }
}

void OEValues::SetTime(QTime time)
{
    MVIterator iter = find(icao);

    if (iter != end())
    {
        MidValue *a = &iter.value();
        a->FrPart = time;
    }

}

void OEValues::GetOdd(int &lat, int &lon)
{
    MVIterator iter = find(icao);

    if (iter != end())
    {
        MidValue *a = &iter.value();
        lat = a->Lat_odd;
        lon = a->Long_odd;
    }
}

void OEValues::GetEven(int &lat, int &lon)
{
    MVIterator iter = find(icao);

    if (iter == end())
        return;

    MidValue *a = &iter.value();
    lat = a->Lat_even;
    lon = a->Long_even;
}

bool OEValues::IsTimeNull()
{
    MVIterator iter = find(icao);

    if (iter == end())
        return false;

    MidValue *a = &iter.value();
    if (a->FrPart ==  QTime::fromString("0", "s"))
        return true;
    else
        return false;
}
