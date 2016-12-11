#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showRadarButton->setEnabled(false);

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
    isRadarOn = false;
    isRadarVisible = false;
    ui->radar->hide();
    setMinimumWidth(230);
    setGeometry(0,0,230,650);
    setMaximumWidth(230);

    ui->LocMenu_1->hide();
    ui->LocMenu_2->hide();

    DB_reader = new dataReader(this);

    timer = new QTimer(this);
    timer->start(1000);

//    if (isRadarOn)
    connect(timer,SIGNAL(timeout()),this,SLOT(SetSources()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSources()
{
    Aircrafts mid;

     mid = DB_reader->getFlights();
     //if (mid.isEmpty())
     ui->radar->setSources(mid);

//     ui->radar->update();
}


void MainWindow::on_showRadarButton_clicked()
{
    if (isRadarVisible){
        ui->radar->setVisible(false);
        setMinimumWidth(230);
        setMaximumWidth(230);
        ui->showRadarButton->setText("Show radar");
        isRadarVisible = ui->radar->isVisible();
    }
    else {
        setMaximumWidth(700);
        setMinimumWidth(700);
        ui->radar->setVisible(true);
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
