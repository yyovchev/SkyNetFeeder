#include <QPaintEvent>
#include "radar.h"
#include <math.h>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QDebug>
#include <math.h>
#include <QMetaMethod>
#include <QTime>
#include <QFile>

Radar::Radar(QWidget *parent) : QWidget(parent), sprite(":/resources/images/sprite-icons.png")
{
    RadarRange = 150;
    draw_cout=0;
    background=NULL;
    d = (this->width() > this->height()) ? this->width() : this->height();
    offset = 18;
    icon_width = 34;
    icon_height = 34;
    hit_x = icon_width / 2.0;
    hit_y = icon_height / 2.0;
    selectedIcao = 0;

    timer = new QTimer();
    timer->start(3000);
    connect(timer,SIGNAL(timeout()),this,SLOT(readFile()));

}

Radar::~Radar()
{
    delete background;
}

double Radar::getX(double L)
{
    return (d/2)+d/2*sin(D2R(L));
}

double Radar::getY(double L)
{
    return (d/2)-d/2*cos(D2R(L));
}

void Radar::paintEvent(QPaintEvent *event) {
    d = ((this->width() > this->height()) ? this->height() : this->width()) - 2 * offset;
    QPainter painter;

    int new_range = settings.value("Radar_Range").toInt();
    if (new_range != RadarRange)
        RadarRange = new_range;

    if (background != NULL && (background->width() != this->width() || background->height() != this->height())) {
        delete background;
        background = NULL;
    }

    if (background == NULL) {
        drawRadarToBackground(painter);
    }

    painter.begin(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(0, 0, *background);
    PrintPlanes(painter);
    painter.end();
    event->accept();
}

void Radar::drawRadarToBackground(QPainter &painter) {
    double step = d/5;
    background = new QPixmap(this->width(), this->height());
    painter.begin(background);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillRect(background->rect(), Qt::darkCyan);
    QPen pen;
    pen.setWidthF(0.8);
    pen.setColor(QColor(10,50,10,224));
    painter.setPen(pen);
    painter.setBrush(QColor(1,182,5,100));
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i=0; i<5; i++) {
        painter.drawEllipse(offset+step/2*i, offset+step/2*i, d-i*step, d-i*step);
    }

    const QChar deg(0xB0);
    for (int i=0, step=30, end=(360/step/2); i < end; i++) {
        int d_start = i * step;
        int d_end = d_start + 180;
        double x1 = offset + getX(d_start);
        double y1 = offset + getY(d_start);
        double x2 = offset + getX(d_end);
        double y2 = offset + getY(d_end);
        painter.setRenderHint(QPainter::Antialiasing, (d_start % 90)); // Спира Antialiasing за всяка линя кратна на 90 градуса
        painter.drawLine(x1, y1, x2, y2);
        painter.drawText(QPointF(x1, y1), QString::number(d_start) + deg);
        painter.drawText(QPointF(x2, y2), QString::number(d_end) + deg);
    }

    int r=d/2;
    for (int i=1; i<=4; i++){
        int RangeStep = RadarRange/5;
        painter.drawText(r+step/2*i+offset, r+offset, QString::number(RangeStep*i));
    }
    painter.end();
}

void Radar::readFile()
{
    Acs.clear();

    Aircraft Ac;
    QFile file("new.txt");
        if (!file.open(QIODevice::ReadOnly)){
            qDebug()<<file.errorString();
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            in>>Ac.icao;
            in>>Ac.Callsign;
            in>>Ac.Longitude;
            in>>Ac.Latitude;
            in>>Ac.Altitude;
            in>>Ac.Heading;
            in>>Ac.Speed;
            in>>Ac.Vertical_Rate;
            in>>Ac.r_x;
            in>>Ac.r_y;
            in>>Ac.Distance;
//            qDebug()<<QString::number(Ac.icao,16).toUpper();
            if (Ac.icao != 0 && Ac.Distance < RadarRange)
            Acs.insert(Ac.icao,Ac);
        }
        this->update();
}

void Radar::PrintPlanes(QPainter &painter)
{
    int row = 1;
    int col = 0;
    for (AConstIterator iter = Acs.begin(), end = Acs.end(); iter != end; iter ++)
    {
        qDebug("print");
        row = 1;
        col = (int)(iter.value().Heading / 11.25);

        if (iter.key() == selectedIcao)
        {
            QPen pen;
            pen.setColor(QColor(180, 200, 0));
            pen.setWidthF(2);
            painter.setBrush(QColor(0, 0, 0, 50));
            painter.setPen(pen);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawEllipse(QPointF(iter.value().r_x, iter.value().r_y), 19, 19);
            painter.setRenderHint(QPainter::Antialiasing, false);
            row = 0;
        }


        painter.drawPixmap(iter.value().r_x - 17, iter.value().r_y - 17, icon_width, icon_height,
                               sprite,
                               col * icon_width, row * icon_height, icon_width, icon_height);
    }
}


void Radar::mousePressEvent(QMouseEvent *event)
{
    qDebug("dddd");
    if (Acs.isEmpty()){
        return;
    }

    hit_iterator = hitTest(event->pos());

    if (hit_iterator != Acs.cend() && selectedIcao != hit_iterator.key() ) {
        selectedIcao = hit_iterator.key();
        //emit planeSelected(hit_iterator);   send the info about the plane to the mainwindows
        this->update();
    }
    event->accept();
}


AConstIterator Radar::hitTest(QPoint p)
{
    if (!Acs.isEmpty()) {
            for (AConstIterator iter=Acs.cbegin(), end = Acs.cend(); iter != end; iter++) {
                int dx = abs(iter.value().r_x - p.x());
                int dy = abs(iter.value().r_y - p.y());
                if (dx <= hit_x && dy <= hit_y) {
                    return iter;
                }
            }
        }
        return Acs.cend();
}
