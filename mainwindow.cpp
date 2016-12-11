#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showRadarButton->setEnabled(false);

///                 CHECK FOR PREVIOUS COORDINATES
    if (settings.value("mylat") != 0 && settings.value("mylong") != 0)
    {
        mycoordinates.x = settings.value("myLat").toDouble();
        mycoordinates.y = settings.value("myLong").toDouble();


        ui->myLat_info->setText(QString::number(mycoordinates.x));
        ui->myLong_info->setText(QString::number(mycoordinates.y));

    }

    if (mycoordinates.x !=0 && mycoordinates.y != 0)
    {
        //ui->showRadarButton->setEnabled(true);
        isRadarOn = true;
    }

///                  HIDE RADAR FORM
    isRadarOn = false;
    isRadarVisible = false;
    ui->radar->hide();
    setMinimumWidth(230);
    setGeometry(0,0,230,650);
    setMaximumWidth(230);

    ui->LocMenu_1->hide();
    ui->LocMenu_2->hide();

///                     CREATE TABLE AND ADD HEADER LABELS
    labels<<"ICAO"<<"Callsign"<<"Latitude"<<"Longitude"<<"Altitude"<<"Headingr"<<"Speed"<<"Vertical speed";
    ui->AircraftsTable->setColumnCount(8);
    ui->AircraftsTable->setHorizontalHeaderLabels(labels);
    ui->AircraftsTable->setRowCount(25);
    tableRows = ui->AircraftsTable->rowCount();

    ui->AircraftsTable->setItem(1,1,new QTableWidgetItem("dsd"));

///                     TIMER 1~5 SEC PRINT, 6ST SEND
///                     SetSourses get Aircrafts form datareader and send them to radar
    DB_reader = new dataReader(this);

    timer = new QTimer(this);
    timer->start(1000);

    connect(timer,SIGNAL(timeout()),this,SLOT(SetSources()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillTable(Aircrafts crafts)
{
     int row = 0;
     int rows = crafts.size();
     qDebug()<<rows;
    // ui->AircraftsTable->clearContents();

     if (tableRows != rows){
         tableRows = rows;
     }

     ui->AircraftsTable->setRowCount(tableRows);

     ui->AircraftsTable->clearContents();
     for (AConstIterator iter = crafts.begin(), end = crafts.end(); iter != end; row++, iter ++){
         ui->AircraftsTable->setItem(row,0,new QTableWidgetItem(QString::number(iter.key(),16)));
         ui->AircraftsTable->setItem(row,1,new QTableWidgetItem(iter.value().Callsign));
         ui->AircraftsTable->setItem(row,2,new QTableWidgetItem(QString::number(iter.value().Latitude)));
         ui->AircraftsTable->setItem(row,3,new QTableWidgetItem(QString::number(iter.value().Longitude)));
         ui->AircraftsTable->setItem(row,4,new QTableWidgetItem(QString::number(iter.value().Altitude)));
         ui->AircraftsTable->setItem(row,5,new QTableWidgetItem(QString::number(iter.value().Heading)));
         ui->AircraftsTable->setItem(row,6,new QTableWidgetItem(QString::number(iter.value().Speed)));
         ui->AircraftsTable->setItem(row,7,new QTableWidgetItem(QString::number(iter.value().Vertical_Rate)));
     }
}

void MainWindow::SetSources()
{

     AircraftsValues = DB_reader->getFlights();
     Aircrafts crafts = AircraftsValues;
     ui->radar->setSources(AircraftsValues);
     FillTable(crafts);
}


void MainWindow::on_showRadarButton_clicked()
{
    if (isRadarVisible){
        ui->radar->setVisible(false);
        ui->AircraftsTable->setVisible(false);
        setMinimumWidth(230);
        setMaximumWidth(230);
        ui->showRadarButton->setText("Show radar");
        isRadarVisible = ui->radar->isVisible();
    }
    else {
        setMaximumWidth(700);
        setMinimumWidth(700);
        ui->radar->setVisible(true);
        ui->AircraftsTable->setVisible(true);
        ui->showRadarButton->setText("Hide radar");
        isRadarVisible = ui->radar->isVisible();
    }
}

void MainWindow::on_rB_auto_clicked()
{
    if(ui->rB_auto->isChecked())
    {
        ui->LocMenu_1->hide();
        ui->LocMenu_2->hide();
    }
}

void MainWindow::on_rB_town_clicked()
{
    if(ui->rB_town->isChecked())
    {
        ui->LocMenu_1->show();
        ui->LocMenu_2->hide();

        ui->label_line_1->setText("Town's name ?");
        ui->lineEdit->setText("");
    }
}

void MainWindow::on_rB_crd_clicked()
{
    if (ui->rB_crd->isChecked())
    {
        ui->LocMenu_1->show();
        ui->LocMenu_2->show();

        ui->label_line_1->setText("Latitude ?");
        ui->lineEdit->setText("");
        ui->label_line_2->setText("Longitude ?");
        ui->lineEdit_2->setText("");

    }
}

void MainWindow::on_button_Start_clicked()
{
    if (!isRadarOn){

        if (ui->rB_crd->isChecked())
        {
            mycoordinates.x = ui->lineEdit->text().toDouble();
            mycoordinates.y = ui->lineEdit_2->text().toDouble();

        }if (ui->rB_auto->isChecked())
        {
            mycoordinates = locs.getCoordinatesOnline();

        }if (ui->rB_town->isChecked())
        {
            mycoordinates = locs.getTownLocation(ui->lineEdit->text());
        }

        ui->myLat_info->setText(QString::number(mycoordinates.x));
        ui->myLong_info->setText(QString::number(mycoordinates.y));
        settings.setValue("myLat",mycoordinates.x);
        settings.setValue("myLong",mycoordinates.y);

        ui->button_Start->setText("Stop");
        isRadarOn = true;
        ui->showRadarButton->setEnabled(true);

        ui->rB_auto->setEnabled(false);
        ui->rB_crd->setEnabled(false);
        ui->rB_town->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
    }
    else {

        ui->button_Start->setText("Start");
        isRadarOn = false;
        ui->showRadarButton->setEnabled(false);
        ui->radar->hide();setMinimumWidth(230);
        setMaximumWidth(230);
        isRadarVisible = false;
        ui->showRadarButton->setText("Show radar");

        ui->rB_auto->setEnabled(true);
        ui->rB_crd->setEnabled(true);
        ui->rB_town->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
}
