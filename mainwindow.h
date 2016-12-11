#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QPoint>
#include <QSettings>
#include <QStringList>

#include "aircrafts.h"
#include "datareader.h"
#include "locations.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void FillTable(Aircrafts crafts);

private slots:
    void SetSources();

    void on_showRadarButton_clicked();


    void on_rB_auto_clicked();

    void on_rB_town_clicked();

    void on_rB_crd_clicked();

    void on_button_Start_clicked();

private:
    Ui::MainWindow *ui;
    QSettings settings;
    QTimer *timer;
    dataReader *DB_reader;
    bool isRadarOn;
    bool isRadarVisible;
    QStringList labels;
    int tableRows;

    CoordinatePoint mycoordinates;
    Aircrafts AircraftsValues;
    Locations locs;

};

#endif // MAINWINDOW_H
