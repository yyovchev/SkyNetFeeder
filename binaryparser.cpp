#include "binaryparser.h"
#include <cmath>
#include <QTime>

BinaryParser::BinaryParser():
    charset("#ABCDEFGHIJKLMNOPQRSTUVWXYZ##### ###############0123456789######")
{
    ICAO = 0;
    Callsign = "";

    Speed = 0;
    Heading = 0;
    Vr_speed = 0;

    Longitude = 0;
    Latitude = 0;
    Altitude = 0;

    LAT_CPR_EVEN = 0;
    LON_CPR_EVEN = 0;
    LAT_CPR_ODD = 0;
    LON_CPR_ODD = 0;

    sources = new AircraftsDB();
    OddEvenValues = new OEValues();
}


BinaryParser::~BinaryParser()
{
    delete charset;
    delete sources;
    delete OddEvenValues;
}


DataTypes BinaryParser::parseData(const unsigned char *data) {
    int df = (data[0] >> 3);

    //check if msg's type id DF17
    if (df!=17) // if it is not DF17 format return TYPE_UNKOWN
        return TYPE_UNKOWN;
    else if (df == 17)
    {
        GetICAO(data);  // Get ICOA and insert it into tables
        int tc = data[4]>>3;
         if (tc <= 4)
         {
             IDDecode(data); // Get Callsign
             return TYPE_DF17_TC4;
         }
         else if (tc == 17 || tc== 19)
         {
             GetSpeed(data); // Get Speed, vertical speed and track
             return TYPE_DF17_TC17;
         }
         else if (tc >= 9 && tc <= 18)
         {
             GetPosition(data); // Get Longitude, latitude and altitude
             return TYPE_DF17_AllPos;
         }
    }
    return TYPE_UNKOWN;
}


void BinaryParser::IDDecode(const unsigned char *data)
{
    int ch_num;
    Callsign = "";

//    Get Callsign
    ch_num = data[5]>>2;
    Callsign+=charset[ch_num];

    ch_num =  ((data[5]&0x3)<<4)|(data[6]>>4);
    Callsign+=charset[ch_num];

    ch_num= ((data[6]&0xF)<<2)|(data[7]>>6);
    Callsign+=charset[ch_num];

    ch_num= data[7]&0x3F;
    Callsign+=charset[ch_num];
    
    ch_num = data[8]>>2;
    Callsign+=charset[ch_num];

    ch_num =  ((data[8]&0x3)<<4)|(data[9]>>4);
    Callsign+=charset[ch_num];

    ch_num= ((data[9]&0xF)<<2)|(data[10]>>6);
    Callsign+=charset[ch_num];

    ch_num= data[10]&0x3F;
    Callsign+=charset[ch_num];

    Callsign.replace("#","");

    //Add callsign in DB
    sources->UpdateValue(ICAO, Callsign);

}


void BinaryParser::GetICAO(const unsigned char *data)
{
    int c;
    ICAO = 0;
//  Get ICAO
    for (int i=1;i<=3;i++)
    {
        c=data[i]>>4;
        ICAO = (ICAO<<4)|c;

        c=data[i]&0x0f;
        ICAO = (ICAO<<4)|c;
    }

//  check and add ICAO
    OddEvenValues->AddIcao(ICAO);
}


void BinaryParser::GetSpeed(const unsigned char*data)
{
    bool SEw = (data[5]>>2)&0x1;
    int VEw = (data[5]&0x3);
    VEw= (VEw<<8)|data[6];

    bool SNs = data[7]>>7;
    int VNs = (data[7]&0x7F);
    VNs= (VNs<<3)|(data[8]>>5);

    if (SEw) VEw*=-1;
    if (SNs) VNs*=-1;

//    Speed
    double v=(VEw*VEw)+(VNs*VNs);
    Speed= sqrt(v);

//    Heading
    Heading = atan2((double)VEw,(double)VNs)*180.0/3.14159265359;
    if (Heading < 0) Heading+=360;

//    Vertical Rate
    int SVr = (data[8]>>3)&0x1;
    int V_r = (data[8]&0x7);
    V_r = (V_r<<6)|(data[9]>>2);
    Vr_speed=V_r;
    if (SVr == 1)
        Vr_speed*=-1;


//  Update Speed, Heading, Vertical speed
    sources->UpdateValues(ICAO, Speed, Vr_speed, Heading);

}


void BinaryParser::GetPosition(const unsigned char *data)
{
    //Decode and update altitude value
    int lt = (data[5] << 4) | (data[6] >> 4);
    int n = ((lt & 0x0FE0) >> 1) | (lt & 0x000F);
    Altitude = ((n * 25) - 1000);
    sources->UpdateValue(ICAO,AircraftsDB::Altitude,Altitude);


    //Decode and update Long Lat value
    if (OddEvenValues->IsTimeNull()) // Check is this is the frirst part
        OddEvenValues->SetTime();

    bool frame = (data[6]>>2)&0x1;

    int lon = 0;
    int lat = 0;

    if (frame) // Odd frame
    {
        lat = ((data[6] & 0x03) << 15) | (data[7] << 7) | (data[8] >> 1);
        lon = ((data[8] & 0x01) << 16) | (data[9] << 8) | data[10];
        OddEvenValues->SetOddValues(lon,lat);
    }
    else // Even frame
    {
        lat = ((data[6] & 0x03) << 15) | (data[7] << 7) | (data[8] >> 1);
        lon = ((data[8] & 0x01) << 16) | (data[9] << 8) | data[10];
        OddEvenValues->SetEvenValues(lon,lat);
    }

    if (OddEvenValues->SecFromFRmsg() > 20) // if the fr msg is older than 20s
    {                                       // clear mid values
        OddEvenValues->SetTime(QTime::fromString("0", "s"));
        OddEvenValues->SetOddValues(0,0);
        OddEvenValues->SetEvenValues(0,0);
        return;
    }

    OddEvenValues->GetEven(LAT_CPR_EVEN,LON_CPR_EVEN);
    OddEvenValues->GetOdd(LAT_CPR_ODD,LON_CPR_ODD);

    if (LAT_CPR_EVEN == 0|| LAT_CPR_ODD == 0)
    {
        return ;
    }

    int j = floor(59.0*(double)LAT_CPR_EVEN/131072.0 - 60.0*(double)LAT_CPR_ODD/131072.0 + 0.5);

    double LAT_E = 6 * (Modulo(j,60) + LAT_CPR_EVEN / 131072.0);
    double LAT_O = 6.101694915254237288 * (Modulo(j,59) + LAT_CPR_ODD / 131072.0);

    if (LAT_E >= 270)
        LAT_E = LAT_E - 360;

    if (LAT_O >= 270)
        LAT_O = LAT_O - 360;

    int NL = getNL(LAT_E);
    if (NL != getNL(LAT_O))
    {
        return ;
    }

    if (!frame)
    {
        Latitude = LAT_E;
        int ni = (NL > 1 ? NL : 1);
        int m = floor(LON_CPR_EVEN / 131072.0 *(NL-1) - LON_CPR_ODD/ 131072.0 * NL + 0.5);
        Longitude = double (360./ni) * (Modulo(m,ni) + LON_CPR_EVEN / 131072.0);
    }
    else
    {
         Latitude = LAT_O;
         int ni = ((NL - 1) > 1 ? (NL - 1) : 1);
         int m = floor(LON_CPR_EVEN / 131072.0 *(NL-1) - LON_CPR_ODD/ 131072.0 * NL + 0.5);
         Longitude = double(360./ni) * (Modulo(m,ni) + LON_CPR_ODD/ 131072.0);
    }

    if (Longitude >= 180)
        Longitude = Longitude - 360;

//  Add Longitude and Latitude
    sources->UpdatePositions(ICAO,Longitude,Latitude);

//  Clear Odd, Even values
    OddEvenValues->SetTime(QTime::fromString("0", "s"));
    OddEvenValues->SetOddValues(0,0);
    OddEvenValues->SetEvenValues(0,0);

    return ;
}


// Number of Longitude Zones
int BinaryParser::getNL(double lat) {
    if (lat<0) lat = -lat;
    if (lat < 10.47047130)
        return (59);
    if (lat < 14.82817437)
        return (58);
    if (lat < 18.18626357)
        return (57);
    if (lat < 21.02939493)
        return (56);
    if (lat < 23.54504487)
        return (55);
    if (lat < 25.82924707)
        return (54);
    if (lat < 27.93898710)
        return (53);
    if (lat < 29.91135686)
        return (52);
    if (lat < 31.77209708)
        return (51);
    if (lat < 33.53993436)
        return (50);
    if (lat < 35.22899598)
        return (49);
    if (lat < 36.85025108)
        return (48);
    if (lat < 38.41241892)
        return (47);
    if (lat < 39.92256684)
        return (46);
    if (lat < 41.38651832)
        return (45);
    if (lat < 42.80914012)
        return (44);
    if (lat < 44.19454951)
        return (43);
    if (lat < 45.54626723)
        return (42);
    if (lat < 46.86733252)
        return (41);
    if (lat < 48.16039128)
        return (40);
    if (lat < 49.42776439)
        return (39);
    if (lat < 50.67150166)
        return (38);
    if (lat < 51.89342469)
        return (37);
    if (lat < 53.09516153)
        return (36);
    if (lat < 54.27817472)
        return (35);
    if (lat < 55.44378444)
        return (34);
    if (lat < 56.59318756)
        return (33);
    if (lat < 57.72747354)
        return (32);
    if (lat < 58.84763776)
        return (31);
    if (lat < 59.95459277)
        return (30);
    if (lat < 61.04917774)
        return (29);
    if (lat < 62.13216659)
        return (28);
    if (lat < 63.20427479)
        return (27);
    if (lat < 64.26616523)
        return (26);
    if (lat < 65.31845310)
        return (25);
    if (lat < 66.36171008)
        return (24);
    if (lat < 67.39646774)
        return (23);
    if (lat < 68.42322022)
        return (22);
    if (lat < 69.44242631)
        return (21);
    if (lat < 70.45451075)
        return (20);
    if (lat < 71.45986473)
        return (19);
    if (lat < 72.45884545)
        return (18);
    if (lat < 73.45177442)
        return (17);
    if (lat < 74.43893416)
        return (16);
    if (lat < 75.42056257)
        return (15);
    if (lat < 76.39684391)
        return (14);
    if (lat < 77.36789461)
        return (13);
    if (lat < 78.33374083)
        return (12);
    if (lat < 79.29428225)
        return (11);
    if (lat < 80.24923213)
        return (10);
    if (lat < 81.19801349)
        return (9);
    if (lat < 82.13956981)
        return (8);
    if (lat < 83.07199445)
        return (7);
    if (lat < 83.99173563)
        return (6);
    if (lat < 84.89166191)
        return (5);
    if (lat < 85.75541621)
        return (4);
    if (lat < 86.53536998)
        return (3);
    if (lat < 87.00000000)
        return (2);
    return (1);
}

int BinaryParser::Modulo(int x, int y) {
        int r;
        r = x % y;
        if (r < 0)
                r += y;
        return (r);
}

