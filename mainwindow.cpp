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
    num1 = num1 + 1;

    if (prueba_bool) {
        ui->prueba_label->setText("HOLA");
        prueba_bool = !prueba_bool;
        print_desdeC(num1, num2);
    } else {
        ui->prueba_label->setText("BYE");
        prueba_bool = !prueba_bool;
        print_desdeC(num1, num2);
    }

}
