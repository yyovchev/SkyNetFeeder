#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmap>
#include <QHash>
#include <QTimerEvent>
#include <QTimer>
#include <QPainter>
#include "defines.h"
#include "aircrafts.h"
#include <QSettings>



class Radar : public QWidget
{
    Q_OBJECT
public:
    int draw_cout;
    double d;
    QPalette Pal;
    double lon, lat;
    double Distance;
    double Bearing;
    int RadarRange;

public:
    explicit Radar(QWidget *parent = 0);
    ~Radar();
    double getX(double L);
    double getY(double L);

private:
    QPixmap *background;
    QPixmap *planeIco;
    QPixmap sprite;
    int icon_width;
    int icon_height;
    float offset;
    double radar_top;
    double radar_left;
    QTimer *timer;
    Aircrafts Acs;
    QSettings settings;
    double hit_x;
    double hit_y;
    int selectedIcao;
    AConstIterator hit_iterator;


private:
    void drawRadarToBackground(QPainter &painter);
    void PrintPlanes(QPainter &painter);
    AConstIterator hitTest(QPoint p);


protected:
    void mousePressEvent(QMouseEvent * event); //
    void paintEvent(QPaintEvent *event);



signals:

private slots:
    void readFile();
};

#endif // RADAR_H
