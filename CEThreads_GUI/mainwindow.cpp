#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <iostream>
#include <vector>
#include "prueba.c"
#include "initialize.c"
#include "linkedList.c"
#include "../SchedulingShips/src/ship.c"

ShipList *pruebashiplist;
std::vector<QLabel*> canal;
std::vector<QLabel*> colaIzquierda;
std::vector<QLabel*> colaDerecha;

struct List *dummy_list_left = createList(sizeof(DummieStruct));
struct List *dummy_list_right = createList(sizeof(DummieStruct));
DummieStruct *dummy_return = new DummieStruct;

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


     //QLabel *element = ui->canal00 ;// Crear un nuevo int en el heap
    setupQueues();
    setupCanal(initial_configuration.largoCanal);

    print_desde_Ship();

    //innitList(pruebashiplist);




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
    //ui->canal09->setScaledContents(true);
    //ui->canal09->setPixmap(scaledBoat1);  //---------------------------------------------------------------------

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
    //ui->canal00->setScaledContents(true);
    //ui->canal00->setPixmap(scaledBoat2);
    //delete boat2;
}

void MainWindow::on_pruebaStructs_clicked()
{
    /*


    DummieStruct *dummie1 = new DummieStruct({10, 1, 101});
    DummieStruct *dummie2 = new DummieStruct({20, 2, 102});
    DummieStruct *dummie3 = new DummieStruct({30, 3, 103});
    DummieStruct *dummie4 = new DummieStruct({40, 4, 104});
    DummieStruct *dummie5 = new DummieStruct({50, 5, 105});

    //struct List *dummy_list = createList(sizeof(DummieStruct));

    addLast(dummy_list1, dummie1);
    addLast(dummy_list1, dummie2);
    addLast(dummy_list1, dummie3);
    addLast(dummy_list1, dummie4);
    addLast(dummy_list1, dummie5);


*/
    printf("Left waiting list \n");
    for (int i = 0; i <= dummy_list_left->length -1; i++) {

        //getFirst(dummy_list1,dummy_return);
        getAt(dummy_list_left,i,dummy_return);
        printf("DummieStruct %d: Position = %d, Velocidad = %d, Thread ID = %d\n",i,dummy_return->position, dummy_return->velocidad, dummy_return->thread_ID);
        //removeFirst(dummy_list1);
        //dummy_return = new DummieStruct;


    }

    printf("Right waiting list \n");
    for (int i = 0; i <= dummy_list_right->length -1; i++) {

        //getFirst(dummy_list1,dummy_return);
        getAt(dummy_list_right,i,dummy_return);
        printf("DummieStruct %d: Position = %d, Velocidad = %d, Thread ID = %d\n",i,dummy_return->position, dummy_return->velocidad, dummy_return->thread_ID);
        //removeFirst(dummy_list1);
        //dummy_return = new DummieStruct;


    }


    //removeFirst(dummy_list1);
/*
    getFirst(dummy_list1,dummy_return);
    printf("DummieStruct 2: Position = %d, Velocidad = %d, Thread ID = %d\n",dummy_return->position, dummy_return->velocidad, dummy_return->thread_ID);
    removeFirst(dummy_list1);*/

}

void MainWindow::setupQueues()
{

    qDebug() << "starting setup";
    colaIzquierda.push_back(ui->waitingL0);
    colaIzquierda.push_back(ui->waitingL1);
    colaIzquierda.push_back(ui->waitingL2);
    colaIzquierda.push_back(ui->waitingL3);
    colaIzquierda.push_back(ui->waitingL4);


    colaDerecha.push_back(ui->waitingR0);
    colaDerecha.push_back(ui->waitingR1);
    colaDerecha.push_back(ui->waitingR2);
    colaDerecha.push_back(ui->waitingR3);
    colaDerecha.push_back(ui->waitingR4);
    qDebug() << "finishing setup";
}

void MainWindow::displayQueues()
{
    QPixmap *boat2 = new QPixmap(":/boat2.jpg");
    //std::cout << "right action pressed" << std::endl;
    QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    for (int i = 0; i <= colaIzquierda.size()-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaIzquierda.at(i);
        temp->clear();
        //temp->setPixmap(scaledBoat2);

    }

    for (int i = 0; i <= colaDerecha.size()-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaDerecha.at(i);
        temp->clear();
        //temp->setPixmap(scaledBoat2);

    }

    for (int i = 0; i <= dummy_list_left->length-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaIzquierda.at(i);
        temp->setScaledContents(true);
        temp->setPixmap(scaledBoat2);

    }

    for (int i = 0; i <= dummy_list_right->length-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaDerecha.at(i);
        temp->setScaledContents(true);
        temp->setPixmap(scaledBoat2);

    }
}

void MainWindow::create_struct(int queue)
{
    if(queue == 0){
        DummieStruct *dummie2 = new DummieStruct({20, 2, 102});
        addLast(dummy_list_left, dummie2);
    }else if(queue == 1){
        DummieStruct *dummie1 = new DummieStruct({10, 1, 101});
        addLast(dummy_list_right, dummie1);
    }else if(queue == 2){

    }else{

    }
}

void MainWindow::delete_first_struct(int queue)
{

    if(queue == 0){
        removeFirst(dummy_list_left);
    }else if(queue == 1){
        removeFirst(dummy_list_right);
    }else if(queue == 2){

    }else{

    }


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q) {
        //qDebug() << "Key A pressed";


        create_struct(0);
        //std::cout << "left action pressed" << std::endl;



        displayQueues();

    }
    else if (event->key() == Qt::Key_E) {
        //qDebug() << "Key E pressed";


        create_struct(1);

        displayQueues();

    }
    else if (event->key() == Qt::Key_A) {
        //qDebug() << "Key E pressed";

        delete_first_struct(0);
        displayQueues();

    } else if (event->key() == Qt::Key_D) {
        //qDebug() << "Key E pressed";


        delete_first_struct(1);
        displayQueues();

    }
    else{
        //qDebug() << "Key pressed: " << event->text();
    }

    // Call the base class event handler for default handling
    QMainWindow::keyPressEvent(event);
}

// Handle key release event
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A) {
        qDebug() << "Key A released";
    }
    else if (event->key() == Qt::Key_Space) {
        qDebug() << "Spacebar released";
    }

    QMainWindow::keyReleaseEvent(event);
}


void MainWindow::on_pruebaAddLabel_clicked()
{
    // Crear un nuevo QLabel con el texto "holaN" donde N puede ser un número o identificador______________________-
    QLabel *label = new QLabel("holaN", this);

    // Asumiendo que ya tienes un QHBoxLayout llamado "El_canal"
    QHBoxLayout *layout = findChild<QHBoxLayout*>("El_canal");

    // Verificar que el layout exista
    if(layout)
    {
        // Agregar el QLabel al QHBoxLayout
        layout->addWidget(label);
    }
    else
    {
        // Si el layout no existe, podrías manejar el error o crear uno nuevo
        qDebug() << "No se encontró el layout 'El_canal'";
    }

}

void MainWindow::setupCanal(int ancho){
    qDebug()<<"LALALLAL";
    for (int i = 0;i < ancho; i++){
        QLabel *label = new QLabel("holaN", this);
        QHBoxLayout *layout = findChild<QHBoxLayout*>("El_canal");
        layout->addWidget(label);
    }
}

