#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <iostream>
#include <vector>
#include "prueba.c"
#include "initialize.c"
#include "linkedList.c"



std::vector<QLabel*> *colaIzquierda ;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bkgnd(":/canalResize.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->centralWidget()->setAutoFillBackground(true);
    this->centralWidget()->setPalette(palette);

/*
    //Doing the SetUp
    QString rutaConfig = QCoreApplication::applicationDirPath() + "/config.txt"; //SetUp("/home/dcastroe/Desktop/En_Git/CEThreads/CEThreads_GUI/config.txt");
    // Convertir QString a const char*
    QByteArray ba = rutaConfig.toLocal8Bit();
    const char *rutaC = ba.data();
    CanalConfig initial_configuration = Initialize_Configuration(rutaC);
*/

    CanalConfig initial_configuration = Initialize_Configuration(":/config.txt");
    qDebug() << "Datos obtenidos desde el txt:";
    qDebug() << "1) Método de Control de Flujo-> "<<initial_configuration.metodoControlFlujo;
    qDebug() << "2) Largo del Canal-> "<<initial_configuration.largoCanal;
    qDebug() << "3) Velocidad del Barco-> "<<initial_configuration.velocidadBarco;
    qDebug() << "4) Cantidad de Barcos-> "<<initial_configuration.cantidadBarcos;
    qDebug() << "5) Tiempo que cambia el Letrero-> "<<initial_configuration.tiempoLetrero;
    qDebug() << "6) Parámetro W-> "<<initial_configuration.parametroW;
}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionleft_triggered()
{
    QPixmap *boat1 = new QPixmap(":/boat1.jpg");
    std::cout << "left action pressed" << std::endl;
    QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->canal09->setScaledContents(true);
    ui->canal09->setPixmap(scaledBoat1);

    delete boat1; //liberar la memoria del boat1

    struct List *prueba_lista = createList(sizeof(int));

    for (int i = 1; i <= 5; i++) {
        int *element = new int(i); // Crear un nuevo int en el heap
        addLast(prueba_lista, element);
    }

    printIntList(prueba_lista);

}



void MainWindow::on_actionright_triggered()
{
    QPixmap *boat2 = new QPixmap(":/boat2.jpg");
    std::cout << "right action pressed" << std::endl;
    QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->canal00->setScaledContents(true);
    ui->canal00->setPixmap(scaledBoat2);
    //delete boat2;
}

void MainWindow::on_pruebaStructs_clicked()
{
    DummieStruct *dummie1 = new DummieStruct({10, 1, 101});
    DummieStruct *dummie2 = new DummieStruct({20, 2, 102});
    DummieStruct *dummie3 = new DummieStruct({30, 3, 103});
    DummieStruct *dummie4 = new DummieStruct({40, 4, 104});
    DummieStruct *dummie5 = new DummieStruct({50, 5, 105});

    struct List *dummy_list = createList(sizeof(DummieStruct));

    addLast(dummy_list, dummie1);
    addLast(dummy_list, dummie2);
    addLast(dummy_list, dummie3);
    addLast(dummy_list, dummie4);
    addLast(dummy_list, dummie5);



    DummieStruct *dummy_return = new DummieStruct;
    getFirst(dummy_list,dummy_return);
    printf("DummieStruct 1: Position = %d, Velocidad = %d, Thread ID = %d\n",dummy_return->position, dummy_return->velocidad, dummy_return->thread_ID);
    removeFirst(dummy_list);

    getFirst(dummy_list,dummy_return);
    printf("DummieStruct 2: Position = %d, Velocidad = %d, Thread ID = %d\n",dummy_return->position, dummy_return->velocidad, dummy_return->thread_ID);
    removeFirst(dummy_list);

}

