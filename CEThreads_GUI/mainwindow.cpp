#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

