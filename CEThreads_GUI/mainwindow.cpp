#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <iostream>
#include "prueba.c"
#include "initialize.c"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Doing the SetUp
    QString rutaConfig = QCoreApplication::applicationDirPath() + "/config.txt"; //SetUp("/home/dcastroe/Desktop/En_Git/CEThreads/CEThreads_GUI/config.txt");
    // Convertir QString a const char*
    QByteArray ba = rutaConfig.toLocal8Bit();
    const char *rutaC = ba.data();
    CanalConfig initial_configuration = Initialize_Configuration(rutaC);

    //printf("Método de Control de Flujo: %s\n", config.metodoControlFlujo);
    //printf("Largo del Canal: %d\n", config.largoCanal);
    //printf("Velocidad del Barco: %d\n", config.velocidadBarco);
    //printf("Cantidad de Barcos: %d\n", config.cantidadBarcos);
    //printf("Tiempo que cambia el Letrero: %d\n", config.tiempoLetrero);
    //printf("Parámetro W: %d\n", config.parametroW);


    qDebug() << "AJJAJAJAJAJJAJA-> "<<initial_configuration.cantidadBarcos;

    QPixmap bkgnd(":/canalResize.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    //Initialize_Configuration(rutaC);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_left_action_clicked(){
    qDebug() << "left action pressed ";
}

void MainWindow::on_prueba_boton_clicked()
{
    /*if (prueba_bool){
        ui->label1->setText("Adiós!!");
    }else{
        ui->label1->setText("HOLA!");
    }*/
    prueba_bool = !prueba_bool;
    print_desdeC(testing, 0);
    testing = testing +1;


//QLabel *canal0 = new QLabel(this);


/*
    QLabel *label_imagen = new QLabel(this);
    label_imagen->setGeometry(50,50,300,300);

    QPixmap pixmap_bote (QCoreApplication::applicationDirPath() + "/images/bote0.png");
    label_imagen->setScaledContents(true);
    label_imagen->setPixmap(pixmap_bote);
    label_imagen->show();
*/
    std::cout<<QCoreApplication::applicationDirPath().toStdString()<<std::endl;
}

