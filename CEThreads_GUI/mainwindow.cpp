#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include "prueba.c"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_prueba_boton_clicked()
{
    if (prueba_bool){
        ui->label1->setText("Adiós!!");
    }else{
        ui->label1->setText("HOLA!");
    }
    prueba_bool = !prueba_bool;
    print_desdeC(testing, 0);
    testing = testing +1;



    QLabel *label_imagen = new QLabel(this);
    label_imagen->setGeometry(50,50,300,300);


    QPixmap pixmap_bote (QCoreApplication::applicationDirPath() + "/images/bote0.png");
    label_imagen->setScaledContents(true);
    label_imagen->setPixmap(pixmap_bote);
    label_imagen->show();


    std::cout<<QCoreApplication::applicationDirPath().toStdString()<<std::endl;
}

